#pragma once
#include "jkreg.h"
#include "WDCommon.h"

static const char WD_REG_PATH[] =	"Software\\JKUtils\\WebDownloader";
static const char WD_REG_DOWN_PATH[] = "DownloadPath";

class CWDReg :
	public CJKReg
{
public:
	CWDReg(void);
	~CWDReg(void);
	BOOL SetProduct(const char* defaultKey=WD_REG_PATH);
	BOOL SetDownloadFolder(const char* downPath=NULL /*if NULL, Set Default Folder*/);
	char* GetDownloadFolder(void);
};
