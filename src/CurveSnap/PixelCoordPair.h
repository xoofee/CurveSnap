#pragma once

class PixelCoordPair
{
public:
    PixelCoordPair(void);
public:
    ~PixelCoordPair(void);

// Operation
public:
    void Clear();

public:
    void SetPixel(double px, double py);
    void GetPixel(double& px, double& py);

    void SetCoord(double x, double y);
    void GetCoord(double& x, double& y);

    double GetX() { return x; }
    double GetY() { return y; }

    double GetPX() { return px; }
    double GetPY() { return py; }

    CString GetCoordString();

public:
    bool IsPixelSet();
    bool IsCoordSet() { return bCoordSet; }

    bool IsSet() { return IsPixelSet() && bCoordSet;}

private:
    double px, py;      // UNIT: pixel
    double x, y;
    bool bCoordSet;
};
