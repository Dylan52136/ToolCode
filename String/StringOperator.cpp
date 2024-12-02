#include "StringOperator.h"
#include <Windows.h>

std::string CStringOperator::Unicode2Utf8(const std::wstring& _wstr)
{
    std::string strRes;
    //WideCharToMultiByte:将 UTF-16 (宽字符) 字符串映射到新的字符串。 新字符串不一定来自多字节字符集。
    int length = ::WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(),NULL, 0, NULL, NULL);
    strRes.resize(length + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(), (LPSTR)strRes.c_str(), length, NULL, NULL);
    return strRes;
}
