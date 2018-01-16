//Download by http://www.NewXing.com
// Wavepic.cpp : implementation file
//

#include "stdafx.h"
#include "Waveshow.h"
#include "Wavepic.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define pi 3.14159

/////////////////////////////////////////////////////////////////////////////
// Wavepic dialog


Wavepic::Wavepic(CWnd* pParent /*=NULL*/)
	: CDialog(Wavepic::IDD, pParent)
{
	//{{AFX_DATA_INIT(Wavepic)
	m_levelsel = 0;
	m_time = 0;
	m_count=0;
	m_cycctl=1;
	//}}AFX_DATA_INIT
}


void Wavepic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Wavepic)
	DDX_Control(pDX, IDC_SLI_HEX, m_shuiping);
	DDX_Control(pDX, IDC_SLI_POS, m_chuizhi);
	DDX_Radio(pDX, IDC_LEVEL1, m_levelsel);
	DDX_Radio(pDX, IDC_RADIO5, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Wavepic, CDialog)
	//{{AFX_MSG_MAP(Wavepic)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STEADY, OnBtnSteady)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Wavepic message handlers

void Wavepic::DrawWave(CDC *pDC)
{	
	UpdateData(TRUE);

    CRect rect;
	CString str;
	int i;

    //对不同选择档进行赋值
	float levelsel,time;
    
	switch (m_levelsel)
	{
	case 0:
		levelsel=4;
		break;
	case 1:
		levelsel=2;
		break;
	case 2:
		levelsel=1;
		break;
	case 3:
		levelsel=0.5;
		break;
	default:
		break;
	}


	switch (m_time)
	{
	case 0:
		time=0.5;
		break;
	case 1:
		time=1;
		break;
	case 2:
		time=2;
		break;
	case 3:
		time=4;
		break;
	default:
		break;
	}

    //滚动滑块位置取值
	int m_sliderctrl,m_tochange;
    m_sliderctrl=m_chuizhi.GetPos();
    m_tochange=(m_sliderctrl-50);
	
    int m_slishui;
    m_slishui=m_shuiping.GetPos()*3;

   
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	pWnd->GetClientRect(&rect);  
	pDC->Rectangle(&rect);

	//坐标轴位置
    int m_left,m_top,m_right,m_bottom;
    m_left = rect.left+20;
	m_top = rect.top+10;
	m_right = rect.right-20;
	m_bottom = rect.bottom-20;

	iTemp = (m_bottom - m_top)/10;

	// 创建红色画笔对象
	CPen* pPenRed = new CPen;
		
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// 创建蓝色画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// 创建黑色画笔对象
	CPen* pPenblack = new CPen;
	
	// 黑色画笔
	pPenblack->CreatePen(PS_DOT,1,RGB(0,0,0));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	
	// 绘制坐标轴
	pDC->MoveTo(m_left,m_bottom-10*iTemp);
	
	// 垂直轴
	//pDC->LineTo(m_left,m_bottom);
	//pDC->LineTo(-10, 500);
	
	// 水平轴
	pDC->LineTo(m_left+440,m_bottom);
	
	// 写X轴刻度值

	/*for(i=0;i<10;i++)
	
	{
		//str.Format(_T("%d"),m_Min+i*m_Interval);
		str.Format(_T("%d"),m_Low+i*m_Interval);
		pDC->TextOut(m_left+i*m_IntervalPan,m_bottom+3,str);		
	}
	//str.Format(_T("%d"),m_Max);
	str.Format(_T("%d"),m_High);
	pDC->TextOut(m_left+10*m_IntervalPan,m_bottom+3,str);*/		
	
	

	/*for (i = m_left; i < m_right-20; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, m_bottom);
			pDC->LineTo(i + 10, m_bottom+4);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, m_bottom);
			pDC->LineTo(i + 10, m_bottom+2);
		}
	}*/
	
	// 绘制x轴箭头
	//pDC->MoveTo(m_right-3,m_bottom-3);
	//pDC->LineTo(m_right,m_bottom);
	//pDC->LineTo(m_right-3,m_bottom+3);
	
	// 绘制y轴箭头	
	//pDC->MoveTo(m_left-3,m_top+3);
	//pDC->LineTo(m_left,m_top);
	//pDC->LineTo(m_left+3,m_top+3);
	

	
	//选择黑色画笔
    //pDC->SelectObject(pPenblack);	

	//绘制网格
    
     
	// 绘制X轴刻度
	CString s;
	s.Format("%d",0);
	pDC->TextOut(m_left,m_bottom+2,s);

    for (i =20; i < m_right-20; i+=20)
	{   

		pDC->MoveTo(m_left+i,m_bottom);
		pDC->LineTo(m_left+i,m_bottom-10*iTemp);
		CString s;
		s.Format("%d",i/20);
		pDC->TextOut(m_left+i,m_bottom+2,s);
	}
    
    //绘制Y轴刻度
    for (i = 1 ;i <= 10 ;i++)
	{
		pDC->MoveTo(m_left,m_bottom - i*iTemp);
		pDC->LineTo(m_left+440,m_bottom - i*iTemp);
		CString s;
		s.Format("%d",i);
		pDC->TextOut(m_left-17,m_bottom - i*iTemp,s);
	}


	//判断是否为周期信号与平稳信号
	if (1==m_count%2)
	{
        m_cycctl=0;
        SetDlgItemText(IDC_BTN_STEADY,"滚动波形");
	}
	else
	{
		m_cycctl=1;
		SetDlgItemText(IDC_BTN_STEADY,"平稳波形");
	}


    //周期循环
	if (1==m_cycctl&&m_now<1024)
	{
	    m_now++;
	}
	if(m_now>=1024)
	{
        m_now=0;
	}
    
	//模拟正弦波信号
	for (i = m_Low; i<m_High; i++)
	{ 
		m_lCount[i] = sin((pi/40)*i+m_now);
	}


	// 更改成蓝色画笔绘图
	pDC->SelectObject(pPenBlue);

	float xTemp;
	float yTemp;
	float xSave;
	float ySave;
    for (i = m_Low; i <m_High; i++)
	{
		xTemp = (m_left+(float)i/time+m_slishui);
		yTemp = (m_bottom - 5*iTemp+m_tochange)-m_lCount[i]*iTemp*levelsel;
		if (i == 1)
		{
			xSave=xTemp;
		    ySave=yTemp;
		}/**/
		if (yTemp <= (m_bottom-10*iTemp)) 
			yTemp = m_bottom-10*iTemp;
		if (yTemp > m_bottom)
            yTemp = m_bottom;
        if ((xTemp >= (m_left+0.0001+m_slishui)) && (xTemp <= (m_left+440)))
		{
			pDC->MoveTo(xSave,ySave );
			pDC->LineTo(xTemp, yTemp);
			xSave=xTemp;
		    ySave=yTemp;
		}
	}


	pDC->SelectObject(pOldPen);	
	
	
	delete pPenRed;
	delete pPenBlue;
	delete pPenblack;

	return;
}

BOOL Wavepic::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_Low = 0;
	m_High = 1024;
	m_now =0;
	memset(m_lCount,0,1024);

    m_chuizhi.SetPos(50);
	m_shuiping.SetPos(0);

	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//DEL void Wavepic::OnCancelMode() 
//DEL {
//DEL 	CDialog::OnCancelMode();
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL 	
//DEL }

void Wavepic::OnTimer(UINT nIDEvent) 
{	
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	pWnd->GetClientRect(&rect);
	// 指针
	pDC = pWnd->GetDC();	
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();
	
	//pDC->Rectangle(&rect);

	
    //内存绘图
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right-rect.left,rect.bottom-rect.top);
	pOldBmp = memDC.SelectObject(&memBitmap);

	DrawWave(&memDC);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
	
	CDialog::OnTimer(nIDEvent);
}

//DEL HBRUSH Wavepic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//DEL {
//DEL 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//DEL 	
//DEL 	// TODO: Change any attributes of the DC here
//DEL     CRect   rc;   
//DEL     CBrush   brNew;   
//DEL     pWnd->GetClientRect(&rc);     
//DEL     if(pWnd->GetDlgCtrlID()==IDC_COORD)   
//DEL 	{   
//DEL         brNew.CreateSolidBrush(RGB(255,0,0));   
//DEL         pDC->SelectObject(brNew);     
//DEL         pDC->Rectangle(rc);     
//DEL         brNew.DeleteObject();     
//DEL    }
//DEL 
//DEL 	
//DEL 	// TODO: Return a different brush if the default is not desired
//DEL 	return hbr;
//DEL }

//DEL int Wavepic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
//DEL {
//DEL 	if (CDialog::OnCreate(lpCreateStruct) == -1)
//DEL 		return -1;
//DEL 	
//DEL 	// TODO: Add your specialized creation code here
//DEL 	CRect m_prect;
//DEL 	CWnd* ppWnd = GetDlgItem(IDC_COORD);
//DEL     ppWnd->GetClientRect(&m_prect);
//DEL 	CClientDC dc(ppWnd);
//DEL 	CBrush brush(RGB(0,0,0));
//DEL 	dc.FillRect(&m_prect,&brush);
//DEL 	
//DEL 	return 0;
//DEL }

void Wavepic::OnBtnSteady() 
{
	// TODO: Add your control notification handler code here
	m_count++;
}
