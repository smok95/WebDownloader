// WebDownloaderDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "WebDownloader.h"
#include "WebDownloaderDlg.h"
#include ".\webdownloaderdlg.h"
#include "JKDir.h"
#include "urlmon.h"
#pragma comment(lib,"urlmon")
#include <wininet.h>
#include "JKDebug.h"
#include "WDReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebDownloaderDlg ��ȭ ����



CWebDownloaderDlg::CWebDownloaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebDownloaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CWebDownloaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ADDRESS, m_cAddress);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}

BEGIN_MESSAGE_MAP(CWebDownloaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CWebDownloaderDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CWebDownloaderDlg �޽��� ó����

BOOL CWebDownloaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnStatus = WD_STATUS_STOP;
	m_bStopDownload = FALSE;
	InitControl();
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWebDownloaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CWebDownloaderDlg::OnQueryDragIcon(){	return static_cast<HCURSOR>(m_hIcon);	}

void CWebDownloaderDlg::OnBnClickedOk()
{
	switch(m_btnStatus)
	{
	case WD_STATUS_DOWNLOAD:
		SetButtonStatus(WD_STATUS_STOP);
		break;
	case WD_STATUS_STOP:
		::CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)CWebDownloaderDlg::DownloadThread,this,0,0);
		SetButtonStatus(WD_STATUS_DOWNLOAD);	
		break;
	}
}


CString CWebDownloaderDlg::GetDownloadFileUrl(void)
{
	CString url;
	GetDlgItemText(IDC_EDIT_FILEURL,url);	
	url.Trim();

	const char* protocols[] = {
		"http://",
		"https://"
	};

	for (size_t i = 0; i < _countof(protocols); i++) {
		if (strnicmp(protocols[i], url.GetString(), strlen(protocols[i])) == 0) {
			return url;
		}
	}

	// �� �� ���� ��� http:// ���δ�.
	CString temp(url.GetBuffer());
	url.Format("http://%s", temp.GetBuffer());
	return url;

}

void CWebDownloaderDlg::OpenFolder(const char* folderPath)
{	
	HWND hWnd = ::FindWindow(NULL,WD_DOWNLOAD_DIRECTORY);
	if(hWnd)
	{
		::SetForegroundWindow(hWnd);
		::ShowWindow(hWnd, SW_SHOWNORMAL);
	}
	else
	{
		if(folderPath)
			::ShellExecute(NULL,"explore",folderPath,NULL,NULL,SW_SHOWNORMAL);		
	}
}

DWORD CWebDownloaderDlg::DownloadThread(void* vp)
{
	CWebDownloaderDlg* pWD = (CWebDownloaderDlg*)vp;
	CString url;
	CWDReg reg;
	
	//CJKHttpDownload m_download;
	if(pWD==NULL) goto exit;

	
	
	url = pWD->GetDownloadFileUrl();
	if(url.GetLength() <= 7)
	{
		pWD->MessageBox("URL�� �Է��ϼ���");
		goto exit;
	}

	pWD->m_download.SetDownloadDirectory(reg.GetDownloadFolder());

	HWND hPercent;
	pWD->GetDlgItem(IDC_STATIC_PERCENT, &hPercent);
	pWD->m_download.SetHwndPercentText(hPercent);

	HWND hSize;
	pWD->GetDlgItem(IDC_STATIC_DN_STATE, &hSize);
	pWD->m_download.SetProgressTextWindow(hSize, STATE_SIZE);

	HWND hSpeed;
	pWD->GetDlgItem(IDC_STATIC_STATE_SPEED, &hSpeed);
	pWD->m_download.SetProgressTextWindow(hSpeed, STATE_SPEED);

	pWD->m_download.SetCancelFlag(&pWD->m_bStopDownload);
	pWD->m_download.SetProgress(&pWD->m_progress);
	if(pWD->m_download.Download(url.GetBuffer()) == E_OK)
		pWD->OpenFolder(pWD->m_download.GetDownloadDirectory());
	else
	{
		CString result;
		result.Format("%s �ٿ�ε� ����",url.GetBuffer());
		pWD->MessageBox(result);	
	}

exit:	
	pWD->SetButtonStatus(WD_STATUS_STOP);
	ExitThread(0);
	return 0;
}

void CWebDownloaderDlg::SetButtonStatus(UINT status)
{
	m_btnStatus = status;
	switch(m_btnStatus)
	{
	case WD_STATUS_STOP:
		{
			SetDlgItemText(IDOK, "download");	
			m_bStopDownload = TRUE;
		}
		break;
	case WD_STATUS_DOWNLOAD:
		{
			m_progress.ShowWindow(SW_SHOW);
			HWND caption;
			GetDlgItem(IDC_STATIC_CAPTION_FILE, &caption);
			::ShowWindow(caption, SW_SHOW);
			CString file;
			GetDlgItemText(IDC_EDIT_FILEURL, file);
			int index = file.ReverseFind('/');
			SetDlgItemText(IDC_STATIC_FILENAME, file.Right(file.GetLength()-index-1));
			SetDlgItemText(IDOK, "stop");
			m_bStopDownload = FALSE;
		}
		break;
	}
}

void CWebDownloaderDlg::InitControl(void)
{
	m_progress.ShowWindow(SW_HIDE);

	m_menu.InitMenuMgr(this);
	
	HWND hWnd[1]={0,};
	UINT idc[1]={IDC_STATIC_CAPTION_FILE};
	
	for(int i=0; i<1; i++)
	{
		GetDlgItem(idc[i], &hWnd[i]);
		::ShowWindow(hWnd[i], SW_HIDE);
	}

}

BOOL CWebDownloaderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	return FALSE;
	return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CWebDownloaderDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// CWDMenuMgr�� Message�� �����Ͽ� Menuó���� �� �� �ֵ��� �Ѵ�.
	m_menu.SendMessage(WM_COMMAND, wParam, lParam);

	return CDialog::OnCommand(wParam, lParam);
}


void CWebDownloaderDlg::OnBnClickedBtnOpen()
{
	CWDReg reg;
	ShellExecute(nullptr, "open", reg.GetDownloadFolder(), nullptr, nullptr, SW_SHOWNORMAL);
}
