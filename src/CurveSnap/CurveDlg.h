#pragma once

#include "../Chart/ChartCtrl.h"
#include "../Chart/ChartLineSerie.h"
#include "../Chart/ChartPointsSerie.h"

#include <vector>

// CCurveDlg dialog

class CCurveDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCurveDlg();

public:
	void SetData(std::vector<double> x, std::vector<double> y) {_x =x; _y=y;}
	void Draw();
	void Draw(std::vector<double> x, std::vector<double> y);
// Dialog Data
	enum { IDD = IDD_DIALOG_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CChartCtrl chart;
	bool bMaximize;
	bool bShowPlotSymbol;

private:
	std::vector<double> _x;
	std::vector<double> _y;
public:
//    afx_msg void OnBnClickedBtnCopy();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
