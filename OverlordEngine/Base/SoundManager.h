#pragma once
#include "../Helpers/Singleton.h"

class SoundManager: public Singleton<SoundManager>
{
public:
	SoundManager(void);
	~SoundManager(void);

	static bool ErrorCheck(FMOD_RESULT res);
	FMOD::System* GetSystem() const { return m_pFmodSystem; }
	FMOD::Sound* LoadSound(const string& path, FMOD_MODE mode);

private:
	void Initialize();
	FMOD::System* m_pFmodSystem = nullptr;

	map<string, FMOD::Sound*> m_Sounds;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoundManager(const SoundManager &obj) = delete;
	SoundManager& operator=(const SoundManager& obj) = delete;

};

