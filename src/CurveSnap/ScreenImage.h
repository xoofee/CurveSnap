// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#pragma once

#include <atlimage.h>

class CScreenImage : public CImage
{
public:
    BOOL CaptureRect(const CRect& rect) throw();
    BOOL CaptureScreen() throw();
    BOOL CaptureWindow(HWND hWnd) throw();
};
