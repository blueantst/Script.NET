// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "ClientSocket.h"

#include "PTerminalView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOADRCSTRING(str,strID) \
	get_dll_resource();\
	str.LoadString(strID);\
	reset_dll_resource();

void get_dll_resource(void);
void reset_dll_resource(void);

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CPTerminalView* pView)
{
	m_pView = pView;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnClose(int nErrorCode) 
{
	CString strTemp;
	LOADRCSTRING(strTemp, IDS_SOCK_CLOSE)
	AfxMessageBox(strTemp,MB_OK);
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
//	AfxMessageBox("Appear to have connected",MB_OK);
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnOutOfBandData(int nErrorCode) 
{
	CString strTemp;
	LOADRCSTRING(strTemp, IDS_SOCK_RC_OUTBAND_DATA)
	AfxMessageBox(strTemp,MB_OK);
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	m_pView->ProcessMessage(this);
}

void CClientSocket::OnSend(int nErrorCode) 
{
	CAsyncSocket::OnSend(nErrorCode);
}
