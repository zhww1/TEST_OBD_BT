
// TEST_OBD.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTEST_OBDApp: 
// �йش����ʵ�֣������ TEST_OBD.cpp
//

class CTEST_OBDApp : public CWinApp
{
public:
	CTEST_OBDApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTEST_OBDApp theApp;