#pragma once


// CBitmapButtonEx

class CBitmapButtonEx : public CBitmapButton
{
	DECLARE_DYNAMIC(CBitmapButtonEx)

public:
	CBitmapButtonEx();
	virtual ~CBitmapButtonEx();

public:
	HWND targetHwnd;
	UINT targetBtnID;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


