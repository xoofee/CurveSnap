#include "StdAfx.h"
#include "ImageCurveExtractor.h"

#include <math.h>
#include <numeric>

ImageCurveExtractor::ImageCurveExtractor(void)
{
  eraserColor = cv::Scalar(255,255,255);
	_choosedShowerBgBlack = false;
	Clear();
}

ImageCurveExtractor::~ImageCurveExtractor(void)
{
}

void ImageCurveExtractor::SetInput(cv::Mat image)
{
  this->imgInput = image.clone();

  imgProcess = imgInput.clone();

	cv::cvtColor(imgInput, imgInputLab, CV_BGR2Lab);

	_matCurve = cv::Mat(imgInput.size(), CV_8U, cv::Scalar(0));

	UpdateShowers();
}

CString ImageCurveExtractor::GetInputSizeString()
{
    CString str;
    str.Format(_T("%dx%d"), imgInput.cols, imgInput.rows);

    return str;
}

bool ImageCurveExtractor::GetInputSize(int& width, int& height)
{
	if (imgInput.empty())
	{
		return false;
	}
	else
	{
		width = imgInput.cols;
		height = imgInput.rows;
		return true;
	}
}

void ImageCurveExtractor::SetEraserColor(COLORREF color)
{
    eraserColor = cv::Scalar(GetBValue(color),
                            GetGValue(color),GetRValue(color));
}

// This function should be optimized.
// Draw the circle in WINDOW directly, and update extractor in the end
void ImageCurveExtractor::Draw(POINT p, int radius)
{
	// Take care of the ::Invalidate to avoid hight CPU
    if (!IsImageSet())
        return;

    cv::Point v = cvx::round(shower.Show2Original(p));

	int r = cvx::round(radius/shower.GetScale());

    cv::circle(imgProcess, v, r, eraserColor, -1);
	cv::Mat matCurveOld = _matCurve.clone();
	cv::circle(_matCurve, v, r, cv::Scalar(0), -1);

	// for efficiency comment following codes, for this function is called in loop

	//if (cvx::IsSame(matCurveOld, _matCurve))
	//{
		imgShow = imgProcess.clone();
		imgShow.setTo(cv::Scalar(0,0,255), _matCurve);
		shower.Set(imgShow);
	//}
	//else
	//{
	//	CheckCurve();
		_extracted = false;
		//if (_choosedShowerBgBlack)
		//{
		//	choosedShower.Set(_matCurve);
		//}
		//else
		//{
		//	cv::Mat mat(_matCurve.size(), CV_8UC3, cv::Scalar::all(255));
		//	mat.setTo(cv::Scalar(0,0,255), _matCurve);
		//	choosedShower.Set(mat);
		//}
	//}

	if (!singlePoints.empty())
	{
		int r = radius;
		if (r<5)
			r=5;
		for (list<cv::Point2d>::iterator it = singlePoints.begin();
				it != singlePoints.end();)
		{
			if (cv::norm(shower.Show2Original(p) - *it) <= r)
			{
				cv::Point v = cvx::round(*it);
				_matCurve.at<uchar>(v.y,v.x) = 0;

				singlePoints.erase(it++);
			}
			else
				it++;
		}
	}


    erased = true;
}

void ImageCurveExtractor::ResetImage()
{
    if (!IsImageSet())
        return;

    imgProcess = imgInput.clone();
    erased = false;

    imgShow = imgProcess.clone();

    //shower.Set(imgShow);

	UpdateShowers();
}

void ImageCurveExtractor::Clear()
{
    imgInput = cv::Mat();
    imgProcess = cv::Mat();
    imgShow = cv::Mat();
	_matCurve = cv::Mat();
	singlePoints.clear();
	_x.clear();
	_y.clear();

	_curveChoosed = false;
	_extracted = false;

    shower.Clear();

    erased = false;

    running = false;

    this->pcCvter.Clear();
}

cv::Mat ImageCurveExtractor::Threshold(cv::Mat img)
{
	cv::Mat imgBin;
    cv::cvtColor(img, imgBin, CV_BGR2GRAY);

    // draw hist

    //cv::imshow("gray", imgBin);
    //cv::imshow("hist", cvx::imhist(imgBin));

    cv::Scalar mean = cv::mean(imgBin);

    //afxDump << mean[0] << " ";

    if (mean[0] < 128)
    {
        imgBin = ~imgBin;
        mean[0] = 255 - mean[0];
    }

    cv::threshold(imgBin, imgBin, mean[0]-20, 255, CV_THRESH_BINARY_INV);
    //cv::threshold(imgBin, imgBin, 128, 255, CV_THRESH_BINARY_INV);

	return imgBin;
}

void ImageCurveExtractor::ChooseConnectCurve()
{
	cv::Point v = cvx::round(shower.Show2Original(choosedPoint));

	if (_matCurve.at<uchar>(v.y,v.x) > 0)
	{
		::MessageBox(shower.GetHwnd(), "Already choosed.", "Try again", MB_OK);
		running = false;
		return;
	}

    cv::Mat imgBin = Threshold(imgProcess);

    //cv::threshold(imgBin, imgBin, 128, 255, CV_THRESH_BINARY);
    //imgBin = ~imgBin;
    //cv::erode(imgBin, imgBin, cv::Mat());
    //cv::morphologyEx(imgBin, imgBin, cv::MORPH_OPEN, cv::Mat());

    //cv::medianBlur(imgBin, imgBin, 3);

    //cv::imshow("threshold", imgBin);
    //cv::waitKey();

	cv::Mat matCurveStart = cvx::FindClosedArea(imgBin, v);


    if (matCurveStart.empty())
    {
		::MessageBox(shower.GetHwnd(), "No curve found under click point.", "Try again", MB_OK);
        running = false;		// for safety
        return;
    }
	else if(cvx::IsContainedBin(_matCurve, matCurveStart))
	{
		::MessageBox(shower.GetHwnd(), "Already choosed.", "Try again", MB_OK);
		running = false;
		return;
	}

	imgBin &= ~_matCurve;	// only choose in the non-chosen part

    // Curve was found

    cv::Mat leftKernel(3,3, CV_8U, cv::Scalar(255));
    leftKernel.at<uchar>(0,0) = 0;
    leftKernel.at<uchar>(1,0) = 0;
    leftKernel.at<uchar>(2,0) = 0;

    cv::Mat rightKernel;
    cv::flip(leftKernel, rightKernel, 1);
    cv::Mat kernels[2] = {leftKernel, rightKernel};

    cv::Mat matCurves[2] = {matCurveStart, matCurveStart.clone()};

    //imgShow = imgProcess.clone() + cv::Scalar(64,64,64);
    //imgShow = imgProcess.clone();

	running = true;
    for (int k = 0; k < 2; k++)
    {
        cv::Mat matCurveLast = matCurves[k].clone();
        int n = 0;

		// Start dilating
        while (true)
        {
            n++;
            cv::dilate(matCurveLast, matCurves[k], kernels[k]);
            matCurves[k] = matCurves[k] & imgBin;

            //cv::Mat diff;
            //cv::bitwise_xor(matCurves[k], matCurveLast, diff);

            ////afxDump <<"dilating: " << ++n <<"," << cv::countNonZero(diff) << "\n";

			if (cvx::IsSame(matCurves[k], matCurveLast))
                break;
            else
                matCurveLast = matCurves[k].clone();    // clone is necessary!

            if (n%20 == 0)
            {
                // Draw the process in CView by multithread
				_matCurve |= matCurves[k];
				CheckCurve();
				UpdateShowers();
				shower.InvalidateRect(NULL, FALSE);
				choosedShower.InvalidateRect(NULL, FALSE);
            }
        }
		_matCurve |= matCurves[k];
		CheckCurve();
		UpdateShowers();
		shower.InvalidateRect(NULL, FALSE);
		choosedShower.InvalidateRect(NULL, FALSE);
    }

 //   _matCurve |= matCurves[0] | matCurves[1];

	////::InvalidateRect(hWnd, NULL, FALSE);
	//CheckCurve();
	//UpdateShowers();

	_extracted = false;

    running = false;
}

void ImageCurveExtractor::ResetCurve()
{
	if (imgInput.empty())
		throw "Input not set.";

	_matCurve = cv::Mat();
	_matCurve = cv::Mat(imgInput.size(), CV_8U, cv::Scalar(0));
	singlePoints.clear();
	_x.clear();
	_y.clear();

	CheckCurve();
	UpdateShowers();
}

void ImageCurveExtractor::ChooseAll()
{
	cv::Mat mat = Threshold(imgProcess);

	if (cv::countNonZero(mat)==0)
		return;

	_matCurve = mat;

	//cv::imshow("",_matCurve);

    cv::Mat white(_matCurve.size(), _matCurve.type(), cv::Scalar(255));

	cv::Mat mats[3] = { ~_matCurve, ~_matCurve, white};
	cv::merge(mats, 3, imgShow);

	CheckCurve();
	_extracted = false;
	//Extract();
	UpdateShowers();
}

bool ImageCurveExtractor::ChooseRectCurve(RECT rc)
{
	POINT topLeft = {rc.left, rc.top};
	POINT bottomRight = {rc.right, rc.bottom};
	return ChooseRectCurve(topLeft, bottomRight);
}

void SaturatePointBoundary(cv::Point& pt, int rows, int cols)
{
	if (pt.x < 0)
		pt.x = 0;
	if (pt.y < 0)
		pt.y = 0;

	if (pt.x >= cols)
		pt.x = cols - 1;
	if (pt.y >= rows)
		pt.y = rows - 1;
}

bool ImageCurveExtractor::ChooseRectCurve(POINT topLeft, POINT bottomRight)
{
	cv::Point p1 = cvx::round(shower.Show2Original(topLeft));
	cv::Point p2 = cvx::round(shower.Show2Original(bottomRight));

	// check boundary
	SaturatePointBoundary(p1, _matCurve.rows, _matCurve.cols);
	SaturatePointBoundary(p2, _matCurve.rows, _matCurve.cols);

	cv::Rect roi(p1,p2);
	cv::Mat mat = Threshold(imgProcess);

	if (cv::countNonZero(mat(roi))==0)
		return false;

	_matCurve(roi) |= mat(roi);

	CheckCurve();
	_extracted = false;

	UpdateShowers();

	return true;
}

void ImageCurveExtractor::Extract()
{
	_x.clear();
	_y.clear();

	if (!IsCurveChoosed())
		return;

	bool calibrated = this->pcCvter.IsCalibrated();

	double x,y;
	for (int i = 0; i < _matCurve.cols; i++)
	{
		int cNonZero = cv::countNonZero(_matCurve.col(i));
		//afxDump << "col: " << i << ", Nozero: " << cNonZero << " ";

		if (cNonZero ==0 || cNonZero > _matCurve.cols/2)
			continue;	// no point, skip
		

		vector<double> vy;
		for (int j=0; j < _matCurve.rows; j++)
		{
			if (_matCurve.at<uchar>(j,i) == 0)
				continue;

			if (calibrated)
			{
				pcCvter.Pixel2Coord(i, j, x, y);

				vy.push_back(y);
			}
			else	// use pixel
			{
				x = i;
				vy.push_back(_matCurve.rows - j);
			}
		}

		double ym = std::accumulate(vy.begin(), vy.end(), 0.0) / vy.size();

		//afxDump << " (" << x << "," << ym << ") \n";

		_x.push_back(x);
		_y.push_back(ym);
	}
	_extracted = true;

	// test smoothing
	//if (_x.size() >=3)
	//{
	//	vector<double> xs, ys;
	//	for (int i = 1; i < _x.size()-1; i++)
	//	{
	//		xs.push_back(_x[i]);
	//		ys.push_back((_y[i-1]+_y[i]+_y[i+1])/3);
	//	}
	//	_x = xs;
	//	_y = ys;
	//	::MessageBeep(0);
	//}
}

bool ImageCurveExtractor::ChooseColorCurve(POINT p)
{
	// 1. color sample area
	cv::Point v = cvx::round(shower.Show2Original(p));
	SaturatePointBoundary(v, _matCurve.rows, _matCurve.cols);

    cv::Mat imgBin = Threshold(imgProcess);
	cv::Mat img;
	cv::cvtColor(imgProcess, img, CV_BGR2Lab);

	cv::Mat matCurveStart = cvx::FindClosedArea(imgBin, v);

    if (matCurveStart.empty())
    {
		AfxMessageBox("No curve found near click point.");
        return false;
    }

    for (int k = 0; k < 5; k++)
    {
		cv::dilate(matCurveStart, matCurveStart, cv::Mat());
        matCurveStart = matCurveStart & imgBin;
	}

	// Get Hist of start area
	//cv::Mat hist;
	//int histSize[] = {255, 255};				// {256, 256, 256} for 3 channels
	//float hranges[] = {0.0, 255};
	//const float* ranges[] = {hranges ,hranges};	// {hranges, hranges, hranges} for 3 channels
	//int channels[] = {1,2};				// {0, 1, 2} for 3 channel
	//cv::calcHist(&img, 1,channels, matCurveStart, hist, 2, histSize, ranges);	// dims equal to channels

	//// back projection
	//cv::Mat back;
	//cv::calcBackProject(&img, 1, channels , hist, back, ranges);

	//back = back>0;
	////cv::imshow("back", back);

	//
	
	img = imgProcess.clone();
	cv::cvtColor(img, img, CV_BGR2Lab);
	img.convertTo(img, CV_64FC3);

	//cv::Mat matmean(1,1,CV_8UC3, cv::Scalar(0,0,255));
	//cv::cvtColor(matmean, matmean, CV_BGR2Lab);
	//matmean.convertTo(matmean, CV_64FC3);

	//cv::Vec3d a = matmean.at<cv::Vec3d>(0,0);
	//cv::Scalar b(a);
	//img -= b;

	cv::Mat matCurveStartReduceRow;
	cv::reduce(matCurveStart, matCurveStartReduceRow, 0, CV_REDUCE_MAX);
	cv::Moments momHorz = cv::moments(matCurveStartReduceRow, true);
	int c = cvRound(momHorz.m10/momHorz.m00);

	cv::Moments momVert = cv::moments(matCurveStart.col(c), true);
	int r = cvRound(momVert.m01/momVert.m00);


	img = (img - cv::mean(img, matCurveStart));
	//img = (img - cv::Scalar(img.at<cv::Vec3d>(r,c)))/2.0;

	img = img.mul(img);
	img.setTo(cv::Scalar::all(0), ~Threshold(imgProcess));
	//cv::imshow("thresh", Threshold(imgProcess));
	double minVal, maxVal;
	cv::minMaxLoc(img, &minVal , &maxVal);
	vector<cv::Mat> lab2;
	cv::split(img, lab2);

	cv::Mat dev;

	// It's not wise to just use a b channel
	cv::sqrt((lab2[0]/(2.55*2.55) + lab2[1] + lab2[2]), dev);	// CIE76 Color difference. JND:2.3

	//cv::Mat devForHist;
	//dev.convertTo(devForHist, CV_8U, 1);
	//cvx::imhist("", devForHist, 0, imgBin);

	//cv::minMaxLoc(dev, &minVal , &maxVal);
	//afxDump << "\n##   " << maxVal << "mean: " << cv::mean(dev, imgBin)[0];

	cv::Mat devBin = dev<25;
	//cv::Mat devBin = dev<(cv::mean(dev, imgBin)[0]/3);
	devBin.setTo(cv::Scalar::all(0), ~imgBin);
	//cv::imshow("dev < 30",  devBin);

	// show in gray
	//cv::normalize(dev, dev, 0, 1, cv::NORM_MINMAX);
	//dev = 1-dev;
	//dev.setTo(cv::Scalar::all(0), ~imgBin);
	//cv::imshow("dev",  dev);

	if (cv::countNonZero(devBin) > 0)
	{
		_matCurve |= devBin;
		CheckCurve();
		_extracted = false;
		UpdateShowers();
		return true;
	}
	else
		return false;
}

bool ImageCurveExtractor::ChoosePoint(POINT p)
{
	cv::Point v = cvx::round(shower.Show2Original(p));

	if (_matCurve.at<uchar>(v.y,v.x) > 0)
	{
		::MessageBox(shower.GetHwnd(), "Already choosed.", "Try again", MB_OK);
		return false;
	}

	singlePoints.push_back(shower.Show2Original(p));
	
	_matCurve.at<uchar>(v.y,v.x) = 255;
	CheckCurve();
	_extracted = false;
	UpdateShowers();

	return true;
}

void ImageCurveExtractor::CheckCurve() 
{
	_curveChoosed = cv::countNonZero(_matCurve) >0;
}

void ImageCurveExtractor::UpdateShowers()
{
	imgShow = imgProcess.clone();
	imgShow.setTo(cv::Scalar(0,0,255), _matCurve);
	shower.Set(imgShow);

	if (_choosedShowerBgBlack)
	{
		choosedShower.Set(_matCurve);
	}
	else
	{
		cv::Mat mat(_matCurve.size(), CV_8UC3, cv::Scalar::all(255));
		mat.setTo(cv::Scalar(0,0,255), _matCurve);
		choosedShower.Set(mat);
	}
}
