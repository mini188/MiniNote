/************************************************************************/
/*
 * ��Ȩ���� �����̳(http://www.mini188.com)
 *
 * ��������
 * Author:		5207
 * Create:	    2009-02-20
 */
/************************************************************************/
#pragma once
#include "stdafx.h"
#include <mzfc_inc.h>

//���Ŀ¼�Ƿ����
//����:
//    TCHAR* filepath : ������Ŀ¼
//����ֵ: Ŀ¼���ڷ���true, ����Ϊfalse;
bool DirectoryExists(TCHAR* filepath);

//����ļ������
//����:
//    TCHAR* filename: �������ļ�����·��
//����ֵ: ���ڷ���true, ����Ϊfalse;
bool FileExists(TCHAR* filename);


//��ȡӦ�ó���Ŀ¼
//����:
//    LPTSTR szPath: ����Ӧ�ó���Ŀ¼
//����ֵ: �ɹ�����true, ����Ϊfalse;
bool GetCurrentPath(LPTSTR szPath);

//��ȡ�ļ���
//����:
//    TCHAR * infile: �ļ�ȫ��
//    bool noExt:  ��������׺
//����ֵ: ��ȡ���ļ��� ��ʽ: txt
wchar_t *ExtractFileName(TCHAR *infile, bool noExt = true);

//��ȡ�ļ���׺
//����:
//    TCHAR filefullname: �ļ�ȫ��
//����ֵ: ��ȡ�ļ���׺
wchar_t *ExtractFileExt(TCHAR *filefullname);

//��ȡһ��Guid(�ַ���)
//����:
//    TCHAR **strGuid: ���ص�GUID�ַ���
//����ֵ: �ɹ�true, ����Ϊfalse;
bool GetGuidString(TCHAR **strGuid);


//����ת��Ϊ�ַ���
//����:
//	SYSTEMTIME systime
//����ֵ:�����ַ�������ʽ��2009-12-11
CMzString DateTimeToString(SYSTEMTIME systime);

//��ȡ��ǰ����
//����ֵ:��ǰϵͳ���ڣ���ʽ��2009-12-11
CMzString Date();

//��ȡ��ǰʱ��
//����ֵ:��ǰϵͳʱ�䣬��ʽ��12:12:15
CMzString Time();

//��ȡ��ǰ����ʱ��
//����ֵ:��ǰϵͳʱ���ַ�������ʽ��2009-12-11 00:23:34
CMzString Now();


//��ȡ�ļ����ݴ��Ŀ¼
//����ֵ:Ŀ¼
CMzString GetDataPath();

//char ת��Ϊ wchar_t
//����
//    const char* buffer : �����char
//    wchar_t* wBuf)     : ת���󴫳���wchar_t
wchar_t * chr2wch(const char *buffer);

//wchar_t ת��Ϊ char
//����
//    LPCTSTR lpString : �����char
//    char* wBuf)      : ת���󴫳���wchar_t
char * wch2chr(LPCTSTR lpString);

//�����ַ���
//����
//    TCHAR *str: ĸ��
//    TCHAR *substr:�Ӵ�
//����ֵ:����λ��
int FindStr(TCHAR *str, TCHAR *substr);

//��ȡ�ļ�������޸�ʱ��
//����
//    HANDLE hFile      : �ļ����
//    LPTSTR lpszString : ���ص�ʱ��
BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString);

//��ȡ�ļ��Ĵ���ʱ��
//����
//    TCHAR *strFile       : �ļ�
//    TCHAR *lpszString : ���ص�ʱ��
BOOL GetCreateTime(TCHAR *strFile, TCHAR **lpszString);