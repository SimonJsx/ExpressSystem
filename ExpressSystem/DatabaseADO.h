#pragma once

#include "msado15.tlh"
#include <string>
#include <vector>

class DatabaseADO
{
public:
	struct DataField;

private:
	typedef std::string DatabaseString;
	typedef std::vector<DatabaseADO::DataField> DataFieldVector;
	typedef std::vector<DatabaseADO::DataField>* DataFieldVectorPtr;

	const static DatabaseString DATABASE_VERSION;
public:
	struct DataField
	{
		unsigned long long tableID;
		int isSent;
		int isUsed;
		char workerName[20];
		char company[100];
		char comments[100];
		char companyPhone[20];
		char sendTime[16];
		char useTime[16];
		
		void setNull()
		{
			tableID = 0;
			isSent = 1;
			isUsed = 1;
			workerName[0] = 0;
			company[0] = 0;
			comments[0] = 0;
			companyPhone[0] = 0;
			sendTime[0] = 0;
			useTime[0] = 0;
		}
	};
	
	const static DatabaseString NormalString;
	const static DatabaseString BadString;

	DatabaseADO();
	~DatabaseADO();

	void close();

	static void initADO()
	{
		CoInitialize(NULL); //初始化COM环境
	}
	static void releaseADO()
	{
		CoUninitialize();
	}

	// create database file
	bool CreateMdb(DatabaseString& fileName);
	void create();
	// insert item
	bool save(DataField& df);
	bool update(DataField& df);
	void saveOrUpdate(DataField& df);

	// connect database
	bool openBase(DatabaseString& fileName);
	void openBase();

	// find item
	DataFieldVectorPtr find(DataField& df);

	// delete item by table id
	// if input 0, the whole table will be deleted
	void deletE(DataField& df);

	// undo operator counter times
	DataFieldVectorPtr undo(int counter);
	void popUndoHead(int counter);

private:
	void extractAllDataField();

private:
	_ConnectionPtr baseConnectPtr_;
	_RecordsetPtr recordPtr_;
	DataFieldVector dataVector_;
	std::vector<_bstr_t> fieldName_;
	std::vector<std::string> commandVector_;

	bool isOpened_;
	DataField dataProperty_;
};

