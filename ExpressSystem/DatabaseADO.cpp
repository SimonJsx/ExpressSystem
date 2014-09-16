#include <stdio.h>
#include "stdafx.h"
#include "DatabaseADO.h"
#include "msadox.tlh"
#include <sstream>

const DatabaseADO::DatabaseString DatabaseADO::DATABASE_VERSION = "Provider = Microsoft.ACE.OLEDB.15.0; Data Source=";
const DatabaseADO::DatabaseString DatabaseADO::NormalString = "正常";
const DatabaseADO::DatabaseString DatabaseADO::BadString = "损坏";

DatabaseADO::DatabaseADO() :
	baseConnectPtr_(__uuidof(Connection)),
	recordPtr_(__uuidof(Recordset)),
	isOpened_(false)
{
}


DatabaseADO::~DatabaseADO()
{
	close();

	if (isOpened_)
	{
		baseConnectPtr_.Release();//释放连接对象指针
		recordPtr_.Release();//释放记录集对象指针  
	}
}

void DatabaseADO::close()
{
	if (isOpened_)
	{
		recordPtr_->Close();     //关闭记录集   
		baseConnectPtr_->Close();//关闭数据库   

		commandVector_.clear();
	}
}


bool DatabaseADO::openBase(DatabaseString& fileName)
{
	close();

	DatabaseString openStr(DATABASE_VERSION);
	openStr += fileName + ";";
	try
	{
		isOpened_ = SUCCEEDED(baseConnectPtr_->Open(openStr.c_str(), "", "", adModeUnknown));
	}
	catch (...)
	{
		return false;
	}
	
	recordPtr_ = baseConnectPtr_->Execute("select * from [table]", NULL, adCmdText);//执行SQL： select * from gendat          
	if (!recordPtr_->BOF)
	{
		recordPtr_->MoveFirst();
	}
	/*存储表的所有列名，显示表的列名*/
	fieldName_.clear();
	for (int i = 0; i < recordPtr_->Fields->GetCount(); i++)
	{
		fieldName_.push_back(recordPtr_->Fields->GetItem(_variant_t((long)i))->Name);
	}

	return true;
}

// used for internet database
void DatabaseADO::openBase()
{
	close();

//	DatabaseString openStr("Provider=MySQL Provider; Data Source=114.215.203.104; User ID=admin; Password=admin; Initial Catalog=test; Activation=RDG7-R6SF-9QU3-X5XF");
//	DatabaseString openStr("Driver={MySQL ODBC 5.3.4 Driver}; Server=114.215.203.104; Option=131072; User=admin; Password=admin; Database=test; Activation=RDG7-R6SF-9QU3-X5XF");
	DatabaseString openStr("DRIVER={MySQL ODBC 5.3 ANSI Driver}; SERVER=114.215.203.104; DATABASE=test; User=admin; Password=admin; OPTION=3");

	try
	{
		isOpened_ = SUCCEEDED(baseConnectPtr_->Open(openStr.c_str(), "", "", adModeUnknown));
	}
	catch (_com_error e)
	{
		_bstr_t bstrDescription(e.Description());
		CString strErro = CString(_T("创建ACCEESS数据库出错: "))
			+ (LPCSTR)e.Description()
			+ CString(_T("Create ACCESS DB error: "))
			+ (LPCSTR)e.Description();
		AfxMessageBox(strErro);
		return;
	}

	recordPtr_ = baseConnectPtr_->Execute("select * from data", NULL, adCmdText);//执行SQL： select * from gendat          
	if (!recordPtr_->BOF)
	{
		recordPtr_->MoveFirst();
	}
	/*存储表的所有列名，显示表的列名*/
	fieldName_.clear();
	for (int i = 0; i < recordPtr_->Fields->GetCount(); i++)
	{
		fieldName_.push_back(recordPtr_->Fields->GetItem(_variant_t((long)i))->Name);
	}
}


DatabaseADO::DataFieldVectorPtr DatabaseADO::find(DataField& df)
{
	dataVector_.clear();

	char sql[1024] = "select * from [table]";
	char whe[1024] = "";

	if (df.tableID >= 1) sprintf(whe, "%s tableID='%llu',", whe, df.tableID);
	if (df.isSent <= 0) sprintf(whe, "%s isSent=%d,", whe, df.isSent);
	if (df.isUsed <= 0) sprintf(whe, "%s isUsed=%d,", whe, df.isUsed);
	if (df.workerName[0] != '\0') sprintf(whe, "%s workerName='%s',", whe, df.workerName);
	if (df.company[0] != '\0') sprintf(whe, "%s company='%s',", whe, df.company);
	if (df.comments[0] != '\0') sprintf(whe, "%s comments='%s',", whe, df.comments);
	if (df.companyPhone[0] != '\0') sprintf(whe, "%s companyPhone='%s',", whe, df.companyPhone);
	if (df.sendTime[0] != '\0') sprintf(whe, "%s sendTime='%s',", whe, df.sendTime);
	if (df.useTime[0] != '\0') sprintf(whe, "%s useTime='%s',", whe, df.useTime);

	if (strlen(whe) > 0) whe[strlen(whe) - 1] = '\0';
	if (strlen(whe) > 0) sprintf(sql, "%s where%s", sql, whe);

	recordPtr_ = baseConnectPtr_->Execute(sql, NULL, adCmdText);

	if (!recordPtr_->BOF)
	{
		recordPtr_->MoveFirst();
		extractAllDataField();
	}

	return &dataVector_;
}

bool DatabaseADO::save(DataField& df)
{
	char sql[1024];

	sprintf(sql, "select * from [table] where tableID='%llu'", df.tableID);
	
	recordPtr_ = baseConnectPtr_->Execute(sql, NULL, adCmdText);        
	if (!recordPtr_->BOF)
	{
		return false;	// existed
	}
	else
	{
		char cmd[1024];

		sprintf(sql, "insert into [table] values ('%llu', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')",
			df.tableID, df.isSent, df.isUsed, df.workerName, df.company, df.comments, df.companyPhone, df.sendTime, df.useTime);
	
		sprintf(cmd, "delete * from [table] where tableID='%llu'", df.tableID);
		commandVector_.push_back(cmd);
	}
	baseConnectPtr_->Execute(sql, NULL, adCmdText);

	return true;
}

bool DatabaseADO::update(DataField& df)
{
	char sql[1024];

	sprintf(sql, "select * from [table] where tableID='%llu'", df.tableID);

	recordPtr_ = baseConnectPtr_->Execute(sql, NULL, adCmdText);
	if (!recordPtr_->BOF)
	{
		DataField d;
		char cmd[1024];

		sprintf(sql, "update [table] set isSent=%d, isUsed=%d, workerName='%s', company='%s', comments='%s', companyPhone='%s', sendTime='%s', useTime='%s' where tableID='%llu'",
			df.isSent, df.isUsed, df.workerName, df.company, df.comments, df.companyPhone, df.sendTime, df.useTime, df.tableID);
	
		d.setNull();
		d.tableID = df.tableID;
		auto found = find(d);
		auto inF = (*found)[0];
		sprintf(cmd, "update [table] set isSent=%d, isUsed=%d, workerName='%s', company='%s', comments='%s', companyPhone='%s', sendTime='%s', useTime='%s' where tableID='%llu'",
			inF.isSent, inF.isUsed, inF.workerName, inF.company, inF.comments, inF.companyPhone, inF.sendTime, inF.useTime, inF.tableID);
		commandVector_.push_back(cmd);
	}
	else
	{
		return false;	// non-exist
	}
	baseConnectPtr_->Execute(sql, NULL, adCmdText);

	return true;
}

void DatabaseADO::saveOrUpdate(DataField& df)
{
	char sql[1024];

	sprintf(sql, "select * from [table] where tableID='%llu'", df.tableID);

	recordPtr_ = baseConnectPtr_->Execute(sql, NULL, adCmdText);
	if (!recordPtr_->BOF)
	{
		sprintf(sql, "update [table] set isSent=%d, isUsed=%d, workerName='%s', company='%s', comments='%s', companyPhone='%s', sendTime='%s', useTime='%s' where tableID='%llu'",
			df.isSent, df.isUsed, df.workerName, df.company, df.comments, df.companyPhone, df.sendTime, df.useTime, df.tableID);
	}
	else
	{
		sprintf(sql, "insert into [table] values ('%llu', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')",
			df.tableID, df.isSent, df.isUsed, df.workerName, df.company, df.comments, df.companyPhone, df.sendTime, df.useTime);
	}
	baseConnectPtr_->Execute(sql, NULL, adCmdText);
}

void DatabaseADO::deletE(DataField& df)
{
	char sql[1024] = "delete * from [table]";

	if (df.tableID >= 1)
	{
		char cmd[1024];

		sprintf(sql, "%s where tableID='%llu'", sql, df.tableID);

		sprintf(cmd, "insert into [table] values ('%llu', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')",
			df.tableID, df.isSent, df.isUsed, df.workerName, df.company, df.comments, df.companyPhone, df.sendTime, df.useTime);
		commandVector_.push_back(cmd);
	}
	else
	{
		commandVector_.clear();
	}

	baseConnectPtr_->Execute(sql, NULL, adCmdText);
}

void DatabaseADO::create()
{
	char sql[1024] = "create table [table]([tableID] varchar(20) primary key, isSent int, isUsed int, workerName varchar(20), company varchar(100), comments varchar(100), companyPhone varchar(20), sendTime varchar(16), useTime varchar(16))";

	baseConnectPtr_->Execute(sql, NULL, adCmdText);

	recordPtr_ = baseConnectPtr_->Execute("select * from [table]", NULL, adCmdText);//执行SQL： select * from gendat 
	if (!recordPtr_->BOF)
	{
		recordPtr_->MoveFirst();
	}
		/*存储表的所有列名，显示表的列名*/
	fieldName_.clear();
	for (int i = 0; i < recordPtr_->Fields->GetCount(); i++)
	{
		fieldName_.push_back(recordPtr_->Fields->GetItem(_variant_t((long)i))->Name);
	}
	
}

bool DatabaseADO::CreateMdb(DatabaseString& strDBName)
{
	using namespace ADOCG;
	DatabaseString strMdbConn(DATABASE_VERSION);
	strMdbConn += strDBName;

	try
	{
		HRESULT hr = S_OK;
		_CatalogPtr pCatalog = NULL;
		_bstr_t cnnstring(strMdbConn.c_str());

		pCatalog.CreateInstance(__uuidof (Catalog));
		pCatalog->Create(cnnstring);
	}
	catch (_com_error e)
	{
		_bstr_t bstrDescription(e.Description());
		CString strErro = CString(_T("创建ACCEESS数据库出错: "))
			+ (LPCSTR)e.Description()
			+ CString(_T("Create ACCESS DB error: "))
			+ (LPCSTR)e.Description();
		AfxMessageBox(strErro);
		return FALSE;
	}

	close();

	DatabaseString openStr(DATABASE_VERSION);
	openStr += strDBName + ";";

	isOpened_ = SUCCEEDED(baseConnectPtr_->Open(openStr.c_str(), "", "", adModeUnknown));

	create();

	return TRUE;
}

DatabaseADO::DataFieldVectorPtr DatabaseADO::undo(int counter)
{
	size_t i = commandVector_.size();
	dataVector_.clear();

	while (counter-- > 0 && i > 0)
	{
		std::string cmd = commandVector_[--i];
		std::string tableID;

		baseConnectPtr_->Execute(cmd.c_str(), NULL, adCmdText);
		commandVector_.pop_back();

		switch (cmd[0])
		{
		case 'i':
			tableID = cmd.substr(cmd.find('\'') + 1, cmd.length() - cmd.find('\'') - 1);
			tableID = tableID.substr(0, tableID.find('\''));
			break;
		case 'u':
		case 'd':
			tableID = cmd.substr(0, cmd.length() - 1);
			tableID = tableID.substr(tableID.find_last_of('\'') + 1, cmd.length() - tableID.find_last_of('\'') - 1);
		}
		switch (cmd[0])
		{
		case 'i':
		case 'u':
			cmd = "select * from [table] where tableID='" + tableID + "'";
			recordPtr_ = baseConnectPtr_->Execute(cmd.c_str(), NULL, adCmdText);

			if (!recordPtr_->BOF)
			{
				recordPtr_->MoveFirst();
				extractAllDataField();
			}
			break;
		case 'd':
			DataField df;
			df.setNull();
			std::stringstream ss;
			ss << tableID;
			ss >> df.tableID;
			strcpy(df.comments, "删除");
			dataVector_.push_back(df);
			break;
		}
	}
	return &dataVector_;
}

void DatabaseADO::popUndoHead(int counter)
{
	commandVector_.erase(commandVector_.begin(), commandVector_.begin() + counter);
}

void DatabaseADO::extractAllDataField()
{
	while (!recordPtr_->adoEOF)
	{
		DataField df;
		int i = 0;

		std::stringstream ss;
		ss << (_bstr_t)recordPtr_->GetCollect(fieldName_[i]);
		ss >> df.tableID;
		++i;

		df.isSent = recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			recordPtr_->GetCollect(fieldName_[i]) : 0;
		++i;
		df.isUsed = recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			recordPtr_->GetCollect(fieldName_[i]) : 0;
		++i;

		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.workerName, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.workerName[0] = 0, 0);
		++i;
		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.company, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.company[0] = 0, 0);
		++i;
		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.comments, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.comments[0] = 0, 0);
		++i;
		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.companyPhone, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.companyPhone[0] = 0, 0);
		++i;
		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.sendTime, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.sendTime[0] = 0, 0);
		++i;
		recordPtr_->GetCollect(fieldName_[i]).vt != VT_NULL ?
			strcpy(df.useTime, (_bstr_t)recordPtr_->GetCollect(fieldName_[i])) : (df.useTime[0] = 0, 0);

		dataVector_.push_back(df);

		recordPtr_->MoveNext();
	}
}
