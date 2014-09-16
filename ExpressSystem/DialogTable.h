#pragma once

#include "DatabaseADO.h"
// CDialogTable �Ի���

class CDialogTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTable)
	typedef DatabaseADO::DataField DataField;
public:
	enum class DialogType : char
	{
		SEARCH_DLG,
		INSERT_DLG,
		MODIFY_DLG
	};

	CDialogTable(DialogType type, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogTable();

	void DataPropertyPtr(DataField& df);
	DataField* DataPropertyPtr()
	{
		return dataPropertyPtr_;
	}

	int TableNumber()
	{
		return tableNumber_;
	}

// �Ի�������
	enum { IDD = IDD_DIALOG_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void initDataPtr();
	virtual BOOL OnInitDialog();
	void updateDlg();

	// init for different types of dialog
	void initSearchDlg();
	void initInsertDlg();
	void initModifyDlg();
	afx_msg void OnBnClickedOk();

private:
	DataField* dataPropertyPtr_;
	DialogType typeDialog_;
	ULONGLONG tableID_;

	int tableNumber_;
};
