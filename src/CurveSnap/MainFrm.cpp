// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CurveSnap.h"

#include "MainFrm.h"

#include "CurveSnapDoc.h"
#include "CurveSnapView.h"
#include "CurveDlg.h"
#include "PlotView.h"
#include "ChoosedView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CALIBRATE,
	ID_INDICATOR_IMAGE_SIZE
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CALIBRATE, SBPS_NORMAL, 150);
    m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_IMAGE_SIZE, SBPS_NORMAL, 60);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


    int index = 0;
    RECT rect;
    //找到指定的工具项
    while(m_wndToolBar.GetItemID(index)!=ID_EDIT_COLOR_PICKER)
          index++;
    //设置指定工具项的宽度并获取新的区域  80是宽度
    m_wndToolBar.SetButtonInfo(index, ID_EDIT_COLOR_PICKER, TBBS_SEPARATOR, 38);
    m_wndToolBar.GetItemRect(index, &rect);

    //设置位置
    //rect.top;
    //rect.bottom -= 10;

    // 创建并显示控件

    if(!m_wndToolBar.colorPicker.Create(WS_CHILD|WS_VISIBLE| CBS_AUTOHSCROLL| CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED, rect, 
    &m_wndToolBar, ID_EDIT_COLOR_PICKER))
    {  
      TRACE0("Failed to create combo-box\n");
           return FALSE;
    }
    m_wndToolBar.colorPicker.SetWindowPos(NULL, 0, 0, 38, 10, SWP_NOZORDER | SWP_NOMOVE);
    m_wndToolBar.colorPicker.ShowWindow(SW_SHOW);

    m_wndToolBar.colorPicker.SetColor(RGB(255,255,255));


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	// split view

	if (!m_wndSplitMain.CreateStatic(this, 1, 2))
	{
		TRACE0("\nFailed to Create splitter.\n");
		return FALSE;
	}

	
	//m_wndSplitMain.SetColumnInfo(0, 500, 0);

	// 左边
	if (!m_wndSplitMain.CreateView(0,0, RUNTIME_CLASS(CCurveSnapView),	CSize(500,0),	pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}	

	// 右边
	if (!m_wndSplitRight.CreateStatic(&m_wndSplitMain, 2, 1,
								WS_CHILD | WS_VISIBLE,
								m_wndSplitMain.IdFromRowCol(0,1)) )
	{
		TRACE0("\nFailed to Create splitter.\n");
		return FALSE;
	}

	// 右上块
	if (!m_wndSplitRight.CreateView(0,0, RUNTIME_CLASS(CChoosedView),	CSize(0, 0), pContext))
	{
		TRACE0("Failed to create right up pane\n");
		return FALSE;
	}
	m_wndSplitRight.SetRowInfo(0, 200, 0);

	// 右下块
	if (!m_wndSplitRight.CreateView(1,0, RUNTIME_CLASS(CPlotView),	CSize(0,0),	pContext))
	{
		TRACE0("Failed to create right bottom pane\n");
		return FALSE;
	}

	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
 
    CCurveSnapDoc* pDoc = (CCurveSnapDoc*)this->GetActiveDocument();

	if (!pDoc || !pDoc->extractor.IsImageSet())
		return;

	int w,h;
	if (pDoc->extractor.GetInputSize(w, h))
	{
		CRect rc;
		m_wndSplitMain.GetClientRect(rc);
		int wNew = w*rc.Height()/h;

		//if (wNew > rc.Width() - 200)
		//	wNew = rc.Width() - 200;

		m_wndSplitMain.SetColumnInfo(0, wNew, 0);

		m_wndSplitMain.RecalcLayout();
	}
}
