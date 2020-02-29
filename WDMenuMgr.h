#pragma once
#include "afxwin.h"

class CWebDownloaderDlg;

class CWDMenuMgr :	public CWnd
{
	CWebDownloaderDlg* m_pDlg;
public:
	CWDMenuMgr();
	~CWDMenuMgr(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateMenuOption(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	BOOL InitMenuMgr(CWnd* pWnd);
};
