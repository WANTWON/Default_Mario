#pragma once
#include "Obj.h"
class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

	void Initialize() override;
	void Release() override;

	// �����Ӹ��� ����Ǵ� �Լ�
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	float m_fAnimSpeed;
	float m_fAnimTime;
	float m_fAnimAccel;
};

