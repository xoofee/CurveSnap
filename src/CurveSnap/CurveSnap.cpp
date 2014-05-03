// CurveSnap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CurveSnap.h"
#include "MainFrm.h"

#include "CurveSnapDoc.h"
#include "CurveSnapView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCurveSnapApp

BEGIN_MESSAGE_MAP(CCurveSnapApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCurveSnapApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
    ON_COMMAND(ID_FILE_OPEN, &CCurveSnapApp::OnFileOpen)
END_MESSAGE_MAP()


// CCurveSnapApp construction

CCurveSnapApp::CCurveSnapApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  //VM_LINK();

}


// The one and only CCurveSnapApp object

CCurveSnapApp theApp;


// CCurveSnapApp initialization

BOOL CCurveSnapApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCurveSnapDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCurveSnapView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CCurveSnapApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CCurveSnapApp message handlers


void CCurveSnapApp::OnFileOpen()
{
    // TODO: Add your command handler code here
	ENSURE(m_pDocManager != NULL);
	//m_pDocManager->OnFileOpen();

   // prompt the user (with all document templates)
   CString newName;
   if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
          OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
       return; // open cancelled

   
   if (m_pDocManager->GetOpenDocumentCount() > 0)
   {
       POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();

       CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);

       if (pTemplate)
           pTemplate->OpenDocumentFile(newName);

       //m_pDocManager->OpenDocumentFile(newName);
   }
    //CWinApp::OnFileOpen();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC_AUTHOR); // 得到 Picture Control 句柄  
	//pWnd->ModifyStyle(0, SS_BITMAP); // 修改它的属性为位图  
	//pWnd->SetBitmap((HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_XOOFEE)));  

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
