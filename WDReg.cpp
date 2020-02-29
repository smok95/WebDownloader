#include "StdAfx.h"
#include ".\wdreg.h"
#include "JKDir.h"

CWDReg::CWDReg(void)
{
}

CWDReg::~CWDReg(void)
{
}

BOOL CWDReg::SetProduct(const char* defaultKey)
{
	return (CreateKey(HKEY_CURRENT_USER, defaultKey, KEY_CREATE_SUB_KEY)) ? TRUE:FALSE;
}

BOOL CWDReg::SetDownloadFolder(const char* downPath)
{	
	if(SetProduct())
	{
		char tempPath[MAX_PATH]={0,};
		if(downPath==NULL)
		{
			CJKDir dir;
			wsprintf(tempPath, "%s%s", dir.GetSystemDrive(), ::WD_DOWNLOAD_DIRECTORY);
			RLSW("%s : path:%s",F, tempPath);
			downPath = tempPath;
		}
		return this->SetString(HKEY_CURRENT_USER, ::WD_REG_PATH, ::WD_REG_DOWN_PATH, downPath);
	}
	return FALSE;
}


char* CWDReg::GetDownloadFolder(void)
{	return GetString(HKEY_CURRENT_USER, ::WD_REG_PATH, ::WD_REG_DOWN_PATH);	}
