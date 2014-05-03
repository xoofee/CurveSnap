#include "StdAfx.h"
#include "DataProcessor.h"

#include "opencv2/opencv.hpp"
#include "xf.h"
#include "../alglib/interpolation.h"

DataProcessor::DataProcessor(void)
{
  separators_.push_back("\t");
  separators_.push_back(" ");
  separators_.push_back(",");

  pixel_interval_ = 1;

  xtickmin_ = 0;
  xtickstep_ = 0;
  xtickmax_ = 0;
  precision_x_ = 0;
  precision_y_ = 0;

  fixed_x_ = FALSE;
  fixed_y_ = FALSE;


  separator_type_ = SEPARATOR_TAB;
  xticktype_ = XTICK_ORIGINAL_PIXELS;
  interptype_ = INTERP_LINEAR;
}

DataProcessor::~DataProcessor(void)
{
}

void DataProcessor::SetInputs(std::vector<double> x, std::vector<double> y) {
  x_ = x;
  y_ =y;
  xout_.clear();
  yout_.clear();

  xtickmin_ = *std::min_element(x_.begin(), x_.end());
  xtickmax_ = *std::max_element(x_.begin(), x_.end());

  if (xtickmax_ > xtickmin_)
    xtickstep_ = (xtickmax_ - xtickmin_)/30.0;
}

void DataProcessor::Resample()
{
	int n = (int)x_.size();
	if (n<1)
		return;

	if (pixel_interval_ <= 1)
	{
    xout_.clear();
		yout_.clear();
		return;
	}
  
	int nOutMax = cvCeil(double(n)/pixel_interval_);	// the exact output size may be less than nOutMax

	if (nOutMax < 1)
		nOutMax = 1;
		//throw "interval too big";

	xout_.clear();
	yout_.clear();

	double xmin = x_[0];
	double xmax = x_[n-1];
	double step = (xmax-xmin)/(nOutMax-1);

	int iLast = -1;
	for (int i = 0; i < nOutMax; i++)
	{
		//int iStart = iLast + 1;
		// dest:
		double dest = xmin + step*i;

		// search for the x that is closest to dest in the rest values
    std::vector<double> xd = x_;
		//vector<double> xRemain(x.begin() + iStart, x.end());
		for (std::vector<double>::iterator it = xd.begin(); it != xd.end(); it++)
			*it = abs(*it - dest);

		int iFound = int(min_element(xd.begin(), xd.end()) - xd.begin());

		if (iFound != iLast)
		{
			xout_.push_back(x_[iFound]);
			yout_.push_back(y_[iFound]);
			iLast = iFound;
		}
	}
}

std::string DataProcessor::GetOutputString() {
  if (IsOutputEmpty())
    return xf::GetXYDataString<double>( x_,y_, 
                                        separators_[separator_type_],
                                        precision_x_, precision_y_,
                                        fixed_x_, fixed_y_);
  else
    return xf::GetXYDataString<double>( xout_,yout_, 
                                        separators_[separator_type_], 
                                        precision_x_, precision_y_,
                                        fixed_x_, fixed_y_);
}

int DataProcessor::GetOutputLength() {
  if (IsOutputEmpty())
    return (int)x_.size();
  else
    return (int)xout_.size();
}

void DataProcessor::Interpolate() {
	int n = (int)x_.size();
	if (n<1)
		return;

  if (xtickmin_ >= xtickmax_ || xtickstep_ <= 0.0)
    return;

  int nout = cvCeil((xtickmax_ - xtickmin_)/xtickstep_) + 1;

  if (nout < 1 || nout > 1e6)
    return;

  xout_.clear();
  yout_.clear();

  for (int i = 0; i < nout; ++i)
    xout_.push_back(xtickmin_ + xtickstep_*i);

  alglib::spline1dinterpolant s;

  alglib::real_1d_array x, y;
  x.setcontent(x_.size(), &x_[0]);
  y.setcontent(y_.size(), &y_[0]);
  
  if (interptype_ == INTERP_LINEAR)
    alglib::spline1dbuildlinear(x, y, s);
  else
    alglib::spline1dbuildmonotone(x, y, s);

  yout_.resize(xout_.size());

  for (size_t i = 0; i < xout_.size(); ++i) {
    yout_[i] = alglib::spline1dcalc(s, xout_[i]);
  }
}