#pragma once
#include "Obj.h"

template <typename T>
class CAbstractFactory
{
private:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	static CObj*  Create() {
		CObj* pInstance = new T;
		pInstance->Initialize();

		return pInstance;
	}

	static CObj* Create(float _fX, float _fY, DIRECTION eDir = DIR_END)
	{
		CObj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Pos(_fX, _fY);

		if (eDir != DIR_END)
			pInstance->Set_Dir(eDir);

		return pInstance;
	}

	static CObj* Create(float _fX, float _fY, float _angle)
	{
		CObj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Pos(_fX, _fY);
		pInstance->Set_Angle(_angle);

		return pInstance;
	}
	static CObj* Create(float _left, float _top, float _right, float _bottom, DIRECTION eDir = DIR_END)
	{
		CObj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Rect(_left, _top, _right, _bottom);
		if (eDir != DIR_END)
			pInstance->Set_Dir(eDir);

		return pInstance;
	}

	// ������ ���丮
	static CObj* Create(float _fX, float _fY, ITEM_TYPE _type)
	{
		CObj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Pos(_fX, _fY);
		static_cast<CItem*>(pInstance)->Set_Type(_type);
	
		return pInstance;
	}
};

