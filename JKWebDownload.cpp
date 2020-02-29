#include "JKWebDownload.h"
//#include "JKDebug.h"
#include "JKDir.h"
//#include "WDCommon.h"
#include <time.h>

#include <wininet.h>
#pragma comment(lib,"wininet.lib")


CJKWebDownload::CJKWebDownload():m_pStatus(NULL), m_bStop(false)
{	
	memset(m_downloadDir, 0, sizeof(m_downloadDir));
}

void CJKWebDownload::SetDownloadDirectory(const char* path/* =NULL */)
{
	CJKDir dir;
	
	if(path==NULL)
		wsprintf(m_downloadDir, "%s%s",dir.GetSystemDrive(), HD_DEFAULT_DOWNPATH);
	else
	{
		// check string length
		if(HD_MAX_PATH <= strlen(path)) return;
		wsprintf(m_downloadDir, path);
	}

	dir.CreateDirectories(m_downloadDir);

}

char* CJKWebDownload::GetDownloadDirectory(){	return m_downloadDir;	}

UINT CJKWebDownload::Download(const char* url)
{
	NullReturn(url)	-1;

	if(m_downloadDir[0]==0)	
	{
		SetDownloadDirectory();
	}

	m_bStop=false;
	char downFile[HD_MAX_PATH]={0,};
	char file[HD_MAX_PATH]={0,};
	m_beginTime = time(NULL);
	GetFileNameFromURL(url, file);
	wsprintf(downFile, "%s\\%s", GetDownloadDirectory(),file);

	//이전에 받았던 파일이 캐쉬에 남아 있을 경우 삭제한다.
	::DeleteUrlCacheEntry(url);

	HRESULT hr = URLDownloadToFile(NULL,url,downFile,0, this);

	if(hr == S_OK || m_bStop )	return 0;
	return hr;
}

BOOL CJKWebDownload::GetFileNameFromURL(const char* url, char* pOutBuffer)
{
	NullReturn(url) FALSE;
	NullReturn(pOutBuffer) FALSE;

	try{
	int urlLen = (int)strlen(url);
	const char* queryPos = url+urlLen;

	for(int i=urlLen; i>=0; i--)
	{
		switch(url[i])
		{
		case '/':
			{
				strncpy(pOutBuffer, url+i+1, queryPos-(url+i+1));
				//strcpy(pOutBuffer, url+i+1);
				return TRUE;
			}
		case '?':
			{
				queryPos=url+i;
			}
		}		
	}
	}catch(...){}
	return FALSE;
}

HRESULT CJKWebDownload::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{	
	if(m_bStop)	return E_ABORT;
	if(m_pStatus)
	{
		int elapsedSec = (int)difftime(time(NULL), m_beginTime);
		m_pStatus->OnProgress(ulProgress, ulProgressMax, elapsedSec<=0?1:elapsedSec);
	}
	return S_OK;
}