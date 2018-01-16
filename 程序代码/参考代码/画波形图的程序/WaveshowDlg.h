//Download by http://www.NewXing.com
// WaveshowDlg.h : header file
//

#if !defined(AFX_WAVESHOWDLG_H__CBBEA0BD_14CE_41E0_8266_F84F103E3C49__INCLUDED_)
#define AFX_WAVESHOWDLG_H__CBBEA0BD_14CE_41E0_8266_F84F103E3C49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWaveshowDlg dialog

class CWaveshowDlg : public CDialog
{
// Construction
public:
	CWaveshowDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWaveshowDlg)
	enum { IDD = IDD_WAVESHOW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveshowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWaveshowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuit();
	afx_msg void OnShowwave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVESHOWDLG_H__CBBEA0BD_14CE_41E0_8266_F84F103E3C49__INCLUDED_)
