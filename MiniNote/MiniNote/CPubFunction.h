/************************************************************************/
/*
 * 版权所有 点滴论坛(http://www.mini188.com)
 *
 * 公共函数
 * Author:		5207
 * Create:	    2009-02-20
 */
/************************************************************************/
#pragma once
#include "stdafx.h"
#include <mzfc_inc.h>

//检查目录是否存在
//参数:
//    TCHAR* filepath : 待检查的目录
//返回值: 目录存在返回true, 否则为false;
bool DirectoryExists(TCHAR* filepath);

//检查文件否存在
//参数:
//    TCHAR* filename: 待检查的文件完整路径
//返回值: 存在返回true, 否则为false;
bool FileExists(TCHAR* filename);


//获取应用程序目录
//参数:
//    LPTSTR szPath: 返回应用程序目录
//返回值: 成功返回true, 否则为false;
bool GetCurrentPath(LPTSTR szPath);

//截取文件名
//参数:
//    TCHAR * infile: 文件全名
//    bool noExt:  不包含后缀
//返回值: 截取的文件名 格式: txt
wchar_t *ExtractFileName(TCHAR *infile, bool noExt = true);

//截取文件后缀
//参数:
//    TCHAR filefullname: 文件全名
//返回值: 截取文件后缀
wchar_t *ExtractFileExt(TCHAR *filefullname);

//获取一个Guid(字符串)
//参数:
//    TCHAR **strGuid: 返回的GUID字符串
//返回值: 成功true, 否则为false;
bool GetGuidString(TCHAR **strGuid);


//日期转换为字符串
//参数:
//	SYSTEMTIME systime
//返回值:日期字符串，格式：2009-12-11
CMzString DateTimeToString(SYSTEMTIME systime);

//获取当前日期
//返回值:当前系统日期，格式：2009-12-11
CMzString Date();

//获取当前时间
//返回值:当前系统时间，格式：12:12:15
CMzString Time();

//获取当前日期时间
//返回值:当前系统时间字符串，格式：2009-12-11 00:23:34
CMzString Now();


//获取文件数据存放目录
//返回值:目录
CMzString GetDataPath();

//char 转换为 wchar_t
//参数
//    const char* buffer : 传入的char
//    wchar_t* wBuf)     : 转换后传出的wchar_t
wchar_t * chr2wch(const char *buffer);

//wchar_t 转换为 char
//参数
//    LPCTSTR lpString : 传入的char
//    char* wBuf)      : 转换后传出的wchar_t
char * wch2chr(LPCTSTR lpString);

//查找字符串
//参数
//    TCHAR *str: 母串
//    TCHAR *substr:子串
//返回值:返回位置
int FindStr(TCHAR *str, TCHAR *substr);

//获取文件的最后修改时间
//参数
//    HANDLE hFile      : 文件句柄
//    LPTSTR lpszString : 返回的时间
BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString);

//获取文件的创建时间
//参数
//    TCHAR *strFile       : 文件
//    TCHAR *lpszString : 返回的时间
BOOL GetCreateTime(TCHAR *strFile, TCHAR **lpszString);