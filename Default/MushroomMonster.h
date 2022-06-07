#pragma once
#include "Monster.h"
class CMushroomMonster :
	public CMonster
{
public:
	CMushroomMonster();
	virtual ~CMushroomMonster();

public:
	void Initialize(void);
	int Update(void);
	void Late_Update(void);
	void Release(void);
	void Render(HDC hDC);

	void Move(void);


public:
	void		Gravity(void);



private:
	DWORD   Direction_time;

};

