
// ExpressSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DatabaseADO.h"

// CExpressSystemApp: 
// �йش����ʵ�֣������ ExpressSystem.cpp
//

class CExpressSystemApp : public CWinApp
{
public:
	static std::string FileNameString;
	CExpressSystemApp();
	~CExpressSystemApp()
	{
		DatabaseADO::releaseADO();
	}
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExpressSystemApp theApp;