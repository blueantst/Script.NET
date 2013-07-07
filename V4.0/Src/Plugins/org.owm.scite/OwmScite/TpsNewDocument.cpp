// TpsNewDocument.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "TpsNewDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsNewDocument dialog


CTpsNewDocument::CTpsNewDocument(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsNewDocument::IDD, pParent)
{
	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
	NewDocDefaultSettings & ndds = (NewDocDefaultSettings &)nppGUI.getNewDocDefaultSettings();
	//{{AFX_DATA_INIT(CTpsNewDocument)
	m_nEncoding = ndds._encoding;
	m_nFormat = ndds._format;
	//}}AFX_DATA_INIT
}


void CTpsNewDocument::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsNewDocument)
	DDX_Radio(pDX, IDC_RADIO_MODE_ANSI, m_nEncoding);
	DDX_Radio(pDX, IDC_RADIO_FORMAT_WINDOWS, m_nFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsNewDocument, CDialog)
	//{{AFX_MSG_MAP(CTpsNewDocument)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsNewDocument message handlers
void CTpsNewDocument::OnOK() 
{
	UpdateData(TRUE);

	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
	NewDocDefaultSettings & ndds = (NewDocDefaultSettings &)nppGUI.getNewDocDefaultSettings();

	ndds._encoding = (UniMode)m_nEncoding;
	ndds._format = (formatType)m_nFormat;
	
	CDialog::OnOK();
}
