// XColorPickerXP.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
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

#ifndef XCOLORPICKERXP_H
#define XCOLORPICKERXP_H

#include "XColorPopupXP.h"

void AFXAPI DDX_XColorPickerXP(CDataExchange * pDX, int nIDC, COLORREF& crColor);
void AFXAPI DDX_XColorPickerXPCustom(CDataExchange * pDX, int nIDC, CString& strCustomColors);

//=============================================================================
class CXColorPickerXP : public CComboBox
//=============================================================================
{
	DECLARE_DYNCREATE(CXColorPickerXP);
	
// Construction
public:
	CXColorPickerXP();

// Attributes
public:
	COLORREF GetColor();
	void GetCustomColors(COLORREF * pCustomColors);
	CString GetCustomColors();

	CXColorPickerXP& SetColor(COLORREF crColor);
	CXColorPickerXP& SetCustomColors(COLORREF * pCustomColors);
	CXColorPickerXP& SetCustomColors(LPCTSTR lpszCustomColors);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorPickerXP)
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL		m_bTrackSelection;
	COLORREF	m_crColor;
	COLORREF	m_crEnabledColor;
	COLORREF	m_CustomColors[16];
	CRect		m_ArrowRect;
	COLORREF	m_DefaultColor;
	BOOL		m_bPopupActive;
	CBrush		m_brush;
	int			m_nSelectedRow, m_nSelectedCol;

	void ShowColorPopup(BOOL bIgnoreLButtonUp);

	// Generated message map functions
	//{{AFX_MSG(CXColorPickerXP)
	afx_msg void OnDropdown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelEndCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSelChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //XCOLORPICKERXP_H
