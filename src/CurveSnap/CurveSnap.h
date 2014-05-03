// CurveSnap.h : main header file for the CurveSnap application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCurveSnapApp:
// See CurveSnap.cpp for the implementation of this class
//

class CCurveSnapApp : public CWinApp
{
public:
	CCurveSnapApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFileOpen();
};

extern CCurveSnapApp theApp;