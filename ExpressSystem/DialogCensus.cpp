// DialogCensus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExpressSystem.h"
#include "DialogCensus.h"
#include "afxdialogex.h"
#include "ExpressSystemDlg.h"

// CDialogCensus �Ի���

IMPLEMENT_DYNAMIC(CDialogCensus, CDialogEx)

CDialogCensus::CDialogCensus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogCensus::IDD, pParent)
{

}

CDialogCensus::~CDialogCensus()
{
}

void CDialogCensus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CENSUS, censusListCtrl_);
}

void CDialogCensus::update(unsigned long long s, unsigned long long e, int unused, int used, int unsent, int sent)
{
	int endItem = censusListCtrl_.GetItemCount();
	censusListCtrl_.InsertItem(endItem, L"");

	CString str;
	int nItem = 0;
	str.Format(L"%llu", s);
	censusListCtrl_.SetItemText(endItem, nItem++, str);

	str.Format(L"%llu", e);
	censusListCtrl_.SetItemText(endItem, nItem++, str);

	str.Format(L"%d", unused);
	censusListCtrl_.SetItemText(endItem, nItem++, str);

	str.Format(L"%d", used);
	censusListCtrl_.SetItemText(endItem, nItem++, str);

	str.Format(L"%d", unsent);
	censusListCtrl_.SetItemText(endItem, nItem++, str);

	str.Format(L"%d", sent);
	censusListCtrl_.SetItemText(endItem, nItem++, str);
}

void CDialogCensus::initListView()
{
	censusListCtrl_.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int item = 0;
	censusListCtrl_.InsertColumn(item++, L"��ʼ����", LVCFMT_CENTER, 160);
	censusListCtrl_.InsertColumn(item++, L"��������", LVCFMT_CENTER, 160);
	censusListCtrl_.InsertColumn(item++, L"δʹ��", LVCFMT_CENTER, 60);
	censusListCtrl_.InsertColumn(item++, L"��ʹ��", LVCFMT_CENTER, 60);
	censusListCtrl_.InsertColumn(item++, L"δ����", LVCFMT_CENTER, 60);
	censusListCtrl_.InsertColumn(item++, L"�ѷ���", LVCFMT_CENTER, 60);
}

BEGIN_MESSAGE_MAP(CDialogCensus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SHOWDETAIL, &CDialogCensus::OnBnClickedButtonShowdetail)
	ON_BN_CLICKED(IDOK, &CDialogCensus::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SENT, &CDialogCensus::OnClickedCheckSent)
	ON_BN_CLICKED(IDC_CHECK_UNSENT, &CDialogCensus::OnClickedCheckUnsent)
	ON_BN_CLICKED(IDC_CHECK_UNUSED, &CDialogCensus::OnClickedCheckUnused)
	ON_BN_CLICKED(IDC_CHECK_USED, &CDialogCensus::OnClickedCheckUsed)
END_MESSAGE_MAP()


// CDialogCensus ��Ϣ�������


void CDialogCensus::OnBnClickedButtonShowdetail()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CExpressSystemDlg* pParent = reinterpret_cast<CExpressSystemDlg*>(this->GetParent());
	bool isUsed = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_USED))->GetCheck());
	bool isUnused = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_UNUSED))->GetCheck());
	bool isSent = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_SENT))->GetCheck());
	bool isUnsent = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_UNSENT))->GetCheck());
	auto pos = censusListCtrl_.GetFirstSelectedItemPosition();
	int nItem = censusListCtrl_.GetNextSelectedItem(pos);

	pParent->updateCensus(isUnused, isUsed, isUnsent, isSent, nItem);
}


void CDialogCensus::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	censusListCtrl_.DeleteAllItems();
	this->ShowWindow(false);
//	CDialogEx::OnOK();
}


void CDialogCensus::OnClickedCheckSent()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton* pSend = (CButton*)GetDlgItem(IDC_CHECK_SENT);

	if (pSend->GetCheck() == BST_CHECKED)
	{
		((CButton*)GetDlgItem(IDC_CHECK_UNSENT))->SetCheck(false);
	}
}


void CDialogCensus::OnClickedCheckUnsent()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton* pSend = (CButton*)GetDlgItem(IDC_CHECK_UNSENT);

	if (pSend->GetCheck() == BST_CHECKED)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SENT))->SetCheck(false);
	}
}


void CDialogCensus::OnClickedCheckUnused()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton* pSend = (CButton*)GetDlgItem(IDC_CHECK_UNUSED);

	if (pSend->GetCheck() == BST_CHECKED)
	{
		((CButton*)GetDlgItem(IDC_CHECK_USED))->SetCheck(false);
	}
}


void CDialogCensus::OnClickedCheckUsed()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton* pSend = (CButton*)GetDlgItem(IDC_CHECK_USED);

	if (pSend->GetCheck() == BST_CHECKED)
	{
		((CButton*)GetDlgItem(IDC_CHECK_UNUSED))->SetCheck(false);
	}
}


BOOL CDialogCensus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	initListView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
