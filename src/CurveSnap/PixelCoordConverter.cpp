#include "StdAfx.h"
#include "PixelCoordConverter.h"

#include <math.h>

PixelCoordConverter::PixelCoordConverter(void)
{
    maxRatio = 50;
    Clear();
}

void PixelCoordConverter::Clear()
{
    bPointsProper = false;

    ax = 0.0;
    bx = 0.0;
    ay = 0.0;
    by = 0.0;

    pixelCoord[0].Clear();
    pixelCoord[1].Clear();
}

PixelCoordConverter::~PixelCoordConverter(void)
{
}

bool PixelCoordConverter::IsCalibrated()
{
    return IsCoordsSet() && IsPointsProper();
}

// If both points set, check if the position relationship ok
void PixelCoordConverter::CheckPointsProper()
{
    if (!IsPixelsSet())
    {
        bPointsProper = false;
        return;
    }

    double dx = abs(pixelCoord[0].GetPX() - pixelCoord[1].GetPX());
    double dy = abs(pixelCoord[0].GetPY() - pixelCoord[1].GetPY());

    if (dx<1 || dy<1)
    {
        bPointsProper = false;
        return;
    }

    double r =  dy/dx;

    //afxDump << r << "\n";

    if (r>maxRatio || r < 1/maxRatio)
    {
        bPointsProper = false;
        return;
    }

    bPointsProper = true;
}

void PixelCoordConverter::CalcCvtCoef()
{
    if (!IsCalibrated())
        return;

    ax = (pixelCoord[1].GetX() - pixelCoord[0].GetX())/(pixelCoord[1].GetPX() - pixelCoord[0].GetPX());
    bx = pixelCoord[0].GetX() - ax * pixelCoord[0].GetPX();

    ay = (pixelCoord[1].GetY() - pixelCoord[0].GetY())/(pixelCoord[1].GetPY() - pixelCoord[0].GetPY());
    by = pixelCoord[0].GetY() - ay * pixelCoord[0].GetPY();
}

void PixelCoordConverter::Pixel2Coord(double px, double py, double& x, double& y)
{
    x = ax*px + bx;
    y = ay*py + by;
}

CString PixelCoordConverter::Pixel2CoordString(double px, double py)
{
    double x, y;
    Pixel2Coord(px, py, x, y);

    CString str;

    str.Format(_T("%f, %f"),x, y);

    return str;
}


// Interface
void PixelCoordConverter::SetPixel(double px, double py, int index)
{
    pixelCoord[index].SetPixel(px, py);

    CheckPointsProper();

    if (IsCalibrated())
        CalcCvtCoef();
}

void PixelCoordConverter::GetPixel(double& px, double& py, int index)
{
    pixelCoord[index].GetPixel(px, py);
}

void PixelCoordConverter::SetCoord(double x, double y, int index)
{
    pixelCoord[index].SetCoord(x, y);

    if (IsCalibrated())
        CalcCvtCoef();
}

void PixelCoordConverter::GetCoord(double& x, double& y, int index)
{
    pixelCoord[index].GetCoord(x, y);
}

double PixelCoordConverter::GetPX(int index)
{
    return pixelCoord[index].GetPX();
}

double PixelCoordConverter::GetPY(int index)
{
    return pixelCoord[index].GetPY();
}

CString PixelCoordConverter::GetCoordString(int index)
{
    return pixelCoord[index].GetCoordString();
}

