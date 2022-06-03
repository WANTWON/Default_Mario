#include "stdafx.h"
#include "BlockMgr.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
{
}


CBlockMgr::~CBlockMgr()
{
}

void CBlockMgr::Initialize(void)
{
	for (auto& iter : m_Blocklist)
		iter->Initialize();
}

void CBlockMgr::Update(void)
{
	for (auto& iter : m_Blocklist)
		iter->Update();
}

void CBlockMgr::Late_Update(void)
{
	for (auto& iter : m_Blocklist)
		iter->Late_Update();
}

void CBlockMgr::Render(HDC hDC)
{
	for (auto& iter : m_Blocklist)
		iter->Render(hDC);
}

void CBlockMgr::Release(void)
{
	
		for (auto& iter = m_Blocklist.begin(); iter != m_Blocklist.end(); )
		{
			Safe_Delete<CObj*>(*iter);
			iter = m_Blocklist.erase(iter);
		}
		m_Blocklist.clear();
	
}

void CBlockMgr::Add_Object(OBJ_LIST _ID, CObj* pObj)
{
	if (_ID == OBJ_END || nullptr == pObj)
		return;

	m_Blocklist.push_back(pObj);
}

bool CBlockMgr::CollisionBlock(RECT Player, float _fX, float * Change_fY)
{
	if (m_Blocklist.empty())
		return false;

	CObj* pTarget = nullptr;

	for (auto& iter : m_Blocklist)
	{
		if ( _fX >= iter->Get_Rect().left && _fX < iter->Get_Rect().right 
			&& Player.bottom < iter->Get_Rect().top+2 && Player.bottom > iter->Get_Rect().top-2)
			// �÷��̾��� X��(����)�� ������ ���ʰ� ������ ���̿� �ְ�, �÷��̾��� Bottom�� ������ Top���� Ŭ ��
			// (�̶�, �÷��̾�� ���ڶ��� ���� �Ÿ��� �ʹ� �����Ӱ� �ָ� �ٷ� Jump�� falseó�� �Ǿ� �����̵� �� �� �־ 2�� ���������� �߰���)
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

	*Change_fY = ((y2 - y1) / (x2 - x1))*(_fX - x1) + y1;

	return true;

}



bool CBlockMgr::Collision_with_Direction(CObj* Player)
{
	if (m_Blocklist.empty())
		return false;

	CObj* pTarget = nullptr;

	for (auto& iter : m_Blocklist)
	{
		if (Player->Get_Info().fX >= iter->Get_Rect().left && Player->Get_Info().fX < iter->Get_Rect().right)
			pTarget = iter;

	}

	if (!pTarget)
		return false;


	float fWidth = 0.f;
	float fHeight = 0.f;

	if (Check_Rect(Player->Get_Info(), pTarget->Get_Info(), &fWidth, &fHeight))
	{
		if (fWidth > fHeight)  //���� �浹
		{
			if (pTarget->Get_Info().fY >= Player->Get_Info().fY)
				Player->Set_PosY(-fHeight);
			else
				Player->Set_PosY(fHeight);
		}
		else //�¿� �浹 
		{
			if (pTarget->Get_Info().fX > Player->Get_Info().fX)
				Player->Set_PosX(-fWidth);
			else
				Player->Set_PosX(fWidth);
		}

	}

}


int CBlockMgr::Check_Rect(INFO Player, INFO pTarget, float* _pX, float* _pY )
{
	


	float fWidth = abs(Player.fX - pTarget.fX);
	float fHeight = abs(Player.fY - pTarget.fY);

	float fRadiusX = (Player.fCX + pTarget.fCX*0.5f);
	float fRadiusY = (Player.fCY + pTarget.fCY*0.5f);

	if ((fWidth <= fRadiusX) && (fHeight <= fRadiusY))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;
		return true;
	}
	else
		return false;
}