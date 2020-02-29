#pragma once
#include "urlmon.h"

#define STATE_PERCENT	0
#define STATE_SPEED	1
#define STATE_TIME	2
#define STATE_SIZE	3


class CJKBSCallBack :
	public IBindStatusCallback
{
	time_t m_startTime;
	CProgressCtrl* m_pPC;
	HWND m_hPercentWnd;	//	다운로드 퍼센트표시
	HWND m_hTimeWnd;	//	다룬로드 시간표시
	HWND m_hSizeWnd;	//	다운로드 파일 크기표시
	HWND m_hSpeedWnd;	//	다운로드 속도 표시
public:
	BOOL* m_bCancel;
	CJKBSCallBack(void);
	~CJKBSCallBack(void);

	virtual HRESULT STDMETHODCALLTYPE OnStartBinding( 
		/* [in] */ DWORD dwReserved,
		/* [in] */ IBinding *pib){ return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE GetPriority( 
		/* [out] */ LONG *pnPriority){ return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE OnLowResource( 
		/* [in] */ DWORD reserved){ return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE OnProgress( 
		/* [in] */ ULONG ulProgress,
		/* [in] */ ULONG ulProgressMax,
		/* [in] */ ULONG ulStatusCode,
		/* [in] */ LPCWSTR szStatusText);

		virtual HRESULT STDMETHODCALLTYPE OnStopBinding( 
		/* [in] */ HRESULT hresult,
		/* [unique][in] */ LPCWSTR szError){ return E_NOTIMPL; };

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetBindInfo( 
		/* [out] */ DWORD *grfBINDF,
		/* [unique][out][in] */ BINDINFO *pbindinfo){ return E_NOTIMPL; };

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE OnDataAvailable( 
		/* [in] */ DWORD grfBSCF,
		/* [in] */ DWORD dwSize,
		/* [in] */ FORMATETC *pformatetc,
		/* [in] */ STGMEDIUM *pstgmed){ return E_NOTIMPL; };

		virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable( 
		/* [in] */ REFIID riid,
		/* [iid_is][in] */ IUnknown *punk){ return E_NOTIMPL; };

		STDMETHOD_(ULONG,AddRef)(){ return 0; }

	STDMETHOD_(ULONG,Release)(){ return 0; }

	STDMETHOD(QueryInterface)(/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{ return E_NOTIMPL; }
	void SetProgressCtrl(CProgressCtrl* pPC);
	void SetPercentTextWnd(HWND hWnd);
	void SetCancelFlag(BOOL* bCancel);
	BOOL SetTextWindow(HWND hWnd, UINT flag);
	void Init(void);
	void SetProgressText(UINT progress, UINT progressMax);
};
