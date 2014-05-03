// ScreenSelectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "ScreenSelectionDialog.h"


// CScreenSelectionDialog dialog

IMPLEMENT_DYNAMIC(CScreenSelectionDialog, CDialog)

CScreenSelectionDialog::CScreenSelectionDialog(CRect& rect)
: CDialog(IDD_SCREEN_SELECTION), m_rect(rect)
{
    rect.SetRectEmpty();

    ptStart = CPoint(-1,-1);

    scrWidth = ::GetSystemMetrics(SM_CXSCREEN);
    scrHeight = ::GetSystemMetrics(SM_CYSCREEN);

    //m_tracker.m_nStyle = CRectTracker::resizeInside;// | CRectTracker::solidLine;
}

CScreenSelectionDialog::~CScreenSelectionDialog()
{
}


BEGIN_MESSAGE_MAP(CScreenSelectionDialog, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
//    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


HBITMAP ConvTo24Bit( HBITMAP hsrc)
{
    BITMAP bm;
    GetObject( hsrc, sizeof( BITMAP ), &bm );
    BITMAPINFOHEADER bi = { 0 };
    void *pBits;

    bi.biSize = sizeof( BITMAPINFOHEADER );
    bi.biBitCount = 24;
    bi.biPlanes = 1;
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;

    HDC hDC = GetDC( NULL );
    HBITMAP h24 = CreateDIBSection( hDC, ( LPBITMAPINFO )&bi, DIB_RGB_COLORS, &pBits, NULL, 0 );
    HDC hMemDC1 = CreateCompatibleDC( hDC );
    HDC hMemDC2 = CreateCompatibleDC( hDC );

    HBITMAP hsrcOld = ( HBITMAP )SelectObject( hMemDC1, hsrc );
    HBITMAP h24Old = ( HBITMAP )SelectObject( hMemDC2, h24 );
    BitBlt( hMemDC2, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCCOPY );
    SelectObject( hMemDC1, hsrcOld );
    SelectObject( hMemDC2, h24Old );
    DeleteDC( hMemDC1 );
    DeleteDC( hMemDC2 );
    ReleaseDC( NULL, hDC );

    return h24;
}


// CScreenSelectionDialog message handlers

BOOL CScreenSelectionDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    AfxGetMainWnd()->ShowWindow(SW_HIDE);
    ::Sleep(100);
    if(!m_image.CaptureScreen())
    {
        EndDialog(IDCANCEL);
    }
    m_image_org = m_image;      // Ref copy, not clone!

    m_tracker.pimg = & m_image_org;

    // 32bit to 24bit. 
    // You can also handle the 32bit directly without converting
    HBITMAP h32 = m_image.Detach();

    HBITMAP h24 = ConvTo24Bit(h32);
    m_image.Attach(h24);

    // Don't delete h32, it's detached to m_image_org!
    // Deleting it will causing error when m_image_org is deconstructed

    // DeleteObject(h32);


    BYTE* lpBits = (BYTE*)m_image.GetBits();

    int height = m_image.GetHeight();
    int width = m_image.GetWidth();
    int widthBytes = (m_image.GetPitch());

    int R, G, B ;
    double t = 0.7;
    for(int i=0 ;i< height;i ++)
    {
        for(int j=0 ;j< width;j ++)
        {
            B = lpBits[ i*widthBytes +j* 3];
            G = lpBits[ i*widthBytes +j* 3+1 ];
            R = lpBits[ i*widthBytes +j* 3+2 ];

            lpBits [i* widthBytes+j *3] = BYTE(B*t);
            lpBits [i* widthBytes+j *3+ 1] = BYTE(G*t);
            lpBits [i* widthBytes+j *3+ 2] = BYTE(R*t);
        }
    }

    SetWindowPos(&CWnd::wndTopMost, 0, 0, scrWidth, scrHeight, SWP_SHOWWINDOW);
#pragma warning(disable: 4311)
    SetClassLong(this->GetSafeHwnd(),
                                 GCL_HCURSOR , 
                                 (LONG)LoadCursor(NULL, IDC_CROSS));    
#pragma warning(default: 4311)
    return TRUE;
}



BOOL CScreenSelectionDialog::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CScreenSelectionDialog::OnPaint()
{
    CPaintDC dc(this);
    m_image.Draw(dc.GetSafeHdc(), 0, 0);

    // Test
    //if (ptStart.x >= 0)
    //{
    //    dc.MoveTo(0, ptStart.y);
    //    dc.LineTo(scrWidth, ptStart.y);
    //}

    // draw in SnapTracker instead
    //m_tracker.Draw(&dc);
}

void CScreenSelectionDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    ptStart = point;
    m_tracker.TrackRubberBand(this, point);
    m_tracker.GetTrueRect(m_rect);
    EndDialog(IDOK);
}
