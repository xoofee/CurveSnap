// CurveSnapDoc.h : interface of the CCurveSnapDoc class
//


#pragma once
#include "ScreenImage.h"

#include "ImageCurveExtractor.h"

class CCurveSnapDoc : public CDocument
{
protected: // create from serialization only
	CCurveSnapDoc();
	DECLARE_DYNCREATE(CCurveSnapDoc)

// Attributes
public:
   //cv::Mat image;

   ImageCurveExtractor extractor;

// Operations
public:
	CView* GetView(CRuntimeClass* pClass);

// Overrides
public:
	virtual BOOL OnNewDocument();

// Implementation
public:
	virtual ~CCurveSnapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFileSnap();
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
    afx_msg void OnEidtReset();
public:
    afx_msg void OnUpdateEditReset(CCmdUI *pCmdUI);
public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public:
	afx_msg void OnCurveClearselection();
public:
	afx_msg void OnCurveCopydata();
public:
	afx_msg void OnUpdateCurveCopydata(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateCurveClearselection(CCmdUI *pCmdUI);
};


