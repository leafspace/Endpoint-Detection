
/////////////////////////////////////////////////////////////////////////////
// Name:        LABEL.H
// Purpose:     ±êÇ©¿Ø¼þ
// Author:      yfqvip@163.com
// Modified by: 
// Created:     2010/12/23 14:27
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef __LABEL_H__
#define __LABEL_H__

class CLabel : public CStatic
{
	DECLARE_DYNCREATE(CLabel)
public:
	CLabel(void);
	~CLabel(void);
	BOOL Create(const RECT& rect, CWnd* pParentWnd);
private: 
	COLORREF	m_BorderColor; 
	COLORREF	m_TextColor; 
	COLORREF	m_BackColor; 
	CBrush		m_Brush; 

	//   Operations 
public:
	void SetBorderColor(COLORREF col);
	void SetTextColor(COLORREF col);
	void SetBackColor(COLORREF col);
	void SetWindowText(LPCSTR text);
	void UpdateCtrl();
private:
	CString m_strText;
	CWnd*	m_pParentWnd;
public:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

};

#endif //__LABEL_H__
