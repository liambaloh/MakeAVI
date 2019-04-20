/////////////////////////////////////////////////////////////////////////////
// CImagePreview
// this is a class that provides an image preview on a dialog box.
// this class relies on the FreeImage library, and can work with
// any file type supported by that library.
//
// sample use:
// Create a static text control on the dialog, call it IDC_PREVIEW_AREA (for example)
// This control is not actually used, its only purpose is to show this control how big
// it's supposed to be.
//
// In your dialog.h file:
// CImagePreview m_Preview;
//
// In OnInitDialog, put in this code:
//	// set up the preview area
//	{
//		CRect rDemo;
//		GetDlgItem(IDC_PREVIEW_AREA)->GetWindowRect(rDemo);
//		ScreenToClient(&rDemo);
//		GetDlgItem(IDC_PREVIEW_AREA)->ShowWindow(SW_HIDE);
//		m_Preview.Create(NULL,"ImagePreview",WS_CHILD,rDemo,this,IDC_PREVIEW_AREA);
//		m_Preview.ShowWindow(SW_SHOW);
//	}
// 
// then, when you want to display a new picture:
//	    m_Preview.SetFilename(filename);


#include "StdAfx.h"
#include "ImagePreview.h"
#include "malloc.h"


CImagePreview::CImagePreview() :
    m_bLoaded(FALSE),
	m_hImage(NULL),
	m_hDrawDib(NULL),
	m_bPaletteCreated(FALSE)
{}

CImagePreview::~CImagePreview()
{}

BEGIN_MESSAGE_MAP(CImagePreview, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImagePreview message handlers
void CImagePreview::SetFilename(CString const &Filename)
{
	if (m_hImage != NULL)
	{
		FreeImage_Free(m_hImage);
		m_hImage = NULL;
	}
	// If it is empty, just return and clear out the picture...
	if (Filename.IsEmpty())
	{
		m_bLoaded = false;
		return;
	}

	m_strFilename = Filename;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(m_strFilename, -1);
	m_hImage = FreeImage_Load(fif,m_strFilename);

	m_bLoaded = (m_hImage >= 0);
	Invalidate();
}


// private utility functions
HDRAWDIB CImagePreview::GetDrawDibContext() const
{
    if (m_hDrawDib) return m_hDrawDib;
    m_hDrawDib = DrawDibOpen();
    return m_hDrawDib;
}


// stretch to DstRect
void CImagePreview::StretchDraw(CDC & DC, const CRect & DstRect)
{
    CreateMyPalette();
    HDRAWDIB hDrawDib = GetDrawDibContext();
    if (hDrawDib)
    {
        DrawDibSetPalette(hDrawDib, (HPALETTE)m_Palette);
        DrawDibRealize( hDrawDib,  DC.GetSafeHdc(),  FALSE);
        BOOL bResult = DrawDibDraw(hDrawDib, DC.GetSafeHdc(), 
                              DstRect.left, DstRect.top, 
                              DstRect.Width(),DstRect.Height(), 
                              FreeImage_GetInfoHeader(m_hImage), FreeImage_GetBits(m_hImage), 
                              0, 0, FreeImage_GetWidth(m_hImage), FreeImage_GetHeight(m_hImage), 
                              0);
    }
}

void CImagePreview::CreateMyPalette() const
{
    if( !m_hImage)
        return;

    if( !m_bPaletteCreated)
        return;
    else
        m_bPaletteCreated = TRUE;
    
    RGBQUAD *pRgb = FreeImage_GetPalette(m_hImage);
    unsigned int count = FreeImage_GetColorsUsed(m_hImage);

    int PalSize = sizeof(LOGPALETTE) + (count-1)*sizeof(PALETTEENTRY);
    LOGPALETTE *pPaletteInfo = reinterpret_cast<LOGPALETTE *>(_alloca(PalSize));
    pPaletteInfo->palVersion = 0x300;
    pPaletteInfo->palNumEntries = count;
                        
    for (unsigned int i = 0; i < count; i++)
    {
        pPaletteInfo->palPalEntry[i].peRed   = pRgb[i].rgbRed;
        pPaletteInfo->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
        pPaletteInfo->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
        pPaletteInfo->palPalEntry[i].peFlags = 0;
    }

    // Create Palette!
    m_Palette.CreatePalette(pPaletteInfo);
}

void CImagePreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    CRect rWnd;
    GetClientRect(&rWnd);
    rWnd.top += 10; // offset from top of dialog
    // erase background
    dc.FillSolidRect(&rWnd, GetSysColor(COLOR_3DFACE));

    if (!m_bLoaded)
        return;

	BITMAPINFOHEADER *lpbi = FreeImage_GetInfoHeader(m_hImage);
    int nWidth = (int) lpbi->biWidth;
    int nHeight = (int) lpbi->biHeight;

    if (nWidth > 0)   // maybe nothing to draw yet (if no label to select)
        {
        // rescale the demo area to be the same aspect ratio as the label
        // we're previewing
        // recalc our pixel size for the ratio we've been set to
        ASSERT(-1 != nWidth);
        ASSERT(-1 != nHeight);
        float LabelRatio = (float)nWidth / (float)nHeight;
        float ScreenRatio = (float)rWnd.Width() / (float)rWnd.Height();
        if (LabelRatio > ScreenRatio)
            { // flatten top to bottom
            int NewHeight = (int)((float)rWnd.right/LabelRatio);
            // amount to move down for centering
            int dY = (rWnd.Height() - NewHeight) / 2;
            rWnd.top += dY;
            rWnd.bottom = rWnd.top + NewHeight;
            }
        else
            { // flatten side to side
            int NewWidth = (int)((float)rWnd.bottom*LabelRatio);
            // amount to move right for centering
            int dX = (rWnd.Width() - NewWidth) / 2;
            rWnd.left += dX;
            rWnd.right = rWnd.left + NewWidth;
            }
        }

    // draw white background
    dc.DrawEdge(rWnd,EDGE_SUNKEN,BF_RECT);
    // shrink rect to avoid overwriting our pretty edge
    rWnd += CRect(-2,-2,-2,-2);

    if (0 == nWidth)
        return;

	StretchDraw(dc, rWnd);
}
