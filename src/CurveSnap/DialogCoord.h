#pragma once
#include "editex.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "cvx.h"

// CDialogCoord dialog

class CDialogCoord : public CDialog
{
	DECLARE_DYNAMIC(CDialogCoord)

public:
	CDialogCoord(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogCoord();

// Dialog Data
	enum { IDD = IDD_DIALOG_COORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
  void SetImage(const cv::Mat& img);
  void UpdateWnd();
public:
  double x;
  double y;

  int px_;
  int py_;

  static const int kWndHalfSize = 10;

private:
  cv::Mat imginput_;
  cv::Mat imgpad_;
  cv::Mat imgwnd_;
  cvx::HwndShower shower;
public:
  CEditEx edt_x_;
  CEditEx edt_y_;

public:
  afx_msg void OnBnClickedBtnUp();
public:
  virtual BOOL OnInitDialog();
public:
  afx_msg void OnPaint();
public:
  afx_msg void OnBnClickedBtnLeft();
public:
  afx_msg void OnBnClickedBtnRight();
public:
  afx_msg void OnBnClickedBtnDown();
};
