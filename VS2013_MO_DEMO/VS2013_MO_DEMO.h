
// VS2013_MO_DEMO.h : main header file for the VS2013_MO_DEMO application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVS2013_MO_DEMOApp:
// See VS2013_MO_DEMO.cpp for the implementation of this class
//

class CVS2013_MO_DEMOApp : public CWinApp
{
public:
	CVS2013_MO_DEMOApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVS2013_MO_DEMOApp theApp;
