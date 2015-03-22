/************************************************************************/
/*
 * ��Ȩ���� �����̳ (http://www.mini188.com)
 *
 * ����M8���±��ı༭���������
 * Author:		5207
 * Create:	    2009-02-20
 */
/************************************************************************/
#pragma once
#include <mzfc_inc.h>
#include "M8NotePubDefine.h"
#include "M8NoteFileReader.h"

class CM8NoteEditWnd : public CMzWndEx
{
MZ_DECLARE_DYNAMIC(CM8NoteEditWnd);
private:
	M8NoteFileReader m_FileReader;
	UiScrollWin m_Scroll;
	UiSingleLineEdit m_Title;
	UiEdit m_Context;
	UiToolbar_Text m_Toolbar;
	EditType m_EditType;
	CMzString m_FileName;
	IM_SIP_MODE m_SipMode;

	virtual BOOL OnInitDialog();
	//����
	BOOL DoLoad();
	LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);

    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);
	void DoChangeEditType();
	bool LoadTextFile();
	bool SaveNoteItem();
	//�����ı��ļ�
	bool SaveTextFile();
	virtual void OnTimer(UINT_PTR nIDEvent);


	void ChangeSipMode(IM_SIP_MODE sipmode);
	void ChangeEditState();
public:
	CM8NoteEditWnd(void);
	CM8NoteEditWnd(EditType etType, TCHAR* filename);

	~CM8NoteEditWnd(void);
};
