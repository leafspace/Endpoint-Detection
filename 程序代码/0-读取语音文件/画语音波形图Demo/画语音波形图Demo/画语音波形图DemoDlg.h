
// 画语音波形图DemoDlg.h : 头文件
//

#pragma once
#include <iostream>
#include <windows.h>
using namespace std;

class Wav_File
{
private:
	FILE *fp;
	char *fileName;
	char id_RIFF[5];
	unsigned long fileSize;
	char id_WAVE[5];
	char id_FMT[5];
	unsigned long formatLength;
	short formatTag;
	short channelsNumber;
	unsigned long sampleRate;
	unsigned long secondBytes;
	short chunkAlign;
	short sampleBits;
	short appendInformation;

	char appendFlag[5];
	unsigned long appendLength;
	unsigned long appendNone;

	char id_DATA[5];
	unsigned long dataSize;
	char *data;
	long *dataTuple;
	unsigned long dataNumber;
	unsigned long dataMax;
	unsigned long dataMin;

	bool Conversion_Tuple(void);
public:
	Wav_File(FILE *f) {
		fp = f;
	}
	Wav_File(void) {
		fp = NULL;
	}
	~Wav_File() {
		exit(0);
	}
	void Give_FP(FILE *f) {
		fp = f;
	}
	bool Read_File(void);
	long Get_Data(unsigned long Number);
};



// C画语音波形图DemoDlg 对话框
class C画语音波形图DemoDlg : public CDialogEx
{
// 构造
public:
	C画语音波形图DemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	FILE *fp;
	Wav_File File;

	CPen newPen;                                                               //用于创建新画笔   
	CPen *pOldPen;                                                             //用于存放旧画笔   
	CBrush newBrush;                                                           //用于创建新画刷   
	CBrush *pOldBrush;                                                         //用于存放旧画刷   
	int width;
	int height;

public:
	void StartDraw(CRect rc, CDC* pdc, int flag);
	void Plot(CRect rc, CDC* pdc, int flag);
};

