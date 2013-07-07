typedef HANDLE HDIB;
class	CDib :public	CObject {
	DECLARE_SERIAL(CDib);
	public:
		LPBYTE				m_lpBuf;
		DWORD				m_dwLength;
		int					m_nBits;
		LPBITMAPFILEHEADER	m_lpBMFH;
		LPBITMAPINFOHEADER	m_lpBMIH;
		LPBITMAPINFO		m_lpBMI;
		LPBYTE				m_lpData;
	public:
		CDib();
		CDib(CBitmap*,int,BOOL =FALSE);
		CDib(CDC *,int,BOOL =FALSE);
		~CDib();
		void		Serialize(CArchive&	ar);
		BOOL		Read(CFile*);
		BOOL		Write(CFile*);
		int			GetColorBits();
		DWORD		GetLength();
		CSize		GetSize();
		BOOL		ReadMemory(LPVOID,DWORD);
		HBITMAP		MakeHBitmap(CDC *pDC,CSize &bmSize);
		CBitmap*	MakeBitmap(CDC *pDC,CSize &bmSize);
		BOOL		Display(CDC *,CPoint);
		BOOL		Stretch(CDC *,CPoint,CSize);
		void		SetMonoColors(DWORD,DWORD);
		BOOL		GetMonoColors(DWORD&,DWORD&);
		HDIB		GetSafeHandle();
		void		CreatFromHandle(HDIB);
		WORD		PaletteSize (LPVOID);
		WORD		DibNumColors (LPVOID);
	private:
		BOOL		AllocateMemory(BOOL	bRealloc=FALSE);
};
