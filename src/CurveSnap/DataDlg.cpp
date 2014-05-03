// DataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "DataDlg.h"

#include "cvx.h"
#include "xf.h"

#include "ImageCurveExtractor.h"
// CDataDlg dialog

#define WM_RESAMPLE_DONE WM_USER + 3

IMPLEMENT_DYNAMIC(CDataDlg, CDialog)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDlg::IDD, pParent)
{
	_pThread = NULL;

	_nChartLeft = 0;
	_nChartTop = 100;
	_nChartRightMargin = 10;
	_nChartBottomMargin = 10;

  update_resample_flag_ = false;
  update_linspace_flag_ = false;
  update_format_flag_ = false;
}

CDataDlg::~CDataDlg()
{
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_DATA, edtData);
  DDX_Control(pDX, IDC_STATIC_POINTS_NUM, staticNumOfPoints);
  DDX_Control(pDX, IDC_STATIC_COPY_STATUS, staticCopyStatus);
  DDX_Text(pDX, IDC_EDIT_INTERVAL, dataproc_.pixel_interval_);
  DDX_Text(pDX, IDC_EDIT_LINSPACE_MIN, dataproc_.xtickmin_);
  DDX_Text(pDX, IDC_EDIT_LINSPACE_STEP, dataproc_.xtickstep_);
  DDX_Text(pDX, IDC_EDIT_LINSPACE_MAX, dataproc_.xtickmax_);
  DDX_Text(pDX, IDC_EDIT_PRECISION_X, dataproc_.precision_x_);
  DDX_Text(pDX, IDC_EDIT_PRECISION_Y, dataproc_.precision_y_);
  DDX_Text(pDX, IDC_EDIT_PRECISION_Y, dataproc_.precision_y_);
  DDX_Check(pDX, IDC_CHECK_PRECISION_FIXED_X, dataproc_.fixed_x_);
  DDX_Check(pDX, IDC_CHECK_PRECISION_FIXED_Y, dataproc_.fixed_y_);
  DDV_MinMaxInt(pDX, dataproc_.pixel_interval_, 1, 10000);
  DDX_Control(pDX, IDC_OUT_CHART, chart);
  DDX_Control(pDX, IDC_SPIN_INTERVAL, _spinInterval);
  DDX_Control(pDX, IDC_EDIT_INTERVAL, edt_inteval_);
  DDX_Control(pDX, IDC_EDIT_LINSPACE_MIN, edt_xtickmin_);
  DDX_Control(pDX, IDC_EDIT_LINSPACE_STEP, edt_xtickstep_);
  DDX_Control(pDX, IDC_EDIT_LINSPACE_MAX, edt_xtickmax_);
  DDX_Control(pDX, IDC_EDIT_PRECISION_X, edt_precision_x_);
  DDX_Control(pDX, IDC_EDIT_PRECISION_Y, edt_precision_y_);
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialog)
	ON_MESSAGE(WM_RESAMPLE_DONE, OnResampleDone)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_TAB, IDC_RADIO_COMMA, OnBnClickedRadioSeparator)
	ON_BN_CLICKED(IDC_BTN_COPY, &CDataDlg::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDataDlg::OnBnClickedBtnSave)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INTERVAL, &CDataDlg::OnDeltaposSpinInterval)
	ON_WM_SIZE()
  ON_BN_CLICKED(IDC_RADIO_XTICK_PIXELS, &CDataDlg::OnBnClickedRadioXtickPixels)
  ON_BN_CLICKED(IDC_RADIO_XTICK_LINESPACE, &CDataDlg::OnBnClickedRadioXtickLinespace)
  ON_EN_CHANGE(IDC_EDIT_INTERVAL, &CDataDlg::OnEnChangeResample)
  ON_EN_CHANGE(IDC_EDIT_PRECISION_X, &CDataDlg::OnEnChangeFormat)
  ON_EN_CHANGE(IDC_EDIT_PRECISION_Y, &CDataDlg::OnEnChangeFormat)
  ON_EN_CHANGE(IDC_EDIT_LINSPACE_MIN, &CDataDlg::OnEnChangeLinspace)
  ON_EN_CHANGE(IDC_EDIT_LINSPACE_STEP, &CDataDlg::OnEnChangeLinspace)
  ON_EN_CHANGE(IDC_EDIT_LINSPACE_MAX, &CDataDlg::OnEnChangeLinspace)
  ON_BN_CLICKED(IDC_BTN_UPDATE, &CDataDlg::OnBnClickedBtnUpdate)
  ON_BN_CLICKED(IDC_CHECK_PRECISION_FIXED_X, &CDataDlg::OnBnClickedCheckPrecisionFixed)
  ON_BN_CLICKED(IDC_CHECK_PRECISION_FIXED_Y, &CDataDlg::OnBnClickedCheckPrecisionFixed)
  ON_BN_CLICKED(IDC_RADIO_INTERP_LINEAR, &CDataDlg::OnBnClickedRadioInterpLinear)
  ON_BN_CLICKED(IDC_RADIO_INTERP_MONO_SPLINE, &CDataDlg::OnBnClickedRadioInterpMonoSpline)
END_MESSAGE_MAP()


// CDataDlg message handlers
void CDataDlg::OnEnChangeFormat()
{
  update_format_flag_ = true;
  GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
  GetDlgItem(IDC_LABEL_UPDATE_ENTER)->EnableWindow(TRUE);
}

void CDataDlg::OnEnChangeResample()
{
  update_resample_flag_ = TRUE;
  GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
  GetDlgItem(IDC_LABEL_UPDATE_ENTER)->EnableWindow(TRUE);
}

void CDataDlg::OnEnChangeLinspace()
{
  update_linspace_flag_ = true;
  GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
  GetDlgItem(IDC_LABEL_UPDATE_ENTER)->EnableWindow(TRUE);
}

void CDataDlg::OnBnClickedBtnCopy()
{
	// TODO: Add your control notification handler code here
	CString str;
	edtData.GetWindowText(str);

  xf::CopyToClipboard(str);

	staticCopyStatus.SetWindowText("Copied!");

	SetTimer(0, 2000, NULL);
}

void CDataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		this->KillTimer(0);
		staticCopyStatus.SetWindowText("");
	}

	CDialog::OnTimer(nIDEvent);
}

void CDataDlg::OnBnClickedRadioSeparator(UINT idCtrl)
{
	switch (idCtrl)
	{
	case IDC_RADIO_TAB:
    dataproc_.separator_type_ = DataProcessor::SEPARATOR_TAB;
		break;
	case IDC_RADIO_SPACE:
    dataproc_.separator_type_ = DataProcessor::SEPARATOR_SPACE;
		break;
	case IDC_RADIO_COMMA:
    dataproc_.separator_type_ = DataProcessor::SEPARATOR_COMMA;
		break;
	default:
    dataproc_.separator_type_ = DataProcessor::SEPARATOR_TAB;
	}
  update_format_flag_ = true;

  UpdateCurve();
}

BOOL CDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	staticCopyStatus.SetWindowText("");

	OnBnClickedRadioSeparator(IDC_RADIO_TAB);
	((CButton*)GetDlgItem(IDC_RADIO_TAB))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_RADIO_XTICK_PIXELS))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_RADIO_INTERP_LINEAR))->SetCheck(TRUE);

  // chart
  chart.EnableRefresh(false);

	chart.SetBackColor(RGB(255,255,255));
	chart.SetZoomRectColor(RGB(0,0,0));

	CChartStandardAxis* pBottomAxis =
		chart.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetAutomatic(true);

	CChartStandardAxis* pLeftAxis =
		chart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetAutomatic(true);
	chart.EnableRefresh(true);

	DrawAndUpdate();

  // chart size control
	CRect rcChart, rcDlg;
	chart.GetWindowRect(rcChart);
	ScreenToClient(rcChart);
	GetClientRect(rcDlg);

	_nChartLeft = rcChart.left;
	_nChartTop = rcChart.top;
	_nChartRightMargin = rcDlg.right - rcChart.right;
	_nChartBottomMargin  = rcDlg.bottom - rcChart.bottom;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataDlg::OnBnClickedBtnSave()
{
	CString strFilePath;
	CString szFilters = _T("Data File (*.*)|*.*||");	
	CFileDialog fileDlg (FALSE, _T("*.*"),_T("*.*"),
		OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT, szFilters, this);

	if( fileDlg.DoModal() == IDOK )
	{	
		CString strContent;
		edtData.GetWindowText(strContent);

		try
		{
			CFile file(fileDlg.GetPathName(), CFile::modeWrite|CFile::modeCreate);
			file.Write(strContent, strContent.GetLength());
			file.Close();
		}
		catch (CException* e)
		{
			CString errMsg;
			e->GetErrorMessage(errMsg.GetBuffer(1024), 1024);
			errMsg.ReleaseBuffer();
			MessageBox(errMsg, "error");
		}
	}
}

void CDataDlg::OnDeltaposSpinInterval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (_pThread)
		return;

	if(pNMUpDown->iDelta ==1)           //如果点击的是Spin中的往上按钮
	{
		dataproc_.pixel_interval_--;
		if (dataproc_.pixel_interval_ < 1)
			dataproc_.pixel_interval_ = 1;
	}
	else		// if(pNMUpDown->iDelta== - 1)    //如果点击的是Spin中往下按钮
	{
		dataproc_.pixel_interval_++;
		if (dataproc_.pixel_interval_ > 10000)
			dataproc_.pixel_interval_;
	}

	UpdateData(FALSE);

	//if (_pThread && _pThread->m_hThread)
	//{
	//	DWORD code;
	//	bool res = GetExitCodeThread(_pThread->m_hThread, &code);
	//	if (!res && code==STILL_ACTIVE)//线程还活着
	//	{
	//		//kill is not safe. post message to thread to end it by itself
	//		return;
	//	}
	//}

	_pThread = AfxBeginThread(ResampleThreadFunc, this);

	*pResult = 0;
}

UINT CDataDlg::ResampleThreadFunc(LPVOID lpParam)
{
	CDataDlg* p = (CDataDlg*)lpParam;

	if (!p)
		return 0;

  p->dataproc_.Resample();
	
	::SendMessage(p->GetSafeHwnd(), WM_RESAMPLE_DONE, 0, 0);
	return 0;
}

LONG CDataDlg::OnResampleDone(WPARAM wParam, LPARAM)
{
	_pThread = NULL;
	//_spinInterval.EnableWindow(TRUE);
	DrawAndUpdate();
	return 1;
}

void CDataDlg::DrawAndUpdate(void)
{
	chart.EnableRefresh(false);

	chart.RemoveAllSeries();

  if (dataproc_.IsInputEmpty())
    return;

  std::vector<double> x, y;
  dataproc_.GetInputs(&x, &y);

	CRect rc;
	chart.GetClientRect(rc);
	if (dataproc_.pixel_interval_ == 1 && (int)x.size()*3 < rc.Width())
	{
		CChartPointsSerie* pPointsSeries = chart.CreatePointsSerie();
		pPointsSeries->SetPoints(&x[0],&y[0],(unsigned)x.size());
		pPointsSeries->SetColor(RGB(255,0,0));
		pPointsSeries->SetBorderColor(RGB(255,0,0));
	}

	CChartLineSerie* pLineSeries = chart.CreateLineSerie();
	pLineSeries->SetPoints(&x[0],&y[0],(unsigned)x.size());
	pLineSeries->SetColor(RGB(255,0,0));

  if (!dataproc_.IsOutputEmpty())
  {
    std::vector<double> xout, yout;
    dataproc_.GetOutputs(&xout, &yout);

		CChartLineSerie* pLineSeriesOut = chart.CreateLineSerie();
		pLineSeriesOut->SetPoints(&xout[0],&yout[0],(unsigned)xout.size());
		pLineSeriesOut->SetColor(RGB(0,0,255));

		CChartPointsSerie* pPointsSeriesOut = chart.CreatePointsSerie();
		pPointsSeriesOut->SetPoints(&xout[0],&yout[0],(unsigned)xout.size());
		pPointsSeriesOut->SetColor(RGB(0,0,255));
		pPointsSeriesOut->SetBorderColor(RGB(0,0,255));
	  //pSeries->SetSmooth(true); It's no wise to do so
  }
  
  chart.EnableRefresh(true);

	SetPointsNum();
	edtData.SetWindowText(dataproc_.GetOutputString().c_str());
}

void CDataDlg::SetPointsNum(void)
{
	std::string total  = xf::tostr(dataproc_.GetInputLength());
	std::string out  = xf::tostr(dataproc_.GetOutputLength());
	staticNumOfPoints.SetWindowText((out + "/" + total) .c_str());
}

void CDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (chart.GetSafeHwnd())
	{
		chart.MoveWindow(_nChartLeft, _nChartTop, 
			cx - _nChartLeft - _nChartRightMargin,
			cy - _nChartTop - _nChartBottomMargin);
		
		DrawAndUpdate();
	}
}

BOOL CDataDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}

void CDataDlg::OnOK()
{
	if (_pThread)
		return;

    CWnd* pwndCtrl = GetFocus();
    int ctrl_ID = pwndCtrl->GetDlgCtrlID();

    int max_interval = dataproc_.GetInputLength() - 1;
    switch (ctrl_ID) {
       // case IDC_EDIT_INTERVAL:
			    //UpdateData(TRUE);
			    //if (dataproc_.pixel_interval_ > max_interval)
			    //{
				   // dataproc_.pixel_interval_ = max_interval;
				   // UpdateData(FALSE);
			    //}
			    //_pThread = AfxBeginThread(ResampleThreadFunc, this);
       //   break;
        case IDOK:
          CDialog::OnOK();
          break;
        default:
          UpdateCurve();
          break;
    }
    pwndCtrl->SetFocus();

	//CDialog::OnOK();
}

void CDataDlg::OnBnClickedRadioXtickPixels()
{
  if (dataproc_.xticktype_ == DataProcessor::XTICK_ORIGINAL_PIXELS)
    return;

  dataproc_.xticktype_ = DataProcessor::XTICK_ORIGINAL_PIXELS;
  SwitchXTickType(true);

  update_resample_flag_ = true;
  UpdateCurve();
}

void CDataDlg::OnBnClickedRadioXtickLinespace()
{
  if (dataproc_.xticktype_ == DataProcessor::XTICK_LINEAR_SPACED)
    return;

  dataproc_.xticktype_ = DataProcessor::XTICK_LINEAR_SPACED;
  SwitchXTickType(false);

  update_linspace_flag_ = true;
  UpdateCurve();
}

void CDataDlg::SwitchXTickType(bool pixels_on)
{
  GetDlgItem(IDC_EDIT_INTERVAL)->EnableWindow(pixels_on);
  GetDlgItem(IDC_SPIN_INTERVAL)->EnableWindow(pixels_on);
  GetDlgItem(IDC_LABEL_INTERVAL)->EnableWindow(pixels_on);
  
  GetDlgItem(IDC_EDIT_LINSPACE_MIN)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_EDIT_LINSPACE_STEP)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_EDIT_LINSPACE_MAX)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_LABEL_LS_MIN)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_LABEL_LS_STEP)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_LABEL_LS_MAX)->EnableWindow(!pixels_on);
  
  GetDlgItem(IDC_GROUP_INTERP_METHOD)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_RADIO_INTERP_LINEAR)->EnableWindow(!pixels_on);
  GetDlgItem(IDC_RADIO_INTERP_MONO_SPLINE)->EnableWindow(!pixels_on);
}

void CDataDlg::OnBnClickedBtnUpdate()
{
  UpdateCurve();
}

void CDataDlg::UpdateCurve()
{
  UpdateData();

  switch (dataproc_.xticktype_)   {
  case DataProcessor::XTICK_ORIGINAL_PIXELS:
    if (update_resample_flag_) {
	    _pThread = AfxBeginThread(ResampleThreadFunc, this);
      update_resample_flag_ = false;
      update_format_flag_ = false;
    }
    break;
  case DataProcessor::XTICK_LINEAR_SPACED:
    if (update_linspace_flag_) {
      dataproc_.Interpolate();
      DrawAndUpdate();
      edtData.SetWindowText(dataproc_.GetOutputString().c_str());
	    /*_pThread = AfxBeginThread(ResampleThreadFunc, this);*/
      update_linspace_flag_ = false;
      update_format_flag_ = false;
    }
    break;
  }

  if (update_format_flag_) {
    edtData.SetWindowText(dataproc_.GetOutputString().c_str());
    update_format_flag_ = false;
  }

  GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
  GetDlgItem(IDC_LABEL_UPDATE_ENTER)->EnableWindow(FALSE);
}

void CDataDlg::OnBnClickedCheckPrecisionFixed()
{
  update_format_flag_ = true;
  UpdateCurve();
}

void CDataDlg::OnBnClickedRadioInterpLinear()
{
  if (dataproc_.interptype_ == DataProcessor::INTERP_LINEAR)
    return;

  dataproc_.interptype_ = DataProcessor::INTERP_LINEAR;
  update_linspace_flag_ = true;
  UpdateCurve();
}

void CDataDlg::OnBnClickedRadioInterpMonoSpline()
{
  if (dataproc_.interptype_ == DataProcessor::INTERP_MONOTONE_SPLINE)
    return;

  dataproc_.interptype_ = DataProcessor::INTERP_MONOTONE_SPLINE;
  update_linspace_flag_ = true;
  UpdateCurve();
}
