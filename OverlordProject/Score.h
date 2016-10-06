#pragma once
#include "../OverlordEngine/Helpers/Singleton.h"

class Score : public Singleton<Score>
{
public:
	Score(void);
	~Score(void);

	const void DestroyedBox();
	const void KilledEnemy();
	const int GetScore() const;
	void ResetScore();

private:
	int m_Score = 0;

	Score(const Score& t);
	Score& operator=(const Score& t);
};

