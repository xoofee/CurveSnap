#pragma once

#include "PixelCoordPair.h"

class PixelCoordConverter
{
public:
    PixelCoordConverter(void);
public:
    ~PixelCoordConverter(void);

public:
    bool IsCalibrated();

// Opeartion
public:
    void Clear();


    // check the position relationship
    bool IsPointsProper() {return bPointsProper; }

    void CheckPointsProper();

// Conversions
    void CalcCvtCoef();
    void Pixel2Coord(double px, double py, double& x, double& y);
    CString Pixel2CoordString(double px, double py);


// Interface for PixelCoordPair
public:
    void SetPixel(double px, double py, int index);
    void GetPixel(double& px, double& py, int index);

    void SetCoord(double x, double y, int index);
    void GetCoord(double& x, double& y, int index);

    double GetPX(int index);
    double GetPY(int index);

    CString GetCoordString(int index);

    bool IsBothPixelCoordsSet() { return pixelCoord[0].IsSet() && pixelCoord[1].IsSet(); }
    bool IsPixelCoordsSet(int index) { return pixelCoord[index].IsSet(); }

    bool IsCoordsSet() { return pixelCoord[0].IsCoordSet() && pixelCoord[1].IsCoordSet(); }
    bool IsPixelsSet() { return pixelCoord[0].IsPixelSet() && pixelCoord[1].IsPixelSet(); }

// Attributes
private:
    PixelCoordPair pixelCoord[2];
    bool bPointsProper;

    // Convertion Coefficients
    // Xreal = ax*Xpixel + bx
    // the pixels are of the original snapped image, not the showing zoomed image.
    double ax, bx;
    double ay, by;

public:
    double maxRatio;
};
