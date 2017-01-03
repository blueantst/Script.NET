// ControlFavorites.h: interface for the CControlFavorites class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLFAVORITES_H__845657C3_FF0D_4909_8D8E_D9C9814C67F7__INCLUDED_)
#define AFX_CONTROLFAVORITES_H__845657C3_FF0D_4909_8D8E_D9C9814C67F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CControlFavoriteLink : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CControlFavoriteLink)

public:
	CString m_strFileName;
};


class CControlFavoriteFolder : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CControlFavoriteFolder)

public:
	CControlFavoriteFolder(LPCTSTR strRootDir = 0);
	
protected:
	virtual void OnCalcDynamicSize(DWORD dwMode);
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);
	virtual void Copy(CXTPControl* pControl, BOOL bRecursive);
	virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
	CString m_strRootDir;
};

#endif // !defined(AFX_CONTROLFAVORITES_H__845657C3_FF0D_4909_8D8E_D9C9814C67F7__INCLUDED_)
