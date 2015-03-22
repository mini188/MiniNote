#include "stdafx.h"
#include "M8NoteMainWnd.h"
#include "CPubFunction.h"
#include "ReadWriteIni.h"
#include "M8NoteEditWnd.h"
#include "ShellNotifyMsg.h"
#include "MiniFileDialog.h"

#define IDC_PPM_ADD 1001
#define IDC_PPM_OPEN 1002
#define IDC_PPM_DEL 1003
#define IDC_PPM_CANCEL 1004

#define M_KEY_1 1111


CM8NoteMainWnd::CM8NoteMainWnd(void)
{
	
}

CM8NoteMainWnd::~CM8NoteMainWnd(void)
{

}

bool CM8NoteMainWnd::CreateNoteItem(LPTSTR strFileName, NoteItem &theItem)
{
	if (FileExists(strFileName))
	{
		TCHAR *sTmp;
		//获取标题	
		if (!IniReadString(L"Header", L"Title", &sTmp, strFileName))
			return false;
		theItem.cTitle = sTmp;
		//GetFileTime(
		//获取日期
		if (!IniReadString(L"Header", L"DateTime", &sTmp, strFileName))
			return false;
		theItem.cDateTime = sTmp;
		//文件目录
		theItem.cFileName = strFileName;
		return true;
	}
	else
	{
		return false;
	}
}

NoteItem* CM8NoteMainWnd::GetNoteItem(int nIndex)
{
	return &m_NoteItems[nIndex];
}

//升级老版的INI数据到新的Txt格式
bool CM8NoteMainWnd::UpdateOldData(LPTSTR strDataDir)
{
	CMzString tmpDataDir = strDataDir;
	if (DirectoryExists(tmpDataDir) == false)
	{
		return false;
	}
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(tmpDataDir + L"*.ini", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	} 
	else 
	{
		CMzString filename;
		TCHAR *str = 0;
		TCHAR *sTmp;
		M8NoteFileReader tmpReader;
		do
		{
			//跳过'.''..'
			if (FindFileData.cFileName[0] == '.')
				continue;

			//找到一个文件
			filename = tmpDataDir + FindFileData.cFileName;
			if (tmpReader.OpenFile(filename) == false)
			{
				continue;
			}
			str = tmpReader.GetText();//获取文件内容
			//加载文件，将清除掉INI信息，留下内容及文件名
			if (IniReadString(L"Header", L"Title", &sTmp, filename))
			{
				int ipos = FindStr(str, L"Context=");
				if (ipos != -1)
				{
					int i = 0;
					ipos += wcslen(L"Content=");
					while(i <= ipos)
					{
						++str;
						++i;
					}
					--str;
					tmpReader.SetText(*str);
					//格式化新的文件名
					LPTSTR szBuffer=new TCHAR[1024];
					wsprintf(szBuffer, L"%s\\%s.%s", tmpDataDir.C_Str(), sTmp, L"txt");//保存文件名
					if (tmpReader.SaveAsFile(szBuffer))
						return DeleteFile(filename) == TRUE;
				}
			}

		}
		while (FindNextFile(hFind, &FindFileData));
	}
	FindClose(hFind);//关闭句柄
	return true;
}

void CM8NoteMainWnd::LoadNoteList(LPTSTR strDataDir)
{
	this->m_NoteList.RemoveAll();
	this->m_NoteItems.clear();
	CMzString tmpDataDir = strDataDir;
	//检查Data文件夹是否存在，不存在则创建
	if (DirectoryExists(tmpDataDir) == false)
	{
		CreateDirectory(tmpDataDir, NULL);
	}
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(tmpDataDir + L"*.*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	
	ListItem listItem;
	do
	{
		//跳过'.''..'
		if (FindFileData.cFileName[0] == '.')
			continue;

		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//遍历子文件夹
		}
		else
		{
			NoteItem *noteItem = new NoteItem(); 
			//找到一个文件
			CMzString tmpfilename = tmpDataDir + FindFileData.cFileName;
			noteItem->cFileName = tmpfilename;
			LPTSTR fname = new TCHAR[1024];
			fname = ExtractFileName(tmpfilename.C_Str());
			noteItem->cTitle = fname;

			TCHAR *tmpFileTime = new TCHAR[40];
			if (!GetCreateTime(tmpfilename.C_Str(), &tmpFileTime))
				return;

			noteItem->cDateTime = tmpFileTime;
			wmemset(tmpFileTime, 0, wcslen(tmpFileTime));
			noteItem->bSelected = false;
			m_NoteItems.push_back(*noteItem);

			//加入UiList
			listItem.Text = noteItem->cTitle;
			listItem.Data = noteItem;
			this->m_NoteList.AddItem(listItem);
		}
	}
	while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);//关闭句柄

	m_NoteList.Invalidate();
	m_NoteList.Update();
}

bool CM8NoteMainWnd::DoDeleteNoteItem(int iIndex)
{
	NoteItem nItem = m_NoteItems[iIndex];
	//删除文件
	return DeleteFile(nItem.cFileName) == TRUE;
}

BOOL CM8NoteMainWnd::OnInitDialog()
{
    if (!CMzWndEx::OnInitDialog())
    {
      return FALSE;
    };

	int y = 0;
	//初始化Caption
	y = 40;
	m_lblTitle.SetID(M8_UID_NOTE_CPATION);
	m_lblTitle.SetPos(0, 0, GetWidth(), y);
	m_lblTitle.SetText(L"日志列表");
	AddUiWin(&m_lblTitle);

	//初始化UiList
	m_NoteList.SetID(M8_UID_NOTE_LIST);
	y = GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR*2;
	m_NoteList.SetPos(0, m_lblTitle.GetHeight(), GetWidth(), y);
	m_NoteList.EnableDragScrollbarV(true);
	m_NoteList.EnableNotifyMessage(true);
	m_NoteList.SetTextSize(35);
	AddUiWin(&m_NoteList);

	//初始化Toolbar
	m_Toolbar.SetID(M8_UID_NOTE_TOOLBAR);
	m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);    
	m_Toolbar.SetButton(0, true, true, L"文件");
    m_Toolbar.SetButton(1, true, true, L"选择");
    m_Toolbar.SetButton(2, true, true, L"退出");
	AddUiWin(&m_Toolbar);

	//m_Toolbar.EnableButton(1, false);
	m_AppFilePath = L"";//暂不需要，先设置为空
    m_DataPath = GetDataPath();//获取Data目录
	UpdateOldData(m_DataPath);//更新老数据
	this->LoadNoteList(m_DataPath);
	return TRUE;
}

LRESULT CM8NoteMainWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
	case MZ_WM_MOUSE_NOTIFY:
		{
			int nID = LOWORD(wParam);
			int nNotify = HIWORD(wParam);
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if (nID==M8_UID_NOTE_LIST && nNotify==MZ_MN_LBUTTONDOWN)
			{
				if (!m_NoteList.IsMouseDownAtScrolling() && !m_NoteList.IsMouseMoved())
				{
					int nIndex = m_NoteList.CalcIndexOfPos(x, y);
					m_NoteList.SetSelectedIndex(nIndex);
					//EnabledBarButton(1, nIndex != -1);
					m_NoteList.Invalidate();
					m_NoteList.Update();
				}
				return 0;
			}

			if (nID==M8_UID_NOTE_LIST && nNotify==MZ_MN_MOUSEMOVE)
			{
				m_NoteList.SetSelectedIndex(-1);
				//EnabledBarButton(1, false);

				m_NoteList.Invalidate();
				m_NoteList.Update();
				return 0;
			}

			if (nID = M8_UID_NOTE_LIST && nNotify == MZ_MN_LBUTTONDBLCLK)
			{
				if (!m_NoteList.IsMouseDownAtScrolling() && !m_NoteList.IsMouseMoved())
				{
					int nIndex = m_NoteList.CalcIndexOfPos(x, y);
					m_NoteList.SetSelectedIndex(nIndex);
					//EnabledBarButton(1, nIndex != -1);
					if (EditNoteItem(nIndex))
					{
						LoadNoteList(m_DataPath);
					}
				}
			}
		}
		break;
	}
	return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

void CM8NoteMainWnd::AddNoteItem()
{
	CM8NoteEditWnd theNoteEdit;
	RECT rcWork = MzGetWorkArea();
	theNoteEdit.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), m_hWnd, 0, WS_POPUP);
	theNoteEdit.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_BOTTOM_TO_TOP_2);
	theNoteEdit.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_TOP_TO_BOTTOM_1);
	int nRet = theNoteEdit.DoModal();
	if (nRet == ID_OK)
	{
		this->LoadNoteList(m_DataPath.C_Str());
	}
}

bool CM8NoteMainWnd::EditNoteItem(int nIndex)
{
	if (nIndex < 0 || nIndex > m_NoteList.GetItemCount()) 
		return false;

	//进入编辑
	NoteItem theItem;
	theItem = m_NoteItems[nIndex];
	CM8NoteEditWnd theNoteEdit(etView, theItem.cFileName);
	RECT rcWork = MzGetWorkArea();
	theNoteEdit.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), m_hWnd, 0, WS_POPUP);
	theNoteEdit.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_BOTTOM_TO_TOP_2);
	theNoteEdit.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_TOP_TO_BOTTOM_1);
	int nRet = theNoteEdit.DoModal();
	return nRet == ID_OK;
}

void CM8NoteMainWnd::OpenFile()
{
	CMiniFileDialog *openFile = NULL;
	openFile = new CMiniFileDialog();
	//openFile->SetDefaultDir(L"");
	openFile->SetExtFilter(L"*.txt;*.ini;");

	TCHAR *thefile;				
	if (openFile->SelectFile(&thefile))
	{
		CMzString mzstr = thefile;
		CM8NoteEditWnd theNoteEdit(etView, mzstr.C_Str());
		RECT rcWork = MzGetWorkArea();
		theNoteEdit.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), m_hWnd, 0, WS_POPUP);
		theNoteEdit.SetAnimateType_Show(MZ_ANIMTYPE_SCROLL_BOTTOM_TO_TOP_2);
		theNoteEdit.SetAnimateType_Hide(MZ_ANIMTYPE_SCROLL_TOP_TO_BOTTOM_1);
		int nRet = theNoteEdit.DoModal();
		if (nRet == ID_OK)
		{
			LoadNoteList(m_DataPath);
		}
	}
	delete openFile;
}

void CM8NoteMainWnd::DeleteNoteItem()
{
	if (MzMessageBoxEx(m_hWnd, L"确认要删除所选日志吗？", L"提醒", MZ_OKCANCEL) != 1)
	{
		return;
	}
	
	if (m_NoteList.GetSelectMode())
	{
		if (m_NoteList.GetItemCount() > 0)
		{
			int i = m_NoteList.GetItemCount()-1;
			ListItem *pItem;
			while (i >= 0)
			{
				pItem = m_NoteList.GetItem(i);
				if (pItem==0) return;
				if (pItem->Data==0) return;

				NoteItem *itemData = (NoteItem*)pItem->Data;					
				if (itemData->bSelected)
				{
					DoDeleteNoteItem(i);
				}
				i--;
			}
			LoadNoteList(m_DataPath);
		}
	}
	else
	{
		int idx = m_NoteList.GetSelectedIndex();
		if (idx != -1)
		{
			//刷新列表
			if (DoDeleteNoteItem(idx))
				LoadNoteList(m_DataPath);
		}
		else
		{
			MzMessageBoxEx(m_hWnd, L"没有选择日志哦！", L"提醒");
		}
	}
}

void CM8NoteMainWnd::EnabledBarButton(int nIndex, bool bEnabled)
{
	m_Toolbar.EnableButton(nIndex, bEnabled);
	m_Toolbar.Invalidate();
	m_Toolbar.Update();
}

void CM8NoteMainWnd::PopupFileMenu()
{
	CPopupMenu ppm;
	struct PopupMenuItemProp pmip;    

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_CANCEL;
	pmip.str = L"取消";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_DEL;
	pmip.str = L"删除日志";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_OPEN;
	pmip.str = L"打开文件(txt/ini)";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_ADD;
	pmip.str = L"新建日志";
	ppm.AddItem(pmip);

	RECT rc = MzGetWorkArea();      
	rc.top = rc.bottom - ppm.GetHeight();
	ppm.Create(rc.left,rc.top,RECT_WIDTH(rc),RECT_HEIGHT(rc),m_hWnd,0,WS_POPUP);      
	int nID = ppm.DoModal();
	switch(nID)
	{
	case IDC_PPM_ADD:
		{
			AddNoteItem();
			break;
		}
	case IDC_PPM_OPEN:
		{
			OpenFile();
			break;
		}
	case IDC_PPM_DEL:
		{
			DeleteNoteItem();
			break;
		}
	case IDC_PPM_CANCEL:
		{
			//
			break;
		}
	}	
}


//处理工具栏菜单命令
void CM8NoteMainWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR uid = LOWORD(wParam);//获取控件的ID
	switch(uid)
	{
	case M8_UID_NOTE_TOOLBAR:
		{
			int iIndex = lParam;
			//文件
			if (iIndex == 0)
			{
				PopupFileMenu();
				break;
			}
			if (iIndex == 1)
			{
				bool bSelected = m_NoteList.GetSelectMode();
				m_NoteList.SetSelectMode(!bSelected);
				if (bSelected)
					m_Toolbar.SetButton(1, true, true, L"选择");
				else
					m_Toolbar.SetButton(1, true, true, L"完成");
				
				m_Toolbar.Invalidate();
				break;
			}
			//退出
			if (iIndex == 2)
			{
				PostQuitMessage(0);//退出整个程序
				break;
			}
		}
	}
}

MZ_IMPLEMENT_DYNAMIC(CM8NoteMainWnd)
