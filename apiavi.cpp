/**************************************************************************
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
* PURPOSE.
*
* Copyright (C) 1992 - 1997 Microsoft Corporation. All Rights Reserved.
*
**************************************************************************/
/* ApiAvi.cpp JUST CUT FROM WRITEAVI.C WINDOWS SAMPLE M.Lievin @1999
So don't ask me for any kind of explanation
*/

// apiavi.h 
// this is a more friendly wrapper around the Windows AVI API
//
// this code was borrowed from  Marc Lievin, from a posting on www.codeguru.com
// There was no direct link to his web site, but I bet this is him:
// http://marclievin.multimania.com/

#include "stdafx.h"

#define AVIIF_KEYFRAME 0x00000010L // this frame is a key frame.

#include <windows.h>
#include <windowsx.h>
#include <memory.h>
#include <mmsystem.h>
#include <vfw.h>

BOOL AVI_Init()
{
	/* first let's make sure we are running on 1.1 */
	WORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a){
		/* oops, we are too old, blow out of here */
		MessageBeep(MB_ICONHAND);
		MessageBox(NULL, "Sorry, Cant't init AVI File !", "Error", MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	AVIFileInit();
	
	return TRUE;
}

BOOL AVI_FileOpenWrite(PAVIFILE * pfile,
					   char *filename)
{
	HRESULT hr = AVIFileOpen(pfile, // returned file pointer
		filename, // file name
		OF_WRITE | OF_CREATE, // mode to open file with
		NULL); // use handler determined
	// from file extension....
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIFileOpen, error %d", hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}

// Fill in the header for the video stream....
// The video stream will run in rate ths of a second....
BOOL AVI_CreateStream(PAVIFILE pfile,
					  PAVISTREAM * ps,
					  int rate, // sample/second
					  unsigned long buffersize,
					  int rectwidth,
					  int rectheight)
{
	
	AVISTREAMINFO strhdr;
	
	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType = streamtypeVIDEO;// stream type
	strhdr.fccHandler = 0;
	strhdr.dwScale = 1;
	strhdr.dwRate = rate; // rate fps
	strhdr.dwSuggestedBufferSize = buffersize;
	SetRect(&strhdr.rcFrame, 0, 0, // rectangle for stream
		(int) rectwidth,
		(int) rectheight);
	
	// And create the stream;
	HRESULT hr = AVIFileCreateStream(pfile, // file pointer
		ps, // returned stream pointer
		&strhdr); // stream header
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIFileCreateStream, error %d", hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}

BOOL AVI_SetOptions(PAVISTREAM * ps,
					PAVISTREAM * psCompressed,
					LPBITMAPINFOHEADER lpbi)
{
	
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
	
	_fmemset(&opts, 0, sizeof(opts));
	
	if (!AVISaveOptions(NULL, 0, 1, ps, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
	{
		CString strMsg;
		strMsg.Format("Error: AVISaveOptions");
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	HRESULT hr = AVIMakeCompressedStream(psCompressed, *ps, &opts, NULL);
	if (hr != AVIERR_OK) {
		CString strMsg;
		strMsg.Format("Failure: AVIMakeCompressedStream, error %d",hr);
		if (AVIERR_NOCOMPRESSOR == hr)
			strMsg += "\nA suitable compressor cannot be found.";

		if (AVIERR_MEMORY == hr)
			strMsg += "\nThere is not enough memory to complete the operation.";

		if (AVIERR_NOCOMPRESSOR == hr)
			strMsg += "\nCompression is not supported for this type of data.";
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	hr = AVIStreamSetFormat(*psCompressed, 0,
		lpbi, // stream format
		lpbi->biSize // format size
		+ lpbi->biClrUsed * sizeof(RGBQUAD)
		);
	if (hr != AVIERR_OK) {
		CString strMsg;
		strMsg.Format("Failure: AVIStreamSetFormat, error %d",hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}

BOOL AVI_SetText(PAVIFILE pfile,
				 PAVISTREAM psText,
				 char *szText,
				 int width,
				 int height,
				 int TextHeight)
{
	// Fill in the stream header for the text stream....
	AVISTREAMINFO strhdr;
	DWORD dwTextFormat;
	// The text stream is in 60ths of a second....
	
	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType = streamtypeTEXT;
	strhdr.fccHandler = mmioFOURCC('D', 'R', 'A', 'W');
	strhdr.dwScale = 1;
	strhdr.dwRate = 60;
	strhdr.dwSuggestedBufferSize = sizeof(szText);
	SetRect(&strhdr.rcFrame, 0, (int) height,
		(int) width, (int) height + TextHeight); // #define TEXT_HEIGHT 20
	
	// ....and create the stream.
	HRESULT hr = AVIFileCreateStream(pfile, &psText, &strhdr);
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIFileCreateStream, error %d", hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	dwTextFormat = sizeof(dwTextFormat);
	hr = AVIStreamSetFormat(psText, 0, &dwTextFormat, sizeof(dwTextFormat));
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIStreamSetFormat, error %d", hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}



BOOL AVI_AddFrame(PAVISTREAM psCompressed,
				  int time,
				  LPBITMAPINFOHEADER lpbi)
{
	int ImageSize = lpbi->biSizeImage;
	if (ImageSize == 0)
	{
		if (lpbi->biBitCount == 24)
		{
			ImageSize = lpbi->biWidth * lpbi->biHeight * 3;
		}
	}
	HRESULT hr = AVIStreamWrite(psCompressed, // stream pointer
		time, // time of this frame
		1, // number to write
		(LPBYTE) lpbi + // pointer to data
		lpbi->biSize +
		lpbi->biClrUsed * sizeof(RGBQUAD),
		ImageSize, // lpbi->biSizeImage, // size of this frame
		AVIIF_KEYFRAME, // flags....
		NULL,
		NULL);
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIStreamWrite, error %d",hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}

BOOL AVI_AddText(PAVISTREAM psText,
				 int time,
				 char *szText)
{
	int iLen = strlen(szText);
	
	HRESULT hr = AVIStreamWrite(psText,
		time,
		1,
		szText,
		iLen + 1,
		AVIIF_KEYFRAME,
		NULL,
		NULL);
	if (hr != AVIERR_OK)
	{
		CString strMsg;
		strMsg.Format("Error: AVIStreamWrite, error %d", hr);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	return TRUE;
}

BOOL AVI_CloseStream(PAVISTREAM ps,
					 PAVISTREAM psCompressed,
					 PAVISTREAM psText)
{
	if (ps)
		AVIStreamClose(ps);
	
	if (psCompressed)
		AVIStreamClose(psCompressed);
	
	if (psText)
		AVIStreamClose(psText);
	
	return TRUE;
}

BOOL AVI_CloseFile(PAVIFILE pfile)
{
	if (pfile)
		AVIFileClose(pfile);
	
	return TRUE;
}

BOOL AVI_Exit()
{
	AVIFileExit();
	
	return TRUE;
}