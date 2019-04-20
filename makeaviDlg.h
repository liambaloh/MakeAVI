// makeaviDlg.h : header file
//

#if !defined(AFX_MAKEAVIDLG_H__4BA16E5E_2AD6_46ED_A494_565DEA02AC9D__INCLUDED_)
#define AFX_MAKEAVIDLG_H__4BA16E5E_2AD6_46ED_A494_565DEA02AC9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "freeimage.h"
#include "ImagePreview.h"
#include "feedbackdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMakeaviDlg dialog

class CSortStringArray : public CStringArray {
public:
	void Sort();
private:
	BOOL CompareAndSwap(int pos);
};


class CMakeaviDlg : public CDialog
{
// Construction
public:
	CMakeaviDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeaviDlg)
	enum { IDD = IDD_MAKEAVI_DIALOG };
	CListCtrl	m_FileList;
	int		m_FrameRate;
	int		m_CropX;
	int		m_CropY;
	BOOL	m_EnableCropping;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeaviDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CSortStringArray m_Files;
	bool m_bDispBufferLocked;
	CString m_strDispBuffer;
	CImagePreview m_Preview;

	void ResizeColumn(CString str);
	void UseItemSize(int idx);

	// Generated message map functions
	//{{AFX_MSG(CMakeaviDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetCompression();
	afx_msg void OnPickFiles();
	afx_msg void OnFileup();
	afx_msg void OnFiledown();
	afx_msg void OnGetdispinfoFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSortFiles();
	virtual void OnOK();
	afx_msg void OnEnableCropping();
	afx_msg void OnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKEAVIDLG_H__4BA16E5E_2AD6_46ED_A494_565DEA02AC9D__INCLUDED_)
