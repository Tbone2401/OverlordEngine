#include "stdafx.h"
#include "Score.h"


Score::Score()
{
}


Score::~Score()
{
}

const void Score::DestroyedBox()
{
	m_Score += 20;
}

const void Score::KilledEnemy()
{
	m_Score += 50;
}

const int Score::GetScore() const
{
	return m_Score;
}

void Score::ResetScore()
{
	m_Score = 0;
}
