#if !defined(AFX_SKINPROGRESS_H__1DF43017_611B_44BD_822A_FAA0E01B2257__INCLUDED_)
#define AFX_SKINPROGRESS_H__1DF43017_611B_44BD_822A_FAA0E01B2257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SkinProgress.h : header file
//

/*** eSkinProgressBitmap ******************************************************/
/* Purpose : Used in 'OnPaint' for internal purposes, in order to index the   */
/*           m_BarImgLst's elements                                           */
/* Unit    : None                                                             */
/* Range   : enum LIST                                                        */
/* List    :  0 = cSPB_START               : image for the start              */
/*            1 = cSPB_BEFORE              : image for before m_nLeft         */
/*            2 = cSPB_LEFT  = cSPB_TOP    : image for m_nLeft or m_nLower    */
/*            3 = cSPB_CENTER              : image for the center of the bar  */
/*            4 = cSPB_BAR                 : image for the bar body           */
/*            5 = cSPB_RIGHT = cSPB_BOTTOM : image for m_nRight               */
/*            6 = cSPB_AFTER               : image for after m_nRight         */
/*            7 = cSPB_BACKGROUND          : image for the bar background     */
/*            8 = cSPB_END                 : image for the end                */
/*            9 = cSPB_EnumElements        : current number of elements       */
/* Example : None                                                             */
/******************************************************************************/
typedef enum eSkinProgressBitmap
{
  cSPB_START = 0,
  cSPB_BEFORE,
  cSPB_LEFT,
  cSPB_CENTER,
  cSPB_BAR,
  cSPB_RIGHT,
  cSPB_AFTER,
  cSPB_BACKGROUND,
  cSPB_END,
  cSPB_EnumElements,
  cSPB_TOP    = cSPB_LEFT,
  cSPB_BOTTOM = cSPB_RIGHT,
};

/*** eProgressSkinText ********************************************************/
/* Purpose : TRUE condition flag                                              */
/* Unit    : None                                                             */
/* Range   : enum LIST                                                        */
/* List    :  0 = cSPT_NONE         : no text information                     */
/*            1 = cSPT_PERCENT      : simple percent completion               */
/*            2 = cSPT_TIMED        : complete timed completion               */
/*            3 = cSPT_AUTOSIZE     : switch between cSPT_PERCENT and         */
/*                                    cSPT_TIMED according to the remaining   */
/*                                    place available                         */
/*            4 = cSPT_EnumElements : current number of elements              */
/* Example : None                                                             */
/******************************************************************************/
typedef enum eProgressSkinText
{
  cSPT_NONE = 0,
  cSPT_PERCENT,
  cSPT_TIMED,
  cSPT_AUTOSIZE,
  cSPT_EnumElements
};

/*** dCSP_TIMED_REDRAW ********************************************************/
/* Purpose : Activate the redraw on timer                                     */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : I implemented this to reduce to calcultation time when activated */
/*           by forcing the bar to redraw/refresh only 50 times a second.     */
/******************************************************************************/
//define dCSP_TIMED_REDRAW

/*** dCSP_VERTICAL_BAR ********************************************************/
/* Purpose : Activate the vertical bar possibilities                          */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
//#define dCSP_VERTICAL_BAR

/*** dCSP_SLIDERBAR_METHOD ****************************************************/
/* Purpose : Activate the full slider bar calculation                         */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
//#define dCSP_SLIDERBAR_METHOD

/*** dCSP_RESOURCE_BITMAP *****************************************************/
/* Purpose : Activate the use of the default resource bitmap, instead of the  */
/*           AQUA_BITMAP array                                                */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
//#define dCSP_RESOURCE_BITMAP

/*** dCSP_CREATE_BITMAP_FILE **************************************************/
/* Purpose : Create the BITMAP file in 'SetBitmap', to change the defaut      */
/*           AQUA_BITMAP array                                                */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : Just activate this, launch the progress bar WITH A PROVIDED      */
/*           bitmap (useless to use the default one, otherwise you'll just    */
/*           get a copy of it in the current video mode) and a '.cpp' file is */
/*           created at the root of drive C: ! Just replace the               */
/*           aucAQUA_BITMAP array and the sbmAQUA_BITMAP structure at the     */
/*           beginning of the 'SkinProgress.cpp' file, desactivate this       */
/*           define and everything should works fine once recompilated...     */
/******************************************************************************/
//#define dCSP_CREATE_BITMAP_FILE

/*** dCSP_DISPLAY_STRETCH *****************************************************/
/* Purpose : Display <m_pStretchBmp> content instead of the progress bar,     */
/*           used to snap shot for documentation upon vertical bar            */
/*           segmentation                                                     */
/* Unit    : None                                                             */
/* Range   : None                                                             */
/* List    : None                                                             */
/* Example : None                                                             */
/* WARNING : Force VERTICAL_BAR calculation if used with dCSP_VERTICAL_BAR    */
/******************************************************************************/
//#define dCSP_DISPLAY_STRETCH

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress window

class CSkinProgress : public CStatic
{
  public: // --- PUBLIC -----------------------------------------------------
    // Construction
	  CSkinProgress();                                    // Default constructor
	  CSkinProgress(LPCTSTR       i_poStrMessage,         // Default constructor with parameters
                  int           i_nSize       = 200,
                  int           i_nUpper      = 100, 
                  int           i_nPane       = 0,
                  int           i_nProgress   = cSPT_PERCENT,
                  CBitmap*      i_poBitmap    = NULL);

    // Attributes

    // Operations
	  BOOL SetRange  (int         i_nLower,               // Set the new range
                    int         i_nUpper,
                    int         i_nStep       = 1,
                    BOOL        i_bResamble   = false,
                    BOOL        i_bDisplay    = true);
	  BOOL SetText   (LPCTSTR     i_poStrMessage,         // Set the new text
                    BOOL        i_bDisplay    = true);
	  BOOL SetSize   (int         i_nSize,                // Set size of the progress bar in pane 0 
                    BOOL        i_bDisplay    = true);
    BOOL SetBitmap (CBitmap*    i_poBitmap    = NULL,   // Change of progress bar image
                    CXTPStatusBar* i_poStatusBar = NULL,
                    BOOL        i_bDisplay    = true);
    BOOL CopyBitmap(CBitmap*    o_poBitmap,             // Copy a bitmap
                    CBitmap*    i_poBitmap);
    BOOL ConvBitmap(CBitmap*    o_poBitmap,             // Convert a bitmap to a specified device context
                    CBitmap*    i_poBitmap,
                    CDC*        i_poDC        = NULL);
	  int  SetPos    (int         i_nPos,                 // Set m_nRight value
                    BOOL        i_bDisplay    = true);
	  int  OffsetPos (int         i_nOffset,              // Forward of nOffset value
                    BOOL        i_bDisplay    = true);
	  int  SetStep   (int         i_nStep);               // Set m_nStep value
	  int  StepIt    (BOOL        i_bDisplay    = true);  // Forward of m_nStep value
	  void Clear     ();                                  // Deletion of the progress bar, restoration of the context

	  int  GetPane() { return m_nPane; }
    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CSkinProgress)
	    //}}AFX_VIRTUAL

    // Implementation
	  virtual ~CSkinProgress();

  protected: // --- PROTECTED -----------------------------------------------
    // Attributes

#ifdef dCSP_VERTICAL_BAR
    int            m_nVertical;                         // true if vertical *NEVER USE IN CSkinProgress, USED IN CSkinSlider*
#endif // dCSP_VERTICAL_BAR
    int            m_nSize;                             // Percentage size of control (if <0, autosize with the text if in pane 0)
    int            m_nPane;                             // ID of status bar pane progress bar is to appear in

    CString        m_oStrMessage;                       // Message to display in pane 0
    CString        m_oStrPrevText;                      // Previous text in pane m_nPane

    int            m_nProgressText;                     // Settings of the Progress text

    int            m_nLower;                            // Min value
    int            m_nUpper;                            // Max value
    int            m_nStep;                             // Step increment

#ifdef dCSP_SLIDERBAR_METHOD
    int            m_nPrevLeftAff;                      // For 'OnPaint'
    int            m_nPrevLeft;                         // Previous position (m_nLower <= m_nPrevLeft <= m_nUpper)
    int            m_nLeft;                             // Current position  (m_nLower <= m_nLeft <= m_nUpper)
#endif // dCSP_SLIDERBAR_METHOD

    int            m_nPrevRightAff;                     // For 'OnPaint'
    int            m_nPrevRight;                        // Previous position (m_nLower <= m_nPrevRight <= m_nUpper)
    int            m_nRight;                              // Current position  (m_nLower <= m_nRight <= m_nUpper)

    int            m_nBmpWidth;                         // Width of each m_BarImgLst element
    int            m_nBmpHeight;                        // Height of each m_BarImgLst element

    CRect          m_oRectPane;                         // Dimension of the progress bar

    CBitmap*       m_poBaseBmp;                         // The base bitmap
    CBitmap*       m_poCompatibleBmp;                   // The client DC compatible base bitmap
    CBitmap*       m_poStretchBmp  ;                    // The stretched picture to adapt it to the bar size
    CImageList     m_oBarImgLst;                        // The ImageList that contains all the progress bar picture elements
    CBitmap*       m_poProgressBmp;                     // The progress bar, created and displayed in 'OnPaint

    SYSTEMTIME     m_sStartTimeSystem;                  // Creation time of the SkinProgress
    ULARGE_INTEGER m_uStartTimeLarge;                   // Same for 'GetTimed'

    // Operations
	  BOOL        Create(LPCTSTR  i_poStrMessage,         // I have put here 'Create', to avoid to call it twice on an existing SkinProgress !
                       int      i_nSize     = 200,
                       int      i_nUpper    = 100, 
                       int      i_nPane     = 0,
                       int      i_nProgress = cSPT_PERCENT,
                       CBitmap* i_poBitmap  = NULL);
    CXTPStatusBar* GetStatusBar();                         // Get the window's status bar handler
    BOOL        ResizeTextPane();                       // The complete calculation of the text paning
    BOOL        CompactText(CString* io_pString,        // Calculation of the '...' text that fits in the (nColumnLen + nOffset) place
                            CDC*     i_pDC, 
                            int      i_nMaxWidth, 
                            int      i_nAddWidth = 0);
    BOOL        GetTimed(CString* o_poTimed);           // Get the complete timed message (64 bits calculation inside)

	  // Generated message map functions
	    //{{AFX_MSG(CSkinProgress)
	    afx_msg BOOL OnEraseBkgnd(CDC* i_pDC);
	    afx_msg void OnPaint();
	    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	    afx_msg void OnSize(UINT nType, int cx, int cy);
	    afx_msg void OnTimer(UINT nIDEvent);
	  //}}AFX_MSG

	  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINPROGRESS_H__1DF43017_611B_44BD_822A_FAA0E01B2257__INCLUDED_)
