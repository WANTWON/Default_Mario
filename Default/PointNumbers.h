#pragma once
class CPointNumbers
{
public:
	CPointNumbers();
	CPointNumbers(int iPoints, float fX, float fY);
	~CPointNumbers();

	void Release();

	// �����Ӹ��� ����Ǵ� �Լ� 
	int Update();
	void Late_Update();
	void Render(HDC hDC);

private:
	int m_iNumber;
	float m_fX;
	float m_fY;
	float m_fSpeed;
	float m_dwTime;
	bool m_bDead;
};

