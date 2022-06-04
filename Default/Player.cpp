#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "KeyMgr.h"


CPlayer::CPlayer() : m_pShield_Angle(0), m_bJump(false), m_fJumpPower(0), m_fTime(0), m_bFalling(false), m_iActiveBuff(ITEM_END), m_dwBuffTime(GetTickCount()), m_bIsBuffActive(false)
{
	ZeroMemory(&m_pGUIDE, sizeof(POINT));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	//�÷��̾� �׸��� ũ�⸦ ���ϴ� ��
	m_tInfo = { 400.f, 300.f, 50.f, 50.f }; //x��ǥ, y��ǥ, ���α���, ���α���
	m_fSpeed = 5.f;
	m_fJumpPower = 15.f;

}

int CPlayer::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Jumping();
	Check_ActiveBuff();
	Key_Input();
	Update_Rect();


	return OBJ_NOEVENT;
}
void  CPlayer::Late_Update(void)
{
	if (m_tInfo.fY >= WINCY)
	{
		m_bFalling = true;
		m_tInfo.fY = 0.f;
	}

}

void CPlayer::Release(void)
{

}
void CPlayer::Render(HDC hDC)
{

	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

}

void CPlayer::Coin_Pickup()
{
	// Increase Coin by 1
	// Increase Points by 200
}

void CPlayer::Buff_Mushroom()
{
	if (GetTickCount() > m_dwBuffTime + 5000)
	{
		// Half Size
		m_tInfo.fCX -= m_tInfo.fCX * .5f;
		m_tInfo.fCY -= m_tInfo.fCY * .5f;

		m_iActiveBuff = ITEM_END;
	}
	else
	{
		if (!m_bIsBuffActive)
		{
			// Double Size
			m_tInfo.fCX += m_tInfo.fCX;
			m_tInfo.fCY += m_tInfo.fCY;

			m_bIsBuffActive = true;
		}
	}
}

void CPlayer::Buff_Star(bool bActive)
{

}

void CPlayer::Buff_Flower(bool bActive)
{

}

void CPlayer::Key_Input(void)
{
	if (GetAsyncKeyState(VK_RIGHT))
		m_tInfo.fX += m_fSpeed;
	else if (GetAsyncKeyState(VK_LEFT))
		m_tInfo.fX -= m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		m_bJump = true;
}

void CPlayer::Jumping(void)
{
	float fY = 0.f;

	bool b_LineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= m_fJumpPower*m_fTime - (9.8f*m_fTime*m_fTime*0.5f);
		m_fTime += 0.13f;

		if (b_LineCol && m_tInfo.fY > fY) //������ �� ������ �� �����ϱ�
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY - m_tInfo.fCY*0.5f;
		}
	}
	else if (b_LineCol)
	{
		if (m_bFalling)
		{
			m_tInfo.fY += m_fSpeed;
			if (m_tInfo.fY >= fY - m_tInfo.fCY*0.5f)
				m_bFalling = false;
		}

		else
			m_tInfo.fY = fY - m_tInfo.fCY*0.5f;
	}
	else
	{
		m_tInfo.fY += m_fSpeed;
		m_bFalling = true;
	}

}

void CPlayer::Check_ActiveBuff(void)
{
	switch (m_iActiveBuff)
	{
	case ITEM_COIN:
		Coin_Pickup();
		break;
	case ITEM_MUSHROOM:
		Buff_Mushroom();
		break;
	case ITEM_STAR:
		Buff_Star(true);
		break;
	case ITEM_FLOWER:
		Buff_Flower(true);
		break;
	}
}