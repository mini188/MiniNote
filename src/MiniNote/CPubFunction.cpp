#include "CPubFunction.h"
#include "M8NotePubDefine.h"

//Ŀ¼�Ƿ���ڵļ��
bool DirectoryExists(TCHAR* filepath)
{
	int code = GetFileAttributes(filepath);
	return ((code != -1) && (FILE_ATTRIBUTE_DIRECTORY && code != 0));
}

//����ļ������
bool FileExists(TCHAR* filename)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;

  hFind = FindFirstFile(filename, &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE) 
  {
    return false;
  } 
  else 
  {
    FindClose(hFind);
    return true;
  }
}

//��ȡӦ�ó���Ŀ¼
bool GetCurrentPath(LPTSTR szPath)  
{  
	HMODULE handle = GetModuleHandle(NULL);
	DWORD dwRet = GetModuleFileName(handle, szPath, MAX_PATH);
	if (dwRet == 0)
	{
		return false;
	}
	else
	{
		TCHAR* p = szPath;
		while(*p)++p; //let p point to '\0'  
		while('\\' != *p)--p; //let p point to '\\'  
		++p;
		*p = '\0'; //get the path
		return true;  
	}
}

wchar_t *ExtractFileName(TCHAR *infile, bool noExt)
{
	TCHAR *p = new TCHAR[1024];
	wcscpy(p, infile);
	while (*p) ++p;//���ƶ������һλ
	while ('\\' != *p)--p;
	++p;

	//�Ƿ�Ҫ��ȡ��׺
	if (noExt)
	{
		TCHAR *value = p;//��ȡ�ļ���

		//����Ĵ���������ȥ����׺
		while (*value) ++value;
		while ('.' != *value)--value;
		*value = '\0';
	}
    return p;
}

wchar_t *ExtractFileExt(TCHAR *filefullname)
{
	TCHAR *tmpfile = new TCHAR[1024];
	wcscpy(tmpfile, filefullname);
	return wcsrchr(tmpfile, '.');
}

//��ȡGuid
bool GetGuidString(TCHAR **strGuid)
{
	GUID theGuid;
	LPOLESTR pstrGuid;

	CoCreateGuid(&theGuid);
	if (StringFromCLSID(theGuid, &pstrGuid) == 0)
	{
		*strGuid = pstrGuid;
		//CoTaskMemFree(pstrGuid);
		return true;
	}
	else
		return false;
}

//����ת��Ϊ�ַ���
//����:
//	SYSTEMTIME systime
//����ֵ:�����ַ�������ʽ��2009-12-11 12:12:15
CMzString DateTimeToString(SYSTEMTIME systime)
{
	CMzString sDateTime;
	wsprintf(sDateTime.C_Str(), L"%4d-%02d-%02d %02d:%02d:%02d" , systime.wYear
	                                                             , systime.wMonth
																 , systime.wDay
																 , systime.wHour
																 , systime.wMinute
																 , systime.wSecond);	

	return sDateTime;
}


//��ȡ��ǰ����
//����ֵ:��ǰϵͳ���ڣ���ʽ��2009-12-11
CMzString Date()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CMzString sDate;
	wsprintf(sDate.C_Str(), L"%4d-%02d-%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);	

	return sDate;
}


//��ȡ��ǰʱ��
//����ֵ:��ǰϵͳʱ�䣬��ʽ��12:12:15
CMzString Time()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CMzString sTime;
	wsprintf(sTime.C_Str(), L"%02d:%02d:%02d.%03d" , sysTime.wHour, sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);	
	return sTime;
}


//��ȡ��ǰʱ��
CMzString Now()
{	
	return Date() + L" " + Time();
}

//��ȡ�ļ����ݴ��Ŀ¼
CMzString GetDataPath()
{
	TCHAR szPath[MAX_PATH] = L"";
	if (GetCurrentPath(szPath))
	{
		CMzString tmp = szPath;
		tmp = tmp + DATA_PATH;
		return tmp;
	}
	else
		return L"";
}

/*
  wBuf ����Ϊָ�뼴�ɡ�
*/
wchar_t *chr2wch(const char *buffer)
{
	size_t len = strlen(buffer);
	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
	wchar_t *wBuf = new wchar_t[wlen + 1];
	MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), wBuf, int(wlen));
	return wBuf;
} 


char * wch2chr(LPCTSTR lpString)
{
 // Calculate unicode string length.
 UINT len = wcslen(lpString)*2;
 char *buf = (char *)malloc(len);
 UINT i = wcstombs(buf,lpString,len);
 return buf;
}


//��ȡ�ļ��Ĵ���ʱ��
BOOL GetCreateTime(TCHAR *strFile, TCHAR **lpszString)
{
	FILETIME ftCreate;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
    SYSTEMTIME stLocal;

	TCHAR *p = new TCHAR[40];
	hFind = FindFirstFile(strFile, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	} 
	else 
	{
		ftCreate = FindFileData.ftCreationTime;
		FileTimeToLocalFileTime(&ftCreate, &ftCreate);
		FileTimeToSystemTime(&ftCreate, &stLocal);

		CMzString strCreateDate;
		wsprintf(strCreateDate.C_Str(), L"%4d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay);	
		wcscpy(*lpszString, strCreateDate.C_Str());
		FindClose(hFind);

		return TRUE;
	}
}

//��ȡ�ļ�������޸�ʱ��
BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;

    // Convert the last-write time to local time.
    //FileTimeToSystemTime(&ftWrite, &stUTC);
	FileTimeToSystemTime(&ftWrite, &stLocal);
//    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    wsprintf(lpszString, TEXT("%d-%02d-%02d  %02d:%02d"),
        stLocal.wYear, stLocal.wMonth, stLocal.wDay, 
        stLocal.wHour, stLocal.wMinute);

    return TRUE;
}

int FindStr(TCHAR *str, TCHAR *substr)
{
    int j=0;
    for (int i=0;str[i]!='\0';i++)
    {
        if (substr[0]==str[i])
        {
            while (substr[j]!='\0'&&str[i+j]!='\0')
            {
                j++;
                if (substr[j]!=str[i+j])
                {
                    break;
                }
            }
        }
        if (substr[j]=='\0')
        {
            return i;
        }
    }
    return -1;	
}