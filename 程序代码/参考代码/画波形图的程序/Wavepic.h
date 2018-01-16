//Download by http://www.NewXing.com
#if !defined(AFX_WAVEPIC_H__079C66BA_6774_4C8C_A9E1_87BD942E9709__INCLUDED_)
#define AFX_WAVEPIC_H__079C66BA_6774_4C8C_A9E1_87BD942E9709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wavepic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Wavepic dialog

class Wavepic : public CDialog
{
// Construction
public:
	void DrawWave(CDC *pDC);
	Wavepic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Wavepic)
	enum { IDD = IDD_WAVEPIC };
	CSliderCtrl	m_shuiping;
	CSliderCtrl	m_chuizhi;
	int		m_levelsel;
	int		m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Wavepic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Wavepic)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSteady();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_cycctl;
	int m_count;
	int iTemp;
	int m_now;
	float m_lCount[1024];
	int m_Low;
	int m_High;
	CDC memDC;
	CDC* pDC;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEPIC_H__079C66BA_6774_4C8C_A9E1_87BD942E9709__INCLUDED_)
