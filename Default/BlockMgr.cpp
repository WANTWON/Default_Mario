#include "stdafx.h"
#include "BlockMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "NormalBlock.h"
#include <typeinfo>
#include "AbstractFactory.h"
#include "Item.h"
#include "ItemBlock.h"

//	������
#include "Coin.h"
#include "Mushroom.h"
#include "Star.h"
#include "Flower.h"
#include "CoinBlock.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
{
}


CBlockMgr::~CBlockMgr()
{
}

void CBlockMgr::Initialize(void)
{
	m_listPlayer = CObjMgr::Get_Instance()->Get_listPlayer();

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
			iter->Initialize();
	}
}

void CBlockMgr::Update(void)
{
	//	Collision with Player-Block
	static bool bPlayerFlying;

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
		{
			Col_Player(iter, CCollisionMgr::Col_ReturnDir(m_listPlayer, iter));
			if (DIR_UP == CCollisionMgr::Col_ReturnDir(m_listPlayer, iter))
				bPlayerFlying = false;
		}
	}

	if (!m_listPlayer.empty())
	{
		if (false == bPlayerFlying)
		{
			static_cast<CPlayer*>(m_listPlayer.front())->Set_Flying(false);
			bPlayerFlying = true;
		}
		else
			static_cast<CPlayer*>(m_listPlayer.front())->Set_Flying(true);
	}

	//	Update
	int iEvent = 0;
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter = m_Blocklist[i].begin(); iter != m_Blocklist[i].end(); )
		{
			iEvent = (*iter)->Update();

			if (iEvent == OBJ_DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_Blocklist[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CBlockMgr::Late_Update(void)
{
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
			iter->Late_Update();
	}
}

void CBlockMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
			iter->Render(hDC);
	}
}

void CBlockMgr::Release(void)
{

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter = m_Blocklist[i].begin(); iter != m_Blocklist[i].end(); )
		{
			Safe_Delete<CObj*>(*iter);
			iter = m_Blocklist[i].erase(iter);
		}
	}

	for (size_t i = 0; i < BLOCK_END; ++i)
		m_Blocklist[i].clear();

}

void CBlockMgr::Add_Object(BLOCK_LIST _ID, CObj* pObj)
{
	if (_ID == OBJ_END || nullptr == pObj)
		return;

	m_Blocklist[_ID].push_back(pObj);
}

bool CBlockMgr::CollisionBlock(RECT Player, float _fX, float * Change_fY)
{
	int iTemp = 0;
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		if (m_Blocklist[i].empty())
			++iTemp;

		if (BLOCK_END == iTemp)
		{
			return false;
		}
	}


	CObj* pTarget = nullptr;

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
		{
			if (Player.right >= iter->Get_Rect().left && Player.left < iter->Get_Rect().right
				&& Player.bottom < iter->Get_Rect().top + 2 && Player.bottom > iter->Get_Rect().top - 2)
				// �÷��̾��� X��(����)�� ������ ���ʰ� ������ ���̿� �ְ�, �÷��̾��� Bottom�� ������ Top���� Ŭ ��
				// (�̶�, �÷��̾�� ������ X���� �������� �ϸ� �ٷ� Jump�� falseó�� �Ǿ� �����̵� �� �� �־, Y���� �������� �ְ� 2�� ���������� �߰���)
				pTarget = iter;
		}
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

bool CBlockMgr::CollisionBlock_Ex(INFO Player, float * inputY)
{
	int iTemp = 0;
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		if (m_Blocklist[i].empty())
			++iTemp;

		if (BLOCK_END == iTemp)
		{
			return false;
		}
	}


	CObj* pTarget = nullptr;

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
		{
			if (((Player.fX >= iter->Get_Rect().left) && (Player.fX < iter->Get_Rect().right))
				&& (Player.fY < iter->Get_Rect().bottom))
			{
				if (NULL == pTarget)
					pTarget = iter;

				if (pTarget && (iter->Get_Info().fY < pTarget->Get_Info().fY))
					pTarget = iter;
			}
		}
	}

	if (!pTarget)
		return false;

	// ������ ������ 
	// Y - y1 = ((y2 - y1) / (x2 - x1)) * (X - x1)
	// Y = ((y2 - y1) / (x2 - x1)) * (X - x1) + y1


	RECT rcTemp;
	RECT rcRect{ Player.fX - Player.fCX * 0.5f,
		Player.fY - Player.fCY * 0.5f,
		Player.fX + Player.fCX * 0.5f,
		Player.fY + Player.fCY * 0.5f };

	if (IntersectRect(&rcTemp, &rcRect, &(pTarget->Get_Rect())))
	{
		float x1 = rcTemp.left;
		float y1 = rcTemp.top;

		float x2 = rcTemp.right;
		float y2 = rcTemp.bottom;

		*inputY = (y2 - y1);
	}
	return true;

}


bool CBlockMgr::Collision_with_Direction(CObj* Player)
{
	int iTemp = 0;
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		if (m_Blocklist[i].empty())
			++iTemp;

		if (BLOCK_END == iTemp)
		{
			return false;
		}
	}

	CObj* pTarget = nullptr;

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& iter : m_Blocklist[i])
		{
			if (Player->Get_Info().fX >= iter->Get_Rect().left && Player->Get_Info().fX < iter->Get_Rect().right)
				pTarget = iter;

		}
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
			//else
				//Player->Set_PosY(fHeight);
		}
		else //�¿� �浹 
		{/*
			if (pTarget->Get_Info().fX > Player->Get_Info().fX)
				Player->Set_PosX(-fWidth);
			else
				Player->Set_PosX(fWidth);*/
		}
	}
}

void CBlockMgr::Col_Player(CObj* _thisBlock, DIRECTION _eDir)
{
	float fY = 0.f;

	switch (_eDir)
	{
	case DIR_UP:
		if (0.f == static_cast<CPlayer*>(m_listPlayer.front())->Get_Time())
		{
			CollisionBlock_Ex(m_listPlayer.front()->Get_Info(), &fY);
			m_listPlayer.front()->Set_PosY(-fY);
		}
		else
		{
			static_cast<CPlayer*>(m_listPlayer.front())->Set_Flying(false);
			static_cast<CPlayer*>(m_listPlayer.front())->Set_bJump(false);

		}
		break;

	case DIR_DOWN:
		static_cast<CPlayer*>(m_listPlayer.front())->Set_GravityTime(2.3f);
		static_cast<CPlayer*>(m_listPlayer.front())->Set_Flying(true);
		static_cast<CPlayer*>(m_listPlayer.front())->Set_bJump(false);
		Check_BreakBlock(_thisBlock);
		break;

	case DIR_LEFT:
		m_listPlayer.front()->Set_PosX(-1.f * m_listPlayer.front()->Get_Speed());
		break;

	case DIR_RIGHT:
		m_listPlayer.front()->Set_PosX(m_listPlayer.front()->Get_Speed());
		break;

	default:
		break;
	}

}

void CBlockMgr::Check_BreakBlock(CObj * _thisBlock)
{
	if ((true == static_cast<CBlock*>(_thisBlock)->Get_IsItem()) && (false == static_cast<CCoinBlock*>(_thisBlock)->Get_Used()))
	{
		Create_RandItem(_thisBlock);
		_thisBlock->Set_Dead(OBJ_DEAD);
	}

	if (1 <= static_cast<CPlayer*>(m_listPlayer.front())->Get_Level())
	{
		_thisBlock->Set_Dead(OBJ_DEAD);
	}
	else
		return;
}

void CBlockMgr::Create_RandItem(CObj* _thisBlock)
{
	ITEM_TYPE iItemNum = ITEM_TYPE(rand() % ITEM_END);

	switch (iItemNum)
	{
	case ITEM_COIN:
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CCoin>::Create(_thisBlock->Get_Info().fX, _thisBlock->Get_Info().fY - (_thisBlock->Get_Info().fCY * 0.5f), iItemNum));
		break;
	case ITEM_MUSHROOM:
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CMushroom>::Create(_thisBlock->Get_Info().fX, _thisBlock->Get_Info().fY - (_thisBlock->Get_Info().fCY * 0.5f), iItemNum));
		break;
	case ITEM_STAR:
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CStar>::Create(_thisBlock->Get_Info().fX, _thisBlock->Get_Info().fY - (_thisBlock->Get_Info().fCY * 0.5f), iItemNum));
		break;
	case ITEM_FLOWER:
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CFlower>::Create(_thisBlock->Get_Info().fX, _thisBlock->Get_Info().fY - (_thisBlock->Get_Info().fCY * 0.5f), iItemNum));
		break;
	default:
		break;
	}
}

DIRECTION CBlockMgr::Col_ReturnDir_LR(INFO _tInfo)
{
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& _Dest : m_Blocklist[i])
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (Check_Rect(_Dest->Get_Info(), _tInfo, &fWidth, &fHeight))
			{
				float fX = _tInfo.fX - _Dest->Get_Info().fX;
				float fY = _tInfo.fY - _Dest->Get_Info().fY;

				float fR = sqrtf((fX * fX) + (fY * fY));

				float fAngle = (180 / PI) * acos(fX / fR);
				if (_tInfo.fY <= _Dest->Get_Info().fY)
					fAngle = 360 + (-1.f * fAngle);

				if (fAngle >= 316.f || fAngle < 44.f)
				{
					return DIR_RIGHT;
				}
				else if (fAngle > 136.f && fAngle <= 224.f)
				{
					return DIR_LEFT;
				}
			}
		}
	}

	return DIR_END;
}

DIRECTION CBlockMgr::Col_ReturnDir(INFO _tInfo)
{
	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& _Dest : m_Blocklist[i])
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (Check_Rect(_Dest->Get_Info(), _tInfo, &fWidth, &fHeight))
			{
				float fX = _tInfo.fX - _Dest->Get_Info().fX;
				float fY = _tInfo.fY - _Dest->Get_Info().fY;

				float fR = sqrtf((fX * fX) + (fY * fY));

				float fAngle = (180 / PI) * acos(fX / fR);
				if (_tInfo.fY <= _Dest->Get_Info().fY)
					fAngle = 360 + (-1.f * fAngle);

				if (fAngle >= 316.f || fAngle < 44.f)
				{
					return DIR_RIGHT;
				}
				else if (fAngle >= 44.f && fAngle <= 134.f)
				{
					return DIR_DOWN;
				}
				else if (fAngle > 136.f && fAngle <= 224.f)
				{
					return DIR_LEFT;
				}
				else if (fAngle > 224.f && fAngle < 316.f)
				{
					return DIR_UP;
				}
			}
		}
	}

	return DIR_END;
}

DIRECTION CBlockMgr::Col_ReturnDir_RecLR(INFO _tInfo)
{
	RECT rc{};
	RECT rcTemp = { _tInfo.fX - (_tInfo.fCX * 0.5f),
		_tInfo.fY - (_tInfo.fCY * 0.5f),
		_tInfo.fX + (_tInfo.fCX * 0.5f),
		_tInfo.fY + (_tInfo.fCY * 0.5f) };

	for (size_t i = 0; i < BLOCK_END; ++i)
	{
		for (auto& _Dest : m_Blocklist[i])
		{
			float fWidth = 0.f;
			float fHeight = 0.f;

			if (IntersectRect(&rc, &rcTemp, &(_Dest->Get_Rect())))
			{
				if (_tInfo.fX - (_tInfo.fCX * 0.5f) >= _Dest->Get_Info().fY)
				{
					return DIR_RIGHT;
				}
				else if (_tInfo.fX + (_tInfo.fCX * 0.5f) <= _Dest->Get_Info().fY)
				{
					return DIR_LEFT;
				}
			}
		}
	}

	return DIR_END;
}

void CBlockMgr::Save_File(void)
{
	//	Block
	HANDLE hFile = CreateFile(L"../Data/SaveTemp/ObjBlock.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save Block", L"Error", MB_OK);
		return;
	}
	DWORD dwByte = 0;
	DWORD dwTypeByte = 0;

	for (size_t i = 0; i < BLOCK_END - 1; ++i)
	{
		for (auto& iter : m_Blocklist[i])
		{
			WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &(static_cast<CBlock*>(iter)->Get_BlockType()), sizeof(int), &dwTypeByte, nullptr);
		}
	}
	CloseHandle(hFile);

#ifdef _DEBUG
	MessageBox(g_hWnd, L"Block Save ����", L"����", MB_OK);
#endif	//_DEBUG
}

void CBlockMgr::Load_File(int _iStage)
{
	HANDLE	hFile = nullptr;
	DWORD	dwByte = 0;
	DWORD	dwTypeByte = 0;
	INFO	tTemp{};
	int		iDest = 0;
	BLOCK_LIST typeTemp = BLOCK_END;

	switch (_iStage)
	{
	case 1:
		//	Block
		hFile = CreateFile(L"../Data/Save1/ObjBlock.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, L"Load Block", L"Error", MB_OK);
			return;
		}
		dwByte = 0;
		dwTypeByte = 0;
		tTemp = {};
		iDest = 0;
		typeTemp = BLOCK_END;

		while (true)
		{
			ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDest, sizeof(int), &dwTypeByte, nullptr);
			if (0 == dwByte)
				break;
			if (0 == dwTypeByte)
				break;

			switch (iDest)
			{
			case BLOCK_NORMAL:
				m_Blocklist[BLOCK_NORMAL].push_back(CAbstractFactory<CNormalBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_COIN:
				m_Blocklist[BLOCK_COIN].push_back(CAbstractFactory<CCoinBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_ITEM:
				m_Blocklist[BLOCK_ITEM].push_back(CAbstractFactory<CItemBlock>::Create(tTemp.fX, tTemp.fY));
				break;
			}
		}
		CloseHandle(hFile);

		return;

	case 2:
		//	Block
		hFile = CreateFile(L"../Data/Save2/ObjBlock.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, L"Load Block", L"Error", MB_OK);
			return;
		}
		dwByte = 0;
		dwTypeByte = 0;
		tTemp = {};
		iDest = 0;
		typeTemp = BLOCK_END;

		while (true)
		{
			ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDest, sizeof(int), &dwTypeByte, nullptr);
			if (0 == dwByte)
				break;
			if (0 == dwTypeByte)
				break;

			switch (iDest)
			{
			case BLOCK_NORMAL:
				m_Blocklist[BLOCK_NORMAL].push_back(CAbstractFactory<CNormalBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_COIN:
				m_Blocklist[BLOCK_COIN].push_back(CAbstractFactory<CCoinBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_ITEM:
				m_Blocklist[BLOCK_ITEM].push_back(CAbstractFactory<CItemBlock>::Create(tTemp.fX, tTemp.fY));
				break;
			}
		}
		CloseHandle(hFile);

		return;

	case 3:
		//	Block
		hFile = CreateFile(L"../Data/Save3/ObjBlock.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, L"Load Block", L"Error", MB_OK);
			return;
		}
		dwByte = 0;
		dwTypeByte = 0;
		tTemp = {};
		iDest = 0;
		typeTemp = BLOCK_END;

		while (true)
		{
			ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDest, sizeof(int), &dwTypeByte, nullptr);
			if (0 == dwByte)
				break;
			if (0 == dwTypeByte)
				break;

			switch (iDest)
			{
			case BLOCK_NORMAL:
				m_Blocklist[BLOCK_NORMAL].push_back(CAbstractFactory<CNormalBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_COIN:
				m_Blocklist[BLOCK_COIN].push_back(CAbstractFactory<CCoinBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_ITEM:
				m_Blocklist[BLOCK_ITEM].push_back(CAbstractFactory<CItemBlock>::Create(tTemp.fX, tTemp.fY));
				break;
			}
		}
		CloseHandle(hFile);

		return;

	case 4:
		//	Block
		hFile = CreateFile(L"../Data/Save4/ObjBlock.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, L"Load Block", L"Error", MB_OK);
			return;
		}
		dwByte = 0;
		dwTypeByte = 0;
		tTemp = {};
		iDest = 0;
		typeTemp = BLOCK_END;

		while (true)
		{
			ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDest, sizeof(int), &dwTypeByte, nullptr);
			if (0 == dwByte)
				break;
			if (0 == dwTypeByte)
				break;

			switch (iDest)
			{
			case BLOCK_NORMAL:
				m_Blocklist[BLOCK_NORMAL].push_back(CAbstractFactory<CNormalBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_COIN:
				m_Blocklist[BLOCK_COIN].push_back(CAbstractFactory<CCoinBlock>::Create(tTemp.fX, tTemp.fY));
				break;

			case BLOCK_ITEM:
				m_Blocklist[BLOCK_ITEM].push_back(CAbstractFactory<CItemBlock>::Create(tTemp.fX, tTemp.fY));
				break;
			}
		}
		CloseHandle(hFile);

		return;
	}

	return;
}


int CBlockMgr::Check_Rect(INFO Player, INFO pTarget, float* _pX, float* _pY)
{



	float fWidth = abs(Player.fX - pTarget.fX);
	float fHeight = abs(Player.fY - pTarget.fY);

	float fRadiusX = (Player.fCX + pTarget.fCX)*0.5f;
	float fRadiusY = (Player.fCY + pTarget.fCY)*0.5f;

	if ((fWidth <= fRadiusX) && (fHeight <= fRadiusY))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;
		return true;
	}
	else
		return false;
}


//	���Ͷ� ���̶� �浹
//	�浹 �� ������


