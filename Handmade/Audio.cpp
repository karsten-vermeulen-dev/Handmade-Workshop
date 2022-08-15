#include <assert.h>
#include <algorithm>
#include <math.h>
#include "Audio.h"
#include "Utility.h"

FMOD::System* Audio::audioSystem = nullptr;
std::string Audio::rootFolder = "Assets/Audio/";
std::map<std::string, FMOD::Sound*> Audio::music;
std::map<std::string, FMOD::Sound*> Audio::sounds;

//======================================================================================================
bool Audio::Initialize()
{
	System_Create(&audioSystem);

	if (audioSystem->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error initializing the FMOD audio system.", Utility::Severity::Failure);
		return false;
	}

	return true;
}
//======================================================================================================
void Audio::Shutdown()
{
	audioSystem->release();
}
//======================================================================================================
void Audio::Update()
{
	audioSystem->update();
}
//======================================================================================================
Audio::Audio(Type type, const std::string& tag, const std::string& filename) : type(type)
{
	if (!filename.empty())
	{
		Load(type, tag, filename);
		SetAudio(tag, type);
	}

	else if (!tag.empty())
	{
		SetAudio(tag, type);
	}
}
//======================================================================================================
bool Audio::Load(Type type, const std::string& tag, const std::string& filename)
{
	FMOD::Sound* audio = nullptr;

	//Sound effects are loaded directly into memory
	if (type == Type::Sound)
	{
		assert(sounds.find(tag) == sounds.end());
		audioSystem->createSound((rootFolder + filename).c_str(), FMOD_DEFAULT, 0, &audio);

		if (!audio)
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error loading sound file \"" + (rootFolder + filename) + "\"\n\n"
				"Possible causes could be a corrupt or missing file. Another reason could be "
				"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
			return false;
		}

		sounds[tag] = audio;
	}

	//Music is streamed directly from the folder on drive
	else
	{
		assert(music.find(tag) == music.end());
		audioSystem->createStream((rootFolder + filename).c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &audio);

		if (!audio)
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error loading music file \"" + (rootFolder + filename) + "\"\n\n"
				"Possible causes could be a corrupt or missing file. Another reason could be "
				"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
			return false;
		}

		music[tag] = audio;
	}

	return true;
}
//======================================================================================================
void Audio::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = music.find(tag);

		if (it == music.end())
		{
			it = sounds.find(tag);
			assert(it != sounds.end());
			it->second->release();
			sounds.erase(it);
		}

		else
		{
			it->second->release();
			music.erase(it);
		}
	}

	else
	{
		for (const auto& music : music)
		{
			music.second->release();
		}

		for (const auto& sound : sounds)
		{
			sound.second->release();
		}

		music.clear();
		sounds.clear();
	}
}
//======================================================================================================
float Audio::GetPan() const
{
	return pan;
}
//======================================================================================================
float Audio::GetVolume() const
{
	return volume;
}
//======================================================================================================
float Audio::GetFrequency() const
{
	return frequency;
}
//======================================================================================================
Fuint Audio::GetLength() const
{
	Fuint length = 0;

	if (audioData)
	{
		audioData->getLength(&length, FMOD_TIMEUNIT_MS);
	}

	return length;
}
//======================================================================================================
Fuint Audio::GetPosition() const
{
	Fuint tempPosition = 0;

	if (channel)
	{
		channel->getPosition(&tempPosition, FMOD_TIMEUNIT_MS);
	}

	return tempPosition;
}
//======================================================================================================
void Audio::SetPan(float pan)
{
	pan = std::clamp(pan, leftPan, rightPan);
	assert(channel != nullptr);
	channel->setPan(pan);
}
//======================================================================================================
void Audio::SetVolume(float volume)
{
	volume = std::clamp(volume, minVolume, maxVolume);
	assert(channel != nullptr);
	channel->setVolume(volume);
}
//======================================================================================================
void Audio::SetFrequency(float frequency)
{
	frequency = std::clamp(frequency, minFrequency, maxFrequency);
	assert(channel != nullptr);
	channel->setFrequency(frequency);
}
//======================================================================================================
void Audio::IsMuted(bool flag)
{
	isMuted = flag;
	assert(channel != nullptr);
	channel->setMute(isMuted);
}
//======================================================================================================
void Audio::SetLoopCount(Loop loopType, Loop loopCount)
{
	if (loopType == Loop::Custom)
	{
		loopCount = loopCount;
	}

	else if (loopType == Loop::Endless || loopType == Loop::None || loopType == Loop::Single)
	{
		loopCount = loopType;
	}

	assert(channel != nullptr);
	channel->setLoopCount(static_cast<int>(loopCount));
}
//======================================================================================================
void Audio::SetAudio(const std::string& tag, Type type)
{
	auto& audioMap = (type == Type::Music) ? music : sounds;
	auto it = audioMap.find(tag);
	assert(it != audioMap.end());
	audioData = (*it).second;
	this->tag = tag;
}
//======================================================================================================
void Audio::SetFrequencyRange(float minFrequency, float maxFrequency)
{
	minFrequency = minFrequency;
	maxFrequency = maxFrequency;
}
//======================================================================================================
void Audio::SetFrequencyInterval(Interval intervalType, float interval)
{
	float ratio = 0.0f;

	if (intervalType == Interval::Semitone)
	{
		ratio = pow(2.0f, 1.0f / 12.0f);
	}

	else if (intervalType == Interval::Octave)
	{
		ratio = 2.0f;
	}

	frequency *= pow(ratio, interval);
	assert(channel != nullptr);
	channel->setFrequency(frequency);
}
//======================================================================================================
bool Audio::Play()
{
	assert(!tag.empty());

	if (!channel || (GetPosition() == 0))
	{
		audioSystem->playSound(audioData, channelGroup, false, &channel);
	}

	if (!channel)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Audio could not be played through any channels.", Utility::Severity::Failure);
		return false;
	}

	return true;
}
//======================================================================================================
void Audio::Pause()
{
	if (channel)
	{
		channel->setPaused(true);
	}
}
//======================================================================================================
void Audio::Resume()
{
	if (channel)
	{
		channel->setPaused(false);
	}
}
//======================================================================================================
void Audio::Stop()
{
	if (channel)
	{
		channel->stop();
	}

	channel = nullptr;
}
//======================================================================================================
void Audio::Move(Position position, Fuint positionValue)
{
	if (channel)
	{
		if (position == Position::Custom)
		{
			channel->setPosition(positionValue, FMOD_TIMEUNIT_MS);
		}

		else if (position == Position::Start)
		{
			channel->setPosition(0, FMOD_TIMEUNIT_MS);
		}

		else if (position == Position::End)
		{
			channel->setPosition(GetLength(), FMOD_TIMEUNIT_MS);
		}
	}
}