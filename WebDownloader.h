// WebDownloader.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CWebDownloaderApp:
// �� Ŭ������ ������ ���ؼ��� WebDownloader.cpp�� �����Ͻʽÿ�.
//

class CWebDownloaderApp : public CWinApp
{
public:
	CWebDownloaderApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CWebDownloaderApp theApp;
