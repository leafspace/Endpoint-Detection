
/////////////////////////////////////////////////////////////////////////////
// Name:        IMGBUTTON.H
// Purpose:     Í¼Æ¬°´Å¥
// Author:      yfqvip@163.com
// Modified by: 
// Created:     2011/2/16 16:13
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef __IMGBUTTON_H__
#define __IMGBUTTON_H__

class CImgButton : public CButton
{
	DECLARE_DYNAMIC(CImgButton)
public:
	CImgButton();
	virtual ~CImgButton();
	BOOL Create(const RECT& rect, CWnd* pParentWnd);
	void SetBitmaps(int nNormal = 0, int nHot = 0, int nPressed = 0, int nDisable = 0);
protected:
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmpNormal;
	CBitmap m_bmpHot;
	CBitmap m_bmpPressed;
	CBitmap m_bmpDisable;
	BOOL	m_MouseHover;
	CWnd*	m_pParentWnd;
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
};
#endif //__IMGBUTTON_H__