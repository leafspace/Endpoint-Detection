
// 端点检测波形展示Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "端点检测波形展示.h"
#include "端点检测波形展示Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double C端点检测波形展示Dlg::DEVIATION = 0.3;                             //偏差值初始化(只能在Cpp中初始化)

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
	DDX_Control(pDX, IDC_SLIDER1, ctrlSlider_threshold_h_EngChart);          //初始化滑块
	DDX_Control(pDX, IDC_SLIDER2, ctrlSlider_threshold_l_EngChart);          //初始化滑块
	DDX_Control(pDX, IDC_SLIDER3, ctrlSlider_threshold_h_CepsDistance);      //初始化滑块
	DDX_Control(pDX, IDC_SLIDER4, ctrlSlider_threshold_l_CepsDistance);      //初始化滑块
	DDX_Control(pDX, IDC_SLIDER5, ctrlSlider_threshold_h_EngDevia);          //初始化滑块
	DDX_Control(pDX, IDC_SLIDER6, ctrlSlider_threshold_l_EngDevia);          //初始化滑块
	DDX_Control(pDX, IDC_SLIDER7, ctrlSlider_threshold_h_SpectrumEnt);       //初始化滑块
	DDX_Control(pDX, IDC_SLIDER8, ctrlSlider_threshold_l_SpectrumEnt);       //初始化滑块

	DDX_Control(pDX, IDC_SLIDER9, ctrlSlider_interim_StateLength);           //初始化滑块
	DDX_Control(pDX, IDC_SLIDER10, ctrlSlider_burst_StateLength);            //初始化滑块
}

BEGIN_MESSAGE_MAP(C端点检测波形展示Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMMAND1, &C端点检测波形展示Dlg::OnBnClickedCommand1)
	ON_BN_CLICKED(IDC_BUTTON1, &C端点检测波形展示Dlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &C端点检测波形展示Dlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &C端点检测波形展示Dlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &C端点检测波形展示Dlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &C端点检测波形展示Dlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &C端点检测波形展示Dlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &C端点检测波形展示Dlg::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &C端点检测波形展示Dlg::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &C端点检测波形展示Dlg::OnNMCustomdrawSlider8)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER9, &C端点检测波形展示Dlg::OnNMCustomdrawSlider9)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER10, &C端点检测波形展示Dlg::OnNMCustomdrawSlider10)
	ON_BN_CLICKED(IDC_BUTTON2, &C端点检测波形展示Dlg::OnBnClickedButton2)
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

	//执行初始化操作

	ctrlSlider_threshold_h_EngChart.SetRange(0, 100);
	ctrlSlider_threshold_l_EngChart.SetRange(0, 100);
	ctrlSlider_threshold_h_CepsDistance.SetRange(0, 100);
	ctrlSlider_threshold_l_CepsDistance.SetRange(0, 100);
	ctrlSlider_threshold_h_EngDevia.SetRange(0, 100);
	ctrlSlider_threshold_l_EngDevia.SetRange(0, 100);
	ctrlSlider_threshold_h_SpectrumEnt.SetRange(0, 100);
	ctrlSlider_threshold_l_SpectrumEnt.SetRange(0, 100);

	ctrlSlider_interim_StateLength.SetRange(5, 30);
	ctrlSlider_burst_StateLength.SetRange(5, 30);

	ctrlSlider_threshold_h_EngChart.SetPos(80);
	ctrlSlider_threshold_l_EngChart.SetPos(20);
	ctrlSlider_threshold_h_CepsDistance.SetPos(80);
	ctrlSlider_threshold_l_CepsDistance.SetPos(20);
	ctrlSlider_threshold_h_EngDevia.SetPos(80);
	ctrlSlider_threshold_l_EngDevia.SetPos(20);
	ctrlSlider_threshold_h_SpectrumEnt.SetPos(80);
	ctrlSlider_threshold_l_SpectrumEnt.SetPos(20);

	ctrlSlider_interim_StateLength.SetPos(10);
	ctrlSlider_burst_StateLength.SetPos(5);

	CString str1 = (CString)("0.80");
	SetDlgItemText(IDC_EDIT2, str1);
	SetDlgItemText(IDC_EDIT4, str1);
	SetDlgItemText(IDC_EDIT6, str1);
	SetDlgItemText(IDC_EDIT8, str1);
	CString str2 = (CString)("0.20");
	SetDlgItemText(IDC_EDIT3, str2);
	SetDlgItemText(IDC_EDIT5, str2);
	SetDlgItemText(IDC_EDIT7, str2);
	SetDlgItemText(IDC_EDIT9, str2);

	SetDlgItemText(IDC_EDIT10, (CString)("10"));
	SetDlgItemText(IDC_EDIT11, (CString)("5"));

	ShowWindow(SW_FORCEMINIMIZE);

	// TODO: 在此添加额外的初始化代码
	repaint = false;
	fp = NULL;
	File = NULL;
	/*
	if(AllocConsole()){                                                        //用于生成一个DOS窗口用于显示数据
		SetConsoleTitleW(_T("噪声环境下端点检测波形展示数据调试"));              //设置DOS窗口的标题
		hdWrite = GetStdHandle(STD_OUTPUT_HANDLE);                             //获取窗口句柄并保存下来
	}
	*/
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
	if (repaint) {                                                           //repaint作为重绘标识符，首次执行时重绘触发器失败，等开始重绘时触发重绘
		Repaint();                                                           //重绘所需要的操作
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C端点检测波形展示Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C端点检测波形展示Dlg::Repaint()
{
	CPaintDC dc(this);                                                       //准备参数
	CWnd* pWnd;
	CRect rc;
	CDC* pdc;

	for (int i = 0; i < 5; ++i) {                                            //分别绘5个区域的图
		pWnd = GetDlgItem(IDC_STATIC1 + i);
		pWnd->GetWindowRect(rc);
		pdc = pWnd->GetDC();
		StartDraw(rc, pdc, 1, i + 1);                                        //绘制某一区域中的图像
	}
}

void C端点检测波形展示Dlg::StartDraw(CRect rc, CDC* pdc, int flag, int Flag)
{
	width = rc.Width();
	height = rc.Height();

	CDC MemDC;                                                                 //首先定义一个显示设备对象   
	CBitmap MemBitmap;                                                         //定义一个位图对象
	MemDC.CreateCompatibleDC(NULL);                                            //随后建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pdc, width, height);                      //下面建立一个与屏幕显示兼容的位图
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);                         //将位图选入到内存显示设备中//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	MemDC.FillSolidRect(0, 00, width, height, RGB(255, 255, 255));             //先用背景色将位图清除干净

	MemDC.Rectangle(1, 1, width - 1, height - 2);
	newPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));                             //创建实心画笔，粗度为1，颜色为红色
	pOldPen = MemDC.SelectObject(&newPen);                                     //选择新画笔，并将旧画笔的指针保存到pOldPen

	switch (Flag)                                                              //绘制图形
	{
	case 1:
		Plot1(rc, &MemDC, 1);
		break;
	case 2:
		pdc->Rectangle(0, 0, width, height);
		Plot2(rc, &MemDC, 1);
		break;
	case 3:
		pdc->Rectangle(0, 0, width, height);
		Plot3(rc, &MemDC, 1);
		break;
	case 4:
		pdc->Rectangle(0, 0, width, height);
		Plot4(rc, &MemDC, 1);
		break;
	case 5:
		pdc->Rectangle(0, 0, width, height);
		Plot5(rc, &MemDC, 1);
		break;
	default:
		break;
	}

	MemDC.SelectObject(pOldPen);                                               //恢复旧画笔
	newPen.DeleteObject();                                                     //删除新画笔

	if (Flag == 1 || Flag == 3) {                                              //绘制横坐标
		MemDC.MoveTo(0, height / 2);                                           //横轴开头
		MemDC.LineTo(width, height / 2);                                       //横轴结尾
		for (int i = 0; i < width / 100; ++i) {                                //绘制标尺
			MemDC.MoveTo((i + 1) * 100, height / 2);
			MemDC.LineTo((i + 1) * 100, height / 2 - 10);
		}
	}

	pdc->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);                   //将内存中的图拷贝到屏幕上进行显示

	MemBitmap.DeleteObject();                                                  //绘图完成后的清理
	MemDC.DeleteDC();
}

void C端点检测波形展示Dlg::Plot1(CRect rc, CDC* pdc, int flag)
{
	/*
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	for (int i = 0; i < width; ++i) {                                          //按像素逐个绘制图形
		max = min = i*File->Get_dataNumber() / width;
		for (double j = i*File->Get_dataNumber() / width; j < (i + 1)*File->Get_dataNumber() / width; ++j) { //控制像素范围之内
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataDouble(max);
		tempNumberMin = File->Get_DataDouble(min);

		highMax = height/2 - (height/2)*tempNumberMax;
		highMin = height/2 - (height/2)*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
	*/
	this->getTempVoiceData();
	this->screenShowData(showMaxData, viabilityMaxData);
	this->screenShowData(showMinData, viabilityMinData);
	double highMax, highMin;
	int topMax, topMin, bottomMax, bottomMin;
	topMax = topMin = bottomMax = bottomMin = 0;

	for (int i = 0; i < width; ++i) {
		if (viabilityMaxData[i] && showMaxData[i] > showMaxData[topMax]) {
			topMax = i;
		}
		if (viabilityMaxData[i] && showMaxData[i] < showMaxData[topMin]) {
			topMin = i;
		}
		if (viabilityMinData[i] && showMinData[i] > showMinData[bottomMax]) {
			bottomMax = i;
		}
		if (viabilityMinData[i] && showMinData[i] < showMinData[bottomMin]) {
			bottomMin = i;
		}
	}
	double unitTop = fabs(fabs(showMaxData[topMax]) - fabs(showMaxData[topMin]));
	double unitBottom = fabs(fabs(showMinData[bottomMax]) - fabs(showMinData[bottomMin]));

	vector<int> highTop, highBottom;
	for (int i = 0; i < width; ++i) {
		highMax = height / 2 - (fabs(showMaxData[i]) - fabs(showMaxData[topMin])) * (height / 2 / unitTop);
		highMin = height / 2 + (fabs(showMinData[i]) - fabs(showMinData[bottomMax])) * (height / 2 / unitBottom);
		if (!viabilityMaxData[i]) {
			highMax = height / 2;
		}
		if (!viabilityMinData[i]) {
			highMin = height / 2;
		}
		highTop.push_back((int)highMin);
		highBottom.push_back((int)highMax);
	}

	for (int i = 0; i < width; ++i) {
		pdc->MoveTo(i, highTop[i]);
		pdc->LineTo(i, highBottom[i]);
	}

	for (unsigned long i = 0; i < File->Get_voiceNumber(); ++i) {
		int num = File->Get_voiceNumber();
		VoiceParagraph tempPoint = File->Get_dataVoicePoint(i);
		int begin = (int)(tempPoint.begin / (File->Get_FrameNumber() / width));
		int end = (int)(tempPoint.end / (File->Get_FrameNumber() / width));
		pdc->MoveTo(begin, 1);
		pdc->LineTo(begin, height);
		pdc->MoveTo(end, 1);
		pdc->LineTo(end, height);
		pdc->DrawText((CString)("begin"), CRect(begin + 10, height - 20, begin + 50, height), 0);
		pdc->DrawText((CString)("end"), CRect(end + 10, height - 20, end + 50, height), 0);
	}

}

void C端点检测波形展示Dlg::Plot2(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, (int)height);
	for (int i = 0; i < width; ++i) {                                          //按像素逐个绘制图形
		int max = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //控制像素范围之内
			if (File->Get_DataCepsDistance((unsigned long)j) > File->Get_DataCepsDistance(max)) {
				max = (int)j;
			}
		}
		double tempNumberMax = File->Get_DataCepsDistance(max);
		double highMax = height - height*tempNumberMax;
		pdc->LineTo(i, (int)highMax);
	}
}
void C端点检测波形展示Dlg::Plot3(CRect rc, CDC* pdc, int flag)
{
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	pdc->MoveTo(0, (int)height / 2);
	for (int i = 0; i < width; ++i) {                                          //按像素逐个绘制图形
		max = min = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //控制像素范围之内
			if (File->Get_DataEngChart((unsigned long)j) > File->Get_DataEngChart(max)
				&& File->Get_DataEngChart((unsigned long)j) <= 1
				&& File->Get_DataEngChart((unsigned long)j) >= -1
				) {
				max = (int)j;
			}
			if (File->Get_DataEngChart((unsigned long)j) < File->Get_DataEngChart(min)
				&& File->Get_DataEngChart((unsigned long)j) <= 1
				&& File->Get_DataEngChart((unsigned long)j) >= -1
				) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataEngChart(max);
		tempNumberMin = File->Get_DataEngChart(min);

		highMax = height / 2 - (height / 2)*tempNumberMax;
		highMin = height / 2 - (height / 2)*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C端点检测波形展示Dlg::Plot4(CRect rc, CDC* pdc, int flag)
{
	int max, min;
	double highMax, highMin;
	double tempNumberMax, tempNumberMin;

	pdc->MoveTo(0, (int)height / 2);
	for (int i = 0; i < width; ++i) {                                          //按像素逐个绘制图形
		max = min = 0;
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //控制像素范围之内
			if (File->Get_DataEngDevia((unsigned long)j) > File->Get_DataEngDevia(max)) {
				max = (int)j;
			}
			if (File->Get_DataEngDevia((unsigned long)j) < File->Get_DataEngDevia(min)) {
				min = (int)j;
			}
		}
		tempNumberMax = File->Get_DataEngDevia(max);
		tempNumberMin = File->Get_DataEngDevia(min);

		tempNumberMax *= 10;
		tempNumberMin *= 10;

		tempNumberMax = tempNumberMax - (int)tempNumberMax;
		tempNumberMin = tempNumberMin - (int)tempNumberMin;

		highMax = height - height*tempNumberMax;
		highMin = height - height*tempNumberMin;

		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}
void C端点检测波形展示Dlg::Plot5(CRect rc, CDC* pdc, int flag)
{
	pdc->MoveTo(0, height);
	for (int i = 0; i < width; ++i) {                                          //按像素逐个绘制图形
		int max = 0, min = 0;
		max = min = (int)(i*File->Get_FrameNumber() / width);
		for (double j = i*File->Get_FrameNumber() / width; j < (i + 1)*File->Get_FrameNumber() / width; ++j) {         //控制像素范围之内
			if (File->Get_DataSpectrumEnt((unsigned long)j) > File->Get_DataSpectrumEnt(max)) {
				max = (int)j;
			}
			if (File->Get_DataSpectrumEnt((unsigned long)j) < File->Get_DataSpectrumEnt(min)) {
				min = (int)j;
			}
		}
		double tempNumberMax = File->Get_DataSpectrumEnt(max);
		double tempNumberMin = File->Get_DataSpectrumEnt(min);
		double highMax = height / 2 - (height / 2)*tempNumberMax;
		double highMin = height / 2 - (height / 2) * tempNumberMin;
		pdc->MoveTo(i, (int)highMin);
		pdc->LineTo(i, (int)highMax);
	}
}

void C端点检测波形展示Dlg::OnBnClickedCommand1()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog opendlg(TRUE, _T("*.wav"), _T("*.wav"), OFN_OVERWRITEPROMPT, _T("所有文件(*.wav*;)|*.wav*||"), NULL);   //打开文件选择框
	if (opendlg.DoModal() == IDOK)
	{
		fileName = opendlg.GetPathName();                                      //获取选择的文件名
	}
	SetDlgItemTextW(IDC_EDIT1, fileName);
}


void C端点检测波形展示Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码	
	int nameLen = WideCharToMultiByte(CP_ACP, 0, fileName, -1, NULL, 0, NULL, NULL);
	char *fileNameChar = new char[nameLen + 1];
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameChar, nameLen, NULL, NULL);     //将CString转为char*
	char *fileNameTemp = new char[nameLen + 20];

	int index = 0;                                                             //用于保存新文件名长度
	for (int i = 0; i < nameLen + 1; ++i) {                                    //处理'\'为'\\'，若文件中的路径分隔符为'\'则无法准确定位
		fileNameTemp[index++] = fileNameChar[i];
		if (fileNameChar[i] == '\\') {
			fileNameTemp[index++] = '\\';
		}
	}
	fileNameTemp[index] = 0;                                                   //防止原先没能保存完文件结束符

	SetWindowText(_T("端点检测波形展示   | Reading file | Please waiting a minute!"));

	if ((fp = fopen(fileNameTemp, "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	File = new Wav_File_Work(fp);

	fclose(fp);
	delete[] fileNameChar;
	delete[] fileNameTemp;

	SetWindowText(_T("端点检测波形展示   | Beging painting | Please waiting a minute!"));

	repaint = true;                                                            //表示即将开始重绘
	InvalidateRect(NULL, true);                                                //设置窗口无效
	UpdateWindow();                                                            //立即重绘，触发WM_PAINT

	showMaxData.clear();
	showMinData.clear();
	viabilityMaxData.clear();
	viabilityMinData.clear();

	SetWindowText(_T("端点检测波形展示"));
}

void C端点检测波形展示Dlg::getTempVoiceData(void)                               //获取要处理的数据
{
	int max, min;
	for (int i = 0; i < width; ++i) {                                          //将数据范围分为宽度段，在段中找到Max，min
		max = min = i*File->Get_dataNumber() / width;
		for (double j = i*File->Get_dataNumber() / width;
			j < (i + 1)*File->Get_dataNumber() / width; ++j) {                 //控制像素范围之内
			if (File->Get_DataDouble((int)j) > File->Get_DataDouble(max)) {
				max = (int)j;
			}
			if (File->Get_DataDouble((int)j) < File->Get_DataDouble(min)) {
				min = (int)j;
			}
		}
		this->showMaxData.push_back(File->Get_DataDouble(max));
		this->showMinData.push_back(File->Get_DataDouble(min));
		this->viabilityMaxData.push_back(true);
		this->viabilityMinData.push_back(true);
	}
}

void C端点检测波形展示Dlg::screenShowData(vector<double> showData, vector<bool> viabilityData)    //输入:保存的数据，可行性数据，返回min的数值，用于计算范围显示用 ；这个函数用于筛选出偏差值在30%以内的数据
{
	vector<double> deviationData;                                            //用于保存计算的偏差值
	bool flag = true;                                                        //作为一个标志，标志着没有筛选排除出新的数据

	while (true) {
		int viabilityNumber = 0;                                             //保存当前可行的数据的个数，用于计算avg
		double sum = 0;
		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {                                 //如果此数为不为可行数据
				continue;
			}
			else {
				sum += showData[i];                                          //计算可行数据的和
				++viabilityNumber;
			}
		}
		double avg = sum / viabilityNumber;

		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {
				deviationData.push_back(0);
			}
			else {                                                           //如果数据为可行数据
				deviationData.push_back(fabs(showData[i] / avg - 1));        //计算偏差值公式
			}
		}

		flag = true;
		for (int i = 0; i < (int)showData.size(); ++i) {
			if (viabilityData[i] == false) {
				continue;
			}
			else {
				if (deviationData[i] > DEVIATION) {                          //超出偏差值外的部分
					viabilityData[i] = false;                                //被筛选排除了，可行性为false
					flag = false;
				}
			}
		}

		deviationData.clear();                                               //清除偏差值，为下次筛选做准备

		if (flag) {                                                          //没有筛选出数据，所以退出循环
			break;
		}
	}
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_h_EngChart.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT2, str);
	Wav_File_Work::threshold_h_EngChart = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_l_EngChart.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT3, str);
	Wav_File_Work::threshold_l_EngChart = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_h_CepsDistance.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT4, str);
	Wav_File_Work::threshold_h_CepsDistance = trueData;

	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_l_CepsDistance.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT5, str);
	Wav_File_Work::threshold_l_CepsDistance = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_h_EngDevia.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT6, str);
	Wav_File_Work::threshold_h_EngDevia = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_l_EngDevia.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT7, str);
	Wav_File_Work::threshold_l_EngDevia = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_h_SpectrumEnt.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT8, str);
	Wav_File_Work::threshold_h_SpectrumEnt = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_threshold_l_SpectrumEnt.GetPos(); //获得滑块的当前位置
	double trueData = (double)nPos / 100;
	CString str = _T("");
	str.Format(_T("%.2lf"), trueData);
	SetDlgItemText(IDC_EDIT9, str);
	Wav_File_Work::threshold_l_SpectrumEnt = trueData;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_interim_StateLength.GetPos(); //获得滑块的当前位置
	CString str = _T("");
	str.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDIT10, str);
	Wav_File_Work::interim_StateLength = nPos;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = ctrlSlider_burst_StateLength.GetPos(); //获得滑块的当前位置
	CString str = _T("");
	str.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDIT11, str);
	Wav_File_Work::burst_StateLength = nPos;
	*pResult = 0;
}


void C端点检测波形展示Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->fp == NULL){
		MessageBoxA(NULL, "ERROR : NO File !", "ERROR", MB_ICONHAND);
	}else{
		File->SaveNewWav();
		MessageBoxA(NULL, "TIP : Voice created ok !", "TIP", MB_ICONASTERISK);
	}
}
