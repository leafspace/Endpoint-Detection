#include "StdAfx.h"
#include "PagerCtrlEx.h"
#include "UCtrlID.h"
#include <assert.h>

#define PAGER_WIDTH			170
#define PAGER_HEIGHT		20
#define BUTTON_WIDTH		20
#define GAP_WIDTH			0			/*< 按钮之前的间隔 */
#define LINE_WIDTH_HEIGHT	1			/*< 边线宽高 */

#pragma warning(disable:4996)
CPagerCtrlEx::CPagerCtrlEx(void)
{
	m_pParentWnd = NULL;
	m_pNotify = NULL;
	m_nPageTotal = 0;
	m_nCurrentIndex = 0;
	m_BorderColor = RGB(41,101,153);
}

CPagerCtrlEx::~CPagerCtrlEx(void)
{
}

IMPLEMENT_DYNCREATE(CPagerCtrlEx, CWnd)
BEGIN_MESSAGE_MAP(CPagerCtrlEx, CWnd)
	//{{AFX_MSG_MAP(COutlookMenu)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()
BOOL CPagerCtrlEx::Create(const CPoint& pt, CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;

	static UINT nID = ID_PAGER_CTRL;

	LPCTSTR lpszClassName =AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW , AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH)GetStockObject(WHITE_BRUSH), NULL);
	BOOL ret = CWnd::Create(lpszClassName, NULL, WS_CHILDWINDOW | WS_VISIBLE, CRect(pt.x,pt.y,pt.x+PAGER_WIDTH+LINE_WIDTH_HEIGHT*2,pt.y+PAGER_HEIGHT+LINE_WIDTH_HEIGHT*2), pParentWnd, ++nID);
	if(ret)
	{
		SetFont(pParentWnd->GetFont());
	}

	CRect rcFirst(LINE_WIDTH_HEIGHT,LINE_WIDTH_HEIGHT,BUTTON_WIDTH+LINE_WIDTH_HEIGHT,PAGER_HEIGHT+LINE_WIDTH_HEIGHT);
	CRect rcLast(PAGER_WIDTH-BUTTON_WIDTH*1-LINE_WIDTH_HEIGHT,LINE_WIDTH_HEIGHT,PAGER_WIDTH-LINE_WIDTH_HEIGHT,PAGER_HEIGHT+LINE_WIDTH_HEIGHT);
	CRect rcPrev(rcFirst.right,rcFirst.top,rcFirst.right+rcFirst.Width(),rcFirst.bottom);
	CRect rcNext(rcLast.left-rcLast.Width(),rcLast.top,rcLast.left,rcLast.bottom);
	CRect rcPanel(rcPrev.right,rcPrev.top,rcPrev.right+40,rcPrev.bottom);
	CRect rcGoto(1,3,rcPanel.Width()-1,rcPanel.Height()-1);
	CRect rcText(rcPanel.right,rcPanel.top,rcNext.left,rcPanel.bottom);

	m_btnFirst.Create(rcFirst, this);
	m_btnPrev.Create(rcPrev, this);
	m_btnNext.Create(rcNext, this);
	m_btnLast.Create(rcLast, this);
	m_lblPanel.Create(rcPanel,this);
	m_txtGoto.Create(WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|ES_NUMBER,rcGoto, &m_lblPanel, ++nID);
	m_lblText.Create(rcText,this);

	m_lblPanel.SetBorderColor(RGB(193,225,253));
	m_lblText.SetBorderColor(RGB(248,248,248));
	m_txtGoto.SetFont(pParentWnd->GetFont());
	m_lblText.SetFont(pParentWnd->GetFont());

	return ret;
}
void CPagerCtrlEx::SetNotifyManager(CPagerNotify *pNotify)
{
	m_pNotify = pNotify;
}
void CPagerCtrlEx::SetProperty(int nPageTotal,int nCurrentIndex)
{
	if (nPageTotal<=0 || nCurrentIndex<=0)
	{
		nPageTotal = 1;
		nCurrentIndex = 1;
	}
	m_nPageTotal = nPageTotal - 1;
	m_nCurrentIndex = nCurrentIndex - 1;
	SetPageText();
}
void CPagerCtrlEx::SetBitmaps(ButtonItem eItem,int nNormal, int nHot, int nPressed, int nDisable)
{
	switch (eItem)
	{
	case ITEM_FIRST:
		m_btnFirst.SetBitmaps(nNormal, nHot, nPressed, nDisable);
		break;
	case ITEM_PREV:
		m_btnPrev.SetBitmaps(nNormal, nHot, nPressed, nDisable);
		break;
	case ITEM_NEXT:
		m_btnNext.SetBitmaps(nNormal, nHot, nPressed, nDisable);
		break;
	case ITEM_LAST:
		m_btnLast.SetBitmaps(nNormal, nHot, nPressed, nDisable);
		break;
	}

}
void CPagerCtrlEx::SetBorderColor(COLORREF col)
{
	m_BorderColor = col;
}
void CPagerCtrlEx::SetPageText()
{
	char strText[20] = {0};
	sprintf(strText,"%d/%d",m_nCurrentIndex+1,m_nPageTotal+1);
	m_lblText.SetWindowText(strText);
	memset(strText,0,20);
	sprintf(strText,"%d",m_nCurrentIndex+1);
	
	int num = MultiByteToWideChar(0, 0, strText, -1, NULL, 0);
	wchar_t *temp = new wchar_t[num];
	MultiByteToWideChar(0, 0, strText, -1, temp, num);

	m_txtGoto.SetWindowText(temp);
	// 刷新控件
	CRect rect;
	m_lblText.GetClientRect(&rect);
	m_lblText.MapWindowPoints(this,rect);
	InvalidateRect(&rect);
}
void CPagerCtrlEx::GotoPager(int nPage)
{
	if (m_nCurrentIndex != nPage-1)
	{
		if (nPage-1 < 0)
		{
			m_nCurrentIndex = 0;
		}
		else if (nPage-1 > m_nPageTotal)
		{
			m_nCurrentIndex = m_nPageTotal;
		}
		else
		{
			m_nCurrentIndex = nPage-1;
		}
		m_pNotify->OnGotoPager(m_nCurrentIndex);
		SetPageText();
	}
}
BOOL CPagerCtrlEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	assert(m_pNotify);
	int nCtrlID = LOWORD(wParam);
	if (nCtrlID == m_btnFirst.GetDlgCtrlID())
	{
		if (m_nCurrentIndex > 0)
		{
			m_nCurrentIndex = 0;
			m_pNotify->OnGotoPager(m_nCurrentIndex);
			SetPageText();
		}
	}
	if (nCtrlID == m_btnPrev.GetDlgCtrlID())
	{
		if (m_nCurrentIndex > 0)
		{
			m_pNotify->OnGotoPager(--m_nCurrentIndex);
			SetPageText();
		}
	}
	if (nCtrlID == m_btnNext.GetDlgCtrlID())
	{
		if (m_nCurrentIndex < m_nPageTotal)
		{
			m_pNotify->OnGotoPager(++m_nCurrentIndex);
			SetPageText();
		}
	}
	if (nCtrlID == m_btnLast.GetDlgCtrlID())
	{
		if (m_nCurrentIndex < m_nPageTotal)
		{
			m_nCurrentIndex = m_nPageTotal;
			m_pNotify->OnGotoPager(m_nCurrentIndex);
			SetPageText();
		}
	}
	return CWnd::OnCommand(wParam, lParam);
}
void CPagerCtrlEx::OnPaint()
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
}
BOOL CPagerCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (!(GetKeyState(VK_CONTROL) & 0x8000))
			{
				if (m_txtGoto.GetSafeHwnd() == pMsg->hwnd)
				{
					assert(m_pNotify);
					CString strText = (CString)"";
					m_txtGoto.GetWindowText(strText);
					if (!strText.IsEmpty())
					{
						GotoPager(atoi((LPSTR)(LPCTSTR)strText));
					}
					return TRUE; //滤掉Enter
				}
			}
		}
		if (pMsg->wParam == 0x56 )
		{
			if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				if (m_txtGoto.GetSafeHwnd() == pMsg->hwnd)
				{
					return TRUE; //滤掉ctrl+v
				}
			}
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}
#pragma warning(default:4996)
