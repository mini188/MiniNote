#include "CPubFunction.h"
#include "M8NotePubDefine.h"

//目录是否存在的检查
bool DirectoryExists(TCHAR* filepath)
{
	int code = GetFileAttributes(filepath);
	return ((code != -1) && (FILE_ATTRIBUTE_DIRECTORY && code != 0));
}

//检查文件否存在
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

//获取应用程序目录
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
	while (*p) ++p;//先移动到最后一位
	while ('\\' != *p)--p;
	++p;

	//是否要截取后缀
	if (noExt)
	{
		TCHAR *value = p;//截取文件名

		//下面的代码是用于去除后缀
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

//获取Guid
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

//日期转换为字符串
//参数:
//	SYSTEMTIME systime
//返回值:日期字符串，格式：2009-12-11 12:12:15
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


//获取当前日期
//返回值:当前系统日期，格式：2009-12-11
CMzString Date()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CMzString sDate;
	wsprintf(sDate.C_Str(), L"%4d-%02d-%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);	

	return sDate;
}


//获取当前时间
//返回值:当前系统时间，格式：12:12:15
CMzString Time()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CMzString sTime;
	wsprintf(sTime.C_Str(), L"%02d:%02d:%02d.%03d" , sysTime.wHour, sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);	
	return sTime;
}


//获取当前时间
CMzString Now()
{	
	return Date() + L" " + Time();
}

//获取文件数据存放目录
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
  wBuf 申明为指针即可。
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


//获取文件的创建时间
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

//获取文件的最后修改时间
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