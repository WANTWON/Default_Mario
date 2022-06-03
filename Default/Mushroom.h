#pragma once
#include "Item.h"
class CMushroom :
	public CItem
{
public:
	CMushroom();
	virtual ~CMushroom();

	void Initialize() override;
	void Release() override;

	// �����Ӹ��� ����Ǵ� �Լ� 
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	void Animate() override;
};

