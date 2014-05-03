#pragma once

// CChoosedView view

class CChoosedView : public CView
{
	DECLARE_DYNCREATE(CChoosedView)

protected:
	CChoosedView();           // protected constructor used by dynamic creation
	virtual ~CChoosedView();

// Attributes
public:
	CCurveSnapDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in CChoosedView.cpp
inline CCurveSnapDoc* CChoosedView::GetDocument() const
   { return reinterpret_cast<CCurveSnapDoc*>(m_pDocument); }
#endif

