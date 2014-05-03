#pragma once
#include "PixelCoordConverter.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cvx.h"

using std::vector;
using std::list;


class ImageCurveExtractor
{
public:
  ImageCurveExtractor(void);
public:
  ~ImageCurveExtractor(void);

public:
  void SetInput(cv::Mat image);

public:
  bool IsImageSet() { return !imgInput.empty(); }
  bool IsErased() { return erased; }
  bool IsRunning() { return running; }
  bool IsCurveChoosed() { return _curveChoosed; }
  bool IsProcessingImageCompletelyGray() { return cvx::IsCompletelyGray(imgProcess); }

  // Check whether any data point choose for a curve
  void CheckCurve();
  void SwitchChoosedShowerBackground() { _choosedShowerBgBlack = !_choosedShowerBgBlack; CheckCurve(); UpdateShowers(); }
  bool IsChoosedShowerBgBlack() { return _choosedShowerBgBlack;}
  void UpdateShowers();

  // Operation
public:
  void Draw(POINT p, int radius);
  void ResetImage();
  void ResetCurve();
  void Clear();

  POINT choosedPoint;
  void ChooseConnectCurve();

  // Choose Rectangle data points. Inputs are the point in WINDOW, e.g.,in the Client of CView
  bool ChooseRectCurve(POINT topLeft, POINT bottomRight);
  bool ChooseRectCurve(RECT rc);

  // Choose data points by color. Inputs are the point in WINDOW, e.g.,in the Client of CView
  bool ChooseColorCurve(POINT p);

  bool ChoosePoint(POINT p);

  void ChooseAll();	// obsolete
  static cv::Mat Threshold(cv::Mat img);


  bool GetInputSize(int& width, int& height);
  CString GetInputSizeString();

  void Extract();
  bool IsExtracted() { return _extracted; }

  void GetData(vector<double>& x, vector<double>& y) { x = _x; y=_y;}

private:
  bool _extracted;


  // Interface
public:
  cv::Mat GetImgShow() { return imgShow; }
  void SetEraserColor(COLORREF color);

public:
  std::list<cv::Point2d> singlePoints;

public:
  cv::Mat imgProcess;  // always 3-channel
private:
  cv::Mat imgInput;  // always 3-channel
  cv::Mat imgInputLab;

  cv::Mat _matCurve;		// White foreground curve, black: background
  bool _curveChoosed;

  vector<double> _x;
  vector<double> _y;


  cv::Scalar eraserColor;
  bool erased;
  cv::Mat imgShow;

  bool running;

  bool _choosedShowerBgBlack;
public:
  PixelCoordConverter pcCvter;

  cvx::HwndShower shower;
  cvx::HwndShower choosedShower;
};
