// MainFrm.h : interface of the CMainFrame class
//


#pragma once


#include "MainToolBar.h"


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

private:
	CSplitterWnd m_wndSplitMain;
	CSplitterWnd m_wndSplitRight;


// Operations
public:

// Interface
public:
    COLORREF GetEraserColor() { return m_wndToolBar.colorPicker.GetColor(); }

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CMainToolBar m_wndToolBar;
	//CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


