#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvx.h"
#include "xf.h"

namespace cvx {

void imhist(const std::string& winname, const cv::Mat img, int channel, const cv::Mat mask, bool uniform, bool accumulate)
{
	cv::Mat hist = calcHistEx(img, channel, mask, uniform, accumulate);
	cv::Mat histImage = drawHist(hist);
	cv::imshow(winname, histImage);
}

cv::Mat drawHist(const cv::Mat hist)
{
	cv::Mat r(256*3,256*3, CV_8U, cv::Scalar(0));

	cv::Mat nhist;
	cv::normalize(hist, nhist, 0.0, 0.9, cv::NORM_MINMAX);

	for (int i=0; i<256; i++)
		cv::rectangle(r,
					cv::Point(3*i, 256*3 - 30),		// 30 for tick
					cv::Point(3*i+1, (int)((256*3-30)*(1-nhist.at<float>(i)))),
					cv:: Scalar(255) );

	int font = cv::FONT_HERSHEY_PLAIN;
	double scale = 1.0;
	int thickness = 1;
	for (int i=0; i<8; i++)
	{
		int baseline;
		std::string text = xf::tostr<int>(i*32);
		cv::Size tsize = cv::getTextSize(text, font,
			scale, thickness, &baseline);
		// center the text
		cv::Point org(i*32*3 - tsize.width/2, 256*3 - 27 + tsize.height);
		cv::putText(r, text,  org, font, scale, cv::Scalar(255), 1, 8);
	}
	return r;
}

cv::Mat calcHistEx(cv::Mat img, int channel, const cv::Mat mask, bool uniform, bool accumulate)
{	
	cv::Mat hist;
	int histSize[] = {256};				// {256, 256, 256} for 3 channels
	float hranges[] = {0.0, 255.0};
	const float* ranges[] = {hranges};	// {hranges, hranges, hranges} for 3 channels
	int channels[] = {channel};				// {0, 1, 2} for 3 channel
	cv::calcHist(&img, 1,channels, mask, hist, 1, histSize, ranges, uniform, accumulate);	// dims equal to channels

	return hist;
}

void imshowAllChannels(const std::string winname, cv::Mat img)
{
	std::vector<cv::Mat> imgs;
	cv::split(img, imgs);


	for (int i = 0; i < img.channels(); i++)
	{
		std::stringstream ss;
		ss << winname <<":" <<i;
		cv::imshow(ss.str(), imgs[i]);
	}
}

cv::Mat getChannel(const cv::Mat& img, int nChannel)
{
	if (nChannel >= img.channels())
		throw "selected channel index must be less than total channels";

	std::vector<cv::Mat> mats;
	cv::split(img, mats);

	return mats[nChannel];
}

cv::Mat RedImprint(cv::Mat bin, cv::Mat dst)
{
	dst.setTo(cv::Scalar(0,0,255), bin);
	return dst;
}

bool IsSame(const cv::Mat a, const cv::Mat b)
{
    cv::Mat diff;
    cv::bitwise_xor(a, b, diff);

    return (cv::countNonZero(diff) ==0);
}


bool IsContainedBin(const cv::Mat container, const cv::Mat mat)
{
    return IsSame(container & mat, mat);
}

cv::Mat sum(cv::Mat mat, int dim)
{
	if (dim==1)
	{
		cv::Mat r(1, mat.rows, CV_64F, cv::Scalar(0.0));

		for (int i = 0; i < mat.rows; i++)
		{
			r.at<double>(1,i) = cv::sum(mat.row(i))[0];
		}
		return r;
	}
	else if (dim==2)
	{
		cv::Mat r(mat.cols, 1, CV_64F, cv::Scalar(0.0));

		for (int i = 0; i < mat.rows; i++)
		{
			r.at<double>(i,1) = cv::sum(mat.col(i))[0];
		}
		return r;
	}
	else
		throw "dim is not 1 or 2.";
}


int round(double x)
{
    return (int) floor(x + 0.5);
}

cv::Point round(cv::Point2d pd)
{
	cv::Point p;
	p.x = round(pd.x);
	p.y = round(pd.y);

	return p;
}

bool IsCompletelyGray(cv::Mat mat)
{
	if (mat.channels() == 1)
		return true;
	else if (mat.channels() == 3)
	{
		std::vector<cv::Mat> v;
		cv::split(mat, v);

		return IsSame(v[0], v[1]) && IsSame(v[0], v[2]);
	}
	else
		throw "channels not 1 or 3.";
}

cv::Mat FindClosedArea(cv::Mat imgBin, cv::Point v, int nMaxDilate)
{
    cv::Mat matResult;

    cv::Mat mp(imgBin.size(), imgBin.type(), cv::Scalar(0));
    mp.at<uchar>(v.y, v.x) = 255;

    for (int i = 0; i < nMaxDilate; i++)
    {
        matResult = imgBin & mp;

        if (cv::countNonZero(matResult) > 0)
			return matResult;
        cv::dilate(mp, mp, cv::Mat());
    }

	return cv::Mat();
}
}