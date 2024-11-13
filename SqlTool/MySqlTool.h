#pragma once
#include <string>
#include <vector>
#include <map>
#include "mysql.h"
using namespace std;
class CMySqlTool
{
public:
	CMySqlTool();
	virtual ~CMySqlTool();
	bool Connect(const string& strIp,const string& strUser,const string& strPwd,const string& strDb,int _iPort);
	bool Query(const string& strSql);
	bool Insert(const string& strTbl,const map<string,string>& _mapData );
	bool Update(const string& strTbl, const map<string, string>& _mapData);
	bool GetResult(vector<map<string, string>>& vctResult);
	std::string GetError();
	bool SetCharacter(const string& _strCharacter);
private:
	void FreeResult();

private:
	MYSQL* m_pMySql;
	MYSQL_RES* m_pResult;
	MYSQL_ROW m_row;
};

