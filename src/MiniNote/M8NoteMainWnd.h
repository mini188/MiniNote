/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 这是M8记事本的主窗体，用于显示日志列表
 * Author:		5207
 * Create:	    2009-02-20
 */
/************************************************************************/

#pragma once
#include <mzfc_inc.h>
#include "M8NotePubDefine.h"
#include "MiniUiList.h"

#define M8_UID_NOTE_LIST 201
#define M8_UID_NOTE_TOOLBAR 202

#define M8_UID_NOTE_CPATION 203
#define M8_UID_NOTE_ADD 204
#define M8_UID_NOTE_EDIT 205
#define M8_UID_NOTE_DEL 206
#define M8_UID_NOTE_VIEW 207

class CM8NoteMainWnd : public CMzWndEx
{
MZ_DECLARE_DYNAMIC(CM8NoteMainWnd);
private:
	MiniUiList m_NoteList;
	UiToolbar_Text m_Toolbar;
	CMzString m_AppFilePath; //应用程序目录
	CMzString m_DataPath;  //日志文件存放目录
	NoteItems m_NoteItems; //日志列表

	UiCaption m_lblTitle;

	virtual BOOL OnInitDialog();

	bool UpdateOldData(LPTSTR strDataDir);

	bool CreateNoteItem(LPTSTR strFileName, NoteItem &theItem);
	NoteItem* GetNoteItem(int nIndex);
	bool DoDeleteNoteItem(int iIndex);
	//加载所有的日记
	void LoadNoteList(LPTSTR strDataDir);
	LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual void OnMzCommand(WPARAM wParam, LPARAM lParam);

	//--------菜单函数--------
	void AddNoteItem();
	void OpenFile();
	void DeleteNoteItem();
	bool EditNoteItem(int nIndex);

	void EnabledBarButton(int nIndex, bool bEnabled);
	//文件菜单
	void PopupFileMenu();
	//------------------------
public:
	CM8NoteMainWnd(void);
	~CM8NoteMainWnd(void);

};

