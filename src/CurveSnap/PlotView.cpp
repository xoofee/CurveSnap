// PlotView.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "CurveDlg.h"
#include "CurveSnapDoc.h"
#include "CurveSnapView.h"
#include "PlotView.h"


// CPlotView

IMPLEMENT_DYNCREATE(CPlotView, CView)

CPlotView::CPlotView()
{
}

CPlotView::~CPlotView()
{

}

BEGIN_MESSAGE_MAP(CPlotView, CView)
	ON_BN_CLICKED(IDC_BTN_FULLSCREEN, CPlotView::OnBtnClickedFullScreen)
	ON_WM_SIZE()
ON_WM_CREATE()
//ON_WM_MOUSEMOVE()
//ON_WM_NCMOUSEMOVE()
//ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//WNDPROC CPlotView::OldChartProc = NULL;
//BOOL CPlotView::bTracking = FALSE;
//HWND CPlotView::hwndView = NULL;;
//HWND CPlotView::hwndBtn = NULL;;
//
//LRESULT CALLBACK CPlotView::HoverProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
//{
//	switch(message)
//	{
//	case WM_MOUSEHOVER:
//		::ShowWindow(hwndBtn, SW_SHOW);
//		break;
//	case WM_MOUSELEAVE:
//		POINT pt;
//		::GetCursorPos(&pt);
//		::ScreenToClient(hwndView, &pt);
//		RECT rc;
//		::GetClientRect(hwndView, &rc);
//		::ShowWindow(hwndBtn, ::PtInRect(&rc, pt));
//		
//		bTracking = FALSE;
//		break;
//	case WM_MOUSEMOVE:
//		if (!bTracking)  
//		{  
//			// 鼠标移入窗时，请求WM_MOUSEHOVER和WM_MOUSELEAVE 消息  
//			TRACKMOUSEEVENT tme;  
//			tme.cbSize = sizeof(tme);  
//			tme.hwndTrack = hWnd;  
//			tme.dwFlags = TME_LEAVE|TME_HOVER;  
//			tme.dwHoverTime = 1;  
//			bTracking = _TrackMouseEvent(&tme);  
//		}		
//		break;
//	}
//	//Send all messages not processed to default edit proc
//	return  CallWindowProc(OldChartProc, hWnd,message, wParam,lParam);
//}

// CPlotView drawing

void CPlotView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPlotView diagnostics

#ifdef _DEBUG
void CPlotView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CCurveSnapDoc* CPlotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCurveSnapDoc)));
	return (CCurveSnapDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlotView message handlers

void CPlotView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (_cdlg.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		rect.InflateRect(2,2);
		_cdlg.MoveWindow(rect);

		if (_btnFullScreen.GetSafeHwnd())
		{
			CRect r;
			_cdlg.chart.GetClientRect(&r);
			CRect rectBtn(r.Width() - 39, 15, r.Width() - 39 + 21,  15 + 21);
			_btnFullScreen.MoveWindow(rectBtn);
		}
	}
		
		
}

#pragma warning(disable : 4312)

int CPlotView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	_cdlg.Create(IDD_DIALOG_CURVE, this);
	SetWindowLong(_cdlg.GetSafeHwnd(),  GWL_STYLE, DS_NOFAILCREATE | WS_CHILD);
	_cdlg.SetParent(this);	
	_cdlg.ShowWindow(SW_SHOW);

	CRect rc(60, 20, 80, 40);
	_btnFullScreen.Create("Zoom", WS_CHILD | BS_FLAT |BS_OWNERDRAW | WS_VISIBLE, 
		rc, &_cdlg.chart, IDC_BTN_FULLSCREEN);
	_btnFullScreen.LoadBitmaps(IDB_BITMAP_FULLSCREEN, IDB_BITMAP_FULLSCREEN_DOWN);
	_btnFullScreen.targetHwnd = this->GetSafeHwnd();
	_btnFullScreen.targetBtnID = IDC_BTN_FULLSCREEN;

	// subclass chart
	//hwndView = this->GetSafeHwnd();
	//hwndBtn = _btnFullScreen.GetSafeHwnd();

	//OldChartProc = (WNDPROC)GetWindowLong(_cdlg.chart.GetSafeHwnd(), GWL_WNDPROC);
	//SetWindowLong(_cdlg.chart.GetSafeHwnd(), GWL_WNDPROC, (LONG)HoverProc);

	return 0;
}
#pragma warning(default : 4312)

void CPlotView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	std::vector<double> xvec;
	std::vector<double> yvec;

	ImageCurveExtractor& extractor =  GetDocument()->extractor;

	if (extractor.IsImageSet() && extractor.IsCurveChoosed())
		GetDocument()->extractor.GetData(xvec, yvec);

    _cdlg.Draw(xvec, yvec);
}

void CPlotView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	CView* pView  = GetDocument()->GetView(RUNTIME_CLASS(CCurveSnapView));

	if (pView)
		this->GetParentFrame()->SetActiveView(pView);

	//CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
	
void CPlotView::OnBtnClickedFullScreen()
{
    CCurveDlg dlg;
	ImageCurveExtractor& extractor =  GetDocument()->extractor;

	std::vector<double> x, y;

	if (extractor.IsImageSet() && extractor.IsCurveChoosed())
		extractor.GetData(x, y);
	dlg.SetData(x, y);

	dlg.bMaximize = true;
	dlg.bShowPlotSymbol = true;
    dlg.DoModal();
}
