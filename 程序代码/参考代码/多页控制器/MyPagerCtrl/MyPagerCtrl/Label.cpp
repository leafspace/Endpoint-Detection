#include "StdAfx.h"
#include "Label.h"
#include "UCtrlID.h"

#define TRANS_BACK	-1

CLabel::CLabel(void)
{
	m_pParentWnd = NULL;
	m_strText = "";
	m_TextColor = RGB(225, 112, 5);
	m_BorderColor = RGB(0,100,200);
	m_BackColor = TRANS_BACK;
}

CLabel::~CLabel(void)
{
}

IMPLEMENT_DYNCREATE(CLabel, CStatic)
BEGIN_MESSAGE_MAP(CLabel, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CLabel::Create(const RECT& rect, CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
	static UINT nID = ID_LABEL_CTRL;
	BOOL ret = CStatic::Create(NULL,WS_CHILDWINDOW | WS_VISIBLE | SS_CENTER, rect, pParentWnd, ++nID);
	if(ret)
	{
		SetFont(pParentWnd->GetFont());
	}
	return ret;
}
void CLabel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (!m_pParentWnd)
	{
		return;
	}
	CRect rect;
	GetClientRect(rect);
	CBrush brushFrame(m_BorderColor);
	dc.FrameRect(&rect,&brushFrame);
	dc.SelectObject(GetFont());
	dc.DrawText(m_strText,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
}
HBRUSH CLabel::CtlColor(CDC* pDC, UINT nCtlColor)
{
	m_Brush.DeleteObject();
	if(m_BackColor == TRANS_BACK)
	{
		m_Brush.CreateStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	else
	{
		m_Brush.CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
	}

	SetTextColor(m_TextColor);

	return (HBRUSH)m_Brush;
}

void CLabel::UpdateCtrl()
{ 
	CWnd* pParent = GetParent();
	CRect rect;

	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	rect.DeflateRect(2,2);

	pParent->InvalidateRect(rect,FALSE);
} 
void CLabel::SetBorderColor(COLORREF col)
{
	m_BorderColor = col;
	UpdateCtrl();
	CRect rect;
	GetClientRect(rect);
	InvalidateRect(rect,FALSE);
}
void CLabel::SetTextColor(COLORREF col)
{
	m_TextColor = col;
	UpdateCtrl();
}
void CLabel::SetBackColor(COLORREF col)
{
	m_BackColor = col;
	UpdateCtrl();
}
void CLabel::SetWindowText(LPCSTR text)
{
	m_strText = text;
	CStatic::SetWindowText((LPCTSTR)text);
	CRect rect;
	GetClientRect(rect);
	InvalidateRect(rect,FALSE);
}