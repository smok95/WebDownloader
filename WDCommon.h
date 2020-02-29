#pragma  once

#define NullReturn(x)	if((x)==NULL)	return
#define HD_MAX_PATH	4096
static const char WD_DOWNLOAD_DIRECTORY[] = "WebDownloadFiles";

typedef struct WDINFO
{
	char downloadDir[HD_MAX_PATH];
}WDINFO;