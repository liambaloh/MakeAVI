// FeedbackDlg.cpp : implementation file
// This is a very simple dialog box, used to display feedback during a long
// process.  All it has is one text line and a progress bar.
// the only special thing about it is that it has a message pump, and every
// time it gets updated information, it runs the pump until there are no more
// messages.  This makes it paint even when there are no messages getting
// processed the normal way.  However, painting only happens when data is
// changed, so try to call it fairly often (once a second or so).

#include "stdafx.h"
#include "makeavi.h"
#include "FeedbackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FeedbackDlg dialog


FeedbackDlg::FeedbackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FeedbackDlg::IDD, pParent),
	m_cancelled(false)
{
	//{{AFX_DATA_INIT(FeedbackDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void FeedbackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FeedbackDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FeedbackDlg, CDialog)
	//{{AFX_MSG_MAP(FeedbackDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FeedbackDlg message handlers

void FeedbackDlg::SetText(const char *pszText)
{
	GetDlgItem(IDC_FEEDBACK_TEXT)->SetWindowText(pszText);
	Wait4Paint();
}

void FeedbackDlg::SetRange(long min, long max)
{
	m_Progress.SetRange(min,max);
	Wait4Paint();
}

void FeedbackDlg::SetPos(long pos)
{
	m_Progress.SetPos(pos);
	Wait4Paint();
}

void FeedbackDlg::Wait4Paint()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
        {
            m_cancelled = true;
			return;   // terminate if WM_QUIT received
        }
    }
}


void FeedbackDlg::OnCancel() 
{
	m_cancelled = true;
}
