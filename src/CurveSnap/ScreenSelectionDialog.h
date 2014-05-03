#pragma once

#include "ScreenImage.h"

#include "SnapTracker.h"

// CScreenSelectionDialog dialog

class CScreenSelectionDialog : public CDialog
{
    DECLARE_DYNAMIC(CScreenSelectionDialog)
private:
    CRect& m_rect;
    CPoint ptStart;

    int scrWidth;
    int scrHeight;

    //CRectTracker m_tracker;
    SnapTracker m_tracker;

    CScreenImage m_image;
    CScreenImage m_image_org;
public:
    CScreenSelectionDialog(CRect& rect);
    virtual ~CScreenSelectionDialog();
    //const CRect& GetRect() const {return m_rect;}
    // Dialog Data
    enum { IDD = IDD_SCREEN_SELECTION };

protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
