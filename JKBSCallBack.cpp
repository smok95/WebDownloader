#include "StdAfx.h"
#include ".\jkbscallback.h"
#include "JKDebug.h"
#include <time.h>

CJKBSCallBack::CJKBSCallBack(void)
{
	Init();
}

CJKBSCallBack::~CJKBSCallBack(void)
{
	m_pPC = NULL;
}

HRESULT CJKBSCallBack::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{	
	RLSW("%s : ulProgress:%d, ulProgressMax:%d, ulStatusCode:%d, statusText:%s",
		F, ulProgress, ulProgressMax, ulStatusCode, szStatusText);

	RLSW("%d",*m_bCancel);
	if(*m_bCancel)	return E_ABORT;
	
	float pos = 0;
	if(ulProgressMax>0)
	{
		pos = ((float)ulProgress/(float)ulProgressMax) *100;
		
	}
	if(m_pPC)
	{		
		m_pPC->SetPos((int)pos);
	}

	SetProgressText((UINT)ulProgress, (UINT)ulProgressMax);

	return S_OK;
}
void CJKBSCallBack::SetProgressCtrl(CProgressCtrl* pPC)
{
	m_pPC = NULL;
	if(pPC)
		m_pPC = pPC;
}

void CJKBSCallBack::SetPercentTextWnd(HWND hWnd)
{
	m_hPercentWnd = NULL;
	if(hWnd)
		m_hPercentWnd = hWnd;
}

void CJKBSCallBack::SetCancelFlag(BOOL* bCancel){	m_bCancel = bCancel;	}

BOOL CJKBSCallBack::SetTextWindow(HWND hWnd, UINT flag)
{
	NullReturn(hWnd) FALSE;
	switch(flag)
	{
	case STATE_PERCENT:
		SetPercentTextWnd(hWnd);
		return TRUE;
		break;
	case STATE_SIZE:
		m_hSizeWnd = hWnd;
		return TRUE;
		break;
	case STATE_SPEED:
		m_hSpeedWnd = hWnd;
		break;
	case STATE_TIME:
		m_hTimeWnd = hWnd;
		break;
	}
	return FALSE;
}

void CJKBSCallBack::Init(void)
{
	SetProgressCtrl(NULL);	
	SetPercentTextWnd(NULL);
	m_bCancel = NULL;
	m_hSpeedWnd = NULL;
	m_hSizeWnd = NULL;
	m_hTimeWnd = NULL;
	m_startTime = time(NULL);
}

void CJKBSCallBack::SetProgressText(UINT progress, UINT progressMax)
{
	float pos = 0;
	if(progressMax>0)
	{
		pos = ((float)progress/(float)progressMax) *100;

	}

	if(m_hPercentWnd)
	{
		ShowWindow(m_hPercentWnd, SW_SHOW);
		char percent[20]={0,};
		wsprintf(percent, "%d %%",(int)pos);
		SetWindowText(m_hPercentWnd, percent);
	}

	if(m_hSizeWnd)
	{
		ShowWindow(m_hSizeWnd, SW_SHOW);
		char size[50]={0,};
		wsprintf(size,"%d Kb / %d Kb", progress/1000, progressMax/1000);
		SetWindowText(m_hSizeWnd, size);
	}

	if(m_hSpeedWnd)
	{
		ShowWindow(m_hSpeedWnd, SW_SHOW);
		char speed[20]={0,};
		time_t curTime;
		time(&curTime);

		float elapsedTime = (float)(curTime - m_startTime);

		wsprintf(speed, "%d Kb/Sec", (int)(((float)progress/elapsedTime)/1000));
		SetWindowText(m_hSpeedWnd, speed);
	}

}
