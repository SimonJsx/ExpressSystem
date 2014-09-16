#pragma once
class ListSortImpl
{
public:
	ListSortImpl(CListCtrl* listCtrl, int columnSet, int isLongLong);
	~ListSortImpl() = default;

	void sortItems();

	void isLongLong(int longLong)
	{
		isLongLong_ = longLong;
	}
	void listHandle(CListCtrl* list)
	{
		listHandle_ = list;
	}
	void columnClicked(int column)
	{
		if (columnClicked_ == column)
		{
			isSameColumn_ = !isSameColumn_;
		}
		else
		{
			isSameColumn_ = false;
			columnClicked_ = column;
		}
	}
private:
	CListCtrl* listHandle_;
	int columnClicked_;
	int isLongLong_;
	bool isSameColumn_;

	friend int CALLBACK compareFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};

