/************************************************************************/
/*
 * ��Ȩ���� �����̳ (http://www.mini188.com)
 *
 * �ļ��Ի���
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

	//����Ĭ��Ŀ¼
	void SetDefaultDir(TCHAR *defalutDir);
	//�����ļ���׺����
	//��ʽ:L"*.wma;*.mp3;*.avi;",��������ô˺�������Ĭ��ΪĿ¼�¿ɼ��������ļ�
	void SetExtFilter(TCHAR *extfilter);
	//ѡ��һ���ļ�
	bool SelectFile(TCHAR **filename);
};
