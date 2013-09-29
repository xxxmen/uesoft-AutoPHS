// ChangMaterilName.cpp : implementation file
//

#include "stdafx.h"
#include "autophs.h"
#include "ChangMaterilName.h"
#include "EDIBgbl.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangMaterilName dialog


CChangMaterilName::CChangMaterilName(CWnd* pParent /*=NULL*/)
	: CDialog(CChangMaterilName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangMaterilName)
	m_newMaterilName = _T("");
	//}}AFX_DATA_INIT
}


void CChangMaterilName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangMaterilName)
	DDX_Control(pDX, IDC_LIST_MATERNAME, m_materNameList);
	DDX_Control(pDX, IDC_LIST_VOLUME, m_volumeIdList);
	DDX_Control(pDX, IDC_LIST_WORKNAME, m_workNameList);
	DDX_Text(pDX, IDC_MATERILNAME_EDIT, m_newMaterilName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangMaterilName, CDialog)
	//{{AFX_MSG_MAP(CChangMaterilName)
	ON_NOTIFY(NM_CLICK, IDC_LIST_WORKNAME, OnClickListWorkname)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VOLUME, OnClickListVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangMaterilName message handlers

BOOL CChangMaterilName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;
	int width = 0;

	//设置报表样式
	DWORD styles = m_workNameList.GetExtendedStyle();
	m_workNameList.SetExtendedStyle(styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_workNameList.GetWindowRect(rect);
	width = rect.right-rect.left;
    m_workNameList.InsertColumn( 0, "工程代号", LVCFMT_LEFT, width/3 );
    m_workNameList.InsertColumn( 1, "工程名称", LVCFMT_LEFT, width - width/3 );

	//设置报表样式
	styles = m_volumeIdList.GetExtendedStyle();
	m_volumeIdList.SetExtendedStyle(styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_volumeIdList.GetWindowRect(rect);
	width = rect.right-rect.left;
    m_volumeIdList.InsertColumn( 0, "卷册代号", LVCFMT_LEFT, width/3 );
    m_volumeIdList.InsertColumn( 1, "卷册名称", LVCFMT_LEFT, width - width/3 );

	//设置报表样式
	styles = m_materNameList.GetExtendedStyle();
	m_materNameList.SetExtendedStyle(styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_materNameList.GetWindowRect(rect);
	width = rect.right-rect.left;
    m_materNameList.InsertColumn( 0, "材料名称", LVCFMT_LEFT, width );
	CDaoRecordset rsTmpEngin(&EDIBgbl::dbPRJDB );
	
	try
	{
		
		CString strSql = "SELECT EnginID,gcmc FROM Engin";
		
		rsTmpEngin.Open(dbOpenSnapshot,strSql);
		
		int i;
		CString strVal;
		
		m_workNameList.DeleteAllItems();
		while( !rsTmpEngin.IsEOF() )
		{
			i = 0;
			strVal = vtos(rsTmpEngin.GetFieldValue("EnginID"));
			m_workNameList.InsertItem(i,strVal);
			strVal = vtos(rsTmpEngin.GetFieldValue("gcmc"));
			m_workNameList.SetItemText(i,1,strVal);
			rsTmpEngin.MoveNext();
			
			
			i++;
		}
		
	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
	}

	if(rsTmpEngin.IsOpen())
	{
		rsTmpEngin.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CChangMaterilName::OnClickListWorkname(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_workNameList.GetItemCount() < 0 )
	{
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int x  = pNMListView->iItem;//KEY
	if(x == -1)
	{
		return;
	}

	CString strEnginID = m_workNameList.GetItemText(x,0);
	CString strSql;
	strSql.Format("SELECT jcdm,jcmc FROM Volume WHERE EnginID = '%s'",strEnginID);
	CDaoRecordset rsTmpVolume(&EDIBgbl::dbPRJDB );

	try
	{	
		rsTmpVolume.Open(dbOpenSnapshot,strSql);
		
		int i;
		CString strVal;
		
		m_volumeIdList.DeleteAllItems();
		while( !rsTmpVolume.IsEOF() )
		{
			i = 0;
			strVal = vtos(rsTmpVolume.GetFieldValue("jcdm"));
			m_volumeIdList.InsertItem(i,strVal);
			strVal = vtos(rsTmpVolume.GetFieldValue("jcmc"));
			m_volumeIdList.SetItemText(i,1,strVal);
			rsTmpVolume.MoveNext();
			
			
			i++;
		}
	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
	}

	if(rsTmpVolume.IsOpen())
	{
		rsTmpVolume.Close();
	}
	

	*pResult = 0;
}

void CChangMaterilName::OnClickListVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	if ( m_volumeIdList.GetItemCount() < 0 )
	{
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int x  = pNMListView->iItem;//KEY
	if(x == -1)
	{
		return;
	}

	CString strjcdm = m_volumeIdList.GetItemText(x,0);
	CString strSql;
	strSql.Format("SELECT CLcl FROM ZB WHERE VolumeID IN (SELECT VolumeID FROM Volume WHERE jcdm = '%s')",strjcdm);
	CDaoRecordset rsTmpMateril(&EDIBgbl::dbPRJDB );

	try
	{
		rsTmpMateril.Open(dbOpenSnapshot,strSql);
		
		int i;
		CString strVal;
		
		m_materNameList.DeleteAllItems();
		while( !rsTmpMateril.IsEOF() )
		{
			i = 0;
			strVal = vtos(rsTmpMateril.GetFieldValue("CLcl"));
			m_materNameList.InsertItem(i,strVal);
			rsTmpMateril.MoveNext();
			i++;
		}
	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
		return;
	}

	if(rsTmpMateril.IsOpen())
	{
		rsTmpMateril.Close();
	}


	*pResult = 0;
}

void CChangMaterilName::OnOK() 
{
	CString strtmp;
	int nVolumeIndex = m_volumeIdList.GetSelectionMark();
	int nMaterilIndex = m_materNameList.GetSelectionMark();
	CString strVolumeID = m_volumeIdList.GetItemText(nVolumeIndex,0);
	CString strMaterilName = m_materNameList.GetItemText(nMaterilIndex,0);
	try
	{
		UpdateData();
		if(m_newMaterilName.IsEmpty())
		{
			AfxMessageBox("请输入材料名");
			return;
		}
		CString strSql;
		strSql.Format("UPDATE ZB SET CLcl='%s' WHERE CLcl='%s' AND VolumeID IN (SELECT VolumeID FROM Volume WHERE jcdm = '%s')",m_newMaterilName,strMaterilName,strVolumeID);

		EDIBgbl::dbPRJDB.Execute(strSql);
		strtmp.Format("材料:'%s'已修改为'%s'",strMaterilName,m_newMaterilName);
		AfxMessageBox(strtmp);

	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
		return;
	}

	CDialog::OnOK();
}
