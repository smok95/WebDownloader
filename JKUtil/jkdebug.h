/*---------------------------------------------------------------
Name: JKDebug
Last Update: 2007.11.12
Description: 디버그 용도로 만들었다.

2007.09.27 
- Stack OverFlow발생 -> heap 영역으로 변경
2007.10.16
- Debug모드에서만 메시지박스를 사용하도록 변경
2007.10.17
- 파일명 및 함수명 출력전용 매크로 추가
2007.1105
- C에서도 사용가능하도록 변경
2007.11.12
- PrintToWindow : 디버그윈도우가 떠있지않으면 디버그메시지는 한번만 띄운다.
2007.12.11
- Critical Error --> Calloc실패후에도 Free()를 해버리는 바람에 에러발생--> 수정함

2008.01.28
- release 로그메시지 출력용 RLOG 선언
2008.01.29
- true/false 메시지 출력용 매크로추가 ( isTrue() )
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
// VC6버전은 __FUNCTION__을 지원하지 않는다.
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
	MessageBox형 printf라고 보면 될듯 가변인자를 사용하여
	printf사용하듯이 메시지박스를 띄운다.
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
	JK Debug Window라는 프로그램으로 메시지를 전달하여
	해당 윈도우에 디버그 메시지를 출력한다.
	JK Debug Window가 실행중이지않으면 메시지박스를 띄운다.
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