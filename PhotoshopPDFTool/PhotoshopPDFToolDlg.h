// PhotoshopPDFToolDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CPhotoshopPDFToolDlg dialog
class CPhotoshopPDFToolDlg : public CDialog
{
// Construction
public:
	CPhotoshopPDFToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PHOTOSHOPPDFTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    void OnDropFiles(HDROP hDropInfo);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedRun();
    DECLARE_MESSAGE_MAP()
public:
    /** ������ */
    CString m_strActionSetName;
    /** ���� */
    CString m_strAction;
    /** PDF�ļ� */
    CString m_strPDFFile;
    /** ��Ϣ */
    CString m_strInfo;
    /** ��Ϣ�ؼ� */
    CStatic m_InfoCtl;
    /** Photoshop�Ƿ�ɼ� */
    BOOL m_bPhotosopVisible;
    /** Ʒ�� */
    UINT m_Quality;
};
