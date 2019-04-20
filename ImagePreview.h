/////////////////////////////////////////////////////////////////////////////
// CImagePreview
// This is a self-drawing image display control


#ifndef _CIMAGEPREVIEW_H_
#define _CIMAGEPREVIEW_H_

#include <vfw.h>    // contains DRAWDIB stuff
#include "freeimage.h"

class CImagePreview : public CWnd
{
// Construction
public:
	CImagePreview();
    void SetFilename(CString const &Filename);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImagePreview)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImagePreview();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImagePreview)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void StretchDraw(CDC & DC, const CRect & DstRect);
	void CreateMyPalette() const;
	HDRAWDIB GetDrawDibContext() const;

    CString		m_strFilename;
    FIBITMAP   *m_hImage;
    BOOL		m_bLoaded;
    mutable BOOL	    m_bPaletteCreated;
    mutable CPalette    m_Palette;
	mutable HDRAWDIB m_hDrawDib;
};

#endif // _CIMAGEPREVIEW_H_
