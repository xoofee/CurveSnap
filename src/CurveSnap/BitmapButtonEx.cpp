// BitmapButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "BitmapButtonEx.h"


// CBitmapButtonEx

IMPLEMENT_DYNAMIC(CBitmapButtonEx, CBitmapButton)

CBitmapButtonEx::CBitmapButtonEx()
{
	targetHwnd = NULL;
}

CBitmapButtonEx::~CBitmapButtonEx()
{
}


BEGIN_MESSAGE_MAP(CBitmapButtonEx, CBitmapButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CBitmapButtonEx::OnBnClicked)
END_MESSAGE_MAP()



// CBitmapButtonEx message handlers



void CBitmapButtonEx::OnBnClicked()
{
	// TODO: Add your control notification handler code here
	if (targetHwnd)
	{	
		WPARAM wParam = MAKEWPARAM(targetBtnID,BN_CLICKED);
		::SendMessage(targetHwnd, WM_COMMAND, wParam, 0);
	}
}

BOOL CBitmapButtonEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	return CBitmapButton::PreCreateWindow(cs);
}
