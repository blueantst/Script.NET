// TpsLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TpsLanguage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsLanguage dialog


CTpsLanguage::CTpsLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsLanguage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsLanguage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurrentLanguageIndex = -1;
}


void CTpsLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsLanguage)
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsLanguage, CDialog)
	//{{AFX_MSG_MAP(CTpsLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsLanguage message handlers

BOOL CTpsLanguage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CXmlParser xmlLanguage;
	CString strLanguageCfgFile = theApp.GetPlatRootPath();
	strLanguageCfgFile += "conf\\language.xml";
	xmlLanguage.Open(strLanguageCfgFile);

	int nCurrentLanguage = theApp.GetProfileInt(REG_VERSION_SUBKEY, REG_REG_LANGUAGE, ::GetSystemDefaultLangID());

	DOMNode* pNode = xmlLanguage.GetNode("language\\SupportLanguage\\Language");
	while (pNode != NULL)
	{
		CString strLanguage = xmlLanguage.GetNodeText(pNode);
		int nLangId = atoi(xmlLanguage.GetNodeAttribute(pNode, "id"));
		int nIndex = m_comboLanguage.AddString(strLanguage);
		m_comboLanguage.SetItemData(nIndex, nLangId);

		if(nCurrentLanguage == nLangId)
		{
			m_comboLanguage.SetCurSel(nIndex);
			m_nCurrentLanguageIndex = nIndex;
		}

		pNode = xmlLanguage.GetNextNode(pNode, "Language");
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsLanguage::OnOK() 
{
	int nSel = m_comboLanguage.GetCurSel();

	// 只有更改了语言选项才会去设置
	if(m_nCurrentLanguageIndex != nSel)
	{
		int nLanguage = m_comboLanguage.GetItemData(nSel);
		theApp.WriteProfileInt(REG_VERSION_SUBKEY, REG_REG_LANGUAGE, nLanguage);
	}

	CDialog::OnOK();
}