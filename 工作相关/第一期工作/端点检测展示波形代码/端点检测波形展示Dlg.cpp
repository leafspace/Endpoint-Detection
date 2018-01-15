
// 端点检测波形展示Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "端点检测波形展示.h"
#include "端点检测波形展示Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C端点检测波形展示Dlg 对话框




C端点检测波形展示Dlg::C端点检测波形展示Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C端点检测波形展示Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C端点检测波形展示Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C端点检测波形展示Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// C端点检测波形展示Dlg 消息处理程序

BOOL C端点检测波形展示Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_FORCEMINIMIZE);

	// TODO: 在此添加额外的初始化代码
	if((fp=fopen("微软默认.wav", "rb"))==NULL){
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	File = new Wav_File_Work(fp);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C端点检测波形展示Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C端点检测波形展示Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//==============================================================================================
	CPaintDC dc(this);
	CWnd* pWnd;
	CRect rc;
	CDC* pdc;

	pWnd = GetDlgItem(IDC_STATIC1);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 1);

	pWnd = GetDlgItem(IDC_STATIC2);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 2);

	pWnd = GetDlgItem(IDC_STATIC3);
	pWnd->GetWindowRect(rc);
	pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1, 3);
	//==============================================================================================
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C端点检测波形展示Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C端点检测波形展示Dlg::StartDraw(CRect rc, CDC* pdc, int flag,int Flag)
{
	width = rc.Width();
	height = rc.Height();

	CDC MemDC;                                                                 //首先定义一个显示设备对象   
	CBitmap MemBitmap;                                                         //定义一个位图对象
	MemDC.CreateCompatibleDC(NULL);                                            //随后建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //下面建立一个与屏幕显示兼容的位图
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //将位图选入到内存显示设备中//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	MemDC.FillSolidRect(0, 00, width, height, RGB(255, 255, 255));             //先用背景色将位图清除干净

	MemDC.Rectangle(1,1,width-1,height-1);
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));                             //创建实心画笔，粗度为1，颜色为红色
	pOldPen = MemDC.SelectObject(&newPen);                                     //选择新画笔，并将旧画笔的指针保存到pOldPen

	//绘制图形
	switch (Flag)
	{
	case 1:
		Plot1(rc, &MemDC, 1);
		break;
	case 2:
		pdc->Rectangle(0,0,width,height);
		Plot2(rc, &MemDC, 1);
		break;
	case 3:
		pdc->Rectangle(0,0,width,height);
		Plot3(rc, &MemDC, 1);
	}
	MemDC.SelectObject(pOldPen);                                               //恢复旧画笔
	newPen.DeleteObject();                                                     //删除新画笔
	if(Flag==1){
		MemDC.MoveTo(0,height/2);
		MemDC.LineTo(width,height/2);
		for(int i=0;i<width/100;++i){
			MemDC.MoveTo((i+1)*100,height/2);
			MemDC.LineTo((i+1)*100,height/2-10);
		}
		for(unsigned long i=0;i<File->Get_voiceNumber();++i){
			int num=File->Get_voiceNumber();
			VoiceParagraph tempPoint=File->Get_dataVoicePoint(i);
			int begin=(int)(tempPoint.begin/(File->Get_dataNumber() / width));
			int end=(int)(tempPoint.end/(File->Get_dataNumber() / width));
			MemDC.MoveTo(begin,1);
			MemDC.LineTo(begin,height);
			MemDC.MoveTo(end,1);
			MemDC.LineTo(end,height);
			MemDC.DrawText((CString)("begin"),CRect(begin+10,height-20,begin+50,height),0);
			MemDC.DrawText((CString)("end"),CRect(end+10,height-20,end+50,height),0);
		}
	}else{
		for(int i=0;i<width/100;++i){
			MemDC.MoveTo((i+1)*100,height);
			MemDC.LineTo((i+1)*100,height-10);
		}
	}

	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //将内存中的图拷贝到屏幕上进行显示
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void C端点检测波形展示Dlg::Plot1(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		int max = 0;
		int min = 0;
		for (double j = i*File->Get_dataNumber() / width; j < (i + 1)*File->Get_dataNumber() / width; ++j){
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		double tempNumberMax=File->Get_DataDouble(max);
		double tempNumberMin=File->Get_DataDouble(min);
		double highMax=height/2+(height/2)*tempNumberMax;
		double highMin=height/2+(height/2)*tempNumberMin;
		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C端点检测波形展示Dlg::Plot2(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		double sum=0;
		for (double j = i*File->Get_dataEZNumber() / width; j < (i + 1)*File->Get_dataEZNumber() / width; ++j){
			sum+=File->Get_DataEnergy((unsigned long)j);
		}
		sum=sum/(File->Get_dataEZNumber() / width);
		double tempNumber=sum;
		double high=tempNumber*height/18;
		pdc->MoveTo(i, height);
		pdc->LineTo(i, (int)(height-high));
	}
}
void C端点检测波形展示Dlg::Plot3(CRect rc, CDC* pdc, int flag)
{
	for (int i = 0; i < width; ++i) {
		int max = 0;
		for (double j = i*File->Get_dataEZNumber() / width; j < (i + 1)*File->Get_dataEZNumber() / width; ++j){
			if (File->Get_DataZCR((unsigned long)j) > File->Get_DataZCR(max)) {
				max = (int)j;
			}
		}
		double tempNumber=File->Get_DataZCR(max);
		double high=tempNumber*height/45;
		pdc->MoveTo(i, height);
		pdc->LineTo(i, (int)(height-high));
	}
}