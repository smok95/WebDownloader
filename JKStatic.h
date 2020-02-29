#pragma once
#include "afxwin.h"

class CJKStatic :
	public CStatic
{
public:
	CJKStatic(void);
	~CJKStatic(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
