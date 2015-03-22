/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 文件阅读器
 * Author:		5207
 * Create:	    2009-03-08
 */
/************************************************************************/
#pragma once
#include <mzfc_inc.h>
#include "M8NotePubDefine.h"

class M8NoteFileReader
{
private:
	CMzString m_FileName;
	CMzString m_Text;
	TextTypeCode m_TextCode;//字符编码
	bool m_IsOpen;
public:
	M8NoteFileReader(void);
	~M8NoteFileReader(void);
	//获取文本编码类型
	virtual void GetTextCode(TCHAR *filename);

	//读取Unicode文本文件
	void ReadFileByUnicode();
	//读取Ansi文本文件
	void ReadFileByAnsi();

	//打开文本文件
	virtual bool OpenFile(TCHAR *filename);
	//
	void SaveFileByUnicode(TCHAR *filename, TCHAR *context);
	void SaveFileByAnsi(TCHAR *filename, TCHAR *context);
	//保存文本文件
	virtual bool SaveFile(TCHAR *filename, TCHAR *context);
	//另存为文件
	virtual bool SaveAsFile(TCHAR *filename);

	CMzString GetText();
	void SetText(CMzString filecontext);
	void SetTextCode(TextTypeCode ttc);//设置编码格式
	//TextTypeCode* GetTextCode11();//
};
