#pragma once

#include <windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace cvx {

class ImageConverter
{
public:
    static cv::Mat HBitmap2Mat(HBITMAP hbmp);

    static HBITMAP ConvTo24Bit( HBITMAP hsrc);

};

}