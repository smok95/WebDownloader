#include "StdAfx.h"
#include ".\wdmenumgr.h"
#include "resource.h"
#include "WebDownloaderDlg.h"
#include "WDOptionDlg.h"


CWDMenuMgr::CWDMenuMgr(void):m_pDlg(NULL)
{
}

CWDMenuMgr::~CWDMenuMgr(void)
{
}
BEGIN_MESSAGE_MAP(CWDMenuMgr, CWnd)
	ON_UPDATE_COMMAND_UI(ID_MENU_OPTION, OnUpdateMenuOption)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CWDMenuMgr::OnUpdateMenuOption(CCmdUI *pCmdUI)
{
	CWDOptionDlg optDlg;
	optDlg.DoModal();
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

int CWDMenuMgr::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CWDMenuMgr::InitMenuMgr(CWnd* pWnd)
{
	m_pDlg = (CWebDownloaderDlg*)pWnd;
	if(m_pDlg)
	{
		RECT rt={0,};
		this->Create(NULL,NULL,WS_CHILD,rt,m_pDlg,0);
	}
	return FALSE;
}
