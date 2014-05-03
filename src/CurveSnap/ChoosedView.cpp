// ChoosedView.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "CurveSnapDoc.h"
#include "ChoosedView.h"
#include "CurveSnapView.h"


// CChoosedView

IMPLEMENT_DYNCREATE(CChoosedView, CView)


void DrawCross(CDC* pDC, CPoint pt, int r, COLORREF color)
{
    CPen pen(PS_SOLID, 1, color);//1是笔的宽度, RGB是宏, 用于生成颜色
    CPen* oldPen = pDC->SelectObject(&pen);

    pDC->MoveTo(pt.x-r, pt.y);
    pDC->LineTo(pt.x+r+1, pt.y);

    pDC->MoveTo(pt.x, pt.y-r);
    pDC->LineTo(pt.x, pt.y+r+1);

    pDC->SelectObject(oldPen);
}

CChoosedView::CChoosedView()
{

}

CChoosedView::~CChoosedView()
{
}

BEGIN_MESSAGE_MAP(CChoosedView, CView)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
//	ON_WM_ERASEBKGND()
//ON_WM_ERASEBKGND()
//ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CChoosedView drawing

void CChoosedView::OnDraw(CDC* /*pDC*/)
{
	CCurveSnapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!pDoc->extractor.choosedShower.IsEmpty())
	{
		// note that the shower will get DC from the WND, so don't use pdc above
		// using more than one dc from the same wnd is not stable
		// it may cause the latter DrawCross bugging.
		pDoc->extractor.choosedShower.Show(this->m_hWnd, cvx::SW_STRETCH);

		if (!pDoc->extractor.singlePoints.empty())
		{
			CDC* pdc = this->GetDC();
			COLORREF colorCross = pDoc->extractor.IsChoosedShowerBgBlack()? RGB(255,255,255) : RGB(255,0,0);

			for (list<cv::Point2d>::iterator it = pDoc->extractor.singlePoints.begin();
					it != pDoc->extractor.singlePoints.end(); ++it)
			{
				DrawCross(pdc, pDoc->extractor.choosedShower.Original2Show(*it),
					3, colorCross);
			}
			this->ReleaseDC(pdc);
		}
	}
}


// CChoosedView diagnostics

#ifdef _DEBUG
void CChoosedView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CChoosedView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CCurveSnapDoc* CChoosedView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCurveSnapDoc)));
	return (CCurveSnapDoc*)m_pDocument;
}

#endif //_DEBUG


// CChoosedView message handlers

BOOL CChoosedView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CHOOSE));    

	//	::MessageBeep(0);

	//	return TRUE;
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CChoosedView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	Invalidate(FALSE);
}

void CChoosedView::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetDocument()->extractor.SwitchChoosedShowerBackground();
	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

void CChoosedView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView* pView  = GetDocument()->GetView(RUNTIME_CLASS(CCurveSnapView));

	if (pView)
		this->GetParentFrame()->SetActiveView(pView);

	//CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CChoosedView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	// Every time opening a new document, this function is called
	Invalidate();
}
