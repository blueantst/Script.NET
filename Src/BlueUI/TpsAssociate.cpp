// TpsAssociate.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TpsAssociate.h"
#include "cgfiltyp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLUMN_DOCNAME		    0
#define COLUMN_EXT              1
#define COLUMN_OWM              2
#define COLUMN_CHECK            3

#define COLUMN_MAIL_ICON    0
#define COLUMN_CHECK_ICON   2

#define ID_REPORT_CONTROL 100

/////////////////////////////////////////////////////////////////////////////
// CTpsAssociate dialog


CTpsAssociate::CTpsAssociate(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsAssociate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsAssociate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTpsAssociate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsAssociate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_ASSOCIATE_TREE, m_wndReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsAssociate, CDialog)
	//{{AFX_MSG_MAP(CTpsAssociate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsAssociate message handlers

BOOL CTpsAssociate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Regist Shell File Type
	CGCFileTypeAccess TheFTA;
	TheFTA.SetApplicationName(theApp.m_xmlPlat.GetNodeText("application\\appName"));

	VERIFY(m_ilIcon.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));

	// 添加图标资源
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(!theApp.m_aPlugIns[i].IsVciOwm())
		{
			continue;
		}

		for(int j=0; j<theApp.m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
		{
			if(theApp.m_aPlugIns[i].m_arDocExtend[j].m_strIcon == "")
			{
				continue;
			}

			CString strIconFile = theApp.m_aPlugIns[i].m_strPath + "\\";
			strIconFile += theApp.m_aPlugIns[i].m_arDocExtend[j].m_strIcon;

			WORD wIndex = 0;
			HICON hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
				strIconFile.GetBuffer(0), &wIndex);
			strIconFile.ReleaseBuffer();
			m_ilIcon.Add(hIcon);
		}
	}

	m_wndReport.SetImageList(&m_ilIcon);

	m_wndReport.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_TABSTOP);
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.SetGridStyle(FALSE, (XTPReportGridStyle)xtpReportGridSmallDots);
	m_wndReport.SetGridStyle(TRUE, (XTPReportGridStyle)xtpReportGridSmallDots);

	//m_wndReport.GetPaintManager()->m_columnStyle = xtpReportColumnOffice2003;
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnOffice2007);

	//
	//  Add columns
	//
	PLAT_LOADSTRING(strTpsAssDocName, IDS_TPS_ASS_DOCNAME);	// 文档名
	PLAT_LOADSTRING(strTpsAssDocExt, IDS_TPS_ASS_DOCEXT);	// 文件后缀
	PLAT_LOADSTRING(strTpsAssOwmId, IDS_TPS_ASS_OWMID);		// 扩展模块ID
	PLAT_LOADSTRING(strTpsAssAssociate, IDS_TPS_ASS_ASSOCIATE);	// 关联
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_DOCNAME, strTpsAssDocName, 150, FALSE, COLUMN_MAIL_ICON));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_EXT, strTpsAssDocExt, 80));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_OWM, strTpsAssOwmId, 120));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CHECK,strTpsAssAssociate, 48));

	CXTPReportRecord* pRecord;

	int nIconIndex = 0;

	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(!theApp.m_aPlugIns[i].IsVciOwm())
		{
			// 只处理owm模块
			continue;
		}

		for(int j=0; j<theApp.m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
		{
			if(!theApp.m_aPlugIns[i].m_arDocExtend[j].m_bEnableLink)
			{
				// 不允许建立关联
				continue;
			}

			pRecord = new CXTPReportRecord();
			pRecord->AddItem(new CXTPReportRecordItemText(theApp.m_aPlugIns[i].m_arDocExtend[j].m_strDocName));
			BOOL bSingleExt = (theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize() == 1);
			if(bSingleExt)
			{
				pRecord->AddItem(new CXTPReportRecordItemText(theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[0]));
			}else
			{
				pRecord->AddItem(new CXTPReportRecordItemText("..."));
			}
			pRecord->AddItem(new CXTPReportRecordItemText(theApp.m_aPlugIns[i].m_strId));
			if(bSingleExt)
			{
				TheFTA.SetExtension(theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[0]);

				CXTPReportRecordItem* pItemCheck = new CXTPReportRecordItem();
				pItemCheck->HasCheckbox(TRUE);
				pItemCheck->SetChecked(TheFTA.CheckIsRegistered());
				pRecord->AddItem(pItemCheck);
			}else
			{
				pRecord->AddItem(new CXTPReportRecordItemText(""));
			}

			pRecord->GetItem(0)->SetIconIndex(nIconIndex);

			pRecord->GetItem(0)->SetBackgroundColor(RGB(225,255,255));
			pRecord->GetItem(1)->SetBackgroundColor(RGB(225,255,255));
			pRecord->GetItem(2)->SetBackgroundColor(RGB(225,255,255));
			pRecord->GetItem(3)->SetBackgroundColor(RGB(225,255,255));

			pRecord->GetItem(0)->SetItemData((DWORD_PTR)(&(theApp.m_aPlugIns[i].m_arDocExtend[j])));

			m_wndReport.AddRecord(pRecord);

			if(theApp.m_aPlugIns[i].m_arDocExtend[j].m_strIcon != "")
			{
				nIconIndex++;
			}

			if(bSingleExt)
			{
				continue;
			}

			CXTPReportRecords* pRecords = pRecord->GetChilds();
			for(int k=0; k<theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize(); k++)
			{
				TheFTA.SetExtension(theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k]);

				CXTPReportRecord* pRecordChild = new CXTPReportRecord();
				pRecordChild->AddItem(new CXTPReportRecordItemText(""));
				pRecordChild->AddItem(new CXTPReportRecordItemText(theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k]));
				pRecordChild->AddItem(new CXTPReportRecordItemText(""));				
				CXTPReportRecordItem* pItemCheck = new CXTPReportRecordItem();
				pItemCheck->HasCheckbox(TRUE);
				pItemCheck->SetChecked(TheFTA.CheckIsRegistered());
				pRecordChild->AddItem(pItemCheck);

				pRecordChild->GetItem(0)->SetBackgroundColor(RGB(255,255,225));
				pRecordChild->GetItem(1)->SetBackgroundColor(RGB(255,255,225));
				pRecordChild->GetItem(2)->SetBackgroundColor(RGB(255,255,225));
				pRecordChild->GetItem(3)->SetBackgroundColor(RGB(255,255,225));

				pRecordChild->GetItem(0)->SetItemData((DWORD_PTR)(&(theApp.m_aPlugIns[i].m_arDocExtend[j])));

				pRecords->Add(pRecordChild);
			}
		}
	}

	m_wndReport.GetColumns()->Find(COLUMN_DOCNAME)->SetTreeColumn(TRUE);

	// Populate all data on control
	m_wndReport.Populate();
	//m_wndReport.ExpandAll();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsAssociate::OnOK() 
{
	UpdateData(TRUE);

	// Regist Shell File Type
	CGCFileTypeAccess TheFTA;
	TheFTA.SetApplicationName(theApp.m_xmlPlat.GetNodeText("application\\appName"));

	TCHAR	szProgPath[MAX_PATH * 2];
	::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));

	CString csTempText;

	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		BOOL b = pRecord->HasChildren();
		CXTPReportRecordItem* p1 = pRecord->GetItem(2);
		BOOL b1 = p1->IsChecked();

		if(!pRecord->HasChildren() && pRecord->GetItem(3)->IsChecked())
		{
			CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strExt = pItemText->GetValue();
			if(strExt != "")
			{
				COwmDocExtend* pOwmDocExtend = (COwmDocExtend*)(pRecord->GetItem(0)->GetItemData());
				if(!pOwmDocExtend)
				{
					continue;
				}

				// 注册文件后缀
				TheFTA.SetExtension(strExt);

				// just pass file path in quotes on command line
				csTempText  = szProgPath;
				csTempText += " \"%1\"";
				TheFTA.SetShellOpenCommand(csTempText);
				TheFTA.SetDocumentShellOpenCommand(csTempText);

				TheFTA.SetDocumentClassName(strExt + ".Document");

				// use first icon in program
				//csTempText  = szProgPath;
				//csTempText += ",1";
				csTempText = theApp.GetOWM(pOwmDocExtend->m_strOwmId)->m_strPath + "\\";
				csTempText += pOwmDocExtend->m_strIcon;
				TheFTA.SetDocumentDefaultIcon(csTempText);
				
				TheFTA.RegSetAllInfo();
			}
		}else
		if(pRecord->HasChildren())
		{
			CXTPReportRecords* pRecordChilds = pRecord->GetChilds();
			for(int j=0; j<pRecordChilds->GetCount(); j++)
			{
				CXTPReportRecord* pRecordChild = pRecordChilds->GetAt(j);
				if(pRecordChild->GetItem(3)->IsChecked())
				{
					CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecordChild->GetItem(1);
					CString strExt = pItemText->GetValue();
					if(strExt != "")
					{
						COwmDocExtend* pOwmDocExtend = (COwmDocExtend*)(pRecordChild->GetItem(0)->GetItemData());
						if(!pOwmDocExtend)
						{
							continue;
						}

						// 注册文件后缀
						TheFTA.SetExtension(strExt);

						// just pass file path in quotes on command line
						csTempText  = szProgPath;
						csTempText += " \"%1\"";
						TheFTA.SetShellOpenCommand(csTempText);
						TheFTA.SetDocumentShellOpenCommand(csTempText);

						TheFTA.SetDocumentClassName(strExt + ".Document");

						// use first icon in program
						//csTempText  = szProgPath;
						//csTempText += ",1";
						csTempText = theApp.GetOWM(pOwmDocExtend->m_strOwmId)->m_strPath + "\\";
						csTempText += pOwmDocExtend->m_strIcon;
						TheFTA.SetDocumentDefaultIcon(csTempText);
						
						TheFTA.RegSetAllInfo();
					}
				}
			}
		}
	}

	CDialog::OnOK();
}
