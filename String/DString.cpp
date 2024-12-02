#include "DString.h"
#include <string.h>
#include <cstdarg>
#include <cctype>

DString::DString()
{
    Init();
}

DString::DString(const DString& _str)
    :m_pData(nullptr),
    m_iLen(0)
{
    if (_str.m_pData)
    {
        m_pData = new char[_str.m_iLen];
        memcpy(m_pData, _str.m_pData, _str.m_iLen);
        m_iLen = _str.m_iLen;
    }
}

DString::DString(DString&& _str) noexcept
    :m_pData(_str.m_pData),
    m_iLen(_str.m_iLen)
{
    _str.m_pData = nullptr;
    _str.m_iLen = 0;
}

DString::DString(const char* _pData, int _iLen)
{
    if (nullptr == _pData || _iLen < 0)
    {
        Init();
        return;
    }

    m_pData = new char[_iLen + 1];
    memcpy(m_pData, _pData, _iLen);
    m_pData[_iLen] = '\0';
    m_iLen = _iLen;
}

DString::DString(const char* _pData)
{
    if (nullptr == _pData)
    {
        Init();
        return;
    }
    int iLen = strlen(_pData);
    m_pData = new char[iLen + 1];
    memcpy(m_pData, _pData, iLen);
    m_pData[iLen] = '\0';
    m_iLen = iLen;
}

DString::~DString()
{
    if (nullptr != m_pData)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }
}

DString& DString::operator=(const DString& _str)
{
    if (&_str == this) return *this;
   
    char* pNewData = new char[_str.m_iLen + 1];
    memcpy(pNewData, _str.m_pData, _str.m_iLen);
    pNewData[_str.m_iLen] = '\0';

    delete[] m_pData;
    m_pData = pNewData;
    m_iLen = _str.m_iLen;

    return *this;
}

DString& DString::operator=(DString&& _str) noexcept
{
    if (&_str == this) return *this;

    delete[] m_pData;

    m_pData = _str.m_pData;
    m_iLen = _str.m_iLen;

    _str.m_pData = nullptr;
    _str.m_iLen = 0;

    return *this;
}

DString& DString::operator+=(const DString& _str)
{
    AddData(_str.m_pData, _str.m_iLen);
    return *this;
}

DString& DString::operator+=(const char* _p)
{
    if (nullptr == _p || *_p == '\0') return *this;
    AddData(_p, strlen(_p));
    return *this;
}

DString& DString::operator+=(char c)
{
    AddData(&c, 1);
    return *this;
}

bool DString::operator==(const DString& _str) const
{
    if (m_iLen != _str.m_iLen) return false;
    return (0 == strcmp(m_pData,_str.m_pData));
}

char DString::operator[](int _iIndex)
{
    if (_iIndex >= m_iLen || _iIndex < 0) return 0;
    return m_pData[_iIndex];
}


void DString::AddData(const char* _pData, int _iDataLen)
{
    /*  基本思路
        1.创建一个新的缓冲区，其长度是当前字符串长度和右操作数长度的总和。
        2.将当前字符串和右操作数的内容复制到新的缓冲区中。
        3.替换当前对象的缓冲区，并更新长度。*/
    char* pNewData = new char[m_iLen + _iDataLen + 1];
    memcpy(pNewData, m_pData, m_iLen);
    memcpy(pNewData + m_iLen, _pData, _iDataLen);

    delete[] m_pData;

    m_pData = pNewData;
    m_iLen = m_iLen + _iDataLen;
    m_pData[m_iLen] = '\0';
}

bool DString::Empty() const
{
    return (0 == m_iLen);
}

int DString::Size() const
{
    return m_iLen;
}

bool DString::Equal(const DString& _str)
{
    return 0 == Compare(_str);
}

bool DString::Equal(const char* _p)
{
    return (0 == strcmp(m_pData, _p));
}

int DString::Compare(const DString& _str)
{
    return strcmp(m_pData, _str.m_pData);
}

int DString::Find(const char* _p, int _iStartPos)
{
    //可用std::strstr函数实现
    if (nullptr == _p || _iStartPos >= m_iLen || strlen(_p) > m_iLen || *_p == '\0') return -1;
    int iLen = strlen(_p);
    for (int i = _iStartPos; i <= m_iLen - iLen; ++i)
    {
        if (0 == memcmp(m_pData + i, _p,iLen))
        {
            return i;
        }
    }
    return -1;
}

int DString::Find(char c, int _iStartPos)
{
    if (_iStartPos < 0 || _iStartPos >= m_iLen)  return -1;
    for (int i = _iStartPos; i < m_iLen; ++i)
    {
        if (c == *(m_pData + i))
        {
            return i;
        }
    }
    return -1;
}

int DString::Find(const DString& _str, int _iStartPos)
{
    if (_iStartPos < 0 || _iStartPos >= m_iLen || _str.m_iLen > m_iLen || _str.m_iLen <= 0) return -1;
    for (int i = _iStartPos; i <= m_iLen - _str.m_iLen; ++i)
    {
        if (0 == memcmp(m_pData + i, _str.m_pData,_str.m_iLen))
        {
            return i;
        }
    }
    return -1;
}

std::vector<int> DString::BuildPrefixTabel(const DString& _pattern)
{
    int m = _pattern.Size();
    std::vector<int> prefix(m, 0);
    int len = 0;  //当前最长前缀的长度
    int i = 1;

    while (i < m)
    {
        if (_pattern.m_pData[i] == _pattern.m_pData[len])
        {
            len++;
            prefix[i] = len;
            i++;
        }
        else
        {
            if (len > 0)
            {
                len = prefix[len - 1]; //回退到前一个最长前缀的位置
            }
            else
            {
                prefix[i] = 0; //无法回退，设为0
                i++;
            }
        }
    }

    return prefix;
}

int DString::KmpSearch(const DString& _pattern)
{
    if (_pattern.Empty())
    {
        return 0;
    }
    std::vector<int> prefix = BuildPrefixTabel(_pattern);

    int m = _pattern.Size();
    int i = 0; //主串索引
    int j = 0; //模式串索引

    while (i < m_iLen)
    {
        if (m_pData[i] == _pattern.m_pData[j])
        {
            i++;
            j++;
        }

        if (j == m)
        {
            return i - j;  //匹配成功，返回起始位置
        }
        else if (i < m_iLen && m_pData[i] != _pattern.m_pData[j])
        {
            if (j > 0)
            {
                j = prefix[j - 1];  //使用部分匹配表进行跳跃
            }
            else
            {
                i++; //无法跳跃，主串索引前进
            }
        }
    }
    return -1;
}

DString& DString::Append(const DString& _str)
{
    AddData(_str.m_pData, _str.m_iLen);
    return *this;
}

DString& DString::Append(const char* _p)
{
    if (nullptr == _p || *_p == '\0') return *this;
    AddData(_p, strlen(_p));
    return *this;
}

DString& DString::Append(char c)
{
    AddData(&c, 1);
    return *this;
}

const char* DString::Data()
{
    return m_pData;
}

void DString::format(const char* _format, ...)
{
    if (!_format)
    {
        Init(); //清空当前字符串
        return;
    }
    //计算格式化后字符串的长度
    va_list args;
    va_start(args, _format);
    int len = std::vsnprintf(nullptr, 0, _format, args);
    va_end(args);

    if (len < 0)
    {
        Init();// 格式化失败，清空当前字符串
        return;
    }

    //分配内存并生成格式化字符串
    char* buffer = new char[len + 1];
    va_start(args, _format);
    std::vsnprintf(buffer, len + 1, _format, args);
    va_end(args);

    //更新当前字符串的内容
    delete[] m_pData;
    m_pData = buffer;
    m_iLen = len + 1;
}

void DString::Replace(const DString& _strOld, const DString& _strNew)
{
    if (0 == _strOld.m_iLen || _strOld == _strNew) return;
    int iIndex = Find(_strOld);
    if (-1 != iIndex)
    {
        DString strRes(m_pData,iIndex);
        strRes += _strNew;
        strRes += (m_pData + iIndex + _strOld.m_iLen);
        *this = strRes;
    }
}

void DString::Replace(const char* _pOld, const char* _pNew)
{
    if (nullptr == _pOld || *_pOld == '\0' || nullptr == _pNew) return;
    Replace(DString(_pOld, strlen(_pOld)), DString(_pNew, strlen(_pNew)));
}

void DString::Replace(char cOld, char cNew)
{
    if (cOld == cNew || 0 == m_iLen) return;
    DString strRes;
    int iIndex = Find(cOld);
    if (-1 != iIndex)
    {
        DString strRes(m_pData, iIndex);
        strRes += cNew;
        strRes += (m_pData + iIndex + 1);
        *this = strRes;
    }
}

void DString::ReplaceAll(const DString& _strOld, const DString& _strNew)
{
    if (0 == _strOld.m_iLen || _strOld.m_pData == _strNew.m_pData)  return;
    int iIndex = -1;
    int iPos = 0;
    DString strRes;
    while ((iIndex = Find(_strOld,iPos)) != -1)
    {
        strRes += DString(m_pData + iPos,iIndex - iPos);
        strRes += _strNew;
        iPos = iIndex + _strOld.m_iLen;
    }
    strRes += (m_pData + iPos);

    delete[] m_pData;
    int iLen = strlen(strRes.m_pData);
    m_pData = new char[iLen + 1];
    memcpy(m_pData, strRes.m_pData, iLen);
    m_iLen = iLen;
    m_pData[m_iLen] = '\0';
}

void DString::ReplaceAll(const char* _pOld, const char* _pNew)
{
    if (nullptr == _pOld || *_pOld == '\0' || nullptr == _pNew)
    {
        return;
    }
    ReplaceAll(DString(_pOld, strlen(_pOld)), DString(_pNew, strlen(_pNew)));
}

void DString::ReplaceAll(char cOld, char cNew)
{
    if (cOld == cNew) return;
    DString strRes;
    for (int i = 0; i < m_iLen; ++i)
    {
        if (m_pData[i] == cOld)
        {
            strRes += cNew;
        }
        else
        {
            strRes += m_pData[i];
        }
    }
    delete[] m_pData;

    m_pData = new char[m_iLen + 1];
    memcpy(m_pData, strRes.m_pData, m_iLen);
    m_pData[m_iLen] = '\0';
}

std::vector<DString> DString::SplitEx(const DString& _str)
{
    return std::vector<DString>();
}

std::vector<DString> DString::Split(const DString& _str)
{
    std::vector<DString> vctRes;
    int iStart = 0;
    int iIndex = -1;
    while ((iIndex = Find(_str, iStart)) != -1)
    {
        vctRes.emplace_back(DString(m_pData + iStart, iIndex - iStart));
        iStart = iIndex + _str.m_iLen;
    }
    vctRes.emplace_back(DString(m_pData + iStart, m_iLen - iStart));
    return vctRes;
}

std::vector<DString> DString::Split(const char* _p)
{
    std::vector<DString> vctRes;
    int iStart = 0;
    int iIndex = -1;
    while ((iIndex = Find(_p, iStart)) != -1)
    {
        vctRes.emplace_back(DString(m_pData + iStart, iIndex - iStart));
        iStart = iIndex + strlen(_p);
    }
    vctRes.emplace_back(DString(m_pData + iStart, m_iLen - iStart));
    return vctRes;
}

std::vector<DString> DString::Split(char c)
{
    std::vector<DString> vctRes;
    int iStart = 0;
    int iIndex = -1;
    while ((iIndex = Find(c, iStart)) != -1)
    {
        vctRes.emplace_back(DString(m_pData, iIndex - iStart));
        iStart = iIndex + 1;
    }
    vctRes.emplace_back(DString(m_pData + iStart, m_iLen - iStart));
    return vctRes;
}

DString& DString::Trim()
{
    if ('\0' == *m_pData || 0 == m_iLen)   return *this;
    int iStart = 0;
    for (int i = iStart; i < m_iLen; i++)
    {
        if (!std::isspace(m_pData[i]))
        {
            break;
        }
        iStart++;
    }

    int iEnd = m_iLen - 1;
    for (int i = iEnd; i > iStart; i--)
    {
        if (!std::isspace(m_pData[i]))
        {
            break;
        }
        iEnd--;
    }

    int iNewLen = iEnd - iStart + 1;
    char* pNew = new char[iNewLen + 1];
    memcpy(pNew, m_pData + iStart, iNewLen);
    delete[] m_pData;
    m_pData = pNew;
    m_iLen = iNewLen;
    m_pData[m_iLen] = '\0';
    return *this;

}

DString& DString::LTrim()
{
    if ('\0' == *m_pData || 0 == m_iLen) return *this;
    int iStart = 0;
    for (int i = 0; i < m_iLen; i++)
    {
        if (!isspace(m_pData[i]))
        {
            break;
        }
        iStart++;
    }

    if (iStart > 0)
    {
        int iLen = m_iLen - iStart + 1;
        char* pNew = new char[iLen + 1];
        memcpy(pNew, m_pData + iStart, iLen);

        delete[] m_pData;
        m_pData = pNew;
        m_iLen = iLen;
        m_pData[m_iLen] = '\0';

    }

    return *this;

}

DString& DString::RTrim()
{
    if ('\0' == *m_pData || 0 == m_iLen) return *this;
    int iEnd = m_iLen - 1;
    for (int i = iEnd; i > 0; i--)
    {
        if (!isspace(m_pData[i]))
        {
            break;
        }
        iEnd--;
    }

    if (iEnd < m_iLen - 1)
    {
        int iLen = iEnd + 1;
        char* pNew = new char[iLen + 1];
        memcpy(pNew, m_pData, iLen);

        delete[] m_pData;
        m_pData = pNew;
        m_iLen = iLen;
        m_pData[m_iLen] = '\0';

    }

    return *this;
}

DString& DString::ToLowerCase()
{
    for (int i = 0; i < m_iLen; ++i)
    {
        if (m_pData[i] >= 'A' && m_pData[i] <= 'Z')
        {
            m_pData[i] = m_pData[i] + 32;
        }
    }
    return *this;
}

DString& DString::ToUpperCase()
{
    for (int i = 0; i < m_iLen; ++i)
    {
        if (m_pData[i] >= 'a' && m_pData[i] <= 'z')
        {
            m_pData[i] = m_pData[i] - 32;
        }
    }
    return *this;
}

std::string DString::ToStdString()
{
    return std::string(m_pData);
}

void DString::Print() const
{
    if (m_pData)
    {
        ::printf("%s\n", m_pData);
    }
}

DString DString::Format(const char* _format, ...)
{
    if (!_format)
    {
        return DString();
    }
    //计算格式化后字符串的长度
    va_list args;
    va_start(args, _format);
    int len = std::vsnprintf(nullptr, 0, _format, args);
    va_end(args);

    if (len < 0)
    {
        return DString();
    }

    //分配内存并生成格式化字符串
    //char* buffer = new char[len + 1];
    std::vector<char> buffer(len + 1);
    va_start(args, _format);
    std::vsnprintf(buffer.data(), len + 1, _format, args);
    va_end(args);

    DString strRes(buffer.data(), len);
    return strRes;
}

void DString::Init()
{
    m_pData = new char[1];
    m_pData[0] = '\0';
    m_iLen = 0;
}

