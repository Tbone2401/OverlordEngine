#pragma once
class BoxManager;
class EnemiesManager;
class FileReader
{
public:
	FileReader(const string location);
	~FileReader();
	void ReadFile(BoxManager* boxManager,
		EnemiesManager * enemyManager,
		const GameContext& gameContext);
private:
	std::string m_File;
};

