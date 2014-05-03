// Show Opencv Image to arbitary window

#pragma once
#include <windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace cvx {

enum SHOW_MODE
{
	SW_FIT		= 0,			// Fit the target window reserving the heigth-width ratio
    SW_STRETCH	= 1,			// Fit the target window exactly
    SW_CENTER   = 2,            // 100% at center
	SW_ORIGINAL	= 3				// 100% at origin(0,0)
};

class HwndShower
{
// Constructor
public:
	HwndShower();

public:
	~HwndShower();

// Operation
public:
	void Show(HWND hwnd, SHOW_MODE showMode = SW_FIT, int margin = 0);
	BOOL InvalidateRect(const RECT *lpRect, BOOL bErase);

	static void FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin );

// Interface
	void Set(cv::Mat image);
	void Clear();
	bool IsEmpty();

    cv::Point2d Show2Original(POINT p);
    POINT Original2Show(cv::Point2d v);

	int GetWidth()		{ return width;}
	int GethHeight()	{ return height;}
	int GetLeft()		{ return left;}
	int GetTop()		{ return top;}
	int GetRight()		{ return left + width;}
	int GetBottom()		{ return top + height;}
	double GetScale()	{ return scale;}
	HWND GetHwnd()		{ return _hwnd; }

	// Relative rect in the DRAWING HWND. Not absolute
	RECT GetShowRect() { RECT rc = {left, top, GetBottom(), GetRight()}; return rc;}

	bool IsPointInShowArea(POINT pt);
// Attributes
private:
	cv::Mat		_img;
	//char*		_data;
	std::vector<uchar>	_dataVec;
    uchar		_buffer[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	BITMAPINFO* _binfo;

	HWND _hwnd;

private:
	int width, height, left, top;
	double scale;

};

}