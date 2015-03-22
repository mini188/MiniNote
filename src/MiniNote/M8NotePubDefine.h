/************************************************************************/
/*
 * ��Ȩ���� �����̳ (http://www.mini188.com)
 *
 * �������嵥Ԫ
 * Author:		5207
 * Create:	    2009-03-08
 */
/************************************************************************/
#pragma once
#include <mzfc_inc.h>
#include <vector>
using namespace std;


//�༭����    �½�   �޸�    ɾ��      �鿴    ����    
enum EditType{etAdd, etEdit, etDelete, etView, etOther};
//�ַ���������    
enum TextTypeCode{ttcAnsi, ttcUnicode, ttcUnicodeBigEndian, ttcUtf8};

struct NoteItem
{
  int id;//���
  CMzString cTitle;//�ļ���
  CMzString cFileName;//�ļ�����Ŀ¼
  CMzString cDateTime;//����
  CMzString cNoteContext;//�ռ�����
  bool bSelected; //ѡ��״̬
};

typedef vector<NoteItem> NoteItems;//��־�б�

#define DATA_PATH L"Data\\"
#define DEFAULT_EXT L"note"//Ĭ�ϵ��ļ���׺