
// ExpressSystemDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "DatabaseADO.h"
#include "DialogCensus.h"
#include "ListSortImpl.h"

// CExpressSystemDlg 对话框
class CExpressSystemDlg : public CDialogEx
{
	typedef DatabaseADO::DataField DataField;
	typedef std::vector<DataField> SystemVector;
	typedef std::vector<SystemVector> SystemDoubleVector;
	typedef std::vector<int>	IntVector;

	static const int MaxCommandNumber{ 10 };
// 构造
public:
	CExpressSystemDlg(DatabaseADO&, DatabaseADO&, CWnd* pParent = NULL);	// 标准构造函数

	void insertItemByTableID(SystemVector& vec);

	void updateCensus(bool isUnused = false, bool isUsed = false, bool isUnsent = false, bool isSent = false, int nItem = 0);
// 对话框数据
	enum { IDD = IDD_EXPRESSSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void disableUndo();

private:
	void openAccess();
	void initDatabaseListView();
	void insertItems(DataField&);
	void getItems(int iItem, DataField*);
	void disableAllButtons();
	void enableAllButtons();

	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnDblclkListDatabase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpen();

	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonCensus();
	afx_msg void OnBnClickedButtonShowall();
	afx_msg void OnBnClickedButtonMerge();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonShowout();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnItemclickListDatabase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusListDatabase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDatabase(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void addCommand(int undoNum);
	void clearAllListItems();

private:
	CListCtrl databaseListViewCtrl_;
	DatabaseADO& databaseADO_;
	DatabaseADO& outbaseADO_;
	CDialogCensus censusDlg_;

	IntVector undoCount_;

	SystemDoubleVector unusedVect_;
	SystemDoubleVector usedVect_;
	SystemDoubleVector unsentVect_;
	SystemDoubleVector sentVect_;	

	ListSortImpl listSortImpl_;
	int itemDataSort_;
};
