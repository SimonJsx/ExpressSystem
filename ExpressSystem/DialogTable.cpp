// DialogTable.cpp : 实现文件
//

#include "stdafx.h"
#include "ExpressSystem.h"
#include "DialogTable.h"
#include "afxdialogex.h"


// CDialogTable 对话框

IMPLEMENT_DYNAMIC(CDialogTable, CDialogEx)

CDialogTable::CDialogTable(DialogType type, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTable::IDD, pParent),
	typeDialog_(type)
	, tableID_(0)
	, tableNumber_(1)
{
	dataPropertyPtr_ = new DataField;
	initDataPtr();
}

CDialogTable::~CDialogTable()
{
	delete dataPropertyPtr_;
}

void CDialogTable::DataPropertyPtr(DataField& df)
{
	memcpy(dataPropertyPtr_, &df, sizeof(dataPropertyPtr_));

	dataPropertyPtr_->tableID = df.tableID;
	strcpy(dataPropertyPtr_->company, df.company);
	strcpy(dataPropertyPtr_->comments, df.comments);
	strcpy(dataPropertyPtr_->companyPhone, df.companyPhone);
	dataPropertyPtr_->isSent = df.isSent;
	dataPropertyPtr_->isUsed = df.isUsed;
	strcpy(dataPropertyPtr_->sendTime, df.sendTime);
	strcpy(dataPropertyPtr_->useTime, df.useTime);
	strcpy(dataPropertyPtr_->workerName, df.workerName);
}

void CDialogTable::initDataPtr()
{
	dataPropertyPtr_->setNull();
}

void CDialogTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TABLEID, tableID_);
	DDX_Text(pDX, IDC_EDIT_TABLENUMBER, tableNumber_);
}


BEGIN_MESSAGE_MAP(CDialogTable, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogTable::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogTable 消息处理程序


BOOL CDialogTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	switch (typeDialog_)
	{
	case CDialogTable::DialogType::SEARCH_DLG:

		initSearchDlg();
		break;
	case CDialogTable::DialogType::INSERT_DLG:

		initInsertDlg();
		break;
	case CDialogTable::DialogType::MODIFY_DLG:

		initModifyDlg();
		break;
	default:
		break;
	}

	updateDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDialogTable::updateDlg()
{
	USES_CONVERSION;
	CString str;
	str.Format(L"%llu", dataPropertyPtr_->tableID);
	SetDlgItemText(IDC_EDIT_TABLEID, str);

	str.Format(L"%s", A2T(dataPropertyPtr_->workerName));
	SetDlgItemText(IDC_EDIT_WORKERNAME, str);

	str.Format(L"%s", A2T(dataPropertyPtr_->company));
	SetDlgItemText(IDC_EDIT_COMPANY, str);

	str.Format(L"%s", A2T(dataPropertyPtr_->companyPhone));
	SetDlgItemText(IDC_EDIT_COMPANYPHONE, str);

	str.Format(L"%s", A2T(dataPropertyPtr_->sendTime));
	SetDlgItemText(IDC_EDIT_SENDTIME, str);

	str.Format(L"%s", A2T(dataPropertyPtr_->useTime));
	SetDlgItemText(IDC_EDIT_USETIME, str);

	if (dataPropertyPtr_->isSent == 0)
		((CButton*)GetDlgItem(IDC_CHECK_SEND))->SetCheck(1);
	if (dataPropertyPtr_->isUsed == 0)
		((CButton*)GetDlgItem(IDC_CHECK_USE))->SetCheck(1);

	if (DatabaseADO::BadString == dataPropertyPtr_->comments)
		((CButton*)GetDlgItem(IDC_CHECK_COMMENTS))->SetCheck(true);
}

void CDialogTable::initSearchDlg()
{
	this->SetWindowTextW(L"查询表单");

	this->GetDlgItem(IDC_STATIC_ISSEND)->ShowWindow(false);
	this->GetDlgItem(IDC_CHECK_USE)->ShowWindow(false);

	this->GetDlgItem(IDC_STATIC_ISUSED)->ShowWindow(false);
	this->GetDlgItem(IDC_CHECK_SEND)->ShowWindow(false);

	this->GetDlgItem(IDC_CHECK_COMMENTS)->ShowWindow(false);
	this->GetDlgItem(IDC_STATIC_COMMENTS)->ShowWindow(false);
}

void CDialogTable::initInsertDlg()
{
	this->SetWindowTextW(L"添加表单");
}

void CDialogTable::initModifyDlg()
{
	this->SetWindowTextW(L"修改表单");

	this->GetDlgItem(IDC_STATIC_TABLENUMBER)->ShowWindow(false);
	this->GetDlgItem(IDC_EDIT_TABLENUMBER)->ShowWindow(false);

	this->GetDlgItem(IDC_EDIT_TABLEID)->EnableWindow(false);		// cannot modify tableid
}

void CDialogTable::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	USES_CONVERSION;
	dataPropertyPtr_->tableID = tableID_;

	CString str;
	GetDlgItemText(IDC_EDIT_WORKERNAME, str);
	strcpy(dataPropertyPtr_->workerName, T2A(str));

	GetDlgItemText(IDC_EDIT_COMPANY, str);
	strcpy(dataPropertyPtr_->company, T2A(str));

	GetDlgItemText(IDC_EDIT_COMPANYPHONE, str);
	strcpy(dataPropertyPtr_->companyPhone, T2A(str));

	GetDlgItemText(IDC_EDIT_SENDTIME, str);
	strcpy(dataPropertyPtr_->sendTime, T2A(str));

	GetDlgItemText(IDC_EDIT_USETIME, str);
	strcpy(dataPropertyPtr_->useTime, T2A(str));

	if (((CButton*)GetDlgItem(IDC_CHECK_COMMENTS))->GetCheck() == 0)
	{
		strcpy(dataPropertyPtr_->comments, DatabaseADO::NormalString.c_str());
	}
	else
	{
		strcpy(dataPropertyPtr_->comments, DatabaseADO::BadString.c_str());
	}

	dataPropertyPtr_->isSent = ((CButton*)GetDlgItem(IDC_CHECK_SEND))->GetCheck() == 0 ? -1 : 0;
	dataPropertyPtr_->isUsed = ((CButton*)GetDlgItem(IDC_CHECK_USE))->GetCheck() == 0 ? -1 : 0;
	
	if (typeDialog_ == DialogType::SEARCH_DLG)
	{
		dataPropertyPtr_->isSent = 1;	// not for search
		dataPropertyPtr_->isUsed = 1;	// not for search
		dataPropertyPtr_->comments[0] = 0;	// not for search
	}


	CDialogEx::OnOK();
}
