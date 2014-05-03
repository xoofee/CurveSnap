#pragma once

#include "BitmapButtonEx.h"


// CPlotView view

class CPlotView : public CView
{
	DECLARE_DYNCREATE(CPlotView)

protected:
	CPlotView();           // protected constructor used by dynamic creation
	virtual ~CPlotView();

// Attributes
public:
	CCurveSnapDoc* GetDocument() const;

private:
	CCurveDlg _cdlg;
	CBitmapButtonEx _btnFullScreen;
	//CChartCtrl* pchart;


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

	//static WNDPROC OldChartProc;
	//static BOOL bTracking;
	//static HWND hwndView;
	//static HWND hwndBtn;
	//static LRESULT CALLBACK HoverProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	//LRESULT OnMouseHover(WPARAM wParam, LPARAM);  
	//LRESULT OnMouseLeave(WPARAM wParam, LPARAM);  
public:
	afx_msg void OnBtnClickedFullScreen();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in CPlotView.cpp
inline CCurveSnapDoc* CPlotView::GetDocument() const
   { return reinterpret_cast<CCurveSnapDoc*>(m_pDocument); }
#endif

