
// ExpressSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExpressSystem.h"
#include "ExpressSystemDlg.h"
#include "afxdialogex.h"
#include "DialogTable.h"
#include <sstream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExpressSystemDlg 对话框



CExpressSystemDlg::CExpressSystemDlg(DatabaseADO& ado, DatabaseADO& ou, CWnd* pParent /*=NULL*/)
	: CDialogEx(CExpressSystemDlg::IDD, pParent),
	databaseADO_(ado),
	outbaseADO_(ou),
	censusDlg_(this),
	itemDataSort_(0),
	listSortImpl_(&databaseListViewCtrl_, -1, 0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExpressSystemDlg::insertItemByTableID(SystemVector& vec)
{
	for (auto& inItem : vec)
	{
		insertItems(inItem);	
	}
}

void CExpressSystemDlg::updateCensus(bool isUnused, bool isUsed, bool isUnsent, bool isSent, int nItem)
{
	if (isUnsent | isUsed | isUnused | isSent)
	{
		clearAllListItems();
	}
	if (isUnused)
		insertItemByTableID(unusedVect_[nItem]);
	if (isUsed)
		insertItemByTableID(usedVect_[nItem]);
	if (isUnsent)
		insertItemByTableID(unsentVect_[nItem]);
	if (isSent)
		insertItemByTableID(sentVect_[nItem]);
}

// push command
void CExpressSystemDlg::addCommand(int undoNum)
{
	if (undoCount_.size() >= CExpressSystemDlg::MaxCommandNumber)
	{
		databaseADO_.popUndoHead(undoCount_[0]);
		undoCount_.erase(undoCount_.begin());
	}

	undoCount_.push_back(undoNum);
}

void CExpressSystemDlg::clearAllListItems()
{
	databaseListViewCtrl_.DeleteAllItems();
	itemDataSort_ = 0;
}

void CExpressSystemDlg::disableUndo()
{
	undoCount_.clear();
	GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(false);
}

void CExpressSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATABASE, databaseListViewCtrl_);
}

BEGIN_MESSAGE_MAP(CExpressSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CExpressSystemDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CExpressSystemDlg::OnBnClickedButtonInsert)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATABASE, &CExpressSystemDlg::OnDblclkListDatabase)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExpressSystemDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CExpressSystemDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CExpressSystemDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CExpressSystemDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CExpressSystemDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_CENSUS, &CExpressSystemDlg::OnBnClickedButtonCensus)
	ON_BN_CLICKED(IDC_BUTTON_SHOWALL, &CExpressSystemDlg::OnBnClickedButtonShowall)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CExpressSystemDlg::OnBnClickedButtonMerge)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CExpressSystemDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_SHOWOUT, &CExpressSystemDlg::OnBnClickedButtonShowout)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CExpressSystemDlg::OnBnClickedButtonUndo)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CExpressSystemDlg::OnItemclickListDatabase)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_DATABASE, &CExpressSystemDlg::OnNMSetfocusListDatabase)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DATABASE, &CExpressSystemDlg::OnNMClickListDatabase)
END_MESSAGE_MAP()


// CExpressSystemDlg 消息处理程序

BOOL CExpressSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	initDatabaseListView();
	// disable buttons
	disableAllButtons();
	// create census dialog
	censusDlg_.Create(IDD_DIALOG_CENSUSINFO, this);
	// diable undo
	GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExpressSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExpressSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExpressSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// open a access file
void CExpressSystemDlg::openAccess()
{
	CFileDialog fileDlg(true, NULL, NULL, OFN_HIDEREADONLY, L"access(*.accdb)|*.accdb||");
	fileDlg.m_pOFN->lpstrTitle = L"打开数据库";

	if (fileDlg.DoModal() == IDOK)
	{
		// convert CStringW to string
		USES_CONVERSION;
		std::string imgpath(T2A(fileDlg.GetPathName()));

		databaseADO_.openBase(imgpath);
	//	databaseADO_.openBase();
		SetWindowText(fileDlg.GetFileName());

		enableAllButtons();
		OnBnClickedButtonShowall();
		disableUndo();	// if opened, undo is disable
	}
}

// init database listview columns
// it is called in OnInitDialog()
void CExpressSystemDlg::initDatabaseListView()
{
	databaseListViewCtrl_.SetParent(this);
	databaseListViewCtrl_.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	int item = 0;
	databaseListViewCtrl_.InsertColumn(item++, L"表单号", LVCFMT_CENTER, 160);
	databaseListViewCtrl_.InsertColumn(item++, L"是否发放", LVCFMT_CENTER, 60);
	databaseListViewCtrl_.InsertColumn(item++, L"是否使用", LVCFMT_CENTER, 60);
	databaseListViewCtrl_.InsertColumn(item++, L"业务员", LVCFMT_CENTER, 60);
	databaseListViewCtrl_.InsertColumn(item++, L"公司", LVCFMT_CENTER, 160);
	databaseListViewCtrl_.InsertColumn(item++, L"备注", LVCFMT_CENTER, 60);
	databaseListViewCtrl_.InsertColumn(item++, L"公司电话", LVCFMT_CENTER, 160);
	databaseListViewCtrl_.InsertColumn(item++, L"发送时间", LVCFMT_CENTER, 80);
	databaseListViewCtrl_.InsertColumn(item++, L"使用时间", LVCFMT_CENTER, 80);
}

// insert a item value
void CExpressSystemDlg::insertItems(DataField& df)
{
	int item = 0;
	CString str;

	int endItem = databaseListViewCtrl_.GetItemCount();
	databaseListViewCtrl_.InsertItem(endItem, L"");

	str.Format(L"%llu", df.tableID);
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	if (df.isSent == -1)
	{
		str = L"否";
	}
	else
	{
		str = L"是";
	}
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	if (df.isUsed == -1)
	{
		str = L"否";
	}
	else
	{
		str = L"是";
	}
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	USES_CONVERSION;
	str.Format(L"%s", A2T(df.workerName));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	str.Format(L"%s", A2T(df.company));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	str.Format(L"%s", A2T(df.comments));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	str.Format(L"%s", A2T(df.companyPhone));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	str.Format(L"%s", A2T(df.sendTime));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	str.Format(L"%s", A2T(df.useTime));
	databaseListViewCtrl_.SetItemText(endItem, item++, str);

	databaseListViewCtrl_.SetItemData(endItem, itemDataSort_++);
}

void CExpressSystemDlg::getItems(int iItem, DataField* pDf)
{
	USES_CONVERSION;
	int isub = 0;
	CString str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	std::stringstream ss;
	ss << T2A(str);
	ss >> pDf->tableID;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	if (str == L"是")
	{
		pDf->isSent = 0;
	}
	else
	{
		pDf->isSent = -1;
	}

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	if (str == L"是")
	{
		pDf->isUsed = 0;
	}
	else
	{
		pDf->isUsed = -1;
	}

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->workerName;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->company;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->comments;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->companyPhone;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->sendTime;

	str = databaseListViewCtrl_.GetItemText(iItem, isub++);
	ss.clear();
	ss << T2A(str);
	ss >> pDf->useTime;
}

void CExpressSystemDlg::OnBnClickedButtonSearch()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogTable dlgTable(CDialogTable::DialogType::SEARCH_DLG, this);
	if (IDOK == dlgTable.DoModal())
	{
		DatabaseADO::DataField* searchData = dlgTable.DataPropertyPtr();

		int tbNum = dlgTable.TableNumber();
		clearAllListItems();

		for (int i = 0; i < tbNum; ++i)
		{
			auto foundItem = databaseADO_.find(*searchData);

			
			if (foundItem->empty())
			{
			//	MessageBox(L"没有找到相关信息", L"Warning");
			}
			else
			{
				for (auto& inItem : *foundItem)
				{
					insertItems(inItem);
				}
			}
			++(searchData->tableID);
		}
	}
}

// add more items
void CExpressSystemDlg::OnBnClickedButtonInsert()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogTable dlgTable(CDialogTable::DialogType::INSERT_DLG, this);

	if (databaseListViewCtrl_.GetSelectedCount() != 0)
	{
		DataField df;
		auto pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();	// read select item
		int nItem = databaseListViewCtrl_.GetNextSelectedItem(pos);
		getItems(nItem, &df);
		dlgTable.DataPropertyPtr(df);
	}
	
	if (IDOK == dlgTable.DoModal())
	{
		clearAllListItems();

		DatabaseADO::DataField* insertData = dlgTable.DataPropertyPtr();
		int tableNumber = dlgTable.TableNumber();
		
		int undo = 0;

		for (int i = 0; i < tableNumber; ++i)
		{	
			if (false == databaseADO_.save(*insertData))
			{
			//	CString str;
			//	str.Format(L"表单号：%llu\r\n已经存在增加失败", insertData->tableID);
			//	MessageBox(str, L"ERROR");
			}
			else
			{
				insertItems(*insertData);
				++undo;
			}
			++(insertData->tableID);
		}		
		if (undo != 0)
		{
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(true);
			addCommand(undo);	
		}
		CString str;
		str.Format(L"成功项/总项：%d/%d", undo, tableNumber);
		SetDlgItemText(IDC_STATIC_SELECTED, str);
	}
}

// modify item
void CExpressSystemDlg::OnDblclkListDatabase(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate->iItem < 0)
		return;

	DatabaseADO::DataField df;
	getItems(pNMItemActivate->iItem, &df);
	CDialogTable dlgTable(CDialogTable::DialogType::MODIFY_DLG, this);
	dlgTable.DataPropertyPtr(df);

	if (IDOK == dlgTable.DoModal())
	{
		DatabaseADO::DataField* modifiedData = dlgTable.DataPropertyPtr();

		if (false == databaseADO_.update(*modifiedData))
		{
			CString str;
			str.Format(L"表单号：%llu\r\n不存在，修改失败");
			MessageBox(str, L"ERROR");
		}
		else
		{
			clearAllListItems();
			insertItems(*modifiedData);
			addCommand(1);
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(true);
		}
	}
}
// modify for multi-items
void CExpressSystemDlg::OnBnClickedButtonModify()
{
	int totalSelect = databaseListViewCtrl_.GetSelectedCount();
	if (totalSelect == 0)
		return;

	auto pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();	// read select item
	int nItem = databaseListViewCtrl_.GetNextSelectedItem(pos);

	DataField df;
	getItems(nItem, &df);
	
	CDialogTable dlgTable(CDialogTable::DialogType::MODIFY_DLG, this);
	dlgTable.DataPropertyPtr(df);

	if (IDOK == dlgTable.DoModal())
	{
		DatabaseADO::DataField* modifiedData = dlgTable.DataPropertyPtr();

		int totalItems = databaseListViewCtrl_.GetItemCount();
		pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();

		int undo = 0;
		while (pos)
		{
			nItem = databaseListViewCtrl_.GetNextSelectedItem(pos);
			USES_CONVERSION;
			std::stringstream ss;
			ss << T2A(databaseListViewCtrl_.GetItemText(nItem, 0));	// get table id
			ss >> modifiedData->tableID;

			if (false == databaseADO_.update(*modifiedData))
			{
				CString str;
				str.Format(L"表单号：%llu\r\n不存在，修改失败");
				MessageBox(str, L"ERROR");
			}
			else
			{
				insertItems(*modifiedData);
				++undo;
			}
		}
		if (undo != 0)
		{
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(true);
			addCommand(undo);		
		}
		CString str;
		str.Format(L"成功项/总项：%d/%d", undo, totalSelect);
		SetDlgItemText(IDC_STATIC_SELECTED, str);

		// delete front items
		for (int i = 0; i < totalItems; ++i)
			databaseListViewCtrl_.DeleteItem(0);
	}
}

// disable all buttons
void CExpressSystemDlg::disableAllButtons()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SHOWALL)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CENSUS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SHOWOUT)->EnableWindow(false);
}

// enable all buttons
void CExpressSystemDlg::enableAllButtons()
{
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_MERGE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SHOWALL)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CENSUS)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SHOWOUT)->EnableWindow(true);
}

void CExpressSystemDlg::OnBnClickedButtonOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	openAccess();
}


void CExpressSystemDlg::OnBnClickedButtonCreate()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(false, L"accdb", NULL, OFN_HIDEREADONLY, L"access(*.accdb)|*.accdb||");
	fileDlg.m_pOFN->lpstrTitle = L"创建空数据库";

	if (fileDlg.DoModal() == IDOK)
	{
		// convert CStringW to string
		USES_CONVERSION;
		std::string imgpath(T2A(fileDlg.GetPathName()));

		if (databaseADO_.CreateMdb(imgpath) == true)
		{
			SetWindowText(fileDlg.GetFileName());
			enableAllButtons();
			OnBnClickedButtonShowall();
			disableUndo();	// created , undo disable
		}	
	}
//	GetMenu()->EnableMenuItem(ID_CREATE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

// DELETE items
void CExpressSystemDlg::OnBnClickedButtonDelete()
{
	// TODO:  在此添加控件通知处理程序代码
	if (databaseListViewCtrl_.GetSelectedCount() == 0)
		return;

	auto pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();
	if (MessageBox(L"是否确定删除选中项？", L"warning", MB_YESNO) == IDYES)
	{
		int undo = 0;
		while (pos)
		{
			int nItem = databaseListViewCtrl_.GetNextSelectedItem(pos);

			DataField df;
			getItems(nItem, &df);
			databaseADO_.deletE(df);

			++undo;

			databaseListViewCtrl_.DeleteItem(nItem);
			pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();
		}
		if (undo != 0)
		{
			addCommand(undo);
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(true);
		}	
	}
}


void CExpressSystemDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	if (MessageBox(L"是否确定清空数据库？\r\n一旦清空，数据库中的数据将无法还原！", L"warning", MB_YESNO) == IDYES)
	{
		clearAllListItems();
		DataField df;
		df.setNull();
		databaseADO_.deletE(df);	// clear all
		undoCount_.clear();
		GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(false);
	}
}



void CExpressSystemDlg::OnBnClickedButtonCensus()
{
	// TODO:  在此添加控件通知处理程序代码
	DataField df;
	df.setNull();

	auto found = databaseADO_.find(df);
	size_t sizeFound = found->size();
	
	std::sort(found->begin(), found->end(), [](DataField& left, DataField& right){
		return left.tableID < right.tableID;
	});

	sentVect_.clear();
	unsentVect_.clear();
	unusedVect_.clear();
	usedVect_.clear();

	for (size_t i = 0; i < sizeFound;)
	{
		int used = 0, unused = 0;
		int sent = 0, unsent = 0;

		// clear all vector data
		SystemVector sentVect;
		SystemVector unsentVect;
		SystemVector unusedVect;
		SystemVector usedVect;

		unsigned long long start = (*found)[i].tableID;
		unsigned long long end = (*found)[i].tableID;
		for (++i; i < sizeFound && end + 1 == (*found)[i].tableID; ++i, ++end)	// continued number
		{
			DataField& inF = (*found)[i];
			if (inF.isSent == 0)
			{
				++sent;
				sentVect.push_back(inF);
			}
			else
			{
				++unsent;
				unsentVect.push_back(inF);
			}


			if (inF.isUsed == 0)
			{
				++used;
				usedVect.push_back(inF);
			}
			else
			{
				++unused;
				unusedVect.push_back(inF);
			}
		}	
		sentVect_.push_back(sentVect);
		unsentVect_.push_back(unsentVect);
		unusedVect_.push_back(unusedVect);
		usedVect_.push_back(usedVect);
		censusDlg_.update(start, end, unused, used, unsent, sent);
	}
	
	censusDlg_.ShowWindow(SW_SHOW);
}


void CExpressSystemDlg::OnBnClickedButtonShowall()
{
	// TODO:  在此添加控件通知处理程序代码
	DataField df;
	df.setNull();

	auto foundItem = databaseADO_.find(df);

	clearAllListItems();
	if (foundItem->empty())
	{
		MessageBox(L"没有找到相关信息", L"Warning");
	}
	else
	{
		for (auto& inItem : *foundItem)
		{
			insertItems(inItem);
		}
	}
}


void CExpressSystemDlg::OnBnClickedButtonMerge()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(true, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY,
		L"access(*.accdb)|*.accdb||");

	fileDlg.m_pOFN->lpstrTitle = L"导入数据库";
	fileDlg.m_ofn.nMaxFile = 500 * MAX_PATH;
	fileDlg.m_ofn.lpstrFile = new TCHAR[fileDlg.m_ofn.nMaxFile];
	ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR)* fileDlg.m_ofn.nMaxFile);

	int total = 0;

	clearAllListItems();

	if (fileDlg.DoModal() == IDOK)
	{
		auto pos = fileDlg.GetStartPosition();
		int undo = 0;
		while (pos)
		{
			// convert CStringW to string
			USES_CONVERSION;
			std::string imgpath(T2A(fileDlg.GetNextPathName(pos)));

			DatabaseADO ado;
			ado.openBase(imgpath);

			DataField df;
			df.setNull();
			auto found = ado.find(df);

			if (found == nullptr)
			{
				CString str;
				str.Format(L"确认文件%s中tableID为数字型,导入失败！", fileDlg.GetFileName());
				MessageBox(str);
				continue;
			}
			for (auto& it : *found)
			{
				++total;
				if (databaseADO_.update(it) == false)
				{
					outbaseADO_.saveOrUpdate(it);			
				}
				else
				{
					++undo;
					insertItems(it);
				}			
			}
		}
		if (undo != 0)
		{
			GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(true);
			addCommand(undo);	
		}
		CString str;
		str.Format(L"入库项/库外项：%d/%d", undo, total - undo);
		SetDlgItemText(IDC_STATIC_SELECTED, str);

	//	OnBnClickedButtonShowall();
	}
}


void CExpressSystemDlg::OnBnClickedButtonExport()
{
	// TODO:  在此添加控件通知处理程序代码
	if (databaseListViewCtrl_.GetSelectedCount() == 0)
		return;

	CFileDialog fileDlg(false, L"accdb", NULL, OFN_HIDEREADONLY, L"access(*.accdb)|*.accdb||");
	fileDlg.m_pOFN->lpstrTitle = L"导出选中数据库";

	if (fileDlg.DoModal() == IDOK)
	{
		auto pos = databaseListViewCtrl_.GetFirstSelectedItemPosition();
		// convert CStringW to string
		USES_CONVERSION;
		std::string imgpath(T2A(fileDlg.GetPathName()));

		DatabaseADO ado;
		if (ado.CreateMdb(imgpath) == true)
		{
			DataField df;
			
			while (pos)
			{
				int nItem = databaseListViewCtrl_.GetNextSelectedItem(pos);
				getItems(nItem, &df);
				ado.save(df);
			}
		}
	}
}


void CExpressSystemDlg::OnBnClickedButtonShowout()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_BUTTON_SHOWOUT, str);
	if (str != L"退出显示")
	{
		disableAllButtons();
		GetDlgItem(IDC_BUTTON_SHOWOUT)->EnableWindow(true);
		str.Format(L"退出显示");
		SetDlgItemText(IDC_BUTTON_SHOWOUT, str);

		DataField df;
		df.setNull();

		auto foundItem = outbaseADO_.find(df);

		clearAllListItems();
		if (foundItem->empty())
		{
			MessageBox(L"没有找到相关信息", L"Warning");
		}
		else
		{
			for (auto& inItem : *foundItem)
			{
				insertItems(inItem);
			}
		}	
	}
	else
	{
		clearAllListItems();
		enableAllButtons();
		str.Format(L"库外表单");
		SetDlgItemText(IDC_BUTTON_SHOWOUT, str);
	}
	
}


void CExpressSystemDlg::OnBnClickedButtonUndo()
{
	// TODO:  在此添加控件通知处理程序代码
	auto foundItem = databaseADO_.undo(undoCount_[undoCount_.size() - 1]);
	undoCount_.pop_back();

	if (undoCount_.empty())
		GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(false);

	clearAllListItems();
	if (!foundItem->empty())
	{
		for (auto& inItem : *foundItem)
		{
			insertItems(inItem);
		}
	}
}



void CExpressSystemDlg::OnItemclickListDatabase(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int longLong = 0;
	if (pNMListView->iItem == 0)
		longLong = 1;
	listSortImpl_.columnClicked(pNMListView->iItem);
	listSortImpl_.isLongLong(longLong);
	listSortImpl_.sortItems();

	*pResult = 0;
}


void CExpressSystemDlg::OnNMSetfocusListDatabase(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int sc = databaseListViewCtrl_.GetSelectedCount();
	int total = databaseListViewCtrl_.GetItemCount();

	CString str;
	str.Format(L"选中项/所有项：%d/%d", sc, total);
	SetDlgItemText(IDC_STATIC_SELECTED, str);

	*pResult = 0;
}


void CExpressSystemDlg::OnNMClickListDatabase(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int sc = databaseListViewCtrl_.GetSelectedCount();
	int total = databaseListViewCtrl_.GetItemCount();

	CString str;
	str.Format(L"选中项/所有项：%d/%d", sc, total);
	SetDlgItemText(IDC_STATIC_SELECTED, str);

	*pResult = 0;
}
