#pragma once
#include <map>
#include <string>
#include <fmod.hpp>

//Better to have a shorter data type name
typedef unsigned int Fuint;

class Audio
{

public:

	enum class Loop
	{
		None = 0,
		Single = 1,
		Endless = -1,
		Custom = -2
	};

	enum class Position
	{
		Start = 1,
		End = 2,
		Custom = 0
	};

	enum class Interval
	{
		Semitone = 0,
		Octave = 1
	};

	enum class Type
	{
		Music,
		Sound
	};

	static bool Initialize();
	static void Shutdown();
	static void Update();

	Audio(Type type = Type::Music, const std::string& tag = "",
		const std::string& filename = "");

	bool Load(Type type, const std::string& tag,
		const std::string& filename);
	void Unload(const std::string& tag = "");

	float GetPan() const;
	float GetVolume() const;
	float GetFrequency() const;

	Fuint GetLength() const;
	Fuint GetPosition() const;

	void SetPan(float pan);
	void SetVolume(float volume);
	void SetFrequency(float frequency);

	void IsMuted(bool flag);
	void SetAudio(const std::string& tag, Type type);
	void SetLoopCount(Loop loopType, Loop loopCount = Loop::None);
	void SetFrequencyRange(float minFrequency, float maxFrequency);
	void SetFrequencyInterval(Interval intervalType, float interval = 1.0f);

	bool Play();
	void Pause();
	void Resume();
	void Stop();
	void Move(Position position, Fuint positionValue = 0);

private:

	const float leftPan{ -1.0 };
	const float rightPan{ 1.0f };
	const float minVolume{ 0.0f };
	const float maxVolume{ 1.0f };

	Type type;
	std::string tag;
	bool isMuted{ false };
	Loop loopCount{ Loop::None };

	float pan{ 0.0f };
	float volume{ 0.5f };
	float frequency{ 44100.0f };
	float minFrequency{ 11025.0f };
	float maxFrequency{ 176400.0f };

	FMOD::Sound* audioData{ nullptr };
	FMOD::Channel* channel{ nullptr };
	FMOD::ChannelGroup* channelGroup{ nullptr };

	static std::string rootFolder;
	static FMOD::System* audioSystem;
	static std::map<std::string, FMOD::Sound*> music;
	static std::map<std::string, FMOD::Sound*> sounds;

};