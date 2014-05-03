#pragma once

#include "XColorPickerXP.h"


// CMainToolBar

class CMainToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CMainToolBar)

public:
	CMainToolBar();
	virtual ~CMainToolBar();

public:
    CXColorPickerXP colorPicker;

protected:
    LRESULT OnColorChange(WPARAM, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


