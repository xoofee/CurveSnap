// DialogCoord.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "DialogCoord.h"


// CDialogCoord dialog

IMPLEMENT_DYNAMIC(CDialogCoord, CDialog)

CDialogCoord::CDialogCoord(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCoord::IDD, pParent)
    , x(0)
    , y(0)
{
  
}

CDialogCoord::~CDialogCoord()
{
}

void CDialogCoord::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_X, x);
  DDX_Text(pDX, IDC_EDIT_Y, y);
  DDX_Control(pDX, IDC_EDIT_X, edt_x_);
  DDX_Control(pDX, IDC_EDIT_Y, edt_y_);
}


BEGIN_MESSAGE_MAP(CDialogCoord, CDialog)
  ON_BN_CLICKED(IDC_BTN_UP, &CDialogCoord::OnBnClickedBtnUp)
  ON_WM_PAINT()
  ON_BN_CLICKED(IDC_BTN_LEFT, &CDialogCoord::OnBnClickedBtnLeft)
  ON_BN_CLICKED(IDC_BTN_RIGHT, &CDialogCoord::OnBnClickedBtnRight)
  ON_BN_CLICKED(IDC_BTN_DOWN, &CDialogCoord::OnBnClickedBtnDown)
END_MESSAGE_MAP()


// CDialogCoord message handlers

BOOL CDialogCoord::OnInitDialog()
{
  CDialog::OnInitDialog();

  UpdateWnd();

  // TODO:  Add extra initialization here

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogCoord::UpdateWnd() {
  imgwnd_ = imgpad_(cv::Range(py_, py_+2*kWndHalfSize+1),
                    cv::Range(px_, px_+2*kWndHalfSize+1));
  cv::Mat mag;
  int halfzoomfactor = 5;
  int zoomfactor = halfzoomfactor*2+1;
  cv::resize(imgwnd_, mag, cv::Size(0, 0), zoomfactor, zoomfactor,cv::INTER_NEAREST);
  mag.col(kWndHalfSize*zoomfactor+halfzoomfactor) = cv::Scalar(0, 0, 255);
  mag.row(kWndHalfSize*zoomfactor+halfzoomfactor) = cv::Scalar(0, 0, 255);

  shower.Set(mag);

  //Invalidate(FALSE);  // also ok

	CRect rcpic;
	GetDlgItem(IDC_PIC)->GetWindowRect(rcpic);
	ScreenToClient(rcpic);
  InvalidateRect(rcpic, FALSE);
}

void CDialogCoord::SetImage(const cv::Mat& img) {
  imginput_ = img;
  cv::copyMakeBorder(imginput_, imgpad_, 
                      kWndHalfSize, kWndHalfSize, kWndHalfSize, kWndHalfSize,
                      cv::BORDER_CONSTANT, cv::Scalar::all(128));
}
void CDialogCoord::OnPaint()
{
  CPaintDC dc(this); // device context for painting
  // TODO: Add your message handler code here
  // Do not call CDialog::OnPaint() for painting messages
  CWnd* picwnd = GetDlgItem(IDC_PIC);

  //shower.Show(picwnd->GetSafeHwnd(), cvx::SW_CENTER);
  shower.Show(picwnd->GetSafeHwnd(), cvx::SW_FIT);
}

void CDialogCoord::OnBnClickedBtnUp()
{
  --py_;
  py_ = std::max(py_, 0);

  UpdateWnd();
}

void CDialogCoord::OnBnClickedBtnDown()
{
  ++py_;
  py_ = std::min(py_, imginput_.rows-1);

  UpdateWnd();
}

void CDialogCoord::OnBnClickedBtnLeft()
{
  --px_;
  px_ = std::max(px_, 0);

  UpdateWnd();
}

void CDialogCoord::OnBnClickedBtnRight()
{
  ++px_;
  px_ = std::min(px_, imginput_.cols-1);

  UpdateWnd();
}
