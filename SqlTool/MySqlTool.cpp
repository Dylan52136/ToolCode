#include "MySqlTool.h"

CMySqlTool::CMySqlTool():
	m_pMySql(nullptr),
	m_pResult(nullptr),
	m_row(nullptr)
{
	//获取一个mysql句柄
	m_pMySql = mysql_init(nullptr);
}

CMySqlTool::~CMySqlTool()
{
	if (nullptr != m_pMySql)
	{
		mysql_close(m_pMySql);
	}
	FreeResult();
}

bool CMySqlTool::Connect(const string& strIp, const string& strUser, const string& strPwd, const string& strDb, int _iPort)
{
	MYSQL* p = mysql_real_connect(m_pMySql, strIp.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), _iPort, nullptr,0);
	return p != nullptr;
}

bool CMySqlTool::Query(const string& strSql)
{
	FreeResult();
	if (0 != mysql_real_query(m_pMySql, strSql.c_str(),(unsigned long)strSql.size()))
	{
		return false;
	}
	//m_pResult = mysql_store_result(m_pMySql);
	m_pResult = mysql_use_result(m_pMySql);
	return true;
}

bool CMySqlTool::Insert(const string& strTbl, const map<string, string>& _mapData)
{
	return false;
}

bool CMySqlTool::Update(const string& strTbl, const map<string, string>& _mapData)
{
	return false;
}

bool CMySqlTool::GetResult(vector<map<string, string>>& vctResult)
{
	if (nullptr == m_pResult)
	{
		return false;
	}
	size_t iRow = mysql_num_fields(m_pResult);
	my_ulonglong nRows = mysql_num_rows(m_pResult);
	MYSQL_FIELD* fields = mysql_fetch_fields(m_pResult);
	do
	{
		map<string, string> mapTmp;
		m_row = mysql_fetch_row(m_pResult);
		if (nullptr != m_row)
		{
			for (size_t i = 0; i < iRow; i++)
			{
				mapTmp[fields[i].name] = m_row[i];
			}
			vctResult.emplace_back(mapTmp);
		}
	} while (m_row != NULL);
	return true;
}

std::string CMySqlTool::GetError()
{
	return mysql_error(m_pMySql);
}

bool CMySqlTool::SetCharacter(const string& _strCharacter)
{
	//设置字符集
	return (0 == mysql_options(m_pMySql, MYSQL_SET_CHARSET_NAME, _strCharacter.c_str()));
	//return (0 == mysql_set_character_set(m_pMySql, _strCharacter.c_str()));
}

void CMySqlTool::FreeResult()
{
	if (nullptr != m_pResult)
	{
		mysql_free_result(m_pResult);
		m_pResult = nullptr;
	}
}
