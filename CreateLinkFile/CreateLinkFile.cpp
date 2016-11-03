// CreateLinkFile.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <Windows.h>
#include <shlobj.h>
#include <string>
#include <tchar.h>

using namespace std;

extern "C"  _declspec(dllexport) bool __stdcall CreateLinkFile(
	LPCTSTR szStartAppPath,
	LPCTSTR szAddCmdLine,
	LPCTSTR szDestLnkPath,
	LPCTSTR szIconPath
);


// szStartAppPath : ����������ĳ���
// szAddCmdLine : ����main������lpCmdLine
// szDestLnkPath : ��ݷ�ʽ�ı���·��
// szIconPath : ��ݷ�ʽ��ʾ��ͼ��
#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif




bool __stdcall CreateLinkFile(
	LPCTSTR szStartAppPath,
	LPCTSTR szAddCmdLine,
	LPCTSTR szDestLnkPath,
	LPCTSTR szIconPath)
{
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink *pShellLink;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (SUCCEEDED(hr))
		{
			pShellLink->SetPath(szStartAppPath);
			tstring strTmp = szStartAppPath;
			int nStart = strTmp.find_last_of(_T("/\\"));
			pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
			pShellLink->SetArguments(szAddCmdLine);
			if (szIconPath)
			{
				pShellLink->SetIconLocation(szIconPath, 0);
			}
			IPersistFile* pPersistFile;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
			if (SUCCEEDED(hr))
			{
				hr = pPersistFile->Save(szDestLnkPath, FALSE);
				if (SUCCEEDED(hr))
				{
					return true;
				}
				pPersistFile->Release();
			}
			pShellLink->Release();
		}
		CoUninitialize();
	}
	return false;
}