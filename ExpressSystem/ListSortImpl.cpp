#include "stdafx.h"
#include "ListSortImpl.h"
#include <sstream>

int CALLBACK compareFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	ListSortImpl* pListImpl = reinterpret_cast<ListSortImpl*>(lParamSort);
	CListCtrl* pList = pListImpl->listHandle_;
	int nItem1, nItem2;

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;    // 指定查找方式
	FindInfo.lParam = lParam1;
	nItem1 = pList->FindItem(&FindInfo, -1); // 得到对应Item索引
	FindInfo.lParam = lParam2;
	nItem2 = pList->FindItem(&FindInfo, -1);

	if ((nItem1 == -1) || (nItem2 == -1))
	{
		TRACE("无法找到!/n");
		return 0;
	}

	CString str1, str2;
	str1 = pList->GetItemText(nItem1, pListImpl->columnClicked_); // 得到排序列的Text
	str2 = pList->GetItemText(nItem2, pListImpl->columnClicked_);
	/*
	HDITEM headerItem;
	headerItem.mask = HDI_LPARAM;
	CHeaderCtrl* pHeaderCtrl = pList->GetHeaderCtrl();
	pHeaderCtrl->GetItem(pListImpl->columnClicked_, &headerItem);
	UINT nType = (UINT)(headerItem.lParam);
*/
	int iCompRes = 0;

	if (pListImpl->isLongLong_ != 0)
	{
		unsigned long long i1, i2;
		USES_CONVERSION;
		std::stringstream ss;
		ss << T2A(str1);
		ss >> i1;

		ss.clear();
		ss << T2A(str2);
		ss >> i2;

		if (i1 > i2)
			iCompRes = 1;
		else if (i1 == i2)
			iCompRes = 0;
		else
			iCompRes = -1;
	}
	else
	{
		if (str1 > str2)
			iCompRes = 1;
		else if (str1 == str2)
			iCompRes = 0;
		else
			iCompRes = -1;
	}

	if (pListImpl->isSameColumn_)
		return -iCompRes;

	return iCompRes;
}

ListSortImpl::ListSortImpl(CListCtrl* listCtrl, int columnSet, int isLongLong)
: listHandle_(listCtrl)
, columnClicked_(columnSet)
, isLongLong_(isLongLong)
{
}

void ListSortImpl::sortItems()
{
	listHandle_->SortItems(compareFun, reinterpret_cast<DWORD>(this));
}
