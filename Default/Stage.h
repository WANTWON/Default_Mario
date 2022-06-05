#pragma once
#include "Include.h"
class CStage
{
public:
	CStage() : m_dwTime(GetTickCount()), m_bClear(false), m_dwView(GetTickCount()), m_bView(false), m_iCount(3) {}
	virtual ~CStage() {}

	virtual void Initialize(void) PURE;
	virtual void Release(void) PURE;

	// �����Ӹ��� ����Ǵ� �Լ� 
	virtual int Update(void) PURE;
	virtual void Late_Update(void) PURE;
	virtual void Render(HDC hDC) PURE;

	// ���Ϳ� ����
	void Set_Clear_true(void) { m_bClear = true; }
	void Set_View(void) { m_bView = true; }

protected:
	DWORD  m_dwTime;
	bool m_bClear;
	DWORD m_dwView;
	bool m_bView;
	int m_iCount;
};

