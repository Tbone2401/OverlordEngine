#include "stdafx.h"
#include "FileReader.h"

#include "..\OverlordProject/CourseObjects/Exam/Boxes/BoxManager.h"
#include "..\OverlordProject/CourseObjects/Exam/Enemies/EnemiesManager.h"

FileReader::FileReader(const string location)
	:m_File(location)
{
}


FileReader::~FileReader()
{
}

void FileReader::ReadFile(BoxManager* boxManager
	, EnemiesManager* enemyManager
	,const GameContext& gameContext)
{
	string line;
	ifstream myFile(m_File);
	while(!myFile.eof())
	{
		std::getline(myFile, line);
		if(line[0] != '<')
		{
			Logger::LogFixMe(L"Incorrect formatting of file, file did not start with '<'");
		}
		if(line.find("Box") != std::string::npos)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			int pos = line.find('(');
			int nextPos = line.find(',');
			x = stof(line.substr(pos+1, nextPos - pos+1));
			pos = ++nextPos;
			nextPos = line.find(',', pos);
			y = stof(line.substr(pos, nextPos - pos));
			pos = ++nextPos;
			nextPos = line.find(',', pos);
			z = stof(line.substr(pos, nextPos - pos));
			boxManager->AddBox(PxVec3(x, y, z),false, gameContext);
			continue;
		}
		if(line.find("Enemy"))
		{
			XMFLOAT3 input[3];

			int pos = 0;
			int nextPos = 0;
			for (int i = 0; i < 3; ++i)
			{
				pos = line.find('(', pos);
				nextPos = line.find(',', pos);
				input[i].x = stof(line.substr(pos + 1, nextPos - pos + 1));

				pos = ++nextPos;
				nextPos = line.find(',', pos);
				input[i].y = stof(line.substr(pos, nextPos - pos));

				pos = ++nextPos;
				nextPos = line.find(',', pos);
				input[i].z = stof(line.substr(pos, nextPos - pos));
			}
			enemyManager->AddEnemy(input[0], input[1], input[2]);
			continue;
		}
	}
	myFile.close();
}
