#include "stdafx.h"
#include "Player.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "BlockMgr.h"


CPlayer::CPlayer() : m_pShield_Angle(0), m_bJump(false), m_fJumpPower(0), m_fTime(0), m_bFalling(false), m_bStep_Monster(false), fY(0), fY2(0)
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
	m_tInfo = { 400.f, 300.f, 30.f, 30.f }; //x��ǥ, y��ǥ, ���α���, ���α���
	m_fSpeed = 5.f;
	m_fJumpPower = 15.f;
	m_fkg = 9.8f;
	Jumping_Time = GetTickCount();
	m_dwTIme = GetTickCount();
}

int CPlayer::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bJump)
		m_fTime = 0;

	Jumping();
	Key_Input();
	Update_Rect();


	return OBJ_NOEVENT;
}
void  CPlayer::Late_Update(void)
{
	if (m_tInfo.fY >= WINCY)
	{
		m_tInfo.fY = 0.f;
	}
	CBlockMgr::Get_Instance()->Collision_with_Direction(this);
}

void CPlayer::Release(void)
{

}
void CPlayer::Render(HDC hDC)
{

	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

}

void CPlayer::Key_Input(void)
{
	if (GetAsyncKeyState(VK_RIGHT))
		m_tInfo.fX += m_fSpeed;
	else if (GetAsyncKeyState(VK_LEFT))
		m_tInfo.fX -= m_fSpeed;

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		m_bJump = true;
	}

	if (m_bStep_Monster)
	{
		//m_bJump = false;
		if (Jumping_Time + 10 < GetTickCount())
		{
			m_bJump = true;
			m_bStep_Monster = false;
		}
	}
		
}

void CPlayer::Jumping(void)
{
	
	//���͸� ����� ���� ������ ���, ������ ���� ������ ��� �ٸ��� �����߽��ϴ�.
	
	bool b_LineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo.fX, &fY);
	
	bool b_BlockCol = CBlockMgr::Get_Instance()->CollisionBlock(m_tRect, m_tInfo.fX, &fY2);


	if (m_bStep_Monster) // ���͸� ����� ��
	{
		m_fJumpPower = 10;
		m_tInfo.fY -= m_fJumpPower*m_fTime - (2.8f*m_fTime*m_fTime*0.5f);
		m_fTime += 0.09f;
		if (m_fTime > 1.2f)
			m_fTime = 1.2f;

		if (b_BlockCol && m_tInfo.fY + m_tInfo.fCY*0.5f >= fY2) 
			//�÷��̾��� Bottom ���� ����� Top�̶� �̼��ϰ� ������ ���� (�� ��� ������ ��)
		{
			m_fTime = 0.0f;
		}
		if (b_LineCol && m_tInfo.fY > fY) //������ �� ������ �� �����ϱ�
		{
			m_fTime = 0.0f;
		}

	}
	else if (m_bJump)  //������ ���� ��
	{
		m_fJumpPower = 15;
		m_tInfo.fY -= m_fJumpPower*m_fTime - (9.8f*m_fTime*m_fTime*0.5f);
		m_fTime += 0.13f;
		if (m_fTime > 3.9f)
			m_fTime = 3.9f;

		if (b_BlockCol && m_tInfo.fY+m_tInfo.fCY*0.5f >= fY2) //�÷��̾��� Bottom ���� ����� Top�̶� �̼��ϰ� ������ ���� (�� ��� ������ ��)
		{
			m_fTime = 0.0f;
			m_bJump = false;
		}
		if (b_LineCol && m_tInfo.fY > fY) //������ �� ������ �� �����ϱ�
		{
			m_bJump = false;
			m_fTime = 0.0f;
		}
	}
	else if (b_LineCol)
	{
		if (b_BlockCol)
		{
			m_tInfo.fY = fY2 - m_tInfo.fCY*0.5f;
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
