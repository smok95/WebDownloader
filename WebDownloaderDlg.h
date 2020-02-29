// WebDownloaderDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "HyperLink.h"
#include "JKHttpDownload.h"
#include "afxcmn.h"
#include "WDMenuMgr.h"



//static const char WD_DOWNLOAD_DIRECTORY[] = "WebDownloadFiles";

enum BtnStatus
{
	WD_STATUS_DOWNLOAD = 0,
	WD_STATUS_STOP
};

// CWebDownloaderDlg 대화 상자
class CWebDownloaderDlg : public CDialog
{
// 생성
public:
	CWebDownloaderDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_WEBDOWNLOADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원
    CWDMenuMgr m_menu;
	
// 구현
protected:
	BOOL m_bStopDownload;
	UINT m_btnStatus;
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CHyperLink m_cAddress;
	CProgressCtrl m_progress;
	CJKHttpDownload m_download;

	CString GetDownloadFileUrl(void);
	void OpenFolder(const char* folderPath);
	static DWORD DownloadThread(void* vp);
	void SetButtonStatus(UINT status);
	void InitControl(void);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnOpen();
};
