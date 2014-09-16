#pragma once
#include <vector>
#include "afxcmn.h"

// CDialogCensus �Ի���

class CDialogCensus : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogCensus)
	typedef std::vector<unsigned long long> CensusVector;
public:
	CDialogCensus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogCensus();

	void update(unsigned long long s, unsigned long long e, int unused, int used, int unsent, int sent);
// �Ի�������
	enum { IDD = IDD_DIALOG_CENSUSINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnBnClickedButtonShowdetail();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckSent();
	afx_msg void OnClickedCheckUnsent();
	afx_msg void OnClickedCheckUnused();
	afx_msg void OnClickedCheckUsed();

private:
	void initListView();

private:
	CListCtrl censusListCtrl_;
};
