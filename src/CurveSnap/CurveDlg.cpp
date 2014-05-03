// CurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "CurveDlg.h"


// CCurveDlg dialog

IMPLEMENT_DYNAMIC(CCurveDlg, CDialog)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurveDlg::IDD, pParent)
{
	bMaximize = false;
	bShowPlotSymbol = false;
}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHART, chart);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialog)
//    ON_BN_CLICKED(IDC_BTN_COPY, &CCurveDlg::OnBnClickedBtnCopy)
ON_WM_SIZE()
//ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CCurveDlg message handlers

//void CCurveDlg::OnBnClickedBtnCopy()
//{
//    //CChartCtrl* chart = new CChartCtrl();
//    //RECT rc = {0,0, 300, 200};
//    //chart->Create(this, rc, IDC_CHART, WS_CHILD | WS_VISIBLE);
//
//}

BOOL CCurveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	chart.EnableRefresh(false);

	chart.SetBackColor(RGB(255,255,255));
	chart.SetZoomRectColor(RGB(0,0,0));

	CChartStandardAxis* pBottomAxis =
		chart.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetAutomatic(true);

	CChartStandardAxis* pLeftAxis =
		chart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetAutomatic(true);

	Draw();
	chart.EnableRefresh(true);

	if (bMaximize)
		ShowWindow(SW_SHOWMAXIMIZED);

	this->SendMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCurveDlg::Draw()
{
	chart.EnableRefresh(false);
	chart.RemoveAllSeries();
	CRect rc;
	chart.GetClientRect(rc);
	if (_x.size() > 0)
	{ 
		if (bShowPlotSymbol || (int)_x.size()*3 < rc.Width())
		{
			CChartPointsSerie* pPointsSeries = chart.CreatePointsSerie();
			pPointsSeries->SetPoints(&_x[0],&_y[0],(unsigned)_x.size());
			pPointsSeries->SetColor(RGB(0,0,255));
			pPointsSeries->SetBorderColor(RGB(0,0,255));
		}
		CChartLineSerie* pLineSeries = chart.CreateLineSerie();
		pLineSeries->SetPoints(&_x[0],&_y[0],(unsigned)_x.size());
		pLineSeries->SetColor(RGB(255,0,0));

		//pSeries->SetSmooth(true); It's no wise to do so
	}
	chart.EnableRefresh(true);
}


void CCurveDlg::Draw(std::vector<double> x, std::vector<double> y)
{
	SetData(x, y);
	Draw();
}

void CCurveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (chart.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		rect.InflateRect(2,2);
		chart.MoveWindow(rect);
		Draw();	// could be improved by hide the curve, not redraw
	}
}


//BOOL CCurveDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	afxDump << "------in dlg" << "\n";
//	return CDialog::PreTranslateMessage(pMsg);
//}
