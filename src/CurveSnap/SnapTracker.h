#pragma once
#include "afxext.h"

#include <atlimage.h>

class SnapTracker : public CRectTracker
{
public:
    SnapTracker(void);
	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);


    CImage* pimg;

    CRgn lastRgn;
public:
    ~SnapTracker(void);
};
