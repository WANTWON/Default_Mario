#pragma once

#include "Item.h"

class CFlower : public CItem
{
public:
	CFlower();
	virtual ~CFlower();

	void Initialize() override;
	void Release() override;

	// �����Ӹ��� ����Ǵ� �Լ� 
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	void Animate() override;
};

