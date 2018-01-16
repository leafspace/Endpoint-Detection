#include "StdAfx.h"
#include "ImgButton.h"

IMPLEMENT_DYNAMIC(CImgButton, CButton)

CImgButton::CImgButton()
: m_MouseHover(FALSE)
, m_pParentWnd(NULL)
{
}

CImgButton::~CImgButton()
{
}

BEGIN_MESSAGE_MAP(CImgButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

BOOL CImgButton::Create(const RECT& rect, CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
	static UINT nID = WM_USER + 200;
	BOOL ret = CButton::Create(NULL,WS_CHILDWINDOW | WS_VISIBLE | SS_CENTER |WS_VISIBLE | BS_PUSHBUTTON /*|WS_BORDER*/, rect, pParentWnd, ++nID);
	if(ret)
	{
		SetFont(pParentWnd->GetFont());
	}
	return ret;
}

void CImgButton::SetBitmaps(int nNormal, int nHot, int nPressed, int nDisable)
{
	if (nNormal)
	{
		m_bmpNormal.LoadBitmap(nNormal);
	}
	if (nHot)
	{
		m_bmpHot.LoadBitmap(nHot);
	}
	if (nPressed)
	{
		m_bmpPressed.LoadBitmap(nPressed);
	}
	if (nDisable)
	{
		m_bmpDisable.LoadBitmap(nDisable);
	}
}

void CImgButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

void CImgButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HBITMAP bitmap = (HBITMAP)m_bmpNormal;
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		bitmap = (HBITMAP)m_bmpDisable;
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		bitmap = (HBITMAP)m_bmpPressed;
	}
	else if (m_MouseHover)
	{
		bitmap = (HBITMAP)m_bmpHot;
	}

	RECT rc;
	GetClientRect(&rc);
	HDC dc = CreateCompatibleDC(lpDrawItemStruct->hDC);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(dc,bitmap);
	BitBlt(lpDrawItemStruct->hDC, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, dc, 0, 0, SRCCOPY);
	SelectObject(dc,oldBitmap);
	DeleteDC(dc);

	CString strText;
	GetWindowText(strText);
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
	DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CImgButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_MouseHover)
	{
		Invalidate();
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
		m_MouseHover = TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CImgButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	m_MouseHover = FALSE;
	return 0;
}