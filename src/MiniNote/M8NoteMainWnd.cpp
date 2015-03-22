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
		//��ȡ����	
		if (!IniReadString(L"Header", L"Title", &sTmp, strFileName))
			return false;
		theItem.cTitle = sTmp;
		//GetFileTime(
		//��ȡ����
		if (!IniReadString(L"Header", L"DateTime", &sTmp, strFileName))
			return false;
		theItem.cDateTime = sTmp;
		//�ļ�Ŀ¼
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

//�����ϰ��INI���ݵ��µ�Txt��ʽ
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
			//����'.''..'
			if (FindFileData.cFileName[0] == '.')
				continue;

			//�ҵ�һ���ļ�
			filename = tmpDataDir + FindFileData.cFileName;
			if (tmpReader.OpenFile(filename) == false)
			{
				continue;
			}
			str = tmpReader.GetText();//��ȡ�ļ�����
			//�����ļ����������INI��Ϣ���������ݼ��ļ���
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
					//��ʽ���µ��ļ���
					LPTSTR szBuffer=new TCHAR[1024];
					wsprintf(szBuffer, L"%s\\%s.%s", tmpDataDir.C_Str(), sTmp, L"txt");//�����ļ���
					if (tmpReader.SaveAsFile(szBuffer))
						return DeleteFile(filename) == TRUE;
				}
			}

		}
		while (FindNextFile(hFind, &FindFileData));
	}
	FindClose(hFind);//�رվ��
	return true;
}

void CM8NoteMainWnd::LoadNoteList(LPTSTR strDataDir)
{
	this->m_NoteList.RemoveAll();
	this->m_NoteItems.clear();
	CMzString tmpDataDir = strDataDir;
	//���Data�ļ����Ƿ���ڣ��������򴴽�
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
		//����'.''..'
		if (FindFileData.cFileName[0] == '.')
			continue;

		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//�������ļ���
		}
		else
		{
			NoteItem *noteItem = new NoteItem(); 
			//�ҵ�һ���ļ�
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

			//����UiList
			listItem.Text = noteItem->cTitle;
			listItem.Data = noteItem;
			this->m_NoteList.AddItem(listItem);
		}
	}
	while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);//�رվ��

	m_NoteList.Invalidate();
	m_NoteList.Update();
}

bool CM8NoteMainWnd::DoDeleteNoteItem(int iIndex)
{
	NoteItem nItem = m_NoteItems[iIndex];
	//ɾ���ļ�
	return DeleteFile(nItem.cFileName) == TRUE;
}

BOOL CM8NoteMainWnd::OnInitDialog()
{
    if (!CMzWndEx::OnInitDialog())
    {
      return FALSE;
    };

	int y = 0;
	//��ʼ��Caption
	y = 40;
	m_lblTitle.SetID(M8_UID_NOTE_CPATION);
	m_lblTitle.SetPos(0, 0, GetWidth(), y);
	m_lblTitle.SetText(L"��־�б�");
	AddUiWin(&m_lblTitle);

	//��ʼ��UiList
	m_NoteList.SetID(M8_UID_NOTE_LIST);
	y = GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR*2;
	m_NoteList.SetPos(0, m_lblTitle.GetHeight(), GetWidth(), y);
	m_NoteList.EnableDragScrollbarV(true);
	m_NoteList.EnableNotifyMessage(true);
	m_NoteList.SetTextSize(35);
	AddUiWin(&m_NoteList);

	//��ʼ��Toolbar
	m_Toolbar.SetID(M8_UID_NOTE_TOOLBAR);
	m_Toolbar.SetPos(0, GetHeight() - MZM_HEIGHT_TEXT_TOOLBAR, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);    
	m_Toolbar.SetButton(0, true, true, L"�ļ�");
    m_Toolbar.SetButton(1, true, true, L"ѡ��");
    m_Toolbar.SetButton(2, true, true, L"�˳�");
	AddUiWin(&m_Toolbar);

	//m_Toolbar.EnableButton(1, false);
	m_AppFilePath = L"";//�ݲ���Ҫ��������Ϊ��
    m_DataPath = GetDataPath();//��ȡDataĿ¼
	UpdateOldData(m_DataPath);//����������
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

	//����༭
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
	if (MzMessageBoxEx(m_hWnd, L"ȷ��Ҫɾ����ѡ��־��", L"����", MZ_OKCANCEL) != 1)
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
			//ˢ���б�
			if (DoDeleteNoteItem(idx))
				LoadNoteList(m_DataPath);
		}
		else
		{
			MzMessageBoxEx(m_hWnd, L"û��ѡ����־Ŷ��", L"����");
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
	pmip.str = L"ȡ��";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_DEL;
	pmip.str = L"ɾ����־";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_OPEN;
	pmip.str = L"���ļ�(txt/ini)";
	ppm.AddItem(pmip);

	pmip.itemCr = MZC_BUTTON_PELLUCID;
	pmip.itemRetID = IDC_PPM_ADD;
	pmip.str = L"�½���־";
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


//���������˵�����
void CM8NoteMainWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR uid = LOWORD(wParam);//��ȡ�ؼ���ID
	switch(uid)
	{
	case M8_UID_NOTE_TOOLBAR:
		{
			int iIndex = lParam;
			//�ļ�
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
					m_Toolbar.SetButton(1, true, true, L"ѡ��");
				else
					m_Toolbar.SetButton(1, true, true, L"���");
				
				m_Toolbar.Invalidate();
				break;
			}
			//�˳�
			if (iIndex == 2)
			{
				PostQuitMessage(0);//�˳���������
				break;
			}
		}
	}
}

MZ_IMPLEMENT_DYNAMIC(CM8NoteMainWnd)
