// XColorPickerXP.cpp  Version 1.0 - see article at CodeProject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XColorPickerXP implements CXColorPickerXP, an ownerdraw combobox that is 
//     theme-aware and that pops up a color picker dialog.
//
//     XColorPickerXP is based on article by Chris Maunder:
//         http://www.codeproject.com/KB/miscctrl/colour_picker.aspx
//
// History
//     Version 1.0 - 2008 April 8
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XColorPickerXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable : 4996)	// disable bogus deprecation warning

#ifndef __noop
#if _MSC_VER < 1300
#define __noop ((void)0)
#endif
#endif

#undef TRACE
#define TRACE __noop
#undef TRACERECT
#define TRACERECT __noop

//=============================================================================
// if you want to see the TRACE output, uncomment this line:
//#include "XTrace.h"
//=============================================================================

#define NO_COLOR ((COLORREF)-1)

// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//=============================================================================
void AFXAPI DDX_XColorPickerXP(CDataExchange * pDX, int nIDC, COLORREF& crColor)
//=============================================================================
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	CXColorPickerXP * pColorPicker = (CXColorPickerXP *) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
	{
		crColor = pColorPicker->GetColor();
	}
	else // initializing
	{
		pColorPicker->SetColor(crColor);
	}
}

//=============================================================================
void AFXAPI DDX_XColorPickerXPCustom(CDataExchange * pDX, int nIDC, CString& strCustomColors)
//=============================================================================
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	CXColorPickerXP * pColorPicker = (CXColorPickerXP *) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
	{
		strCustomColors = pColorPicker->GetCustomColors();
	}
	else // initializing
	{
		pColorPicker->SetCustomColors(strCustomColors);
	}
}

IMPLEMENT_DYNCREATE(CXColorPickerXP, CComboBox)

//=============================================================================
BEGIN_MESSAGE_MAP(CXColorPickerXP, CComboBox)
//=============================================================================
	//{{AFX_MSG_MAP(CXColorPickerXP)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()


//=============================================================================
CXColorPickerXP::CXColorPickerXP()
//=============================================================================
{
	m_crEnabledColor = m_crColor = ::GetSysColor(COLOR_3DFACE);    // initial color
	m_bTrackSelection = FALSE;
	m_nSelectedRow = -1;
	m_nSelectedCol = -1;

	m_crColor = RGB(255,0,0);//RGB(255,255,255);
	m_DefaultColor = ::GetSysColor(COLOR_APPWORKSPACE);
	m_bPopupActive = FALSE;
	m_brush.CreateHatchBrush(HS_BDIAGONAL, GetSysColor(COLOR_BTNSHADOW));

	for (int i = 0; i < 16; i++)
		m_CustomColors[i] = RGB(255,255,255);
}

//=============================================================================
void CXColorPickerXP::PreSubclassWindow()
//=============================================================================
{
	if ((GetStyle() & CBS_OWNERDRAWFIXED) == 0)
	{
		TRACE(_T("ERROR - must have CBS_OWNERDRAWFIXED style\n"));
		ASSERT(FALSE);
	}

	CComboBox::PreSubclassWindow();							// Subclass Control

	SetItemHeight(-1, 15);

	//SetCurSel(0);											// Select First Item By Default
}

//=============================================================================
void CXColorPickerXP::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
//=============================================================================
{
	TRACE(_T("in CXColorPickerXP::DrawItem: %d\n"), lpDrawItemStruct->CtlID);

	CRect	rectItem(lpDrawItemStruct->rcItem);
	UINT	nState = lpDrawItemStruct->itemState;
	CBrush	brFrameBrush;

	CDC	dc;
	if (!dc.Attach(lpDrawItemStruct->hDC))			// Attach CDC Object
		return;										// Stop If Attach Failed

	COLORREF crColor = m_crColor;

	if (nState & ODS_DISABLED)
		crColor = GetSysColor(COLOR_INACTIVECAPTIONTEXT);

	if (nState & ODS_FOCUS)
		brFrameBrush.CreateStockObject(BLACK_BRUSH);	// Create Black Brush
	else
		brFrameBrush.CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));	// Create Black Brush

	dc.FrameRect(&rectItem, &brFrameBrush);	// draw frame

	rectItem.DeflateRect(1, 1);
	dc.FillSolidRect(&rectItem, crColor);		// draw color block

	dc.Detach();
}

//=============================================================================
void CXColorPickerXP::ShowColorPopup(BOOL bIgnoreLButtonUp) 
//=============================================================================
{
	CRect rect;
	GetWindowRect(rect);

	// m_CustomColors will be updated by CXColorPopup handler
	// when user selects a color
	new CXColorPopupXP(this, 
					  CPoint(rect.left, rect.bottom), 
					  m_crColor, 
					  m_CustomColors,
					  bIgnoreLButtonUp);

	CWnd *pParent = GetParent();
	if (pParent)
		pParent->SendMessage(CPN_DROPDOWN, (LPARAM)m_crColor, (WPARAM) GetDlgCtrlID());
}

//=============================================================================
void CXColorPickerXP::OnDropdown() 
//=============================================================================
{
	TRACE(_T("in CXColorPickerXP::OnDropdown\n"));

	// It is possible to get here if the user hits the F4 key or 
	// presses ALT + down arrow

	// stop the painting, or user will see black line under combobox
	SetRedraw(FALSE);

	// use a timer to let processing complete for this message
	SetTimer(1, 10, NULL);
}

//=============================================================================
void CXColorPickerXP::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
//=============================================================================
{
	TRACE(_T("in CXColorPickerXP::OnLButtonDown\n"));
	
	ShowColorPopup(TRUE);
	//CComboBox::OnLButtonDown(nFlags, point);
}

//=============================================================================
void CXColorPickerXP::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
//=============================================================================
{
	TRACE(_T("in CXColorPickerXP::OnLButtonDblClk\n"));
	
	//CComboBox::OnLButtonDblClk(nFlags, point);
}

//=============================================================================
void CXColorPickerXP::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/) 
//=============================================================================
{
	TRACE(_T("in CXColorPickerXP::MeasureItem\n"));
}

//=============================================================================
void CXColorPickerXP::OnTimer(UINT nIDEvent) 
//=============================================================================
{
	KillTimer(nIDEvent);
	ShowDropDown(FALSE);
	SetRedraw(TRUE);
	ShowColorPopup(FALSE);
	CComboBox::OnTimer(nIDEvent);
}

//=============================================================================
void CXColorPickerXP::OnEnable(BOOL bEnable)
//=============================================================================
{
	if (bEnable)
	{
		m_crColor = m_crEnabledColor;
	}
	else
	{
		m_crEnabledColor = m_crColor;
		m_crColor = RGB(192,192,192);
	}
	RedrawWindow();

	CComboBox::OnEnable(bEnable);
}

//=============================================================================
CXColorPickerXP& CXColorPickerXP::SetColor(COLORREF crColor)
//=============================================================================
{
	m_crEnabledColor = m_crColor = crColor;
	RedrawWindow();
	return *this;
}

//=============================================================================
COLORREF CXColorPickerXP::GetColor()
//=============================================================================
{
	return m_crColor;
}

//=============================================================================
CXColorPickerXP& CXColorPickerXP::SetCustomColors(COLORREF * pCustomColors)
//=============================================================================
{
	ASSERT(pCustomColors);
	if (pCustomColors)
	{
		for (int i = 0; i < 16; i++)
			m_CustomColors[i] = pCustomColors[i];
	}
	return *this;
}

//=============================================================================
void CXColorPickerXP::GetCustomColors(COLORREF * pCustomColors)
//=============================================================================
{
	ASSERT(pCustomColors);
	if (pCustomColors)
	{
		for (int i = 0; i < 16; i++)
			pCustomColors[i] = m_CustomColors[i];
	}
}

//=============================================================================
CXColorPickerXP& CXColorPickerXP::SetCustomColors(LPCTSTR lpszCustomColors)
//=============================================================================
{
	int i = 0;

	// initialize to white
	for (i = 0; i < 16; i++)
		m_CustomColors[i] = RGB(255,255,255);

	ASSERT(lpszCustomColors);
	if (lpszCustomColors)
	{
		TCHAR szCustomColors[256];
		memset(szCustomColors, 0, sizeof(szCustomColors));
		_tcsncpy(szCustomColors, lpszCustomColors, _countof(szCustomColors)-2);

		TCHAR *cp = _tcstok(szCustomColors, _T(" "));

		for (i = 0; i < 16; i++)
		{
			if (!cp)
				break;
			m_CustomColors[i] = _tcstoul(cp, NULL, 16);
			cp = _tcstok(NULL, _T(" "));
		}
	}

	return *this;
}

//=============================================================================
CString CXColorPickerXP::GetCustomColors()
//=============================================================================
{
	CString strCustomColors = _T("");
	CString str = _T("");
	for (int i = 0; i < 16; i++)
	{
		str.Format(_T("%06X"), m_CustomColors[i]);
		if (!strCustomColors.IsEmpty())
			strCustomColors += _T(" ");
		strCustomColors += str;
	}
	return strCustomColors;
}

//=============================================================================
LONG CXColorPickerXP::OnSelEndOK(WPARAM wParam, LPARAM lParam)
//=============================================================================
{
	m_nSelectedRow = LOWORD(lParam);
	m_nSelectedCol = HIWORD(lParam);

	CWnd * pParent = GetParent();
	if (pParent)
	{
		pParent->SendMessage(CPN_CLOSEUP, wParam, (LPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDOK, wParam, (LPARAM) GetDlgCtrlID());
	}

	COLORREF crNewColor = (COLORREF) wParam;
	if (crNewColor != m_crColor)
	{
		SetColor(crNewColor);
		if (pParent)
			pParent->SendMessage(CPN_SELCHANGE, wParam, (LPARAM) GetDlgCtrlID());
	}

	return TRUE;
}

//=============================================================================
LONG CXColorPickerXP::OnSelEndCancel(WPARAM wParam, LPARAM)
//=============================================================================
{
	SetColor((COLORREF) wParam);

	CWnd * pParent = GetParent();
	if (pParent)
	{
		pParent->SendMessage(CPN_CLOSEUP, wParam, (LPARAM) GetDlgCtrlID());
		pParent->SendMessage(CPN_SELENDCANCEL, wParam, (LPARAM) GetDlgCtrlID());
	}

	return TRUE;
}

//=============================================================================
LONG CXColorPickerXP::OnSelChange(WPARAM wParam, LPARAM)
//=============================================================================
{
	if (m_bTrackSelection)
		SetColor((COLORREF) wParam);

	return TRUE;
}
