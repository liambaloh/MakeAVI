// makeaviDlg.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include <vfw.h>
#include "makeavi.h"
#include "makeaviDlg.h"
#include "freeimage.h"
#include "apiavi.h"
#include "FeedbackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ----------------------------------------------------------
//   cropping
// this code stolen from version 1.4.3 of FreeImage; for some reason
// it's not in the newer versions
// ----------------------------------------------------------
inline int
CalculateLine(int width, int bitdepth) {
	return ((width * bitdepth) + 7) / 8;
}

inline int
CalculatePitch(int line) {
	return line + 3 & ~3;
}

FIBITMAP *
FreeImage_Crop(FIBITMAP *dib, int left, int top, int right, int bottom) {
	if (dib) {
		BYTE *bits = FreeImage_GetBits(dib) + ((FreeImage_GetHeight(dib) - top - (bottom - top)) * FreeImage_GetPitch(dib));
		
		if (right > left && bottom > top) {
			FIBITMAP *clip = FreeImage_Allocate(right - left, bottom - top, FreeImage_GetBPP(dib));
			BYTE *clip_bits = FreeImage_GetBits(clip);

			int line_large = CalculateLine(FreeImage_GetWidth(dib), FreeImage_GetBPP(dib));
			int pitch_large = CalculatePitch(line_large);

			int line = CalculateLine(right - left, FreeImage_GetBPP(dib));
			int pitch = CalculatePitch(line);

			for (int y = 0; y < bottom - top; y++) {
				memcpy(clip_bits + y * pitch, bits + (y * pitch_large) + (left * sizeof(RGBTRIPLE)), line);
			}

			return clip;
		}		
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeaviDlg dialog

CMakeaviDlg::CMakeaviDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeaviDlg::IDD, pParent),
	m_bDispBufferLocked(false),
	m_EnableCropping(FALSE)
{
	//{{AFX_DATA_INIT(CMakeaviDlg)
	m_FrameRate = 0;
	m_CropX = 0;
	m_CropY = 0;
	m_EnableCropping = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeaviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeaviDlg)
	DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
	DDX_Text(pDX, IDC_FRAMERATE, m_FrameRate);
	DDX_Text(pDX, IDC_XRES, m_CropX);
	DDX_Text(pDX, IDC_YRES, m_CropY);
	DDX_Check(pDX, IDC_ENABLE_CROPPING, m_EnableCropping);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakeaviDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeaviDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PICK_FILES, OnPickFiles)
	ON_BN_CLICKED(IDC_FILEUP, OnFileup)
	ON_BN_CLICKED(IDC_FILEDOWN, OnFiledown)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_FILE_LIST, OnGetdispinfoFileList)
	ON_BN_CLICKED(IDC_SORT_FILES, OnSortFiles)
	ON_BN_CLICKED(IDC_ENABLE_CROPPING, OnEnableCropping)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, OnItemchangedFileList)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnDblclkFileList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeaviDlg message handlers

BOOL CMakeaviDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	// set up listbox
	RECT ListRect;
	m_FileList.GetClientRect(&ListRect);
    m_FileList.InsertColumn(0,"");
	m_FileList.SetColumnWidth(0,ListRect.right - ListRect.left);

	FreeImage_Initialise();
	// set up the preview area
	{
		CRect rDemo;
		GetDlgItem(IDC_PREVIEW_AREA)->GetWindowRect(rDemo);
		ScreenToClient(&rDemo);
		GetDlgItem(IDC_PREVIEW_AREA)->ShowWindow(SW_HIDE);
		m_Preview.Create(NULL,"ImagePreview",WS_CHILD,rDemo,this,IDC_PREVIEW_AREA);
		m_Preview.ShowWindow(SW_SHOW);
	}

	GetDlgItem(IDC_XRES)->EnableWindow(FALSE);
	GetDlgItem(IDC_YRES)->EnableWindow(FALSE);

	m_FrameRate = 15;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMakeaviDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMakeaviDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMakeaviDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMakeaviDlg::OnSetCompression() 
{
}


void CMakeaviDlg::ResizeColumn(CString str)
{
	int StrWidth = m_FileList.GetStringWidth(str);
	StrWidth += 10;
	int CurWidth = m_FileList.GetColumnWidth(0);
	if (StrWidth > CurWidth)
		m_FileList.SetColumnWidth(0,StrWidth);
}

void CMakeaviDlg::OnPickFiles() 
{
	static char BASED_CODE szFilter[] = "Image files|*.jpg;*.jpeg;*.png;*.tif;*.bmp||";

	char *FileNameBuffer;
	FileNameBuffer = (char *)malloc(128000); // hope this is long enough
	*FileNameBuffer = 0;
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter,this);
	FileDlg.m_ofn.lpstrFile = FileNameBuffer;
	FileDlg.m_ofn.nMaxFile = 128000;

	if (FileDlg.DoModal() == IDOK)
	{
		POSITION pos = FileDlg.GetStartPosition();
		int StringPos;
		while (NULL != pos)
		{
			CString strPath = FileDlg.GetNextPathName(pos);
			StringPos = m_Files.Add(strPath);
			m_FileList.InsertItem(0,LPSTR_TEXTCALLBACK);
			ResizeColumn(strPath);
		}

		UseItemSize(StringPos);
		m_FileList.Invalidate();
	}
}
void CMakeaviDlg::UseItemSize(int idx)
{
	if (   (idx < 0)
		|| (idx >= m_Files.GetSize()))
		return;
	CString strFNm = m_Files[idx];
	if (strFNm.IsEmpty())
		return;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(strFNm, -1);
	FIBITMAP *hImage = FreeImage_Load(fif,strFNm);
	if (0 < hImage)
	{
		UpdateData(TRUE);
		BITMAPINFOHEADER *lpbi = FreeImage_GetInfoHeader(hImage);
		m_CropX = (int) lpbi->biWidth;
		m_CropY = (int) lpbi->biHeight;
		FreeImage_Free(hImage);
		UpdateData(FALSE);
	}
}

void CMakeaviDlg::OnGetdispinfoFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	int idx = pDispInfo->item.iItem;
	if (m_bDispBufferLocked)
		m_strDispBuffer.ReleaseBuffer();
	m_strDispBuffer = m_Files[idx];
	pDispInfo->item.pszText = m_strDispBuffer.GetBuffer(0);
	m_bDispBufferLocked = true;
	
	*pResult = 0;
}

void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;
	
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < GetUpperBound();pos++)
			bNotDone |= CompareAndSwap(pos);
	}
}

BOOL CSortStringArray::CompareAndSwap(int pos)
{
	CString temp;
	int posFirst = pos;
	int posNext = pos + 1;
	
	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
	{
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return TRUE;
		
	}
	return FALSE;
}

void CMakeaviDlg::OnSortFiles() 
{
	m_Files.Sort();
	m_FileList.Invalidate();
}


void CMakeaviDlg::OnFileup() 
{
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int idx = m_FileList.GetNextSelectedItem(pos);

	if (0 >= idx) // can't move first item up
		return;

	CString temp;
	temp = m_Files.GetAt(idx);
	// copy previous item to this one
	m_Files.SetAt(idx,m_Files.GetAt(idx-1));
	m_Files.SetAt(idx-1, temp);
	m_FileList.SetItemState(idx-1,LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_FileList.Invalidate();
}

void CMakeaviDlg::OnFiledown() 
{
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int idx = m_FileList.GetNextSelectedItem(pos);

	if ((idx+1) >= m_FileList.GetItemCount()) // can't move last item down
		return;


	CString temp;
	temp = m_Files.GetAt(idx);
	// copy next item to this one
	m_Files.SetAt(idx,m_Files.GetAt(idx+1));
	m_Files.SetAt(idx+1, temp);
	m_FileList.SetItemState(idx+1,LVIS_SELECTED, LVIS_SELECTED);
	m_FileList.Invalidate();
}


void CMakeaviDlg::OnOK() 
{
	static char BASED_CODE szFilter[] = "AVI files (*.avi)|*.avi||";

	UpdateData(TRUE);

	CFileDialog FileDlg(FALSE, ".avi", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (FileDlg.DoModal() != IDOK)
		return;
	CString strOutputFilename = FileDlg.GetPathName();

	_unlink(strOutputFilename); // user already confirmed overwrite, just nuke it.

		
	PAVIFILE pfile = NULL; 
	PAVISTREAM ps = NULL, psCompressed = NULL;

	bool bInitDone = false;
	FeedbackDlg Feedback;
    Feedback.Create(FeedbackDlg::IDD);

	int NumFiles = m_Files.GetSize();
	Feedback.SetRange(0,NumFiles);
	for (int i=0; i < NumFiles; i++)
	{
		Feedback.SetPos(i);
		CString strMsg;
		strMsg.Format("File %d of %d (%d %%)",i,NumFiles,(i*100)/NumFiles);
		Feedback.SetText(strMsg);

		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(m_Files[i], -1);
		FIBITMAP *hImage = FreeImage_Load(fif,m_Files[i]);
		if (0 >= hImage)
		{
			CString strMsg;
			strMsg.Format("Unable to load image %s; aborting\n", m_Files[i]);
			AfxMessageBox(strMsg);
			return;
		}

		BITMAPINFOHEADER *lpbi = FreeImage_GetInfoHeader(hImage);
		if (m_EnableCropping)
		{
			int Width = lpbi->biWidth;
			int Height = lpbi->biHeight;
			if (   (Width < m_CropX)
				|| (Height < m_CropY))
			{
				CString strMsg;
				strMsg.Format("ERROR! file %s size %d x %d smaller than crop size %d x %d - stopping",
					m_Files[i], Width, Height, m_CropX, m_CropY);
				AfxMessageBox(strMsg);
				goto stopcompressing;
			}

			if (   (Width != m_CropX)
				|| (Height != m_CropY))
			{
				int left, right, top, bottom;
				left = (Width - m_CropX) / 2;
				right = left + m_CropX;
				top = (Height - m_CropY) / 2;
				bottom = top + m_CropY;
				FIBITMAP *hClip = FreeImage_Crop(hImage, left, top, right, bottom);
				FreeImage_Free(hImage);
				hImage = hClip;
				lpbi = FreeImage_GetInfoHeader(hImage);
			}
		}
		if (!bInitDone)
		{
			bInitDone = true;
			if (!AVI_Init())
				goto stopcompressing;
			if (!AVI_FileOpenWrite(&pfile, strOutputFilename.GetBuffer(0)))
				goto stopcompressing;

			if (!AVI_CreateStream(pfile, &ps, m_FrameRate,
				(unsigned long) lpbi->biSizeImage,
				lpbi->biWidth,
				lpbi->biHeight))
				goto stopcompressing;

			if (!AVI_SetOptions(&ps, &psCompressed, lpbi))
				goto stopcompressing;
		}
		if (!AVI_AddFrame(psCompressed, i * 1, lpbi))
			goto stopcompressing;
		FreeImage_Free(hImage);
	    // check for cancelled
		if (Feedback.m_cancelled)
			goto stopcompressing;
	}

stopcompressing:
	// The end...
	AVI_CloseStream(ps, psCompressed, NULL);

	AVI_CloseFile(pfile);

	AVI_Exit();
}

void CMakeaviDlg::OnEnableCropping() 
{
	// enable/disable value fields
	BOOL bEnable = IsDlgButtonChecked(IDC_ENABLE_CROPPING);
	GetDlgItem(IDC_XRES)->EnableWindow(bEnable);
	GetDlgItem(IDC_YRES)->EnableWindow(bEnable);
}



void CMakeaviDlg::OnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// if the state change is a new selection, then find the filename and set the preview control there.
	if (   ((pNMListView->uNewState & LVIS_SELECTED) == LVIS_SELECTED)
		&& ((pNMListView->uOldState & LVIS_SELECTED) != LVIS_SELECTED))
	{
	    m_Preview.SetFilename(m_Files[pNMListView->iItem]);
	}
	
	*pResult = 0;
}

void CMakeaviDlg::OnDelete() 
{
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	if (NULL == pos)
		return;

	int idx = m_FileList.GetNextSelectedItem(pos);

	if (0 > idx)
		return;

	m_Files.RemoveAt(idx,1);
	m_FileList.DeleteItem(idx);
	if (idx > (m_FileList.GetItemCount()-1))
		idx--;
	if (idx > -1)
		m_FileList.SetItemState(idx,LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
}

void CMakeaviDlg::OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	UseItemSize(pNMListView->iItem);	
	*pResult = 0;
}
