#pragma once
#include "mzfc_inc.h"

class MyListItemData
{
public:
  CMzString StringTitle;  // each item's main text
  CMzString StringDescription;  // each item's description
  BOOL Selected; // the selected status of the item

};

class MiniUiList :
	public UiList
{
public:
	MiniUiList(void);
	~MiniUiList(void);
	// override the DrawItem member function to do your own drawing of the list
	virtual void DrawItem(HDC hdcDst, int nIndex, RECT* prcItem, RECT *prcWin, RECT *prcUpdate);
	virtual void SelectItem(int nIndex, bool bSelect);
    virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos);
	bool IsSelected(int nIndex);

	void SetSelectMode(bool bSelectMode);
	bool GetSelectMode();
protected:	
private:
	bool m_SelectMode;
};

//#define MZ_IDC_LIST      101
//#define MZ_IDC_TOOLBAR1    102
//#define IDC_PPM_OK      103
//#define IDC_PPM_CANCEL    104
//#define MZ_IDC_SCROLLWIN  105

//// define my own list-item-data for each item
//class MyListItemData
//{
//public:
//  CMzString StringTitle;  // each item's main text
//  CMzString StringDescription;  // each item's description
//  BOOL Selected; // the selected status of the item
//
//};
//
//// A new list control derived from UiList
//class MyList:
//  public UiList
//{
//public:
//  // delete the data pointer where an item is removed.
//  virtual void OnRemoveItem(int nIndex)
//  {
//    ListItem* pItem = GetItem(nIndex);
//    if(pItem)
//    {
//      MyListItemData* mlid = (MyListItemData*)pItem->Data;
//      if(mlid)
//          delete mlid;
//    }
//  }
//
//  // select/unselect an item
//  void MultiSelectItem(int nIndex, bool bSelect)
//  {
//    ListItem* pItem = GetItem(nIndex);
//    if(pItem)
//    {
//      MyListItemData* mlid = (MyListItemData*)pItem->Data;
//      if(mlid)
//          mlid->Selected = bSelect;
//    }
//  }
//
//  // return if an item is selected
//  bool IsMultiSelect(int nIndex)
//  {
//    ListItem* pItem = GetItem(nIndex);
//    if(pItem)
//    {
//      MyListItemData* mlid = (MyListItemData*)pItem->Data;
//      if(mlid)
//          return mlid->Selected;
//    }
//    return false;
//  }
//
//  // change the selected status of an item in OnLButtonUp
//  virtual int OnLButtonUp(UINT fwKeys, int xPos, int yPos)
//  {
//    // get some mouse properties before calling the UiLst::OnLButtonUp()
//    bool b1 = IsMouseDownAtScrolling();
//    bool b2 = IsMouseMoved();
//
//    int Ret = UiList::OnLButtonUp(fwKeys, xPos, yPos);
//
//    if((!b1) && (!b2))
//    {
//      // get the item index of the mouse up position
//      int nIndex = CalcIndexOfPos(xPos, yPos);
//      if(nIndex>=0)
//      {
//        bool bSelect = IsMultiSelect(nIndex);
//
//        // change the selected status of item
//        MultiSelectItem(nIndex, !bSelect);
//
//        // update the item
//        InvalidateItem(nIndex);
//        Update();
//      }
//    }
//    return Ret;
//  }
//
//  // override the DrawItem member function to do your own drawing of the list
//  virtual void DrawItem(HDC hdcDst, int nIndex, RECT* prcItem, RECT *prcWin, RECT *prcUpdate)
//  {
//    ListItem* pItem = GetItem(nIndex);
//    if (pItem==0)
//      return;
//    if (pItem->Data==0)
//      return;
//
//    // get MyListItemData
//    MyListItemData *pmlid = (MyListItemData*)pItem->Data;
//
//    // selected ?
//    bool bSelected = pmlid->Selected;
//
//    // draw an image on the left
//    ImagingHelper *pimg = ImagingHelper::GetImageObject(MzGetInstanceHandle(), IDR_PNG_SMS, true);
//    RECT rcImg = *prcItem;
//    rcImg.right = rcImg.left + MZM_MARGIN_MAX*2;
//    if (pimg)
//    {
//      pimg->Draw(hdcDst, &rcImg, false, false);
//    }
//
//    // draw the Title
//    RECT rcText = *prcItem;
//    rcText.left = rcImg.right;
//    rcText.bottom = rcText.top + RECT_HEIGHT(rcText)/2;
//    ::SetTextColor(hdcDst, RGB(0,200,0));
//    MzDrawText(hdcDst, pmlid->StringTitle.C_Str(), &rcText, DT_LEFT|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);
//
//    // draw the Description
//    rcText.top = rcText.bottom;
//    rcText.bottom = prcItem->bottom;
//    ::SetTextColor(hdcDst, RGB(200,200,200));
//    MzDrawText(hdcDst, pmlid->StringDescription.C_Str(), &rcText, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);
//
//    // draw the select status icon
//    RECT rcSelectedIcon = *prcItem;
//    rcSelectedIcon.left = rcSelectedIcon.right - 50;
//    MzDrawControl(hdcDst, &rcSelectedIcon,  bSelected?MZC_SELECTED:MZC_UNSELECTED, 0);
//  }
//protected:
//private:
//};
//
//// Main window derived from CMzWndEx
//class CSample1MainWnd: public CMzWndEx
//{
//  MZ_DECLARE_DYNAMIC(CSample1MainWnd);
//public:
//  UiToolbar_Text m_Toolbar;
//  
//  // the list control
//  MyList m_List;
//
//protected:
//  // Initialization of the window (dialog)
//  virtual BOOL OnInitDialog()
//  {
//    // Must all the Init of parent class first!
//    if (!CMzWndEx::OnInitDialog())
//    {
//      return FALSE;
//    }
//
//    // Then init the controls & other things in the window
//
//    m_List.SetPos(0,0,GetWidth(),GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR);
//    m_List.SetID(MZ_IDC_LIST);
//    m_List.EnableScrollBarV(true);
//    m_List.EnableNotifyMessage(true);
//    m_List.SetItemHeight(90);
//    m_List.SetTextColor(RGB(255,0,0));
//    AddUiWin(&m_List);
//
//    //add items to list
//    wchar_t *bufTitle = L"Title %d";
//    wchar_t *bufDescription = L"Description %d";
//    ListItem li;
//    for (int i=0;i<50;i++)
//    {
//      CMzString strTitle(256);
//      CMzString strDescription(256);
//      wsprintf(strTitle.C_Str(), bufTitle, i);
//      wsprintf(strDescription.C_Str(), bufDescription, i);
//
//      // set the values of the item's Data
//      MyListItemData *pmlid = new MyListItemData;
//      pmlid->StringTitle = strTitle;
//      pmlid->StringDescription = strDescription;
//      pmlid->Selected = false;
//
//      // add the item's data to the list
//      li.Data = pmlid;
//      m_List.AddItem(li);
//    }
//
//    m_Toolbar.SetPos(0,GetHeight()-MZM_HEIGHT_TEXT_TOOLBAR,GetWidth(),MZM_HEIGHT_TEXT_TOOLBAR);
//    m_Toolbar.SetButton(0, true, true, L"Exit");
//    m_Toolbar.SetButton(1, true, true, L"Delete");
//    m_Toolbar.SetButton(2, true, true, L"Setting");
//    m_Toolbar.SetID(MZ_IDC_TOOLBAR1);
//    AddUiWin(&m_Toolbar);
//
//
//    return TRUE;
//  }
//
//  // override the MZFC window messages handler
//  LRESULT MzDefWndProc(UINT message, WPARAM wParam, LPARAM lParam)
//  {
//    switch(message)
//    {
//    case MZ_WM_MOUSE_NOTIFY:
//      {
//        int nID = LOWORD(wParam);
//        int nNotify = HIWORD(wParam);
//        int x = LOWORD(lParam);
//        int y = HIWORD(lParam);
//
//        // process the mouse left button down notification
//        if (nID==MZ_IDC_LIST && nNotify==MZ_MN_LBUTTONDOWN)
//        {
//          if (!m_List.IsMouseDownAtScrolling() && !m_List.IsMouseMoved())
//          {
//            int nIndex = m_List.CalcIndexOfPos(x, y);
//            m_List.SetSelectedIndex(nIndex);
//            m_List.Invalidate();
//            m_List.Update();
//          }
//          return 0;
//        }
//
//        // process the mouse move notification
//        if (nID==MZ_IDC_LIST && nNotify==MZ_MN_MOUSEMOVE)
//        {
//          m_List.SetSelectedIndex(-1);
//          m_List.Invalidate();
//          m_List.Update();
//          return 0;
//        }
//        //if (nID==MZ_IDC_LIST && nNotify==MZ_MN_LBUTTONDOWN)
//        //{
//        //  return 0;
//        //}
//      }
//      return 0;
//    }
//    return CMzWndEx::MzDefWndProc(message,wParam,lParam);
//  }
//
//  // override the MZFC command handler
//  virtual void OnMzCommand(WPARAM wParam, LPARAM lParam)
//  {
//    UINT_PTR id = LOWORD(wParam);
//    switch(id)
//    {
//    case MZ_IDC_TOOLBAR1:
//      {
//        int nIndex = lParam;
//        if (nIndex==0)
//        {
//          //CMzString str(128);
//          //wsprintf(str.C_Str(), L"You pressed the %s button!", m_Toolbar.GetButtonText(0).C_Str());
//          //MzMessageBoxEx(m_hWnd, str.C_Str(), L"Test", MB_OK);
//          PostQuitMessage(0);
//          return;
//        }
//        if (nIndex==1)
//        {
//          // pop out a PopupMenu:
//          CPopupMenu ppm;
//          struct PopupMenuItemProp pmip;      
//
//          pmip.itemCr = MZC_BUTTON_PELLUCID;
//          pmip.itemRetID = IDC_PPM_CANCEL;
//          pmip.str = L"Cancel";
//          ppm.AddItem(pmip);
//
//          pmip.itemCr = MZC_BUTTON_ORANGE;
//          pmip.itemRetID = IDC_PPM_OK;
//          pmip.str = L"OK";
//          ppm.AddItem(pmip);  
//
//          RECT rc = MzGetWorkArea();      
//          rc.top = rc.bottom - ppm.GetHeight();
//          ppm.Create(rc.left,rc.top,RECT_WIDTH(rc),RECT_HEIGHT(rc),m_hWnd,0,WS_POPUP);      
//          int nID = ppm.DoModal();
//          if (nID==IDC_PPM_OK)
//          {
//            // delete the items selected
//            for (int i=m_List.GetItemCount()-1; i>=0; i--)
//            {
//              if(m_List.IsMultiSelect(i))
//              {
//                m_List.RemoveItem(i);
//              }
//            }
//
//            // update the list
//            m_List.Invalidate();
//            m_List.Update();
//          }
//          if (nID==IDC_PPM_CANCEL)
//          {
//            // do what you want...
//          }
//          return;
//        }
//        if (nIndex==2)
//        {
//          //...
//          return;
//        }
//      }
//      break;
//    }
//  }
//};
//
//MZ_IMPLEMENT_DYNAMIC(CSample1MainWnd)
//
//// Application class derived from CMzApp
//class CSample1App: public CMzApp
//{
//public:
//  // The main window of the app.
//  CSample1MainWnd m_MainWnd;
//
//  // Initialization of the application
//  virtual BOOL Init()
//  {
//    // Init the COM relative library.
//    CoInitializeEx(0, COINIT_MULTITHREADED);
//
//    // Create the main window
//    RECT rcWork = MzGetWorkArea();
//    m_MainWnd.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
//    m_MainWnd.SetBgColor(MzGetThemeColor(TCI_WINDOW_BG));
//    m_MainWnd.Show();
//
//    // return TRUE means init success.
//    return TRUE;
//  }
//};
//
//// The global variable of the application.
//CSample1App theApp;