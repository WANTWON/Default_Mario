#include "stdafx.h"
#include "CollisionMgr.h"
#include "Item.h"
#include "Monster.h";
#include "Player.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

RECT rc = {};

int CCollisionMgr::Collision_Rect(list<CObj*> Sour, list<CObj*> Dest)
{
	int iScore = 0;
	for (auto& iter = Sour.begin(); iter != Sour.end(); )
	{
		int iter_count = 0;
		for (auto& Monster_iter = Dest.begin(); Monster_iter != Dest.end(); )
		{
			if ((Dest.size() != 0) && (IntersectRect(&rc, &((*iter)->Get_Rect()), &((*Monster_iter)->Get_Rect()))))
			{
				(*iter)->Set_Dead(true);
				(*Monster_iter)->Set_Dead(true);
				iScore += rand() % 10 + 5;
			}
			Monster_iter++;
		}
		iter++;
	}
	return iScore;
}



void CCollisionMgr::Collision_Rect_Ex(list<CObj*> _Sour, list<CObj*> _Dest)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (Check_Rect(Dest, Sour, &fWidth, &fHeight))
			{
				if (fWidth > fHeight)  //���� �浹
				{
					if (Dest->Get_Info().fY > Sour->Get_Info().fY)
					{
						Sour->Set_PosY(-fHeight);
					}
					else
					{
						Sour->Set_PosY(fHeight);
					}

				}
				else //�¿� �浹 
				{
					if (Dest->Get_Info().fX > Sour->Get_Info().fX)
					{
						Sour->Set_PosX(-fWidth);
					}
					else
					{
						Sour->Set_PosX(fWidth);
					}
				}

			}

		}
	}
}

bool CCollisionMgr::Collision_Block_Ex(float _fX, float *_fY, CObj* player  ,list<CObj*> _Dest)
{
	if (_Dest.empty())
		return false;

	CObj* pTarget = nullptr;

	for (auto& iter : _Dest)
	{
		if (_fX >= iter->Get_Rect().left && _fX < iter->Get_Rect().right && player->Get_Rect().bottom < iter->Get_Rect().top  )
			pTarget = iter;

	}

	if (!pTarget)
		return false;

	// ������ ������ 
	// Y - y1 = ((y2 - y1) / (x2 - x1)) * (X - x1)
	// Y = ((y2 - y1) / (x2 - x1)) * (X - x1) + y1

	float x1 = pTarget->Get_Rect().left;
	float y1 = pTarget->Get_Rect().top;

	float x2 = pTarget->Get_Rect().right;
	float y2 = pTarget->Get_Rect().top;

	*_fY = ((y2 - y1) / (x2 - x1))*(_fX - x1) + y1;

	return true;
}


void CCollisionMgr::Collision_Block(list<CObj*> _Sour, list<CObj*> _Dest)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (Check_Rect(Dest, Sour, &fWidth, &fHeight))
			{
				if (fWidth > fHeight)  //���� �浹
				{
					if (Dest->Get_Info().fY >= Sour->Get_Info().fY)
					{
							Sour->Set_PosY(-fHeight);
							dynamic_cast<CPlayer*>(Sour)->Set_FixPoint((float)Dest->Get_Rect().top);
							dynamic_cast<CPlayer*>(Sour)->Set_StepBlock(true);
						
					}
					else
					{
						Sour->Set_PosY(fHeight);
						dynamic_cast<CPlayer*>(Sour)->Set_StepBlock(false);
					}

				}
				else //�¿� �浹 
				{
					if (Dest->Get_Info().fX > Sour->Get_Info().fX)
					{
						Sour->Set_PosX(-fWidth);
						dynamic_cast<CPlayer*>(Sour)->Set_StepBlock(false);
					}
					else
					{
						Sour->Set_PosX(fWidth);
						dynamic_cast<CPlayer*>(Sour)->Set_StepBlock(false);
					}
					
				}

			}

		}
	}
}


int CCollisionMgr::Check_Rect(CObj* Sour, CObj* Dest, float* _pX, float* _pY)
{
	float fWidth = abs(Sour->Get_Info().fX - Dest->Get_Info().fX);
	float fHeight = abs(Sour->Get_Info().fY - Dest->Get_Info().fY);

	float fRadiusX = (Sour->Get_Info().fCX + Dest->Get_Info().fCX)*0.5f;
	float fRadiusY = (Sour->Get_Info().fCY + Dest->Get_Info().fCY)*0.5f;

	if ((fWidth <= fRadiusX) && (fHeight <= fRadiusY))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;
		return true;
	}
	else
		return false;
}


void CCollisionMgr::Step_on_Mushroom(list<CObj*> _Sour, list<CObj*> _Dest)
{

	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (Check_Rect(Dest, Sour, &fWidth, &fHeight))
			{
				if (fWidth > fHeight)  //���� �浹
				{
					if (Dest->Get_Info().fY >= Sour->Get_Info().fY)
					{
						dynamic_cast<CMonster*>(Dest)->Be_Attacked();
						dynamic_cast<CPlayer*>(Sour)->Set_bJump(true);
					}
					else
					{
						Sour->Set_PosY(fHeight);
					}
					
				}
				else //�¿� �浹 
				{
					if (Dest->Get_Info().fX > Sour->Get_Info().fX)
					{
						Sour->Set_PosX(-fWidth);
					}
					else
					{
						Sour->Set_PosX(fWidth);
					}
				}
			}
		}
	}
}


int CCollisionMgr::Collision_Sphere(list<CObj*> Sour, list<CObj*> Dest)
{
	int iScore = 0;

	for (auto& Sour_iter = Sour.begin(); Sour_iter != Sour.end(); )
	{
		int iter_count = 0;
		for (auto& Dest_iter = Dest.begin(); Dest_iter != Dest.end(); )
		{
			if ((Dest.size() != 0) && (ChecK_Sphere(*Sour_iter, *Dest_iter)))
			{
				(*Sour_iter)->Set_Dead(true);
				(*Dest_iter)->Set_Dead(true);
				iScore += rand() % 10 + 5;

			}
			Dest_iter++;
		}
		Sour_iter++;
	}
	return iScore;
}

int CCollisionMgr::Collision_Coin(CObj* Player, list<CObj*> Items)
{
	int iScore = 0;

	for (auto& item : Items )
	{
		if (IntersectRect(&rc, &Player->Get_Rect(), &item->Get_Rect()))
		{
			CItem* _Item = dynamic_cast<CItem*>(item);

			if (_Item->Get_Type() == ITEM_COIN)
			{
				iScore += 1;
				_Item->Set_Dead(true);
			}
		}
	}

	return iScore;
}

bool CCollisionMgr::ChecK_Sphere(CObj* Sour, CObj* Dest)
{
	float fRadius = (Sour->Get_Info().fCX + Dest->Get_Info().fCX) * 0.5f; //�� ���� �������� ���� ��

																		  //���밪�� �����ִ� �Լ�
	float fWidth = fabs(Sour->Get_Info().fX - Dest->Get_Info().fX);
	float fHeight = fabs(Sour->Get_Info().fY - Dest->Get_Info().fY);

	float fDistance = sqrtf((fWidth* fWidth) + (fHeight*fHeight));

	if (fRadius >= fDistance)
		return true;
	else
		return false;
}