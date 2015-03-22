/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 主程序入口
 * Author:		5207
 * Create:	    2009-02-20
 */
/************************************************************************/

#include "stdafx.h"
#include <mzfc_inc.h>
#include "M8NoteMainWnd.h"


class CM8NoteApp : public CMzApp
{
public:
  // The main window of the app.
  CM8NoteMainWnd m_MainWnd;

  virtual BOOL Init()
  {
    CoInitializeEx(0, COINIT_MULTITHREADED);

    RECT rcWork = MzGetWorkArea();
    m_MainWnd.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
    m_MainWnd.SetBgColor(MzGetThemeColor(TCI_WINDOW_BG));
    m_MainWnd.Show();

	//this->EnableNeverQuit(true);
    return TRUE;
  }
};

CM8NoteApp noteApp;