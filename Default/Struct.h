#pragma once
#include "Enum.h"

// x��ǥ, y��ǥ, ���� ����, ���� ������ ���� �޴� ��

class CObj;
typedef struct tagInfo
{
	float fX;
	float fY;
	float fCX;	//���α���
	float fCY;	//���α���
}INFO;

typedef struct tagPoint
{
	float fX;
	float fY;

	tagPoint(void) { ZeroMemory(this, sizeof(tagPoint)); }
	tagPoint(float _fX, float _fY) { fX = _fX; fY = _fY; }

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT fRPoint;
	LINEPOINT fLPoint;

	tagLine(void) { ZeroMemory(this, sizeof(tagLine)); }
	//tagLine(LINEPOINT& LeftPoint, LINEPOINT& RightPoint) : fLPoint(LeftPoint), fRPoint(RightPoint) {}

	tagLine(LINEPOINT& LeftPoint, LINEPOINT& RightPoint)
	{
		fRPoint.fX = RightPoint.fX;
		fRPoint.fY = RightPoint.fY;

		fLPoint.fX = LeftPoint.fX;
		fLPoint.fY = LeftPoint.fY;
	}

}LINE;


typedef struct tagSave
{
	INFO  m_tInfo; //x��ǥ, y��ǥ ���� ���� ����ü
	RECT  m_tRect; //m_tInfo�� ������� Rect�� left,top,right,bottom�� ����
	DIRECTION m_eDir;

	float m_fSpeed;
	bool  m_bDead;

	float m_fAngle;
	CObj* m_pTarget;

	OBJ_LIST  m_eID;


}SAVEOBJ;