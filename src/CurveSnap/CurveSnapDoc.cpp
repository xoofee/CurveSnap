// CurveSnapDoc.cpp : implementation of the CCurveSnapDoc class
//

#include "stdafx.h"
#include "CurveSnap.h"

#include "CurveSnapDoc.h"
#include "CurveSnapView.h"
#include "ChoosedView.h"

#include "ScreenSelectionDialog.h"

#include "CurveDlg.h"
#include "DataDlg.h"

#include <string>
#include <sstream>
#include <iostream> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCurveSnapDoc

IMPLEMENT_DYNCREATE(CCurveSnapDoc, CDocument)

BEGIN_MESSAGE_MAP(CCurveSnapDoc, CDocument)
  ON_COMMAND(ID_FILE_SNAP, &CCurveSnapDoc::OnFileSnap)
  ON_COMMAND(ID_EDIT_RESET, &CCurveSnapDoc::OnEidtReset)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RESET, &CCurveSnapDoc::OnUpdateEditReset)
  //    ON_COMMAND(ID_CURVE_PLOT, &CCurveSnapDoc::OnCurvePlot)
  //	ON_UPDATE_COMMAND_UI(ID_CURVE_PLOT, &CCurveSnapDoc::OnUpdateCurvePlot)
  ON_COMMAND(ID_CURVE_CLEARSELECTION, &CCurveSnapDoc::OnCurveClearselection)
  ON_COMMAND(ID_CURVE_COPYDATA, &CCurveSnapDoc::OnCurveCopydata)
  ON_UPDATE_COMMAND_UI(ID_CURVE_COPYDATA, &CCurveSnapDoc::OnUpdateCurveCopydata)
  ON_UPDATE_COMMAND_UI(ID_CURVE_CLEARSELECTION, &CCurveSnapDoc::OnUpdateCurveClearselection)
END_MESSAGE_MAP()


// CCurveSnapDoc construction/destruction

CCurveSnapDoc::CCurveSnapDoc()
{
}

CCurveSnapDoc::~CCurveSnapDoc()
{
}

BOOL CCurveSnapDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  extractor.Clear();


  POSITION pos = GetFirstViewPosition();
  while (pos)
  {
    CView* pView = GetNextView(pos);
    if (pView && pView->IsKindOf(RUNTIME_CLASS(CCurveSnapView)))
    {
      CRect rc;
      pView->GetClientRect(rc);
      extractor.SetInput(cv::Mat(rc.Height(), rc.Width(), CV_8UC3, cv::Scalar(255,255,255)));
    }

    if (pView && pView->IsKindOf(RUNTIME_CLASS(CChoosedView)))
    {
      pView->Invalidate();
    }
  }

  return TRUE;
}


BOOL CCurveSnapDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  // force to using color image
  cv::Mat image = cv::imread(lpszPathName, 1);

  if (image.empty())
  {
    AfxMessageBox(CString("Fail to open:\n\n") + lpszPathName);
    return FALSE;
  }

  extractor.Clear();
  extractor.SetInput(image);

  return TRUE;
}


// CCurveSnapDoc diagnostics

#ifdef _DEBUG
void CCurveSnapDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CCurveSnapDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


// CCurveSnapDoc commands

void CCurveSnapDoc::OnFileSnap()
{
  CRect rect;
  CScreenSelectionDialog dlg(rect);
  dlg.DoModal();

  CScreenImage cimg;

  cimg.CaptureRect(rect);

  if (cimg.IsNull())
  {
    AfxMessageBox(_T("Error"));
    return;
  }

  AfxGetApp()->OnCmdMsg(ID_FILE_NEW, 0, 0, 0);

  HBITMAP hbmp = cimg.Detach();
  cv::Mat image = cvx::ImageConverter::HBitmap2Mat(hbmp);
  DeleteObject(hbmp);

  extractor.Clear();
  extractor.SetInput(image);
  //cv::cvtColor(image, image, CV_BGR2GRAY);

  AfxGetMainWnd()->ShowWindow(SW_SHOW);

  UpdateAllViews(NULL);
}

void CCurveSnapDoc::OnEidtReset()
{
  //int r = MessageBox(NULL, _T("All the erasing operation will be lost. \n\nCalibration will remain."),
  //                                _T("Reset the Image?"),
  //                                         MB_YESNO|MB_ICONEXCLAMATION);

  //if (r == IDNO)
  //    return;

  extractor.ResetImage();

  UpdateAllViews(NULL);
}

void CCurveSnapDoc::OnUpdateEditReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(extractor.IsErased());
}

BOOL CCurveSnapDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
  // TODO: Add your specialized code here and/or call the base class

  if (extractor.GetImgShow().empty())
    return FALSE;

  cv::imwrite(lpszPathName, extractor.GetImgShow());

  return TRUE;
  //return CDocument::OnSaveDocument(lpszPathName);
}

void CCurveSnapDoc::OnUpdateCurveCopydata(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(extractor.IsCurveChoosed());
}

void CCurveSnapDoc::OnUpdateCurveClearselection(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(extractor.IsCurveChoosed());
}

CView* CCurveSnapDoc::GetView(CRuntimeClass* pClass)
{
  POSITION pos = GetFirstViewPosition();
  while (pos != NULL)
  {
    CView* pView = GetNextView(pos);

    if (pView && pView->IsKindOf(pClass))
      return pView;
  }

  return NULL;
}
void CCurveSnapDoc::OnCurveClearselection()
{
  extractor.ResetCurve();
  UpdateAllViews(NULL);
}

void CCurveSnapDoc::OnCurveCopydata()
{
  CDataDlg dlg;

  std::vector<double> x, y;
  extractor.GetData(x, y);
  dlg.dataproc_.SetInputs(x, y); 

  dlg.DoModal();
}
