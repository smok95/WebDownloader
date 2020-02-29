// WDOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WebDownloader.h"
#include "WDOptionDlg.h"
#include ".\wdoptiondlg.h"
#include "JKDir.h"
#include "JKReg.h"


// CWDOptionDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CWDOptionDlg, CDHtmlDialog)

CWDOptionDlg::CWDOptionDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWDOptionDlg::IDD, pParent)
{
}

CWDOptionDlg::~CWDOptionDlg()
{
}

CWDOptionDlg::CWDOptionDlg(WDINFO* pInfo)
{
}

void CWDOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CWDOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitControls();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CWDOptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_DOWNLOAD_FOLDER, OnBnClickedButtonChangeDownloadFolder)
END_MESSAGE_MAP()



void CWDOptionDlg::InitControls(void)
{
	this->SetDownloadDirectory(NULL);
}



BOOL CWDOptionDlg::SetDownloadDirectory(const char* dir)
{	
	char* folder = (char*)dir;
	if(folder ==NULL)
	{
		folder = m_reg.GetDownloadFolder();
		if(folder==NULL)	
		{
			m_reg.SetDownloadFolder(NULL);
			folder = m_reg.GetDownloadFolder();
		}
	}
	else	m_reg.SetDownloadFolder(folder);

    SetDlgItemText(IDC_EDIT_DOWNLOAD_FOLDER, folder);

	return FALSE;
}

void CWDOptionDlg::OnBnClickedButtonChangeDownloadFolder()
{
	CJKDir dir;
	CString newDir = dir.GetPathFromBrowser("변경할 폴더를 선택하세요");
	if(!newDir.IsEmpty())
	{
		if(newDir != m_reg.GetDownloadFolder())
		{
			if(MessageBox("다운로드 폴더를 변경하시겠습니까?",0,MB_YESNO) == IDYES)
			{
				this->SetDownloadDirectory(newDir.GetBuffer());
			}
		}
	}
}
