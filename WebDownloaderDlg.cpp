// WebDownloaderDlg.cpp : 구현 파일
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


// CWebDownloaderDlg 대화 상자



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


// CWebDownloaderDlg 메시지 처리기

BOOL CWebDownloaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_btnStatus = WD_STATUS_STOP;
	m_bStopDownload = FALSE;
	InitControl();
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CWebDownloaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
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

	// 알 수 없는 경우 http:// 붙인다.
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
		pWD->MessageBox("URL을 입력하세요");
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
		result.Format("%s 다운로드 실패",url.GetBuffer());
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
	// CWDMenuMgr로 Message를 전달하여 Menu처리를 할 수 있도록 한다.
	m_menu.SendMessage(WM_COMMAND, wParam, lParam);

	return CDialog::OnCommand(wParam, lParam);
}


void CWebDownloaderDlg::OnBnClickedBtnOpen()
{
	CWDReg reg;
	ShellExecute(nullptr, "open", reg.GetDownloadFolder(), nullptr, nullptr, SW_SHOWNORMAL);
}
