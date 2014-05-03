#include "StdAfx.h"
#include "SnapTracker.h"


SnapTracker::SnapTracker(void)
{
    pimg = NULL;

    const int cx = ::GetSystemMetrics(SM_CXSCREEN);
    const int cy = ::GetSystemMetrics(SM_CYSCREEN);
    lastRgn.CreateRectRgn(0, 0, 0, 0);
    //lastRgn.CreateRectRgn(0, 0, cx, cy);
}

SnapTracker::~SnapTracker(void)
{
}

void SnapTracker::DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd)
{
	CRect rc(lpRect);
	rc.NormalizeRect();

    pimg->Draw(pDC->m_hDC,rc,rc);

	//CString str;
	//str.Format("%d,%d", rc.Width(), rc.Height());
	//pDC->TextOut(rc.left, rc.top, str, str.GetLength());

	//afxDump << rc << "\n";

	//CRgn rgnText;
	//rgnText.CreateRectRgn(rc.left, rc.top,
	//						rc.left + 100, rc.top + 30 );
	//rgnText.CreateRectRgn(lpRect->right, lpRect->bottom,
	//						lpRect->right + 100, lpRect->bottom + 12 );

	//currentRgn.CombineRgn(&currentRgn, &rgnText, RGN_OR);

    CRgn rgnErase, currentRgn;
    currentRgn.CreateRectRgnIndirect(rc);
	rgnErase.CreateRectRgn(0,0,0,0);
    rgnErase.CombineRgn(&lastRgn, &currentRgn, RGN_DIFF);

    pWnd->InvalidateRgn(&rgnErase, FALSE);

	lastRgn.CopyRgn(&currentRgn);
}
