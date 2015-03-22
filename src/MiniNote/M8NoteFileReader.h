/************************************************************************/
/*
 * ��Ȩ���� �����̳ (http://www.mini188.com)
 *
 * �ļ��Ķ���
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
	TextTypeCode m_TextCode;//�ַ�����
	bool m_IsOpen;
public:
	M8NoteFileReader(void);
	~M8NoteFileReader(void);
	//��ȡ�ı���������
	virtual void GetTextCode(TCHAR *filename);

	//��ȡUnicode�ı��ļ�
	void ReadFileByUnicode();
	//��ȡAnsi�ı��ļ�
	void ReadFileByAnsi();

	//���ı��ļ�
	virtual bool OpenFile(TCHAR *filename);
	//
	void SaveFileByUnicode(TCHAR *filename, TCHAR *context);
	void SaveFileByAnsi(TCHAR *filename, TCHAR *context);
	//�����ı��ļ�
	virtual bool SaveFile(TCHAR *filename, TCHAR *context);
	//���Ϊ�ļ�
	virtual bool SaveAsFile(TCHAR *filename);

	CMzString GetText();
	void SetText(CMzString filecontext);
	void SetTextCode(TextTypeCode ttc);//���ñ����ʽ
	//TextTypeCode* GetTextCode11();//
};
