#include "stdafx.h"
#include "M8NoteEditWnd.h"
#include "CPubFunction.h"
#include "ReadWriteIni.h"

#define NOTE_UID_TITLE 1001
#define NOTE_UID_CONTEXT 1002
#define NOTE_UID_TOOLBAR 1003
#define NOTE_UID_SCROLL 1004

CM8NoteEditWnd::CM8NoteEditWnd(void)
{
	m_EditType = etAdd;
}

CM8NoteEditWnd::CM8NoteEditWnd(EditType etType, TCHAR *filename)
{
	m_EditType = etType;
	m_FileName = filename;
}

CM8NoteEditWnd::~CM8NoteEditWnd(void)
{
}

BOOL CM8NoteEditWnd::OnInitDialog()
{
    if (!CMzWndEx::OnInitDialog())
    {
      return FALSE;
    };

	int top = 0;
	//��ʼ��������
	m_Title.SetID(NOTE_UID_TITLE);
	m_Title.SetPos(0, 0, GetWidth(), MZM_HEIGHT_SINGLELINE_EDIT);
	m_Title.SetDrawBg(true);
	m_Title.SetTip(L"�ڴ��������");
	m_Title.SetTip2(L"����");
	m_Title.SetMaxChars(MAX_PATH);
	AddUiWin(&m_Title);
	top = m_Title.GetHeight();

	//��ʼ���༭��
	int h = GetHeight() - MZM_HEIGHT_SINGLELINE_EDIT - MZM_HEIGHT_TEXT_TOOLBAR;
	m_Context.SetID(NOTE_UID_CONTEXT);
	m_Context.SetPos(0, top, GetWidth(), h); 
	m_Context.EnableNotifyMessage(true);
//	m_Context.SetInsideScroll(true);
	m_Context.EnableGridlines(true);
	m_Context.OpenFace(true);
	m_Context.EnableScrollBarV(true);
	AddUiWin(&m_Context);
	top += m_Context.GetHeight();

	//��ʼ��Toolbar
	m_Toolbar.SetID(NOTE_UID_TOOLBAR);
	m_Toolbar.SetPos(0, top, GetWidth(), MZM_HEIGHT_TEXT_TOOLBAR);
	m_Toolbar.SetButton(1, true, true, L"�رռ���");
    m_Toolbar.SetButton(2, true, true, L"�ر�");      
	AddUiWin(&m_Toolbar);

	SetTimer(m_hWnd, 1, 1000, NULL);
	m_SipMode = IM_SIP_MODE_NONE;
	return DoLoad();
}

void CM8NoteEditWnd::OnTimer(UINT_PTR nIDEvent)
{
	//MzMessageBoxEx(m_hWnd, L"", L"");
}

BOOL CM8NoteEditWnd::DoLoad()
{
	DoChangeEditType();

	//����Ǳ༭���߲鿴����Ҫ�����ļ�����
	if ((m_EditType == etEdit) || (m_EditType == etView))
	{
		if (LoadTextFile())
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (m_EditType == etAdd)
	{
		//MzMessageBoxEx(m_hWnd, Date(), L"");
		CMzString sTitle = L"����־";
		m_Title.SetText(sTitle);
		return TRUE;
	}
}

void CM8NoteEditWnd::DoChangeEditType()
{
    //������Ķ�������Ϊֻ��
	if(m_EditType == etView)
	{
		m_Title.SetEnable(false);
		m_Context.SetReadOnly(true);
		m_Toolbar.SetButton(0, true, true, L"�༭");
		m_Toolbar.SetButton(1, false, false, L"");
	}
	else if (m_EditType == etEdit || m_EditType == etAdd)
	{
		m_Title.SetEnable(true);
		m_Context.SetReadOnly(false);
		m_Context.SetFocus(true);
		m_Toolbar.SetButton(0, true, true, L"����");
		m_Toolbar.SetButton(1, true, true, L"�رռ���");
	}

	m_Toolbar.Invalidate();
	m_Toolbar.Update();
}

bool CM8NoteEditWnd::LoadTextFile()
{
	if (m_FileReader.OpenFile(m_FileName))
	{
		TCHAR *tmpfile;
		tmpfile = ExtractFileName(m_FileName.C_Str());
		m_Title.SetText(tmpfile);//��ȡ�ļ���
		m_Context.SetText(m_FileReader.GetText());//��ȡ�ļ�����
		return true;
	}
	else
		return false;
}

bool CM8NoteEditWnd::SaveTextFile()
{
	if (m_Title.GetText().IsEmpty())
	{
		MzMessageBoxEx(m_hWnd, L"���ⲻ��Ϊ��", L"����");
		m_Title.SetFocus(true);
		return false;
	}
	else if (m_Context.GetText().IsEmpty())
	{
		MzMessageBoxEx(m_hWnd, L"���ݲ���Ϊ��", L"����");
		m_Context.SetFocus(true);
		return false;
	}

	LPTSTR newfile = new TCHAR[MAX_PATH];
	//׼������
	if (m_EditType == etAdd)
	{
		m_FileReader.SetTextCode(ttcUnicode);
		wsprintf(newfile, L"%s\\%s.%s", GetDataPath().C_Str(), m_Title.GetText().C_Str(), L"txt");//�����ļ���
		m_FileName = newfile;
		if (FileExists(m_FileName))
		{
			MzMessageBoxEx(m_hWnd, L"Ŷ���Ѿ�����ͬ���ļ����ˣ���������ɣ�", L"����");
			return false;
		}
	}
	else if (m_EditType == etEdit)
	{
		//����ļ������޸�
		TCHAR *newfilename = m_Title.GetText().C_Str();
		TCHAR *oldfilename = ExtractFileName(m_FileName.C_Str());
		if (wcscmp(newfilename, oldfilename) != 0)
		{
			TCHAR *ext = ExtractFileExt(m_FileName.C_Str());
			wsprintf(newfile, L"%s\\%s%s", GetDataPath().C_Str(), newfilename, ext);//�µ��ļ���;
			if (FileExists(newfile))
			{
				MzMessageBoxEx(m_hWnd, L"Ŷ���Ѿ�����ͬ���ļ����ˣ���������ɣ�", L"����");
				return false;
			}
			else if (MoveFile(m_FileName.C_Str(), newfile) == TRUE)
			{
				m_FileName = newfile;
			}
			else
				return false;
		}
	}
	//����ַ�������
	wmemset(newfile, 0, wcslen(newfile));
	//�����ļ�
	return m_FileReader.SaveFile(m_FileName, m_Context.GetText());
}

//����
bool CM8NoteEditWnd::SaveNoteItem()
{
	CMzString newFileName;
	TCHAR szPath[MAX_PATH] = L"";
	if (m_EditType == etAdd)
	{
		if (!GetCurrentPath(szPath))
			return false;

		TCHAR *szGuid;
		GetGuidString(&szGuid);
		CMzString mzGuid = szGuid;

		newFileName = szPath;
		newFileName = newFileName + L"Data\\";
		newFileName = newFileName + mzGuid;
		newFileName = newFileName + L".INI";
		//�½�Ini�ļ�
		if (!IniCreateFile(newFileName))
			return false;
	}
	else if (m_EditType == etEdit)
	{
		//newFileName = m_NoteItem.cFileName;	
	}

	if (!FileExists(newFileName))
	{
		return false;
	}
	else
	{
		//д����⡢ʱ�䡢����
		IniWriteString(L"Header", L"Title", m_Title.GetText(), newFileName);
		IniWriteString(L"Header", L"DateTime", Now(), newFileName);
		IniWriteString(L"NoteMain", L"Context", m_Context.GetText(), newFileName);
		return true;
	}
}


LRESULT CM8NoteEditWnd::MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case MZ_WM_MOUSE_NOTIFY:
		{
			int nID = LOWORD(wParam);
			int nNotify = HIWORD(wParam);
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (nID==NOTE_UID_CONTEXT && nNotify==MZ_MN_LBUTTONDOWN)
			{
				//m_Context.ScrollStart(10);
			}
			if (nID==NOTE_UID_CONTEXT && nNotify==MZ_MN_MOUSEMOVE)
			{	
				//m_Context.ScrollTo(UI_SCROLLTO_POS, m_Context.getCurPos(), false);
			}
			if (nID==NOTE_UID_CONTEXT && nNotify== MZ_MN_LBUTTONUP)
			{
				if (!m_Context.IsMouseMoved() && !m_Context.IsMouseDownAtScrolling() && (m_EditType != etView))
				{
					ChangeSipMode(IM_SIP_MODE_GEL_PY);
				}
			}
		}
		break;
	}
	return CMzWndEx::MzDefWndProc(message, wParam, lParam);
}

void CM8NoteEditWnd::OnMzCommand(WPARAM wParam, LPARAM lParam)
{
	UINT_PTR uid = LOWORD(wParam);
	switch(uid)
	{
	case NOTE_UID_TOOLBAR:
		{
			int iIndex = lParam;
			if (iIndex == 0)
			{
				if ((m_EditType == etAdd) || (m_EditType == etEdit))
				{
					if (SaveTextFile())
					{
						if (m_EditType == etAdd)
							m_EditType = etEdit;
						//�رռ���
						ChangeSipMode(IM_SIP_MODE_NONE);
					}
				}
				else if (m_EditType == etView)
				{
					m_EditType = etEdit;
					DoChangeEditType();
					return;
				}
			}
			else if (iIndex == 1)
			{
				if (m_Toolbar.GetButtonText(iIndex) == L"�رռ���")
				{										
					ChangeSipMode(IM_SIP_MODE_NONE);
				}
				else if (m_Toolbar.GetButtonText(iIndex) == L"��������")
				{
					ChangeSipMode(IM_SIP_MODE_GEL_PY);
				}
			}
			else if (iIndex == 2)
			{
				EndModal(ID_OK);
			}
			break;
		}
	}
}

void CM8NoteEditWnd::ChangeEditState()
{
	int y = GetHeight();
	if (m_SipMode == IM_SIP_MODE_NONE)
	{
		m_Context.SetPos(0
			           , MZM_HEIGHT_SINGLELINE_EDIT
					   , GetWidth()
					   , GetHeight() - MZM_HEIGHT_SINGLELINE_EDIT - MZM_HEIGHT_TEXT_TOOLBAR); 
	}
	else
	{
		m_Context.SetPos(0
			           , MZM_HEIGHT_SINGLELINE_EDIT
			           , GetWidth()
					   , 252 - MZM_HEIGHT_SINGLELINE_EDIT);
	}
	m_Context.SwitchSipMode(m_SipMode);
	Invalidate();
    UpdateWindow();
}

void CM8NoteEditWnd::ChangeSipMode(IM_SIP_MODE sipmode)
{
	if (m_SipMode == sipmode) return;//�����ͬ��������

	m_SipMode = sipmode;

	if (m_SipMode == IM_SIP_MODE_NONE)
	{										
		m_Toolbar.SetButtonText(1, L"��������");
	}
	else if (m_SipMode == IM_SIP_MODE_GEL_PY)
	{
		m_Toolbar.SetButtonText(1, L"�رռ���");
	}

	ChangeEditState();
	m_Toolbar.Invalidate();
	m_Toolbar.Update();
}


MZ_IMPLEMENT_DYNAMIC(CM8NoteEditWnd)
