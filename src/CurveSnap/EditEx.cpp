// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "EditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
: first_click_in_(false)
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CEditEx message handlers



void CEditEx::OnLButtonUp(UINT nFlags, CPoint point)
{
  // Note that when Left Button Up, the control is CERTAINLY on focus

  if(first_click_in_)
  {
    CEdit::OnLButtonUp(nFlags, point);
    SetSel(0,-1);
  }else
  {
    CEdit::OnLButtonUp(nFlags, point);
  }	
}


void CEditEx::OnLButtonDown(UINT nFlags, CPoint point)
{
  first_click_in_ = GetFocus() != this;

  CEdit::OnLButtonDown(nFlags, point);
}

void CEditEx::OnMouseMove(UINT nFlags, CPoint point)
{
  if (first_click_in_)
    first_click_in_ = false;

  CEdit::OnMouseMove(nFlags, point);
}
