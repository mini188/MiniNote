/************************************************************************/
/*
 * 版权所有 点滴论坛 (http://www.mini188.com)
 *
 * 文件对话框
 * Author:		5207
 * Create:	    2009-03-08
 */
/************************************************************************/
#include <mzfc_inc.h>

class CMiniFileDialog :	public CMzWndEx
{
private:
	CMzString m_DefaultDir;
	CMzString m_ExtFilter;

public:
	CMiniFileDialog(void);
	~CMiniFileDialog(void);

	//设置默认目录
	void SetDefaultDir(TCHAR *defalutDir);
	//设置文件后缀过滤
	//格式:L"*.wma;*.mp3;*.avi;",如果不调用此函数，则默认为目录下可见的所有文件
	void SetExtFilter(TCHAR *extfilter);
	//选择一份文件
	bool SelectFile(TCHAR **filename);
};
