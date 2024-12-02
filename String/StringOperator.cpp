#include "StringOperator.h"
#include <Windows.h>

std::string CStringOperator::Unicode2Utf8(const std::wstring& _wstr)
{
    std::string strRes;
    //WideCharToMultiByte:�� UTF-16 (���ַ�) �ַ���ӳ�䵽�µ��ַ����� ���ַ�����һ�����Զ��ֽ��ַ�����
    int length = ::WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(),NULL, 0, NULL, NULL);
    strRes.resize(length + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), _wstr.size(), (LPSTR)strRes.c_str(), length, NULL, NULL);
    return strRes;
}
