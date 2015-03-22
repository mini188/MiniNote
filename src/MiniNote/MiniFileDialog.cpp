#include "stdafx.h"
#include "MiniFileDialog.h"

#include <InitGuid.h>
#include <IMzUnknown.h>
#include <IMzUnknown_IID.h>
#include <IFileBrowser.h>
#include <IFileBrowser_GUID.h>
#include "CPubFunction.h"


CMiniFileDialog::CMiniFileDialog(void)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	m_DefaultDir = NULL;
	m_ExtFilter = L"*.*;";
}

CMiniFileDialog::~CMiniFileDialog(void)
{
	CoUninitialize();
}


void CMiniFileDialog::SetDefaultDir(TCHAR *defaultDir)
{
	m_DefaultDir = defaultDir;
}

void CMiniFileDialog::SetExtFilter(TCHAR *extfilter)
{
	m_ExtFilter = extfilter;
}

bool CMiniFileDialog::SelectFile(TCHAR **filename)
{
	IFileBrowser *pFileBrow;
	IMzSelect *pSelect = NULL;
	if ( SUCCEEDED( CoCreateInstance( CLSID_FileBrowser, NULL,CLSCTX_INPROC_SERVER ,IID_MZ_FileBrowser,(void **)&pFileBrow ) ) )
	{     
		if( SUCCEEDED( pFileBrow->QueryInterface( IID_MZ_Select, (void**)&pSelect ) ) )
		{
			TCHAR file[ MAX_PATH ] = { 0 };
			pFileBrow->SetParentWnd( m_hWnd );
			if (!m_DefaultDir.IsEmpty())
				pFileBrow->SetOpenDirectoryPath( m_DefaultDir );

            pFileBrow->SetExtFilter( m_ExtFilter );//���˺�׺

			pFileBrow->SetOpenDocumentType(DOCUMENT_SELECT_SINGLE_FILE); //Ӧ�ø�����������ĵ��򿪷�ʽ������
			if( pSelect->Invoke() ) 
			{
				//��Ӧ�ø����Լ������ȡ�ĵ��ķ���ֵ						
				_tcscpy( file, pFileBrow->GetSelectedFileName());	
				*filename = file;
			}
			pSelect->Release();
		}     
		pFileBrow->Release();
	}
	else
		return false;

	::InvalidateRect( m_hWnd, NULL, FALSE );
	::UpdateWindow( m_hWnd );

	return FileExists(*filename);
}

