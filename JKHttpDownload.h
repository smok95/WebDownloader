#pragma once
#include "JKBSCallBack.h"
#include "JKDebug.h"
#include "JKDir.h"
#include "urlmon.h"
#pragma comment(lib,"urlmon")
#include <wininet.h>
#include "WDCommon.h"
#pragma comment(lib,"Wininet.lib")

typedef enum HD_ERR_CODE
{
	E_OK = 0,
	E_NULL_POINTER
}HD_ERR_CODE;

#define HD_DEFAULT_DOWNPATH "WebDownloadFiles"


class CJKHttpDownload
{
	CJKBSCallBack m_cb;
	WDINFO m_envInfo;
	BOOL GetFileNameFromURL(const char* url, char* pOutBuffer);
public:
	CJKHttpDownload(void);
	~CJKHttpDownload(void){}
	void SetDownloadDirectory(const char* path=NULL);	
	char* GetDownloadDirectory();
	UINT Download(const char* url);

	void SetProgress(CProgressCtrl* pPC);
	void SetHwndPercentText(HWND hWnd);
	void SetCancelFlag(BOOL* bCancel);
	

	BOOL SetProgressTextWindow(HWND hWnd, UINT flag);	
};

//********************************************************************************
inline CJKHttpDownload::CJKHttpDownload(){	SetDownloadDirectory();	}

inline void CJKHttpDownload::SetDownloadDirectory(const char* path/* =NULL */)
{
		CJKDir dir;
	
	if(path==NULL)
		wsprintf(m_envInfo.downloadDir, "%s%s",dir.GetSystemDrive(), HD_DEFAULT_DOWNPATH);
	else
	{
		// check string length
		if(HD_MAX_PATH <= strlen(path)) return;
		wsprintf(m_envInfo.downloadDir, path);
	}

	dir.CreateDirectories(m_envInfo.downloadDir);

}

inline char* CJKHttpDownload::GetDownloadDirectory(){	return m_envInfo.downloadDir;	}

inline UINT CJKHttpDownload::Download(const char* url)
{
	NullReturn(url)	E_NULL_POINTER;

	char downFile[HD_MAX_PATH]={0,};
	char file[HD_MAX_PATH]={0,};

	GetFileNameFromURL(url, file);
	wsprintf(downFile, "%s\\%s", GetDownloadDirectory(), file);

	//이전에 받았던 파일이 캐쉬에 남아 있을 경우 삭제한다.
	::DeleteUrlCacheEntry(url);

	HRESULT hr = URLDownloadToFile(NULL,url,downFile,0, &m_cb);

	if(hr == S_OK || *m_cb.m_bCancel)	return E_OK;
	return hr;
}

inline BOOL CJKHttpDownload::GetFileNameFromURL(const char* url, char* pOutBuffer)
{
	NullReturn(url) FALSE;
	NullReturn(pOutBuffer) FALSE;

	try{
	int urlLen = (int)strlen(url);

	for(int i=urlLen; i>=0; i--)
	{
		if(url[i] == '/')
		{
			strcpy(pOutBuffer, url+i+1);
			return TRUE;
		}
	}
	}catch(...){}
	return FALSE;
}

inline void CJKHttpDownload::SetProgress(CProgressCtrl* pPC)
{
	NullReturn(pPC);
	m_cb.SetProgressCtrl(pPC);
}

inline void CJKHttpDownload::SetHwndPercentText(HWND hWnd)
{
	NullReturn(hWnd);
	m_cb.SetPercentTextWnd(hWnd);
}

inline void CJKHttpDownload::SetCancelFlag(BOOL* bCancel){	m_cb.SetCancelFlag(bCancel);	}
inline BOOL CJKHttpDownload::SetProgressTextWindow(HWND hWnd, UINT flag)
{
	return m_cb.SetTextWindow(hWnd, flag);
}