#pragma once
#include <vector>
#include <string>
class DString
{
public:
	DString();
	DString(const DString& _str);
	DString(DString&& _str) noexcept;  //移动构造函数
	DString(const char* _pData, int _iLen);
	DString(const char* _pData);
	virtual ~DString();
	DString& operator=(const DString & _str);
	DString& operator=(DString&& _str) noexcept;
	DString& operator+=(const DString& _str);
	DString& operator+=(const char* _p);
	DString& operator+=(char c);
	bool operator==(const DString& _str) const;
	char operator[](int _iIndex);
	bool Empty() const;
	int Size() const;
	bool Equal(const DString& _str);
	bool Equal(const char* _p);
	int Compare(const DString& _str);
	int Find(const char* _p, int _iStartPos = 0);
	int Find(char c, int _iStartPos = 0);
	int Find(const DString& _str,int _iStartPos = 0);
	DString& Append(const DString& _str);
	DString& Append(const char* _p);
	DString& Append(char c);
	const char* Data();
	//Format函数
	void format(const char* _format, ...);
	void Replace(const DString& _strOld, const DString& _strNew);
	void Replace(const char* _pOld, const char* _pNew);
	void Replace(char cOld, char cNew);
	void ReplaceAll(const DString& _strOld, const DString& _strNew);
	void ReplaceAll(const char* _pOld, const char* _pNew);
	void ReplaceAll(char cOld, char cNew);
	std::vector<DString> SplitEx(const DString& _str);
	std::vector<DString> Split(const DString& _str);
	std::vector<DString> Split(const char* _p);
	std::vector<DString> Split(char c);
	DString& Trim();
	DString& LTrim();
	DString& RTrim();
	DString& ToLowerCase();
	DString& ToUpperCase();
	std::string ToStdString();
	void Print() const;

	std::vector<int> BuildPrefixTabel(const DString& _pattern);
	int KmpSearch(const DString& _pattern);

	static DString Format(const char* _format, ...);

private:
	void Init();
	void AddData(const char* _pData,int _iDataLen);

private:
	char* m_pData;
	int m_iLen;  //字符串的长度不包含结尾的'\0'
};

