#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
IMPLEMENT_SERIAL(CDib,CObject,0)

CDib::CDib()
{
	m_dwLength=0L;
	m_nBits=0;
	m_lpBuf=NULL;
}

CDib::CDib(CBitmap* pBitmap,int nBt,BOOL	bCompr)
{
	BITMAP	bm;
	int		nPaletteSize;
	HDC		hDC=::GetDC(NULL);
	CDC		*pDC=CDC::FromHandle(hDC);
	pBitmap->GetObject(sizeof(bm),&bm);
//	HPALETTE	hPal;
//	hPal=::GetStockObject(DEFAULT_PALETTE);
	if((nBt==1)||(nBt==4)||(nBt==8)||(nBt==24))
	{
		m_nBits=nBt;
	}
	else
		m_nBits=bm.bmPlanes*bm.bmBitsPixel;
	switch(m_nBits)
	{
		case 1:
			nPaletteSize=2;
			break;
		case 2:
			nPaletteSize=4;
			break;
		case 4:
			nPaletteSize=16;
			break;
		case 8:
			nPaletteSize=256;
			break;
		default:
			nPaletteSize=0;
	}
	DWORD	dwBytes=((DWORD)bm.bmWidth*m_nBits)/32;
	if(((DWORD)bm.bmWidth*m_nBits)%32)
		 dwBytes++;
	dwBytes*=4;
	m_dwLength=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize;
	if(!AllocateMemory())
		return;
	m_lpBMIH->biSize=sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth=bm.bmWidth;
	m_lpBMIH->biHeight=bm.bmHeight;
	m_lpBMIH->biPlanes=1;
	m_lpBMIH->biBitCount=m_nBits; //1,4,8,24
	if(bCompr&&(m_nBits==4))
		m_lpBMIH->biCompression=BI_RLE4;
	if(bCompr&&(m_nBits==8))
		m_lpBMIH->biCompression=BI_RLE8;
	else
		m_lpBMIH->biCompression=BI_RGB;
	m_lpBMIH->biSizeImage=0;
	m_lpBMIH->biXPelsPerMeter=0;

	m_lpBMIH->biYPelsPerMeter=0;
	m_lpBMIH->biClrUsed=0;
	m_lpBMIH->biClrImportant=0;
	::GetDIBits(pDC->GetSafeHdc(),(HBITMAP)pBitmap->GetSafeHandle(),
		0,(WORD)bm.bmHeight,NULL,m_lpBMI,DIB_RGB_COLORS);
	if(m_lpBMIH->biSizeImage==0)
	{
		m_dwLength+=dwBytes*bm.bmHeight;
		m_lpBMIH->biCompression=BI_RGB;
		TRACE("Can't do Compression\n");
	}
	else
		m_dwLength+=m_lpBMIH->biSizeImage;
	if(!AllocateMemory(TRUE))
		return;
	m_lpData=(LPBYTE)m_lpBMIH+sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize;
	m_lpBMFH->bfType=0x4d42;//"BM"
	m_lpBMFH->bfSize=m_dwLength;
	m_lpBMFH->bfReserved1=0;
	m_lpBMFH->bfReserved2=0;
	m_lpBMFH->bfOffBits=(LPBYTE)m_lpData - m_lpBuf;
	DWORD Len=::GetDIBits(pDC->GetSafeHdc(),(HBITMAP)pBitmap->GetSafeHandle(),
		0,(WORD)bm.bmHeight,m_lpData,m_lpBMI,DIB_RGB_COLORS);
	if(!Len)
		m_dwLength=0L;
	ReleaseDC(NULL,hDC);
}

CDib::CDib(CDC *pDC,int nBt,BOOL	bCompr)
{
	BITMAP	bm;
	int		nPaletteSize;
	CBitmap*	pEmptyBitmap=new  CBitmap;
	pEmptyBitmap->CreateCompatibleBitmap(pDC,1,1);
	CBitmap*	pBitmap=(CBitmap*)(pDC->SelectObject(pEmptyBitmap));
//	CBitmap*	pBitmap=new  CBitmap;
//	pBitmap->CreateCompatibleBitmap(pDC,Size.cx,Size.cy);
	pBitmap->GetObject(sizeof(bm),&bm);
	if((nBt==1)||(nBt==4)||(nBt==8)||(nBt==24))
	{
		m_nBits=nBt;
	}
	else
		m_nBits=bm.bmPlanes*bm.bmBitsPixel;
	switch(m_nBits)
	{
		case 1:
			nPaletteSize=2;
			break;
		case 2:
			nPaletteSize=4;
			break;
		case 4:
			nPaletteSize=16;
			break;
		case 8:
			nPaletteSize=256;
			break;
		default:
			nPaletteSize=0;
	}
	DWORD	dwBytes=((DWORD)bm.bmWidth*m_nBits)/32;
	if(((DWORD)bm.bmWidth*m_nBits)%32)
		 dwBytes++;
	dwBytes*=4;
	m_dwLength=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize;
	if(!AllocateMemory())
		return;
	m_lpBMIH->biSize=sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth=bm.bmWidth;
	m_lpBMIH->biHeight=bm.bmHeight;
	m_lpBMIH->biPlanes=1;
	m_lpBMIH->biBitCount=m_nBits;
	if(bCompr&&(m_nBits==4))
		m_lpBMIH->biCompression=BI_RLE4;
	if(bCompr&&(m_nBits==8))
		m_lpBMIH->biCompression=BI_RLE8;
	else
		m_lpBMIH->biCompression=BI_RGB;
	m_lpBMIH->biSizeImage=0;
	m_lpBMIH->biXPelsPerMeter=0;
	m_lpBMIH->biYPelsPerMeter=0;
	m_lpBMIH->biClrUsed=0;
	m_lpBMIH->biClrImportant=0;
	::GetDIBits(pDC->GetSafeHdc(),(HBITMAP)pBitmap->GetSafeHandle(),
		0,(WORD)bm.bmHeight,NULL,m_lpBMI,DIB_RGB_COLORS);
	if(m_lpBMIH->biSizeImage==0)
	{
		m_dwLength+=dwBytes*bm.bmHeight;
		m_lpBMIH->biCompression=BI_RGB;
		TRACE("Can't do Compression\n");
	}
	else
		m_dwLength+=m_lpBMIH->biSizeImage;
	if(!AllocateMemory(TRUE))
		return;
	m_lpData=(LPBYTE)m_lpBMI+sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize;
	m_lpBMFH->bfType=0x4d42;//"BM"
	m_lpBMFH->bfSize=m_dwLength;
	m_lpBMFH->bfReserved1=0;
	m_lpBMFH->bfReserved2=0;
	m_lpBMFH->bfOffBits=(LPBYTE)m_lpData-m_lpBuf;
	if(!::GetDIBits(pDC->GetSafeHdc(),(HBITMAP)pBitmap->GetSafeHandle(),
		0,(WORD)bm.bmHeight,m_lpData,m_lpBMI,DIB_RGB_COLORS))
		m_dwLength=0L;
	pDC->SelectObject(pBitmap);
	delete	pEmptyBitmap;
//	delete	pBitmap;
}

CDib::~CDib()
{
	if(m_lpBuf)
		GlobalFreePtr(m_lpBuf);
}

void	CDib::Serialize(CArchive&	ar)
{
	ar.Flush();
	if(ar.IsStoring())
		Write(ar.GetFile());
	else
		Read(ar.GetFile());
}
	
BOOL	CDib::Read(CFile *pFile)
{
	ASSERT(m_dwLength==0L);
	m_dwLength=pFile->GetLength();
	ASSERT(m_dwLength!=0L);
	LPBYTE	m_lpBuf1=(LPBYTE)GlobalAllocPtr(GHND,m_dwLength);
	DWORD	dwCount=pFile->Read(m_lpBuf1,m_dwLength);
	if(dwCount!=m_dwLength)
	{
		AfxMessageBox("Read Error");
		GlobalFreePtr(m_lpBuf1);
		return FALSE;
	}
	BOOL	Ret=ReadMemory(m_lpBuf1,m_dwLength);
	GlobalFreePtr(m_lpBuf1);
	return Ret;
}

BOOL	CDib::ReadMemory(LPVOID	MemorInfo,DWORD	Length)
{
	ASSERT(MemorInfo!=NULL);
	ASSERT(Length!=0L);
	m_dwLength=Length;
	try {
		if(!AllocateMemory())
		{
			return FALSE;
		}
		memcpy(m_lpBuf,MemorInfo,m_dwLength);
	}
	catch(...)
	{
		AfxMessageBox("Memory Error");
		return	FALSE;
	}

	if(m_lpBMFH->bfType!=0x4d42)
	{
		return	FALSE;
	}
	ASSERT((m_lpBMIH->biBitCount==1)||(m_lpBMIH->biBitCount==4)||
		(m_lpBMIH->biBitCount==8)||(m_lpBMIH->biBitCount==24));
	if(m_lpBMFH->bfOffBits>sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		PaletteSize(m_lpBMI))
		return FALSE;
	m_lpData=(LPBYTE)m_lpBMFH+m_lpBMFH->bfOffBits;
	m_nBits=m_lpBMIH->biBitCount;
	return TRUE;
}

BOOL	CDib::Write(CFile *pFile)
{
	try
	{
		pFile->Write(m_lpBuf,m_dwLength);
	}
	catch(...)
	{
		AfxMessageBox("Wrtie Error");
		return	FALSE;
	}
	return TRUE;
}
int CDib::GetColorBits()
{
	return	m_nBits;
}

DWORD  CDib::GetLength()
{
	return	 m_dwLength;
}

CSize  CDib::GetSize()
{
	return CSize((int)m_lpBMIH->biWidth,(int)m_lpBMIH->biHeight);
}

BOOL	CDib::AllocateMemory(BOOL	bRealloc)
{
	if(bRealloc)
		m_lpBuf=(LPBYTE)GlobalReAllocPtr(m_lpBuf,m_dwLength,GHND);
	else
		m_lpBuf=(LPBYTE)GlobalAllocPtr(GHND,m_dwLength);
	if(!m_lpBuf)
	{
		AfxMessageBox("Unable to Allocate Dib Memory");
		m_dwLength=0L;
		m_nBits=0;
		m_lpBuf=NULL;
		return FALSE;
	}
	m_lpBMFH=(LPBITMAPFILEHEADER)m_lpBuf;
	m_lpBMIH=(LPBITMAPINFOHEADER)(m_lpBuf+sizeof(BITMAPFILEHEADER));;
	m_lpBMI=(LPBITMAPINFO)m_lpBMIH;
	return	TRUE;
}

BOOL	CDib::Display(CDC *pDC,CPoint origin)
{
	if(!m_lpBuf)
	{
		return	FALSE;
		//nothing to Display
	}
	if(!::SetDIBitsToDevice(pDC->GetSafeHdc(),origin.x,origin.y,
		(WORD)m_lpBMIH->biWidth,(WORD)m_lpBMIH->biHeight,0,0,0,
		(WORD)m_lpBMIH->biHeight,m_lpData,m_lpBMI,
		DIB_RGB_COLORS))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CDib::Stretch(CDC *pDC,CPoint	origin,CSize size)
{
	if(!m_lpBuf)
		 return FALSE;
//	pDC->SetStretchBltMode(COLORONCOLOR);
	if(!::StretchDIBits(pDC->GetSafeHdc(),origin.x,origin.y,
		size.cx,size.cy,0,0,(WORD)m_lpBMIH->biWidth,
		(WORD)m_lpBMIH->biHeight,m_lpData,m_lpBMI,
		DIB_RGB_COLORS,SRCCOPY)){
		return FALSE;
	}
	return TRUE;
}

void	CDib::SetMonoColors(DWORD	dwForegroud,DWORD	dwBackgound)
{
	if(m_nBits!=1)
		return;
	ULONG	*pPalette=(ULONG *)((LPBYTE)m_lpBMIH+sizeof(BITMAPINFOHEADER));
	*pPalette=dwForegroud;
	*(++pPalette)=dwBackgound;
}

BOOL	CDib::GetMonoColors(DWORD&	dwForegroud,DWORD&	dwBackgound)
{
	if(m_nBits!=1)
		return FALSE;
	ULONG	*pPalette=(ULONG *)((LPBYTE)m_lpBMIH+sizeof(BITMAPINFOHEADER));
	dwForegroud=*pPalette;
	dwBackgound=*(++pPalette);
	return TRUE;
}

HBITMAP		CDib::MakeHBitmap(CDC *pDC,CSize &bmSize)
{
	BITMAP	bm;
	DWORD	dwFore,dwBack;
	if(m_dwLength==0L) {
		bmSize.cx=bmSize.cy=0;
		return NULL;
	}

	int	nPlanes=pDC->GetDeviceCaps(PLANES);
	int	nBitsPixel=pDC->GetDeviceCaps(BITSPIXEL);
	CBitmap	*pConfigBitmap=new CBitmap;
	char	bits[100];
	if(m_lpBMIH->biBitCount==1) {
		pConfigBitmap->CreateBitmap(1,1,1,1,bits);
	}
	else {
		pConfigBitmap->CreateBitmap(1,1,nPlanes,nBitsPixel,bits);
	}

	CBitmap	*pOriginalBitmap=(CBitmap*)pDC->SelectObject(pConfigBitmap);
	if(GetMonoColors(dwFore,dwBack)) {
		SetMonoColors(0L,0xFFFFFFL);
	}

#ifdef	_WIN32
	HBITMAP	hBitmap=::CreateDIBitmap(pDC->GetSafeHdc(),m_lpBMIH,
		CBM_INIT,(CONST BYTE*)(m_lpBuf+m_lpBMFH->bfOffBits),
		m_lpBMI,DIB_RGB_COLORS);
#else
	HBITMAP	hBitmap=::CreateDIBitmap(pDC->GetSafeHdc(),m_lpBMIH,
		CBM_INIT,(LPBYTE)(m_lpBuf+m_lpBMFH->bfOffBits),
		m_lpBMI,DIB_RGB_COLORS);
#endif

	if(hBitmap==NULL) {
		TRACE("Null Bitmap\n");
		pDC->SelectObject(pOriginalBitmap);	//delete pConfigBitmap
		delete	pConfigBitmap;
		return	NULL;
	}
	SetMonoColors(dwFore,dwBack);
	pDC->SelectObject(pOriginalBitmap);//delete pConfigBitmap
	CBitmap	*pBitmap=CBitmap::FromHandle(hBitmap);
	pBitmap->GetObject(sizeof(bm),&bm);
	bmSize.cx=bm.bmWidth;
	bmSize.cy=bm.bmHeight;
	delete	pConfigBitmap;
	return	hBitmap;
}

CBitmap*	CDib::MakeBitmap(CDC *pDC,CSize &bmSize)
{
	HBITMAP	hBitmap=MakeHBitmap(pDC,bmSize);
	if(!hBitmap)
		return NULL;
	CBitmap	*pBitmap=new CBitmap;
	pBitmap->Attach(hBitmap);
	return	pBitmap;
}

HDIB	CDib::GetSafeHandle()
{
	ASSERT(m_lpBuf!=NULL);
	ASSERT(m_dwLength!=0L);
	DWORD	dwLenHead=sizeof(BITMAPFILEHEADER);
	DWORD	dwLen = m_dwLength-dwLenHead;
	HDIB	hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock(hCopy);
		memcpy(lpCopy,(LPBYTE)m_lpBMIH, dwLen);
		::GlobalUnlock(hCopy);
	}
	return hCopy;
}

void	CDib::CreatFromHandle(HDIB	hDib)
{
	ASSERT (hDib!=NULL);
	DWORD	dwSizeDib=GlobalSize(hDib);
	void* lpDib = ::GlobalLock(hDib);
	m_dwLength=dwSizeDib+sizeof(BITMAPFILEHEADER);
	DWORD	dwLenHead=sizeof(BITMAPFILEHEADER);
	DWORD	dwLen = m_dwLength-dwLenHead;

	if(!AllocateMemory())
		return;

	memcpy((LPBYTE)m_lpBMIH,lpDib,dwLen);
	int		nPaletteSize=PaletteSize(m_lpBMI);
	m_lpBMFH->bfType=0x4d42;//"BM"
	m_lpBMFH->bfSize=m_dwLength;
	m_lpBMFH->bfReserved1=0;
	m_lpBMFH->bfReserved2=0;
	m_lpBMFH->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nPaletteSize;

	::GlobalUnlock(hDib);
}

WORD CDib::DibNumColors (LPVOID pv)
{
    INT                 bits;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPCOREHEADER  lpbc;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)){
        if (lpbi->biClrUsed != 0)
            return (WORD)lpbi->biClrUsed;
        bits = lpbi->biBitCount;
    }
    else
        bits = lpbc->bcBitCount;

    switch (bits){
        case 1:
                return 2;
        case 4:
                return 16;
        case 8:
                return 256;
        default:
                /* A 24 bitcount DIB has no color table */
                return 0;
    }
}

WORD CDib::PaletteSize (LPVOID pv)
{
    LPBITMAPINFOHEADER lpbi;
    WORD               NumColors;

    lpbi      = (LPBITMAPINFOHEADER)pv;
    NumColors = DibNumColors(lpbi);

    if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
        return (WORD)(NumColors * sizeof(RGBTRIPLE));
    else
        return (WORD)(NumColors * sizeof(RGBQUAD));
}
