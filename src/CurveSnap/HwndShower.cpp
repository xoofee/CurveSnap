#include "stdafx.h"
#include "HwndShower.h"
#include <math.h>
#include "cvx.h"

namespace cvx {

HwndShower::HwndShower()
{
	//_data = 0;
	_binfo = (BITMAPINFO*)_buffer;

    scale = 1.0;
	width = 0;
    height = 0;
    left = 0;
    top = 0;

	_hwnd = NULL;
}

HwndShower::~HwndShower()
{
	//if (_data)
	//{
	//	delete[] _data;
	//	_data = 0;
	//}
}

void HwndShower::Set(cv::Mat image)
{
	if (image.empty())
		return;

	// Preparing BITMAP
	FillBitmapInfo(_binfo, image.cols, image.rows, image.channels()*8, 1);

	// Preparing Aligned data for BITMAP
	//if (_data)
	//	delete[] _data;

	int step = (image.cols * image.channels() + 3 ) & -4;		// aligned data
	size_t total = step * image.rows;

	//_data = new char[total];

	if (_dataVec.size() < total)
		_dataVec.resize(total);
	//_dataMat = cv::Mat(image.rows, step, CV_8U, cv::Scalar(0));

	_img = cv::Mat(image.rows, image.cols, image.type(), &_dataVec[0],size_t(step) );
	//_img = cv::Mat(image.rows, image.cols, image.type(), (void*)_data,size_t(step) );
	//_img = cv::Mat(image.rows, image.cols, CV_8U, cv::Scalar(0),size_t(step) );

	//	Vertical flipping of the image (flipCode == 0) to switch between top-left and bottom-left image origin. This
	//is a typical operation in video processing on Microsoft Windows OS.
	cv::flip(image, _img, 0);
}

void HwndShower::Clear()
{
	_img = cv::Mat();
}

bool HwndShower::IsEmpty()
{
	return _img.empty();
}

// the following function is a copy from OpenCV's source windows_32.cpp
void HwndShower::FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

// This may be accelerated by using memory dc, like the method in imshow

void HwndShower::Show(HWND hwnd, SHOW_MODE showMode, int margin)
{
	if (_img.empty())
		return;

	_hwnd = hwnd;

    // Image stretching to fit the window
	// ratio reserved

	RECT rectClient;
	::GetClientRect(hwnd, &rectClient);
    
	if (2*margin <rectClient.right && 2*margin <rectClient.bottom)
	{
		switch (showMode)
		{
			case SW_FIT:
				scale = cv::min((double(rectClient.right - 2*margin))/_img.cols, 
										(double(rectClient.bottom - 2*margin))/_img.rows); 

				width = (int)(scale * _img.cols);
				height = (int)(scale * _img.rows);

				left = (rectClient.right - width)/2;
				top = (rectClient.bottom - height)/2;
				break;
			case SW_STRETCH:
				width = rectClient.right - 2*margin;
				height = rectClient.bottom - 2*margin;
				left = margin;
				top = margin;		
				break;
			case SW_CENTER:
				width = _img.cols;
				height = _img.rows;
				left = (rectClient.right - _img.cols)/2;
                top = (rectClient.bottom - _img.rows)/2;

                if (left < 0)
                    left = 0;
                if (top < 0)
                    top = 0;
				break;
			case SW_ORIGINAL:
				width = _img.cols;
				height = _img.rows;
				left = margin;
				top = margin;
				break;
			default:
				assert(false);
				break;
		}
	  HDC hdc = ::GetDC(hwnd);
    //HDC memdc = ::CreateCompatibleDC(hdc);

		SetStretchBltMode(hdc,STRETCH_HALFTONE);
		int bSuccess = StretchDIBits( hdc,left, top, width, height, 0, 0, _img.cols, _img.rows,
			_img.data, _binfo, DIB_RGB_COLORS, SRCCOPY );

    //BitBlt(hdc, 0,0,width,height,memdc,0,0,SRCCOPY);
    //::DeleteDC(memdc);
	  ::ReleaseDC(hwnd, hdc);


	 // HDC hdc = ::GetDC(hwnd);
  //  HDC memdc = ::CreateCompatibleDC(hdc);

		//SetStretchBltMode(memdc,STRETCH_HALFTONE);
		//int bSuccess = StretchDIBits( memdc,left, top, width, height, 0, 0, _img.cols, _img.rows,
		//	_img.data, _binfo, DIB_RGB_COLORS, SRCCOPY );

  //  //BitBlt(hdc, 0,0,width,height,memdc,0,0,SRCCOPY);
  //  ::DeleteDC(memdc);
	 // ::ReleaseDC(hwnd, hdc);

	}

}

cv::Point2d HwndShower::Show2Original(POINT p)
{
    cv::Point2d v;
    v.x = (p.x - left)*double(_img.cols)/width;
	v.y= (p.y - top)*double(_img.rows)/height;

    return v;
}

POINT HwndShower::Original2Show(cv::Point2d v)
{
	cv::Point2d pd;
	pd.x = v.x * width/_img.cols + left;
	pd.y = v.y * height/_img.rows + top;

	cv::Point p= cvx::round(pd);

    POINT pr;
    pr.x = (LONG)p.x;
    pr.y = (LONG)p.y;

    return pr;
}

bool HwndShower::IsPointInShowArea(POINT pt)
{
	return pt.x >= left && pt.x < GetRight() && pt.y >= top && pt.y < GetBottom();
}

BOOL HwndShower::InvalidateRect(const RECT *lpRect, BOOL bErase)
{
	if (_hwnd)
		return ::InvalidateRect(_hwnd, lpRect, bErase);
	else
		return FALSE;
}

}