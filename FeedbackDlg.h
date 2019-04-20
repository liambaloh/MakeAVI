#if !defined(AFX_FEEDBACKDLG_H__638C3749_7BD2_4CE7_8E98_ADA9D3AD1759__INCLUDED_)
#define AFX_FEEDBACKDLG_H__638C3749_7BD2_4CE7_8E98_ADA9D3AD1759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeedbackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FeedbackDlg dialog

class FeedbackDlg : public CDialog
{
// Construction
public:
	FeedbackDlg(CWnd* pParent = NULL);   // standard constructor

	void SetText(const char *pszText);
	void SetRange(long min, long max);
	void SetPos(long pos);

	bool m_cancelled;

// Dialog Data
	//{{AFX_DATA(FeedbackDlg)
	enum { IDD = IDD_FEEDBACK };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FeedbackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Wait4Paint();

	// Generated message map functions
	//{{AFX_MSG(FeedbackDlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEEDBACKDLG_H__638C3749_7BD2_4CE7_8E98_ADA9D3AD1759__INCLUDED_)
