// ComboBoxSearch.cpp: implementation of the CComboBoxSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HtmlViewOWM.h"
#include "ComboBoxWord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_XTP_CONTROL(CComboBoxWord, CXTPControlComboBox)

CComboBoxWord::CComboBoxWord()
{
	m_nEditIconId = 0;//ID_FAVORITE_LINK;
}

void CComboBoxWord::OnClick(BOOL bKeyboard , CPoint pt)
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
	}else
	if(bKeyboard)
	{
		//SaveWordItem();
		m_pParent->GetSite()->SendMessage(WM_COMMAND, ID_HELP_SEARCH, NULL);
	}
	
	CXTPControlComboBox::OnClick(bKeyboard, pt);
}

void CComboBoxWord::OnMouseMove(CPoint pt) 
{
	if (pt.x < m_rcControl.left + 16 + 5)
	{
		SetCursor(XTAuxData().hcurHand);
	}
}

void CComboBoxWord::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControlComboBox::DoPropExchange(pPX);
}

void CComboBoxWord::UpdateComboBox()
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	CString strEntry;
	HKEY hReg;
	TCHAR chData[MAX_PATH];
	DWORD dwType = REG_SZ;

	ResetContent();

	CString strRegKey = theApp.m_pIPlatUI->GetPlatRegistry();
	strRegKey += "\\owm\\Workspace\\DynamicHelp\\Word";
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, 
		strRegKey, 0, KEY_READ, &hReg) == ERROR_SUCCESS)
	{
		for (int iMRU = 0; ;iMRU++ )
		{
			strEntry.Format(_T("item%i"), iMRU + 1);

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

#define MAX_HISTORY_COUNT 20

void CComboBoxWord::SaveWordItem()
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	HKEY            hKey;
	try
	{
		CString strRegKey = theApp.m_pIPlatUI->GetPlatRegistry();
		strRegKey += "\\owm\\Workspace\\DynamicHelp\\Word";

		RegDeleteKey(HKEY_CURRENT_USER, strRegKey);

		if(RegCreateKey(HKEY_CURRENT_USER, strRegKey, &hKey) != ERROR_SUCCESS)
			return ;

		CString strText = GetEditText();
		int nIndex = FindString(-1, strText);
		if (nIndex == -1)
		{
			InsertString(0, strText);
			while(GetCount() > MAX_HISTORY_COUNT)
			{
				DeleteItem(GetCount()-1);
			}
		}else
		if(nIndex != 0)
		{
			DeleteItem(nIndex);
			InsertString(0, strText);
		}

		int nCount = GetCount();
		
		CString str;
		CString id;
		int j = 0;
		for(int i = 0; i < nCount; i++)
		{			
			GetLBText(i, str);
			
			if (!str.IsEmpty())
			{
				id.Format(_T("item%i"), ++j);	
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