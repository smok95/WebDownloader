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
	BRinfo.hwndOwner = hWndOwner;			// �θ� �������� �ڵ�
	BRinfo.pidlRoot = NULL;							// ����¡ �� ��Ʈ ���丮
	BRinfo.pszDisplayName = (LPSTR)m_buffer;			// ������ ���丮�� ����� ����
	BRinfo.lpszTitle = browserTitle;					// ��� ���ڿ�
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;  // ���丮 ���� �ɼ�
	BRinfo.lpfn = NULL;								   // �̺�Ʈ�� ���� ����� ���� ó�� �Լ�
	BRinfo.lParam = 0;									 // ó�� �Լ��� �Ѱ��� �μ�

	::OleInitialize(NULL);
	pidlBrowse = SHBrowseForFolder(&BRinfo); // ���̾�α׸� ����.

	if(pidlBrowse != NULL) 
	{		
		SHGetPathFromIDList(pidlBrowse, (LPSTR)this->m_buffer); // �н��� ���´�.
	}
	::OleUninitialize();
	return (char*)m_buffer;
}
inline BOOL CJKDir::CreateDirectories(char* path)
{
	NullReturn(path) FALSE;

	char DirName[MAX_PATH]={0,};  //������ ���ʸ� �̸� 
	char* p = path;     //���ڷ� ���� ���丮 
	char* q = DirName;   

	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))   //��Ʈ���丮 Ȥ�� Sub���丮 
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
