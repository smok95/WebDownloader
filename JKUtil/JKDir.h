#pragma once
#include "jkobject.h"
#include <shlwapi.h >
#include <shlobj.h>
#pragma comment(lib,"shlwapi.lib")

class CJKDir: public CJKObject
{
public:
	CJKDir(void){}
	virtual ~CJKDir(void){}

	BOOL CreateDirectories(char* path);
	BOOL GetOnlyPathName(char* fileFullPath);
	char* GetSystemDrive(void);
	char* GetPathFromBrowser(const char* browserTitle=NULL, HWND hWndOwner=NULL);
	char* GetClassName(void)	{	return CLASS_NAME(CJKDir);		}
};

/***************************************************************************************************************************/
inline char* CJKDir::GetPathFromBrowser(const char* browserTitle, HWND hWndOwner)
{
	LPITEMIDLIST pidlBrowse=NULL;
	BROWSEINFO BRinfo={0,};
	New(MAX_PATH);
	//::GetCurrentDirectory(MAX_PATH, (LPSTR)m_buffer);
	if(hWndOwner==NULL)	hWndOwner = ::GetActiveWindow();
	BRinfo.hwndOwner = hWndOwner;			// 부모 윈도우의 핸들
	BRinfo.pidlRoot = NULL;							// 브라우징 할 루트 디렉토리
	BRinfo.pszDisplayName = (LPSTR)m_buffer;			// 선택할 디렉토리가 저장될 버퍼
	BRinfo.lpszTitle = browserTitle;					// 출력 문자열
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;  // 디렉토리 선택 옵션
	BRinfo.lpfn = NULL;								   // 이벤트에 대한 사용자 정의 처리 함수
	BRinfo.lParam = 0;									 // 처리 함수에 넘겨질 인수

	::OleInitialize(NULL);
	pidlBrowse = SHBrowseForFolder(&BRinfo); // 다이얼로그를 띄운다.

	if(pidlBrowse != NULL) 
	{		
		SHGetPathFromIDList(pidlBrowse, (LPSTR)this->m_buffer); // 패스를 얻어온다.
	}
	::OleUninitialize();
	return (char*)m_buffer;
}
inline BOOL CJKDir::CreateDirectories(char* path)
{
	NullReturn(path) FALSE;

	char DirName[MAX_PATH]={0,};  //생성할 디렉초리 이름 
	char* p = path;     //인자로 받은 디렉토리 
	char* q = DirName;   

	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))   //루트디렉토리 혹은 Sub디렉토리 
		{
			if (':' != *(p-1)) 
				::CreateDirectory(DirName,NULL);            
		}
		*q++ = *p++;
		*q = '\0';
	}
	::CreateDirectory(DirName,NULL);            

	return PathFileExists(path);	
}

inline BOOL CJKDir::GetOnlyPathName(char* fileFullPath)
{
	if(!fileFullPath) return FALSE;

	size_t len =  strlen(fileFullPath);
	for(size_t index = len;index>=0;index--)
	{
		if(fileFullPath[index] == '\\')
		{
			fileFullPath[index+1] = 0;
			return TRUE;
		}
	}
	return FALSE;
}

inline char* CJKDir::GetSystemDrive(void)
{
	New(MAX_PATH);
	if(GetSystemDirectory((char*)m_buffer,MAX_PATH) == 0) return NULL;
	size_t len = strlen((char*)m_buffer);
	for(size_t index=0;index<len;index++)
	{
		if(m_buffer[index] == '\\')
		{
			m_buffer[index+1] = 0;
			break;
		}
	}
	return (char*)m_buffer;
}
