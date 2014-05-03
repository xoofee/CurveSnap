#pragma once
#include <vector>
#include "afxwin.h"

#include "../Chart/ChartCtrl.h"
#include "../Chart/ChartLineSerie.h"
#include "../Chart/ChartPointsSerie.h"
#include "afxcmn.h"

#include "DataProcessor.h"
#include "editex.h"

// CDataDlg dialog

class CDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataDlg)

	//enum Saperator
	//{
	//	SEPARATOR_TAB,
	//	SEPARATOR_SAPCE,
	//	SEPARATOR_COMMA,
	//}_saperator;

public:
	CDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataDlg();

public:

	afx_msg void OnBnClickedRadioSeparator(UINT idCtrl);

public:
  void UpdateCurve();

public:
  DataProcessor dataproc_;

private:
	CWinThread* _pThread;

	int _nChartLeft;
	int _nChartTop;
	int _nChartRightMargin;
	int _nChartBottomMargin;

  bool update_resample_flag_;
  bool update_linspace_flag_;
  bool update_format_flag_;

// Dialog Data
	enum { IDD = IDD_DIALOG_TEXT_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCopy();
public:
	CEdit edtData;
public:
	virtual BOOL OnInitDialog();
public:
	CStatic staticNumOfPoints;
public:
	afx_msg void OnBnClickedBtnSave();
public:
	CStatic staticCopyStatus;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnDeltaposSpinInterval(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CChartCtrl chart;
public:
	void DrawAndUpdate(void);
public:
	void SetPointsNum(void);
public:
	static UINT ResampleThreadFunc(LPVOID lpParam);

	afx_msg LONG OnResampleDone(WPARAM wParam, LPARAM);
public:
	CSpinButtonCtrl _spinInterval;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
public:
  afx_msg void OnBnClickedRadioXtickPixels();
  void SwitchXTickType(bool pixels_on);

public:
  afx_msg void OnBnClickedRadioXtickLinespace();
public:
  afx_msg void OnEnChangeFormat();
  afx_msg void OnEnChangeResample();
  afx_msg void OnEnChangeLinspace();
public:
  afx_msg void OnBnClickedBtnUpdate();
public:
  afx_msg void OnBnClickedCheckPrecisionFixed();
public:
  afx_msg void OnBnClickedRadioInterpLinear();
public:
  afx_msg void OnBnClickedRadioInterpMonoSpline();
public:
  CEditEx edt_inteval_;
public:
  CEditEx edt_xtickmin_;
public:
  CEditEx edt_xtickstep_;
public:
  CEditEx edt_xtickmax_;
public:
  CEditEx edt_precision_x_;
public:
  CEditEx edt_precision_y_;
};
