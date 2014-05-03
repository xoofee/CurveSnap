// CurveSnapView.h : interface of the CCurveSnapView class
//
#pragma once

#include "PixelCoordPair.h"


class CCurveSnapView : public CView
{
protected: // create from serialization only
	CCurveSnapView();
	DECLARE_DYNCREATE(CCurveSnapView)

// Attributes
public:
	CCurveSnapDoc* GetDocument() const;

    enum OPERATION
    {
        NONE        = -1,           // Nothing
	    SETPOINT1   = 0,			// Set Calibrate Point 1
        SETPOINT2   = 1,			// 2
        CHOOSING_CURVE = 2,          //
        CHOOSING_RECT = 3,          // 
        CHOOSING_COLOR = 4,          // 
        CHOOSING_POINTS = 5,          // 
        ERASING     = 109,            // Erasing size: 109-100=9
        PEN         = 101,            // Pen size:101-100=1
    };

private:
    OPERATION operation;

    CWinThread* _pChooseConnectCurveThread;
    CWinThread* _pExtractThread;

    COLORREF crossColors[2];

    int nPointCrossRadius;

    // point in original image
    CPoint _tempPt[2];

private:
	HCURSOR _cursorCross, _cursorWait, _cursorEraser, _cursorPen, _cursorChoose;

// User message
	afx_msg LONG OnChooseConnectCurveDone(WPARAM wParam, LPARAM);
	afx_msg LONG OnExtractDone(WPARAM wParam, LPARAM);

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCurveSnapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
public:
//    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
public:
    virtual void OnInitialUpdate();
public:
//    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
    afx_msg void OnCalibrateSetpoint1();
public:
    afx_msg void OnCalibrateSetpoint2();
public:
    afx_msg void OnUpdateCalibrateSetpoint1(CCmdUI *pCmdUI);
public:
    afx_msg void OnUpdateCalibrateSetpoint2(CCmdUI *pCmdUI);
public:
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
    void DrawCross(CDC* pDC, CPoint pt, int r, COLORREF color);
public:
//    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
    afx_msg void OnEditEraser();
public:
    afx_msg void OnUpdateEditEraser(CCmdUI *pCmdUI);
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
    afx_msg void OnCurveChooseConnectCurve();
public:
    afx_msg void OnUpdateCurveChooseConnectCurve(CCmdUI *pCmdUI);
public:
    afx_msg void OnEditPen();
public:
    afx_msg void OnUpdateEditPen(CCmdUI *pCmdUI);
public:
	afx_msg void OnCurveChooserectangle();
public:
	afx_msg void OnCurveChoosecolor();
public:
	afx_msg void OnUpdateCurveChoosecolor(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateCurveChooserectangle(CCmdUI *pCmdUI);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnCurveChoosepoints();
public:
	afx_msg void OnUpdateCurveChoosepoints(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CurveSnapView.cpp
inline CCurveSnapDoc* CCurveSnapView::GetDocument() const
   { return reinterpret_cast<CCurveSnapDoc*>(m_pDocument); }
#endif

