/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 文件阅读器
 * Author:		5207
 * Create:	    2009-03-08
 */
/************************************************************************/
#include "stdafx.h"
#include "M8NoteFileReader.h"
#include "CPubFunction.h"
#include <fstream>
using namespace std;

M8NoteFileReader::M8NoteFileReader(void)
{
	m_IsOpen = false;
}

M8NoteFileReader::~M8NoteFileReader(void)
{
}


void M8NoteFileReader::GetTextCode(TCHAR *filename)
{
	if (FileExists(filename))
	{
		fstream openfile;
		openfile.open(filename, ios::in | ios::binary);
		openfile.seekg(0, ios::beg);
		char *code = new char[2];
		openfile.read(code, 2);
		openfile.close();
		
		unsigned char cc = (unsigned char)code[0];
		unsigned char cc1 = (unsigned char)code[1];
		if ((cc == 0xFF) && (cc1 == 0xFE))
		{
			m_TextCode = ttcUnicode;
		}
		else if ((cc == 0xFE) && (cc1 == 0xFF))
		{
			m_TextCode = ttcUnicodeBigEndian;
		}
		else if ((cc == 0xEF) && (cc1 == 0xBB))
		{
			m_TextCode = ttcUnicode;
		}
		else
		{
			m_TextCode = ttcAnsi;
		}
	}
}

void M8NoteFileReader::ReadFileByUnicode()
{
	wifstream ofile;
	ofile.open(m_FileName.C_Str(), ios::in | ios ::binary);
	if (ofile.is_open())
	{
		ofile.seekg(0, ios::end);
		int nLen = ofile.tellg();
		ofile.seekg(2, ios::beg);
		TCHAR *tmpstr = new TCHAR[nLen];
		wmemset(tmpstr, 0, nLen);
		ofile.read(tmpstr, nLen);			
		tmpstr[nLen] = '\0\0';
		m_Text = tmpstr;
		delete [] tmpstr;	
	}
	ofile.close();
}

void M8NoteFileReader::ReadFileByAnsi()
{
	//读取文本
	ifstream file1;
	file1.open(m_FileName.C_Str(),  ios::in | ios::binary);
	if (file1.is_open())
	{
		file1.seekg(0, ios::end);
		int nLen = file1.tellg();
		char *ss = new char[nLen+1];
		memset(ss, 0, nLen);
		file1.seekg(0, ios::beg);
		file1.read(ss, nLen);
		ss[nLen] = '\0';
		m_Text = chr2wch(ss);
	}
	file1.close();
}

//打开文本文件
bool M8NoteFileReader::OpenFile(TCHAR *filename)
{
	m_TextCode = ttcUnicode;//默认为unicode
	m_FileName = filename;
	GetTextCode(m_FileName);

	if (m_TextCode == ttcUnicode)
	{
		ReadFileByUnicode();
	}
	else if (m_TextCode == ttcAnsi)
	{
		ReadFileByAnsi();		
	}

    return true;
}

//保存Unicode文本
void M8NoteFileReader::SaveFileByUnicode(TCHAR *filename, TCHAR *context)
{
	wofstream outFile;
	outFile.open(filename, ios::out | ios::binary);
	if (outFile.is_open())
	{
		outFile.seekp(0, ios::beg);
		//先写入bom结构
		TCHAR bom = 0xfeff;
		outFile.write(&bom, 1); 
		//再写入文本内容
		int nLen = wcslen(context);
		outFile.write(context, nLen);
	}
	outFile.close();
}

//保存Ansi文本
void M8NoteFileReader::SaveFileByAnsi(TCHAR *filename, TCHAR *context)
{
	ofstream outFile;
	outFile.open(filename, ios::out | ios::binary);
	if (outFile.is_open())
	{		
		char * ss = wch2chr(context);
		int nLen = strlen(ss);
		outFile.seekp(0, ios::beg);
		outFile.write(ss, nLen);
	}
	outFile.close();
}

//保存文本文件
bool M8NoteFileReader::SaveFile(TCHAR *filename, TCHAR *context)
{
	if (m_TextCode  == ttcUnicode)
	{
		SaveFileByUnicode(filename, context);
	}
	else if (m_TextCode == ttcAnsi)
	{
		SaveFileByAnsi(filename, context);
	}
	return true;
}

//另存为文件
bool M8NoteFileReader::SaveAsFile(TCHAR *filename)
{
	return SaveFile(filename, m_Text);
}

CMzString M8NoteFileReader::GetText()
{
	return m_Text;
}

void M8NoteFileReader::SetText(CMzString filecontext)
{
	m_Text = filecontext;
}

void M8NoteFileReader::SetTextCode(TextTypeCode ttc)
{
	if (m_TextCode != ttc)
		m_TextCode = ttc;
}


//TextTypeCode* M8NoteFileReader::GetTextCode11()
//{
//
//}

