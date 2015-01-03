// set-led-guiDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CsetledguiDlg dialog
class CsetledguiDlg : public CDialog
{
// Construction
public:
	CsetledguiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SETLEDGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
    afx_msg void OnBnClickedCheck1();
    CButton ledOn;
};
