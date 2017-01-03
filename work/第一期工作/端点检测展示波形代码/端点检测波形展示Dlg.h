
// 端点检测波形展示Dlg.h : 头文件
//

#pragma once
#include "Wav_File_Handle.h"

// C端点检测波形展示Dlg 对话框
class C端点检测波形展示Dlg : public CDialogEx
{
// 构造
public:
	C端点检测波形展示Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

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
	Wav_File_Work *File;
	
	CPen newPen;                                                               //用于创建新画笔   
	CPen *pOldPen;                                                             //用于存放旧画笔   
	CBrush newBrush;                                                           //用于创建新画刷   
	CBrush *pOldBrush;                                                         //用于存放旧画刷   
	int width;
	int height;

public:
	void StartDraw(CRect rc, CDC* pdc, int flag,int f);
	void Plot1(CRect rc, CDC* pdc, int flag);
	void Plot2(CRect rc, CDC* pdc, int flag);
	void Plot3(CRect rc, CDC* pdc, int flag);
};
