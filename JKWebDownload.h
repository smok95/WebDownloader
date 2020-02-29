
/************************************************************************
file: c:\Develoment\PROJECT\MyProject\Personal\WebDownloader\JKWebDownload.h
date: 2010/07/14
	
	Created by Kim,Jong-kook
	
	CJKWebDownload class

	웹파일 다운로드 클래스로
	wininet함수를 이용하여 파일을 다운로드하며
	urlmon의 IBindStatusCallback 인터페이스를 통해서 진행상태를 수신받는다.

************************************************************************/
#pragma once


#include <UrlMon.h>
#pragma comment(lib,"urlmon")

#define HD_DEFAULT_DOWNPATH "WebDownloadFiles"
#define HD_MAX_PATH	4096

/***
 *	IJKWebDownloadStatus 인터페이스

 *	1. 다운로드 진행시 다운로드 상태정보를 받고자 하는 객체는 해당 인터페이스를 상속받아 
 *	OnProgress함수를 구현한다.

 *	2. 해당인터페이스를 상속받은 객체가 실제로 다운로드상태정보를 받기 위해서는
 *	CJKWebDownload클래스의 SetStatusCallback함수에 자신의 포인터를 넘겨주면 된다.

 */
interface IJKWebDownloadStatus
{
	virtual void OnProgress(ULONG nProgress, ULONG nProgressMax, int elapsedSec)=0;
};

class CJKWebDownload:public IBindStatusCallback
{
	bool m_bStop;					/* 다운로드 중단여부 */
	IJKWebDownloadStatus *m_pStatus;/* 상태정보 전달 인터페이스 */
	char m_downloadDir[MAX_PATH];	/* 다운로드 파일 저장 경로 */
	time_t  m_beginTime;			/* 다운로드 시작시간 */

	// desc  : url에서 다운로드할 파일명 추출
	// param :
	//			url : 다운로드url
	//			pOutBuffer : 파일명을 담을 버퍼
	// return: 성공시 TRUE
	BOOL GetFileNameFromURL(const char* url, char* pOutBuffer);
public:
	CJKWebDownload(void);
	~CJKWebDownload(void){}

	///<summary>다운로드상태정보를 전달할 객체 설정</summary>
	///<input name="pStatus">상태정보를 수신할 객체</input>
	void SetStatusCallback(IJKWebDownloadStatus* pStatus){ m_pStatus=pStatus; }

	///<summary>파일 저장경로 설정</summary>
	///<input name=
	///
	void SetDownloadDirectory(const char* path=NULL);	
	char* GetDownloadDirectory();
	UINT Download(const char* url);
	void Stop(){ m_bStop=true; }

	/* IBindStatusCallback 구현부 시작	*/
	virtual HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved,IBinding *pib){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE GetPriority( LONG *pnPriority){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE OnLowResource( DWORD reserved){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult,LPCWSTR szError){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF,BINDINFO *pbindinfo){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize,
		FORMATETC *pformatetc,STGMEDIUM *pstgmed){ return E_NOTIMPL; };
	virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid,IUnknown *punk){ return E_NOTIMPL; };
	STDMETHOD_(ULONG,AddRef)(){ return 0; }
	STDMETHOD_(ULONG,Release)(){ return 0; }
	STDMETHOD(QueryInterface)(REFIID riid,void __RPC_FAR *__RPC_FAR *ppvObject)	{ return E_NOTIMPL; }

	virtual HRESULT STDMETHODCALLTYPE OnProgress(
		ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR szStatusText);
 	/* IBindStatusCallback 구현부 끝	*/
};
