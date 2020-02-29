#pragma once
#include "WDCommon.h"
#include "WDReg.h"


// CWDOptionDlg 대화 상자입니다.

class CWDOptionDlg : public CDialog
{
	DECLARE_DYNCREATE(CWDOptionDlg)

	CWDReg m_reg;
public:
	CWDOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CWDOptionDlg(WDINFO* pInfo);
	virtual ~CWDOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OPTION};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	void InitControls(void);
	BOOL SetDownloadDirectory(const char* dir);
	
public:
	afx_msg void OnBnClickedButtonChangeDownloadFolder();
};
