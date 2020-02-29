/*---------------------------------------------------------------
Name: JKDebug
Last Update: 2007.11.12
Description: ����� �뵵�� �������.

2007.09.27 
- Stack OverFlow�߻� -> heap �������� ����
2007.10.16
- Debug��忡���� �޽����ڽ��� ����ϵ��� ����
2007.10.17
- ���ϸ� �� �Լ��� ������� ��ũ�� �߰�
2007.1105
- C������ ��밡���ϵ��� ����
2007.11.12
- PrintToWindow : ����������찡 ������������ ����׸޽����� �ѹ��� ����.
2007.12.11
- Critical Error --> Calloc�����Ŀ��� Free()�� �ع����� �ٶ��� �����߻�--> ������

2008.01.28
- release �α׸޽��� ��¿� RLOG ����
2008.01.29
- true/false �޽��� ��¿� ��ũ���߰� ( isTrue() )
---------------------------------------------------------------*/
#ifndef __JKDEBUG_H__
#define __JKDEBUG_H__
#pragma once

#ifndef __WINDOWS_H__
#define __WINDOWS_H__
#include <windows.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void PrintToWindow(const char *fmt,...);
static void PrintToMessageBox(const char *fmt,...);


/**********************************************************************************************************/
// VC6������ __FUNCTION__�� �������� �ʴ´�.
#if defined (_MSC_VER) && (_MSC_VER <= 1200)
	#define F "F macro is not supported at VC6" 
#else
	#define F __FUNCTION__
#endif

#define MAX_LENGTH	100000
#define isTrue(x)	(x)? ("true"):("false")
#define NullReturn(x) if((x)==NULL) return
#define FalseReturn(x) if((x)==FALSE) return
#define JKASSERT(x)	if((x)==FALSE)	PrintToWindow("Assert Faild : %s\r\n%s",F, ##x); return FALSE

#ifdef _DEBUG
#define IFDEBUG		PrintToMessageBox
#define IFDEBUGW PrintToWindow
#define RLSW		
#define RLOG
#define JKCALL	PrintToWindow("%s Call",F)

#elif NDEBUG
#define IFDEBUG		
#define IFDEBUGW
#define RLSW		PrintToWindow
#define RLOG		PrintToWindow
#define JKCALL	PrintToWindow("%s Call",F)
#else

#define IFDEBUG		
#define IFDEBUGW	
#define RLSW			
#define RLOG
#define JKCALL
#define F
#endif
/**********************************************************************************************************/

#ifdef __cplusplus
#define JKDBGTry	try{
#define JKDBGCatch }catch(...){	RLSW("JKDebug Exception occured");	}
#define JKTry	try{
#define JKCatch	}catch(...){	RLSW("Exception occured : file:%s, function:%s, line:%d",__FILE__,F,__LINE__);}
#else
#define JKDBGTry
#define JKDBGCatch
#define JKTry
#define JKCatch
#endif//__cplusplus


/*---------------------------------------------------------------
Name: PrintToMessageBox
Last Update: 2007.09.26
Parameters: 
Description:
	MessageBox�� printf��� ���� �ɵ� �������ڸ� ����Ͽ�
	printf����ϵ��� �޽����ڽ��� ����.
---------------------------------------------------------------*/
static void PrintToMessageBox(const char *fmt,...)
{	
	JKDBGTry
	char *buff = NULL;
	char* msg = NULL;
	HWND hWnd = NULL;
	buff= (char*)calloc(1, MAX_LENGTH);
	
	if(buff)
	{
		if(fmt != NULL)
		{
			va_list argptr;
			va_start(argptr,fmt);
			vsprintf(buff,fmt,argptr);
			va_end(argptr);
		}
		else	strcpy(buff,"JK Debug : null");
		buff[MAX_LENGTH-1] = 0;
	}
	
	hWnd = GetActiveWindow();
	if(buff) msg = buff;
	else msg = "Failed to Allocate Memory";
	::MessageBoxA(NULL, msg, "JK Debug MessageBox",MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

	free(buff);
	JKDBGCatch
}

/*---------------------------------------------------------------
Name: PrintToWindow
Last Update: 2007.11.12
Parameters: 
Description:
	JK Debug Window��� ���α׷����� �޽����� �����Ͽ�
	�ش� �����쿡 ����� �޽����� ����Ѵ�.
	JK Debug Window�� ���������������� �޽����ڽ��� ����.
---------------------------------------------------------------*/
static void PrintToWindow(const char *fmt,...)
{		
	JKDBGTry
	HWND hWnd = NULL;
	hWnd = FindWindowA(NULL,"JK Debug Window");
	if(hWnd == NULL) return;
	char *buff = (char*)calloc(1, MAX_LENGTH);
	
	if(buff)
	{
		if(fmt != NULL)
		{
			va_list argptr;
			va_start(argptr,fmt);
			if(vsprintf(buff,fmt,argptr)==NULL)
			{
				free(buff);
				return;
			}
			va_end(argptr);	
		}
		else	strcpy(buff,"JK Debug : null");
		buff[MAX_LENGTH-1] = 0;
	}
	
	if(hWnd)
	{		
		char* msg = NULL;
		if(buff) msg = buff;
		else msg = "Failed to Allocate Memory";
		COPYDATASTRUCT data={0,};
		data.cbData = (DWORD)(strlen(msg)+1);
		data.lpData = (char*)msg;
		SendMessage(hWnd,WM_COPYDATA,0,(LPARAM)&data);
	}
#ifdef _DEBUG
	else
	{
		static BOOL isFirst = FALSE;
		if(!isFirst)
		{
			HWND hWnd = NULL;
			char* msg=NULL;
			if(buff)
			{
			isFirst = TRUE;	
			msg = "Please Execute Debug Window";
			hWnd = GetActiveWindow();
			MessageBoxA(hWnd,buff,msg,MB_OK|MB_SETFOREGROUND);
		}		
	}
	}
#endif
	free(buff);
	JKDBGCatch
}




#endif //__JKDEBUG_H__