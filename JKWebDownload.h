
/************************************************************************
file: c:\Develoment\PROJECT\MyProject\Personal\WebDownloader\JKWebDownload.h
date: 2010/07/14
	
	Created by Kim,Jong-kook
	
	CJKWebDownload class

	������ �ٿ�ε� Ŭ������
	wininet�Լ��� �̿��Ͽ� ������ �ٿ�ε��ϸ�
	urlmon�� IBindStatusCallback �������̽��� ���ؼ� ������¸� ���Ź޴´�.

************************************************************************/
#pragma once


#include <UrlMon.h>
#pragma comment(lib,"urlmon")

#define HD_DEFAULT_DOWNPATH "WebDownloadFiles"
#define HD_MAX_PATH	4096

/***
 *	IJKWebDownloadStatus �������̽�

 *	1. �ٿ�ε� ����� �ٿ�ε� ���������� �ް��� �ϴ� ��ü�� �ش� �������̽��� ��ӹ޾� 
 *	OnProgress�Լ��� �����Ѵ�.

 *	2. �ش��������̽��� ��ӹ��� ��ü�� ������ �ٿ�ε���������� �ޱ� ���ؼ���
 *	CJKWebDownloadŬ������ SetStatusCallback�Լ��� �ڽ��� �����͸� �Ѱ��ָ� �ȴ�.

 */
interface IJKWebDownloadStatus
{
	virtual void OnProgress(ULONG nProgress, ULONG nProgressMax, int elapsedSec)=0;
};

class CJKWebDownload:public IBindStatusCallback
{
	bool m_bStop;					/* �ٿ�ε� �ߴܿ��� */
	IJKWebDownloadStatus *m_pStatus;/* �������� ���� �������̽� */
	char m_downloadDir[MAX_PATH];	/* �ٿ�ε� ���� ���� ��� */
	time_t  m_beginTime;			/* �ٿ�ε� ���۽ð� */

	// desc  : url���� �ٿ�ε��� ���ϸ� ����
	// param :
	//			url : �ٿ�ε�url
	//			pOutBuffer : ���ϸ��� ���� ����
	// return: ������ TRUE
	BOOL GetFileNameFromURL(const char* url, char* pOutBuffer);
public:
	CJKWebDownload(void);
	~CJKWebDownload(void){}

	///<summary>�ٿ�ε���������� ������ ��ü ����</summary>
	///<input name="pStatus">���������� ������ ��ü</input>
	void SetStatusCallback(IJKWebDownloadStatus* pStatus){ m_pStatus=pStatus; }

	///<summary>���� ������ ����</summary>
	///<input name=
	///
	void SetDownloadDirectory(const char* path=NULL);	
	char* GetDownloadDirectory();
	UINT Download(const char* url);
	void Stop(){ m_bStop=true; }

	/* IBindStatusCallback ������ ����	*/
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
 	/* IBindStatusCallback ������ ��	*/
};
