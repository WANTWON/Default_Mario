#pragma once


// x��ǥ, y��ǥ, ���� ����, ���� ������ ���� �޴� ��

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
