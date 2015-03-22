/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 公共定义单元
 * Author:		5207
 * Create:	    2009-03-08
 */
/************************************************************************/
#pragma once
#include <mzfc_inc.h>
#include <vector>
using namespace std;


//编辑类型    新建   修改    删除      查看    其他    
enum EditType{etAdd, etEdit, etDelete, etView, etOther};
//字符编码类型    
enum TextTypeCode{ttcAnsi, ttcUnicode, ttcUnicodeBigEndian, ttcUtf8};

struct NoteItem
{
  int id;//序号
  CMzString cTitle;//文件名
  CMzString cFileName;//文件完整目录
  CMzString cDateTime;//日期
  CMzString cNoteContext;//日记内容
  bool bSelected; //选择状态
};

typedef vector<NoteItem> NoteItems;//日志列表

#define DATA_PATH L"Data\\"
#define DEFAULT_EXT L"note"//默认的文件后缀