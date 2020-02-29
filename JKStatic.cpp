#include "StdAfx.h"
#include ".\jkstatic.h"
#include "JKDebug.h"
#include "resource.h"

CJKStatic::CJKStatic(void)
{
}

CJKStatic::~CJKStatic(void)
{
}
BEGIN_MESSAGE_MAP(CJKStatic, CStatic)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CJKStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	RLSW("%s Call",F);
	CDC* pDC = this->GetDC();
	if(pDC)
	{
		CString str;
		this->GetWindowText(str);
		RLSW(str.GetBuffer());
		pDC->SetTextColor(RGB(255,0,0));
		UpdateWindow();
		
	}
	CStatic::OnMouseMove(nFlags, point);
}
