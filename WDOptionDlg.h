#pragma once
#include "WDCommon.h"
#include "WDReg.h"


// CWDOptionDlg ��ȭ �����Դϴ�.

class CWDOptionDlg : public CDialog
{
	DECLARE_DYNCREATE(CWDOptionDlg)

	CWDReg m_reg;
public:
	CWDOptionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CWDOptionDlg(WDINFO* pInfo);
	virtual ~CWDOptionDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_OPTION};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	void InitControls(void);
	BOOL SetDownloadDirectory(const char* dir);
	
public:
	afx_msg void OnBnClickedButtonChangeDownloadFolder();
};
