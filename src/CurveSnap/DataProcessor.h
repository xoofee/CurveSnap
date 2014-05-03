#pragma once

#include <vector>
#include <string>

class DataProcessor
{
public:
  enum XTickType {
    XTICK_ORIGINAL_PIXELS,
    XTICK_LINEAR_SPACED
  };

  enum SeparatorType {
    SEPARATOR_TAB = 0,
    SEPARATOR_SPACE = 1,
    SEPARATOR_COMMA = 2
  };

  enum InterpolateType {
    INTERP_LINEAR,
    INTERP_MONOTONE_SPLINE
  };

public:
  DataProcessor(void);
  ~DataProcessor(void);

public:
	void SetInputs(std::vector<double> x, std::vector<double> y);
	void GetInputs(std::vector<double>* x, std::vector<double>* y) {*x = x_; *y = y_;}
	void GetOutputs(std::vector<double>* xout, std::vector<double>* yout) {*xout = xout_; *yout = yout_;}
  bool IsInputEmpty() { return x_.empty() || y_.empty(); }
  bool IsOutputEmpty() { return xout_.empty() || yout_.empty(); }
  std::string GetOutputString();

  int GetInputLength() { return (int)x_.size(); }
  int GetOutputLength();

  void Interpolate();

public:
  void Resample();

public:
  SeparatorType separator_type_;
	int pixel_interval_;

  XTickType xticktype_;
  InterpolateType interptype_;

  double xtickmin_;
  double xtickstep_;
  double xtickmax_;

  int precision_x_;
  int precision_y_;

  BOOL fixed_x_;
  BOOL fixed_y_;

private:
	std::vector<double> x_;
	std::vector<double> y_;

	std::vector<double> xout_;
	std::vector<double> yout_;

  std::vector<std::string> separators_;
};
