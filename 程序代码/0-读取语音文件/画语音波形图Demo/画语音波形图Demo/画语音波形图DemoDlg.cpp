
// 画语音波形图DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "画语音波形图Demo.h"
#include "画语音波形图DemoDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C画语音波形图DemoDlg 对话框



C画语音波形图DemoDlg::C画语音波形图DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C画语音波形图DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C画语音波形图DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// C画语音波形图DemoDlg 消息处理程序

BOOL C画语音波形图DemoDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	if ((fp = fopen("E:\\程序\\团队项目\\个性化文本语音转换系统\\语音\\微软默认.wav", "rb")) == NULL) {
		MessageBox(_T("ERROR : File open failed !"), _T("ERROR"), MB_ICONHAND | 0);
	}
	File.Give_FP(fp);
	if (File.Read_File() == false) {
		exit(-1);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C画语音波形图DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C画语音波形图DemoDlg::OnPaint()
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
	pWnd = GetDlgItem(IDC_STATIC);
	pWnd->GetWindowRect(rc);
	CDC* pdc = pWnd->GetDC();
	StartDraw(rc, pdc, 1);
	//==============================================================================================
}


//===================================================================================================
void C画语音波形图DemoDlg::StartDraw(CRect rc, CDC* pdc, int flag)
{
	width = rc.Width();
	height = rc.Height();
  
	CDC MemDC;                                                                 //首先定义一个显示设备对象   
	CBitmap MemBitmap;                                                         //定义一个位图对象
	MemDC.CreateCompatibleDC(NULL);                                            //随后建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //下面建立一个与屏幕显示兼容的位图
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //将位图选入到内存显示设备中//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	MemDC.FillSolidRect(0, 0, width, height, RGB(255, 255, 255));              //先用背景色将位图清除干净

	//newBrush.CreateSolidBrush(RGB(255, 255, 255));                           //创建黑色新画刷
	//pOldBrush = MemDC.SelectObject(&newBrush);                                 
	//MemDC.Rectangle(rc);                                                     //以黑色画刷为绘图控件填充黑色，形成黑色背景
	//MemDC.SelectObject(pOldBrush);                                           //恢复旧画刷
	//newBrush.DeleteObject();                                                 //删除新画刷
	newPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));                             //创建实心画笔，粗度为1，颜色为绿色
	pOldPen = MemDC.SelectObject(&newPen);                                     //选择新画笔，并将旧画笔的指针保存到pOldPen

	//绘制图形
	Plot(rc, &MemDC, 1);

	MemDC.SelectObject(pOldPen);                                               //恢复旧画笔
	newPen.DeleteObject();                                                     //删除新画笔									  
	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //将内存中的图拷贝到屏幕上进行显示
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void C画语音波形图DemoDlg::Plot(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, 0);
	for (int i = 0; i < width; ++i) {
		pdc->LineTo(i, (height - 150)*File.Get_Data(i + 10000) / 65535 + 75);
	}
}

//===================================================================================================



//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C画语音波形图DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool Wav_File::Conversion_Tuple(void)
{
	try
	{
		dataMax = 0;
		dataMin = 1000000;
		dataNumber = dataSize * 8 / sampleBits;
		dataTuple = (long*)malloc(sizeof(long)*dataNumber);
		long tempNum = 0;
		for (int i = 0; i < dataNumber; ++i) {
			for (int j = 0; j < sampleBits / 8; ++j) {
				tempNum = MAKEWORD(data[i*sampleBits / 8 + j], tempNum);
			}
			dataTuple[i] = tempNum;
			tempNum = 0;
			if (dataMax < dataTuple[i]) {
				dataMax = dataTuple[i];
			}
			if (dataMin > dataTuple[i]) {
				dataMin = dataTuple[i];
			}
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", 0);
		return false;
	}
	return true;
}

bool Wav_File::Read_File(void)
{
	try
	{
		fread(id_RIFF, sizeof(char), 4, fp);                                   //读取'RIFF'
		if (strcmp(id_RIFF, "RIFF")) {
			throw invalid_argument("ERROR : File not RIFF file");
		}
		id_RIFF[4] = 0;                                                        //末尾添零
		fread(&fileSize, sizeof(unsigned long), 1, fp);                        //读取文件的大小
		fread(id_WAVE, sizeof(char), 4, fp);                                   //读取'WAVE'
		id_WAVE[4] = 0;
		if (strcmp(id_WAVE, "WAVE")) {
			throw invalid_argument("ERROR : File not WAVE file");
		}
		fread(id_FMT, sizeof(char), 4, fp);                                    //读取'FMT'
		id_FMT[3] = 0;
		fread(&formatLength, sizeof(unsigned long), 1, fp);                    //
		fread(&formatTag, sizeof(short), 1, fp);                               //读取文件标签
		fread(&channelsNumber, sizeof(short), 1, fp);                          //读取通道数目
		fread(&sampleRate, sizeof(unsigned long), 1, fp);                      //读取采样频率
		fread(&secondBytes, sizeof(unsigned long), 1, fp);                     //读取每秒数据量
		fread(&chunkAlign, sizeof(short), 1, fp);                              //读取块对其
		fread(&sampleBits, sizeof(short), 1, fp);                              //读取样本大小
		if (formatLength > 16) {
			fread(&appendInformation, sizeof(short), 1, fp);                   //读取附加信息
		}
		fread(id_DATA, sizeof(char), 4, fp);                                   //读取'DATA'
		id_DATA[4] = 0;
		fread(&dataSize, sizeof(unsigned long), 1, fp);                        //读取数据大小
		data = (char*)malloc(sizeof(char)*dataSize);                           //申请数据的存储空间
		fread(data, sizeof(char), dataSize, fp);                               //读取数据
	}
	catch (invalid_argument &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", 0);
		return false;
	}
	MessageBox(NULL, _T("TIP : File read ok !"), _T("TIP"), MB_ICONASTERISK);
	if (Conversion_Tuple() == true) {
		MessageBox(NULL, _T("TIP : Data change OK !"), _T("TIP"), MB_ICONASTERISK);
	}
	else {
		return false;
	}

	return true;
}

long Wav_File::Get_Data(unsigned long Number)
{
	if (Number >= dataNumber) {
		MessageBox(NULL, _T("ERROR : Over list"), _T("ERROR"), MB_ICONHAND);
		return -1;
	}
	else {
		return dataTuple[Number];
	}
}