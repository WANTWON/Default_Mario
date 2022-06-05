#pragma once
#include "Struct.h"
#include "Define.h"
#include "Function.h"
#include "Enum.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

	virtual void Initialize() PURE;
	virtual void Release() PURE;

	// �����Ӹ��� ����Ǵ� �Լ� 
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;

	void Update_Rect();

	// ���Ϳ� ����
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const DIRECTION& Get_Dir() const { return m_eDir; }
	const float& Get_Speed() const { return m_fSpeed; }
	void Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	void Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	
	void Set_PosX(float _fX) { m_tInfo.fX += _fX; }
	void Set_Angle(int angle) { m_fAngle = float(angle); }
	void Set_Rect(float _left, float _top, float _right, float _bottom) { m_tRect.left = int(_left); m_tRect.top = int(_top); m_tRect.right = int(_right); m_tRect.bottom = int(_bottom); }
	void Set_Dir(DIRECTION _eDir) { m_eDir = _eDir; }
	void Set_Dead(bool _dead);
	void Set_Target(CObj* _temp) { m_pTarget = _temp; }

	bool Get_bDead(void) { return m_bDead; }
	void Set_PosYTemp(float _fY) { m_tInfo.fY = _fY; }
	bool Get_Bye(void) { return m_bBye; }
	void Set_Bye(void) { m_bBye = false; }

protected:
	INFO m_tInfo;
	RECT m_tRect;
	float m_fSpeed;
	float m_fAngle;
	DIRECTION m_eDir;
	CObj* m_pTarget;
	bool m_bDead;
	bool m_bBye;
	bool m_bDead_Count;
	int m_iPoints_Given;
	int m_iCoins_Given;
};

