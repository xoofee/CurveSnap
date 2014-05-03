#include "StdAfx.h"
#include "PixelCoordPair.h"

PixelCoordPair::PixelCoordPair(void)
{
    Clear();
}

void PixelCoordPair::Clear()
{
    px = -1;
    py = -1;
    x = 0.0;
    y = 0.0;
    bCoordSet = false;
}

PixelCoordPair::~PixelCoordPair(void)
{
}

void PixelCoordPair::SetPixel(double px, double py)
{
    this->px = px;
    this->py = py;
}

void PixelCoordPair::GetPixel(double& px, double& py)
{
    px = this->px;
    py = this->py;
}

void PixelCoordPair::SetCoord(double x, double y)
{
    this->x = x;
    this->y = y;
    bCoordSet = true;
}

void PixelCoordPair::GetCoord(double& x, double& y)
{
    x = this->x;
    y = this->y;
}

bool PixelCoordPair::IsPixelSet()
{
    return px>=0 && py>=0;
}

CString PixelCoordPair::GetCoordString()
{
    CString strx, stry;

    strx.Format(_T("%f"),x);
    if (strx.Find('.') != NULL)
        strx.TrimRight('0');

    strx.TrimRight('.');

    stry.Format(_T("%f"),y);
    if (stry.Find('.') != NULL)
        stry.TrimRight('0');
    
    stry.TrimRight('.');

    return "(" + strx + ", " + stry + ")";
}
