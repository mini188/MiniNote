#include "MiniUiList.h"
#include "M8NotePubDefine.h"
#include <string>

MiniUiList::MiniUiList(void)
{
	m_SelectMode = false;
}

MiniUiList::~MiniUiList(void)
{
}

void MiniUiList::DrawItem(HDC hdcDst, int nIndex, RECT* prcItem, RECT *prcWin, RECT *prcUpdate)
{
	ListItem *pItem = GetItem(nIndex);
	if (pItem==0) return;
	if (pItem->Data==0) return;

	NoteItem *itemData = (NoteItem*)pItem->Data;

	//画背景
	if (GetSelectedIndex() == nIndex)
      MzDrawSelectedBg(hdcDst, prcItem);

	//画标题
	RECT rectTitle = *prcItem;
	rectTitle.top = prcItem->top + 3;
	rectTitle.left = 20;
    rectTitle.bottom = rectTitle.top + RECT_HEIGHT(rectTitle)/2;
	::SetTextColor(hdcDst, RGB(0,200,0));
	SelectObject(hdcDst, FontHelper::GetFont(25));
	std::wstring strTitle = L"日志：";
	strTitle = strTitle + itemData->cTitle.C_Str();
	MzDrawText(hdcDst, strTitle.c_str(), &rectTitle, DT_LEFT|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);

	//画日期
	RECT rectDate = *prcItem;
	rectDate.left = 20;
	rectDate.top = rectTitle.bottom;
	rectDate.bottom = prcItem->bottom - 3;
	::SetTextColor(hdcDst, RGB(200,200,200));
	SelectObject(hdcDst, FontHelper::GetFont(18));
	std::wstring strDate = L" 日期：";
	strDate = strDate + itemData->cDateTime.C_Str(); 
	MzDrawText(hdcDst, strDate.c_str(), &rectDate, DT_LEFT|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);
	
	if (m_SelectMode)
	{
		// 画选择按钮
		RECT rcSelectedIcon = *prcItem;
		rcSelectedIcon.left = rcSelectedIcon.right - 50;
		bool bSelected = itemData->bSelected;
		MzDrawControl(hdcDst, &rcSelectedIcon,  bSelected?MZC_SELECTED:MZC_UNSELECTED, 0);
	}
}

void MiniUiList::SelectItem(int nIndex, bool bSelect)
{
	ListItem *pItem = GetItem(nIndex);
	if (pItem == 0) return;
	if (pItem->Data == 0) return;

	NoteItem *noteData = (NoteItem*)pItem->Data;
	noteData->bSelected = bSelect;
}

int MiniUiList::OnLButtonUp(UINT fwKeys, int xPos, int yPos)
{
    // get some mouse properties before calling the UiLst::OnLButtonUp()
    bool b1 = IsMouseDownAtScrolling();
    bool b2 = IsMouseMoved();

    int Ret = UiList::OnLButtonUp(fwKeys, xPos, yPos);

    if((!b1) && (!b2))
    {
      // get the item index of the mouse up position
      int nIndex = CalcIndexOfPos(xPos, yPos);
      if(nIndex>=0)
      {
        bool bSelect = IsSelected(nIndex);

        // change the selected status of item
        SelectItem(nIndex, !bSelect);

        // update the item
        InvalidateItem(nIndex);
        Update();
      }
    }
    return Ret;
}

bool MiniUiList::IsSelected(int nIndex)
{
	ListItem *pItem = GetItem(nIndex);
	if (pItem == 0) return false;
	if (pItem->Data == 0) return false;

	NoteItem *noteData = (NoteItem*)pItem->Data;
	return noteData->bSelected;
}

void MiniUiList::SetSelectMode(bool bSelectMode)
{
	m_SelectMode = bSelectMode;
	this->Invalidate();
}

bool MiniUiList::GetSelectMode()
{
	return m_SelectMode;
}