#include "stdafx.h"
#include "ImageConverter.h"

namespace cvx {
cv::Mat ImageConverter::HBitmap2Mat(HBITMAP hbmp)
{
    HBITMAP hbmp24 = ConvTo24Bit(hbmp);

    BITMAP bmp;
    GetObject(hbmp24, sizeof(BITMAP), &bmp);

    cv::Mat mat(bmp.bmHeight, bmp.bmWidth, CV_8UC3, (void*)bmp.bmBits, size_t(bmp.bmWidthBytes));

	//	Vertical flipping of the image (flipCode == 0) to switch between top-left and bottom-left image origin. This
	//is a typical operation in video processing on Microsoft Windows OS.
    cv::Mat image = mat.clone();
    cv::flip(image, image, 0);

    DeleteObject(hbmp24);
    return image;
}

HBITMAP ImageConverter::ConvTo24Bit( HBITMAP hsrc)
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


}