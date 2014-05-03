// OpenCV extended Library by xoofee

#pragma once

#include "HwndShower.h"
#include "ImageConverter.h"

#include <vector>
#include <string>
#include <sstream>

namespace cvx {

// Histogram
void imhist(const std::string& winname, const cv::Mat img, int channel = 0, const cv::Mat mask = cv::Mat(), bool uniform=true, bool accumulate=false);
cv::Mat drawHist(const cv::Mat hist);
cv::Mat calcHistEx(cv::Mat img, int channel = 0, const cv::Mat mask = cv::Mat(), bool uniform=true, bool accumulate=false);

// Channels
void imshowAllChannels(const std::string winname, cv::Mat img);
cv::Mat getChannel(const cv::Mat& img, int nChannel);

cv::Mat RedImprint(cv::Mat bin, cv::Mat dst);

bool IsSame(const cv::Mat a, const cv::Mat b);

bool IsContainedBin(const cv::Mat container, const cv::Mat mat);

cv::Mat sum(cv::Mat mat, int dim);

int round(double x);

cv::Point round(cv::Point2d pd);

bool IsCompletelyGray(cv::Mat mat);


cv::Mat FindClosedArea(cv::Mat imgBin, cv::Point v, int nMaxDilate = 10);

}