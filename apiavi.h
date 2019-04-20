// apiavi.h 
// this is a more friendly wrapper around the Windows AVI API
//
// this code was borrowed from  Marc Lievin, from a posting on www.codeguru.com
// There was no direct link to his web site, but I bet this is him:
// http://marclievin.multimania.com/
// however, he says in the .cpp file that he just borrowed it from a Microsoft
// sample, and I'm too lazy to go look.

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <vfw.h>


BOOL AVI_Init();
BOOL AVI_FileOpenWrite(PAVIFILE * pfile,char *filename);
BOOL AVI_CreateStream(PAVIFILE pfile,PAVISTREAM * ps,
					  int rate, // sample/second	
					  unsigned long buffersize,
					  int rectwidth,
					  int rectheight);
BOOL AVI_SetOptions(PAVISTREAM * ps,
					PAVISTREAM * psCompressed,
					LPBITMAPINFOHEADER lpbi);
BOOL AVI_SetText(PAVIFILE pfile,
				 PAVISTREAM psText,
				 char *szText,
				 int width,
				 int height,
				 int TextHeight);
BOOL AVI_AddFrame(PAVISTREAM psCompressed,
				  int time,
				  LPBITMAPINFOHEADER lpbi);
BOOL AVI_AddText(PAVISTREAM psText,
				 int time,
				 char *szText);
BOOL AVI_CloseStream(PAVISTREAM ps,
					 PAVISTREAM psCompressed,
					 PAVISTREAM psText);
BOOL AVI_CloseFile(PAVIFILE pfile);
BOOL AVI_Exit();
