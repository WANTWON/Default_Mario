#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}


CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR * pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitMap = (HBITMAP)LoadImage(NULL,		// ���α׷� �ν��Ͻ� �ڵ�, �̹����� ���α׷����� ������ ���� �ƴ϶� ���Ϸκ��� �о� ���̱� ������ NULL��
		pFilePath,  // ���� ���
		IMAGE_BITMAP, // � Ÿ���� ���� ���ΰ�
		0,			// ����, ���� ������
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
}

void CMyBmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
