// PhotoshopPDFToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhotoshopPDFTool.h"
#include "PhotoshopPDFToolDlg.h"
#include "CApplication.h"
#include "CDocuments.h"
#include "CPDFOpenOptions.h"
#include "CDocument0.h"
#include "CGIFSaveOptions.h"
#include "CJPEGSaveOptions.h"
#include <string>

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


// CPhotoshopPDFToolDlg dialog




CPhotoshopPDFToolDlg::CPhotoshopPDFToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoshopPDFToolDlg::IDD, pParent)
    , m_strActionSetName(_T("1"))
    , m_strPDFFile(_T("������Ҫ�����PDF�ļ�, ����ļ�����Դ�ļ�Ŀ¼�в���"))
    , m_strInfo(_T("����������ϵQQ:103266509"))
    , m_bPhotosopVisible(FALSE)
    , m_Quality(4)
    , m_strAction(_T("1"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoshopPDFToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ACTION_SET, m_strActionSetName);
    DDX_Text(pDX, IDC_PDF, m_strPDFFile);
    DDX_Text(pDX, IDC_INFO, m_strInfo);
    DDX_Control(pDX, IDC_INFO, m_InfoCtl);
    DDX_Check(pDX, IDC_VISIBLE, m_bPhotosopVisible);
    DDX_Text(pDX, IDC_QUALITY, m_Quality);
    DDV_MinMaxUInt(pDX, m_Quality, 1, 12);
    DDX_Text(pDX, IDC_ACTION, m_strAction);
}

BEGIN_MESSAGE_MAP(CPhotoshopPDFToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RUN, &CPhotoshopPDFToolDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// CPhotoshopPDFToolDlg message handlers

BOOL CPhotoshopPDFToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPhotoshopPDFToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPhotoshopPDFToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CPhotoshopPDFToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static std::wstring GetFileName(const std::wstring& pdfFile)
{
    size_t pos= pdfFile.rfind('.');
    if (pos== std::wstring::npos)
    {
        return pdfFile;
    }

    return pdfFile.substr(0, pos);
}

bool CheckFolderExist(const std::wstring& strPath)
{
    WIN32_FIND_DATA wfd;
    bool rValue = false;
    HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        rValue = true; 
    }
    FindClose(hFind);
    return rValue;
}

/** ����Photoshop�ļ�
* @param [in] szInputPdf �����PDF�ļ�
* @param [in] szActionSetName ������
* @param [in] szActionName ������
* @param [in] hWnd ��ʾ���ȵľ��
* @param [in] bPsVisible Photoshop�Ƿ�ɼ�, ����ɼ�, ���û�������ʱ���
* @param [in] quality ����Jpeg��Ʒ��, 1~12 ֵԽ��Ʒ��Խ��
*/
static unsigned int ProcessPdfFile(const WCHAR* szInputPdf, 
   const WCHAR* szActionSetName, const WCHAR* szActionName,
   HWND hWnd, bool bPsVisible, UINT quality)
{
    std::wstring DirName= GetFileName(szInputPdf);

    if (!CheckFolderExist(DirName))
    {
        if (!CreateDirectory(DirName.c_str(), NULL))
        {
            return 0xFFFFFFFF;
        }
    }

    CApplication* pPhotoshopApplication= NULL;
    unsigned int iPage= 1;

    try
    {
        const WCHAR* szPsAppID = L"Photoshop.Application";
        const WCHAR* szPsPdfID = L"Photoshop.PDFOpenOptions";

        /** ����Photoshop App���� */
        pPhotoshopApplication = new CApplication;
        pPhotoshopApplication->CreateDispatch(szPsAppID);
        pPhotoshopApplication->put_Visible(bPsVisible);

        do
        {
            /** ���� "PDF��ѡ��" ���� */
            CPDFOpenOptions* PdfOpenOpt = new CPDFOpenOptions();
            PdfOpenOpt->CreateDispatch(szPsPdfID);
            PdfOpenOpt->put_Page(iPage);

            VARIANT OpenOptions;
            OpenOptions.vt= VT_DISPATCH;
            OpenOptions.pdispVal= PdfOpenOpt->m_lpDispatch;

            try
            {
                /** ���Դ�PDF�ļ� */
                pPhotoshopApplication->Open(szInputPdf, OpenOptions);           
            }
            catch (...)
            {
                delete PdfOpenOpt;
                break;
            }

            delete PdfOpenOpt;

            /** ִ��ָ���Ķ��� */
            pPhotoshopApplication->DoAction(szActionSetName, szActionName);

            /** ����Jpeg����ѡ�� ���� */
            CJPEGSaveOptions JpegSaveOpt;
            JpegSaveOpt.CreateDispatch(L"Photoshop.JPEGSaveOptions");
            JpegSaveOpt.put_Quality(quality);

            VARIANT SaveOptions, SaveAsCopy, ExtentType;
            SaveOptions.vt = VT_DISPATCH;
            SaveOptions.pdispVal = JpegSaveOpt.m_lpDispatch;
            SaveAsCopy.vt = VT_BOOL;
            SaveAsCopy.boolVal = false;
            ExtentType.vt = VT_EMPTY;

            WCHAR szOutputFilename[MAX_PATH];
            swprintf_s(szOutputFilename, MAX_PATH, L"%s\\%04d.jpeg", 
                DirName.c_str(), iPage);

            LPDISPATCH docItem= pPhotoshopApplication->get_ActiveDocument();
            CDocument0 SingleDocument(docItem);
            /** ִ��������� */
            SingleDocument.SaveAs(szOutputFilename, SaveOptions, SaveAsCopy, ExtentType);

            VARIANT CloseOptions;
            CloseOptions.vt= VT_EMPTY;
            SingleDocument.Close(CloseOptions);

            WCHAR buff[256];
            swprintf_s(buff, L"��ǰ����%dҳ", iPage);
            ::SetWindowText(hWnd, buff);
        }
        while(iPage++);
    }
    catch(...)
    {
        return iPage= 0xFFFFFFFF;
    }

    /** �˳�Photoshop */
    pPhotoshopApplication->Quit();
    delete pPhotoshopApplication;

    return iPage;
}

void CPhotoshopPDFToolDlg::OnBnClickedRun()
{
    UpdateData(TRUE);

    if (m_strActionSetName.IsEmpty() || m_strPDFFile.IsEmpty() || m_strAction.IsEmpty())
    {
        MessageBox(L"�����������Ĳ���!");
        return;
    }

    FILE* fp= NULL;
    _wfopen_s(&fp, m_strPDFFile, L"rb");
    if (!fp)
    {
        MessageBox(L"�����ļ�������!");
        return;
    }

    m_strInfo= L"��ʼ�����ļ�:"+ m_strPDFFile;
    UpdateData(FALSE);

    unsigned int iPage= ProcessPdfFile(m_strPDFFile, m_strActionSetName, 
        m_strActionSetName, m_InfoCtl.GetSafeHwnd(), m_bPhotosopVisible!= 0,
        m_Quality);

    WCHAR buff[256];
    if (iPage != 0xFFFFFFFF)
    {
        swprintf_s(buff, L"������%dҳ", iPage);
    }
    else
    {
        wcscpy_s(buff, L"����Photoshop���û���ֹ,���߷����˴���,������ֹͣ.");
    }

    m_strInfo= buff;
    UpdateData(FALSE);

    MessageBox(buff);
}

void CPhotoshopPDFToolDlg::OnDropFiles( HDROP hDropInfo )
{
    UINT nFileNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    if (nFileNum!= 1)
    {
        return;
    }

    WCHAR wcStr[MAX_PATH];
    DragQueryFile(hDropInfo, 0, wcStr,MAX_PATH);
    DragFinish(hDropInfo);

    m_strPDFFile= wcStr;

    UpdateData(FALSE);

    CDialog::OnDropFiles(hDropInfo);
}
