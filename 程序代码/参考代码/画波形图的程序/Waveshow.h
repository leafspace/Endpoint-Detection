//Download by http://www.NewXing.com
// Waveshow.h : main header file for the WAVESHOW application
//

#if !defined(AFX_WAVESHOW_H__85485D7C_DC46_4F1C_BD27_B5CB510C77B2__INCLUDED_)
#define AFX_WAVESHOW_H__85485D7C_DC46_4F1C_BD27_B5CB510C77B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWaveshowApp:
// See Waveshow.cpp for the implementation of this class
//

class CWaveshowApp : public CWinApp
{
public:
	CWaveshowApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveshowApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWaveshowApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVESHOW_H__85485D7C_DC46_4F1C_BD27_B5CB510C77B2__INCLUDED_)
