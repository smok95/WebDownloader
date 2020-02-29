// WDOptionDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WebDownloader.h"
#include "WDOptionDlg.h"
#include ".\wdoptiondlg.h"
#include "JKDir.h"
#include "JKReg.h"


// CWDOptionDlg ��ȭ �����Դϴ�.

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
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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
	CString newDir = dir.GetPathFromBrowser("������ ������ �����ϼ���");
	if(!newDir.IsEmpty())
	{
		if(newDir != m_reg.GetDownloadFolder())
		{
			if(MessageBox("�ٿ�ε� ������ �����Ͻðڽ��ϱ�?",0,MB_YESNO) == IDYES)
			{
				this->SetDownloadDirectory(newDir.GetBuffer());
			}
		}
	}
}
