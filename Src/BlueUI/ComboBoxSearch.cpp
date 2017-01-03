// ComboBoxSearch.cpp: implementation of the CComboBoxSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "ComboBoxSearch.h"
#include "InetTools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_XTP_CONTROL(CComboBoxURL, CXTPControlComboBox)

CComboBoxURL::CComboBoxURL()
{
	m_nEditIconId = 0;//ID_FAVORITE_LINK;
}

void CComboBoxURL::OnClick(BOOL bKeyboard , CPoint pt)
{
	if (!bKeyboard && pt.x < m_rcControl.left + 16 + 5 && !m_pParent->IsCustomizeMode())
	{
	
		NMXTPCONTROL tagNMCONTROL;
		tagNMCONTROL.hdr.code = ID_LINK_STARTDRAG;
		tagNMCONTROL.hdr.idFrom = m_nId;
		tagNMCONTROL.hdr.hwndFrom = 0;
		tagNMCONTROL.pControl = this;
		m_pParent->GetSite()->SendMessage(WM_XTP_COMMAND, m_nId, (LPARAM)&tagNMCONTROL);

		return;
	}
	
	CXTPControlComboBox::OnClick(bKeyboard, pt);
}

void CComboBoxURL::OnMouseMove(CPoint pt) 
{
	if (pt.x < m_rcControl.left + 16 + 5)
	{
		SetCursor(XTAuxData().hcurHand);
	}
	
}


void CComboBoxURL::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControlComboBox::DoPropExchange(pPX);
}

void CComboBoxURL::UpdateComboBox()
{
	CString strEntry;
	HKEY hReg;
	TCHAR chData[MAX_PATH];
	DWORD dwType = REG_SZ;

	ResetContent();
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), 0, KEY_READ, &hReg) == ERROR_SUCCESS)
	{

		for (int iMRU = 0; ;iMRU++ )
		{
			strEntry.Format(_T("url%i"), iMRU + 1);

			DWORD dwData = MAX_PATH - 1;
			if (RegQueryValueEx(hReg, strEntry, NULL, &dwType , (LPBYTE)&chData, &dwData) == ERROR_SUCCESS)
			{
				AddString(chData);
			} 
			else break;
		}
		
		RegCloseKey(hReg);
	}
}

void CComboBoxURL::SaveTypedURL()
{
	
	HKEY            hKey;
	try
	{
		RegDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"));

		if(RegCreateKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedUrls"), &hKey) != ERROR_SUCCESS)
			return ;

		int nCount = GetCount();
		
		CString str;
		CString id;
		int j = 0;
		for(int i = 0; i < nCount; i++)
		{			
			GetLBText(i, str);
			
			if (!str.IsEmpty())
			{
				id.Format(_T("url%i"), ++j);	
				RegSetValueEx(hKey, id, NULL, REG_SZ, (LPBYTE)(LPCTSTR)str, (str.GetLength() + 1) * sizeof(TCHAR));
			}
		}

		RegCloseKey(hKey);		
	}
	catch(...)
	{
	}	

}

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
IMPLEMENT_XTP_CONTROL(CComboBoxSearch, CXTPControlComboBox)

CComboBoxSearch::CComboBoxSearch()
{
	m_brWindow.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	
	CPageSearch::UpdateComboBox(this);

	if (CPageSearch::m_arrEngines.GetSize() > 0)
	{
		m_strEditHint = CPageSearch::m_arrEngines[0].strTitle;
		m_nCurrentEngine = 0;
	}
	else
		m_nCurrentEngine = -1;

	
	m_strEditText = m_strEditHint;
}

CComboBoxSearch::~CComboBoxSearch()
{

}

void CComboBoxSearch::OnEditChanged()
{
	
}

void CComboBoxSearch::OnSelChanged()
{
	BOOL bUpdateText = (GetEditText().IsEmpty() && !IsFocused()) || GetEditText() == m_strEditHint;

	m_nCurrentEngine = GetCurSel();
	m_strEditHint = m_nCurrentEngine != -1? CPageSearch::m_arrEngines[m_nCurrentEngine].strTitle: _T("");

	if (bUpdateText)
		CXTPControlComboBox::OnSelChanged();	

}

BOOL CComboBoxSearch::OnSetPopup(BOOL bPopup)
{
	if (bPopup && m_pCommandBar)
	{
		CPageSearch::UpdateComboBox(this);
	}
	
	return CXTPControlComboBox::OnSetPopup(bPopup);
}
*/