
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

	CSliderCtrl ctrlSlider_threshold_h_EngChart;
	CSliderCtrl ctrlSlider_threshold_l_EngChart;
	CSliderCtrl ctrlSlider_threshold_h_CepsDistance;
	CSliderCtrl ctrlSlider_threshold_l_CepsDistance;
	CSliderCtrl ctrlSlider_threshold_h_EngDevia;
	CSliderCtrl ctrlSlider_threshold_l_EngDevia;
	CSliderCtrl ctrlSlider_threshold_h_SpectrumEnt;
	CSliderCtrl ctrlSlider_threshold_l_SpectrumEnt;

	CSliderCtrl ctrlSlider_interim_StateLength;
	CSliderCtrl ctrlSlider_burst_StateLength;

	bool repaint;                                                              //判断是否为重绘图标
	CString fileName;                                                          //保存路径

	HANDLE hdWrite;                                                            //用于保存DOS窗口的句柄

	static const double DEVIATION;                                             //定义的偏差值范围
	vector<double> showMaxData, showMinData;                                   //保存获取的数据
	vector<bool> viabilityMaxData, viabilityMinData;                           //保存数据的可行性

	void getTempVoiceData(void);                                               //获取要处理的数据
	void screenShowData(vector<double> showData, vector<bool> viabilityData);  //输入:保存的数据，可行性数据，返回min的数值，用于计算范围显示用 ；这个函数用于筛选出偏差值在30%以内的数据
public:
	void Repaint();
	void StartDraw(CRect rc, CDC* pdc, int flag, int f);
	void Plot1(CRect rc, CDC* pdc, int flag);
	void Plot2(CRect rc, CDC* pdc, int flag);
	void Plot3(CRect rc, CDC* pdc, int flag);
	void Plot4(CRect rc, CDC* pdc, int flag);
	void Plot5(CRect rc, CDC* pdc, int flag);
	afx_msg void OnBnClickedCommand1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult);
};
