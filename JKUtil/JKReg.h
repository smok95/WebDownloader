/*******************************************************************************************************************
	CJKReg Class

#	2008. 1. 1
		- CreateKey, SetString 추가
#	2008.1.2
		- SetDword 추가
#	2008.01.09
		- BOOL GetDword(HKEY rootKey, const char* subKey, const char* valueName, DWORD* pOut) 추가
		- BOOL GetValue 추가
#	2008.01.10
		- GetString 추가
#	2008.01.14
		- DeleteValue 추가
#	2008.01.30
		- RegDeleteSubKey 설정 변경(서브키도 함께 삭제되도록)
			- ex. HKLM/Software/test : test이하의 키와데이터만 삭제 --> test까지 삭제

********************************************************************************************************************/
#pragma once
#include <windows.h>
#include <assert.h>

#define MAX_VALUE_SIZE		1024
static const char* REG_KNOWN_STARTUP = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

class CJKReg
{
	HKEY m_key;
	BYTE m_buffer[MAX_VALUE_SIZE];
public:
	CJKReg(void){	m_key = NULL;	}
	~CJKReg(void);
	HKEY& GetKey();
	// 키를 연다.
	HKEY OpenKey(HKEY rootKey, const char* subKey, REGSAM samDesired=KEY_ALL_ACCESS);
	HKEY CreateKey(HKEY rootKey, const char* subKey, REGSAM samDesired=KEY_ALL_ACCESS, LPDWORD lpDisposition=NULL);
	BOOL SetString(HKEY key, const char* valueName, const char* data);
	BOOL SetString(HKEY rootKey, const char* subKey, const char* valueName, const char* data, BOOL bCreateKey=TRUE);
	BOOL SetDword(HKEY key, const char* valueName, DWORD data);
	BOOL SetDword(HKEY rootKey, const char* subKey, const char* valueName, DWORD data, BOOL bCreateKey=TRUE);
	BOOL DeleteValue(HKEY rootKey, const char* subKey, const char* valueName);

	BOOL GetValue(HKEY rootKey, const char* subKey, const char* valueName, DWORD type, BYTE* pData, DWORD dataSize);
	BOOL GetDword(HKEY rootKey, const char* subKey, const char* valueName, DWORD* pOut);
	char*	GetString(HKEY rootKey, const char* subKey, const char* valueName);
		
	// 키를 해제한다.
	BOOL CloseKey(HKEY& key);
	// 서브키이하의 모든키 및 value들을 몽땅 삭제한다.
	BOOL RegDeleteSubKey(HKEY rootKey, const char* subKey, BOOL is32BitPlatform=TRUE);
};

//----------------------------------------------------------------------------------------------------------------------
inline CJKReg::~CJKReg()
{
	CloseKey(m_key);
}
inline HKEY& CJKReg::GetKey()
{
	return m_key;
}
inline BOOL CJKReg::CloseKey(HKEY& key)
{
	BOOL ret = FALSE;
	if(key)
	{
		if(RegCloseKey(key) == ERROR_SUCCESS) ret = TRUE;
		key = NULL;
	}

	return ret;  
}

inline HKEY CJKReg::CreateKey(HKEY rootKey, const char* subKey, REGSAM samDesired, LPDWORD lpDisposition)
{
	assert(rootKey != NULL && subKey != NULL);

	CloseKey(m_key);

	if(RegCreateKeyEx(rootKey, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, &m_key, lpDisposition)
		== ERROR_SUCCESS)
	{
		return m_key;
	}
	return NULL;
}

inline HKEY CJKReg::OpenKey(HKEY rootKey, const char* subKey, REGSAM samDesired)
{
	assert(rootKey != NULL && subKey != NULL);

	// 혹시 키가 열려있을 수 있으니 삭제를 한다.
	CloseKey(m_key);

	if(RegOpenKeyEx(rootKey, subKey, 0, samDesired, &m_key) == ERROR_SUCCESS)		
		return m_key;
	else	return NULL;
}

inline BOOL CJKReg::DeleteValue(HKEY rootKey, const char* subKey, const char* valueName)
{
	assert(rootKey != NULL && subKey != NULL && valueName != NULL);
	if(OpenKey(rootKey, subKey, KEY_SET_VALUE)!=NULL)
	{
		BOOL ret = (RegDeleteValue(m_key, valueName) == ERROR_SUCCESS)? TRUE:FALSE;
		CloseKey(m_key);
		return ret;
	}

	return FALSE;
}

inline BOOL CJKReg::SetString(HKEY key, const char* valueName, const char* data)
{
	assert(key != NULL && valueName != NULL && data != NULL);
    
	if(RegSetValueEx(key, valueName, 0, REG_SZ, (BYTE*)data, (DWORD)strlen(data)) == ERROR_SUCCESS)
		return TRUE;
	else
		return FALSE;	
}

inline BOOL CJKReg::SetDword(HKEY key, const char* valueName, DWORD data)
{
	assert(key != NULL && valueName != NULL);

	if(RegSetValueEx(key, valueName, 0, REG_DWORD, (BYTE*)&data, sizeof(data)) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

inline BOOL CJKReg::RegDeleteSubKey(HKEY rootKey, const char* subKey, BOOL is32BitPlatform)
{
	LONG result = 0;
	HKEY key=NULL;
	result = RegOpenKeyEx(rootKey, subKey, 0, KEY_ALL_ACCESS, &key);
	if(result != ERROR_SUCCESS)	
	{	
		return FALSE;
	}
	
	if(key)
	{
		DWORD keyIndex = 0;
		while(1)
		{
			char keyName[MAX_VALUE_SIZE]={0,};
			REGSAM samDesired = is32BitPlatform? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
			PFILETIME lpLastWriteTime=NULL;
            DWORD cbName = MAX_VALUE_SIZE;
			result = RegEnumKeyEx(key, keyIndex, keyName, &cbName, 0, NULL, NULL, lpLastWriteTime);
			switch(result)
			{
			case ERROR_SUCCESS:
				{
					LONG errNo = 0;
					errNo = RegDeleteKey(key, keyName);
					if(errNo != ERROR_SUCCESS)
					{
						char tempSubKey[MAX_VALUE_SIZE];
						wsprintf(tempSubKey, "%s\\%s", subKey, keyName);
						RegDeleteSubKey(rootKey, tempSubKey, is32BitPlatform);			
					}
				}break;
			case ERROR_NO_MORE_ITEMS:
				RegDeleteKey(rootKey, subKey);
				RegCloseKey(key);
				return TRUE;
			default:
				RegCloseKey(key);
				return FALSE;
			}
		}
	}	
	RegCloseKey(key);
	return FALSE;
}

inline BOOL CJKReg::SetDword(HKEY rootKey, const char* subKey, const char* valueName, DWORD data, BOOL bCreateKey)
{
	assert(rootKey!=NULL && subKey!=NULL && valueName != NULL);
	CloseKey(m_key);
	if(bCreateKey)
	{
		if(CreateKey(rootKey,subKey, KEY_WRITE) == NULL) return FALSE;
	}
	else
	{
		if(OpenKey(rootKey, subKey, KEY_SET_VALUE) == NULL) return FALSE;
	}
	
	BOOL ret = SetDword(m_key, valueName, data);
	CloseKey(m_key);
	return ret;
}

inline BOOL CJKReg::SetString(HKEY rootKey, const char* subKey, const char* valueName, const char* data, BOOL bCreateKey)
{
	assert(rootKey != NULL && subKey != NULL && valueName != NULL && data != NULL);
	CloseKey(m_key);

	if(bCreateKey)
	{
		if(CreateKey(rootKey, subKey, KEY_WRITE) == NULL) return FALSE;
	}
	else
	{
		if(OpenKey(rootKey, subKey, KEY_SET_VALUE) == NULL) return FALSE;
	}
    
	BOOL ret = SetString(m_key, valueName, data);
	CloseKey(m_key);
	return ret;
}

inline BOOL CJKReg::GetValue(HKEY rootKey, const char* subKey, const char* valueName, DWORD type, BYTE* pData, DWORD dataSize)
{
	assert(rootKey != NULL && subKey != NULL && valueName != NULL && pData != NULL);
	CloseKey(m_key);
	if(OpenKey(rootKey, subKey, KEY_READ))
	{
		BOOL ret = FALSE;
		ret = (RegQueryValueEx(m_key, valueName, NULL, &type, pData, &dataSize)==ERROR_SUCCESS);
		CloseKey(m_key);
		return ret;
	}
	return FALSE;
}
inline char*	CJKReg::GetString(HKEY rootKey, const char* subKey, const char* valueName)
{	
	assert(rootKey != NULL && subKey != NULL && valueName != NULL);
	memset(m_buffer,0, MAX_VALUE_SIZE);
	if(GetValue(rootKey, subKey, valueName, REG_SZ, (BYTE*)m_buffer, MAX_VALUE_SIZE))
	{
		return (char*)m_buffer;
	}
	return NULL;
}

inline BOOL CJKReg::GetDword(HKEY rootKey, const char* subKey, const char* valueName, DWORD* pOut)
{
	assert(rootKey != NULL && subKey != NULL && valueName != NULL && pOut != NULL);
	if(GetValue(rootKey, subKey, valueName, REG_DWORD, (BYTE*)pOut, sizeof(DWORD)))
	{
		return TRUE;
	}
	return FALSE;
}