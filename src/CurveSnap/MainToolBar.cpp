// MainToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "MainToolBar.h"

#include "CurveSnapDoc.h"


// CMainToolBar

IMPLEMENT_DYNAMIC(CMainToolBar, CToolBar)

CMainToolBar::CMainToolBar()
{

}

CMainToolBar::~CMainToolBar()
{
}


BEGIN_MESSAGE_MAP(CMainToolBar, CToolBar)
	ON_MESSAGE(CPN_SELCHANGE, OnColorChange)
END_MESSAGE_MAP()



// CMainToolBar message handlers

LRESULT CMainToolBar::OnColorChange(WPARAM, LPARAM)
{
    ((CCurveSnapDoc*)GetParentFrame()->GetActiveDocument())->extractor.SetEraserColor(colorPicker.GetColor());
    
    return 1;
}


