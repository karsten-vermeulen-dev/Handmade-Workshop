#include <assert.h>
#include <algorithm>
#include <math.h>
#include "Audio.h"
#include "Utility.h"

FMOD::System* Audio::s_audioSystem = nullptr;
std::string Audio::s_rootFolder = "Assets/Audio/";
std::map<std::string, FMOD::Sound*> Audio::s_music;
std::map<std::string, FMOD::Sound*> Audio::s_sounds;

//======================================================================================================
bool Audio::Initialize()
{
	System_Create(&s_audioSystem);

	if (s_audioSystem->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK)
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
	Unload();
	s_audioSystem->release();
}
//======================================================================================================
void Audio::Update()
{
	s_audioSystem->update();
}
//======================================================================================================
bool Audio::Load(Type type, const std::string& tag, const std::string& filename)
{
	FMOD::Sound* audio = nullptr;

	//Sound effects are loaded directly into memory
	if (type == Type::Sound)
	{
		assert(s_sounds.find(tag) == s_sounds.end());
		s_audioSystem->createSound((s_rootFolder + filename).c_str(), FMOD_DEFAULT, 0, &audio);

		if (!audio)
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error loading sound file \"" + (s_rootFolder + filename) + "\"\n\n"
				"Possible causes could be a corrupt or missing file. Another reason could be "
				"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
			return false;
		}

		s_sounds[tag] = audio;
	}

	//Music is streamed directly from the folder on drive
	else
	{
		assert(s_music.find(tag) == s_music.end());
		s_audioSystem->createStream((s_rootFolder + filename).c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &audio);

		if (!audio)
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error loading music file \"" + (s_rootFolder + filename) + "\"\n\n"
				"Possible causes could be a corrupt or missing file. Another reason could be "
				"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
			return false;
		}

		s_music[tag] = audio;
	}

	return true;
}
//======================================================================================================
void Audio::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = s_music.find(tag);

		if (it == s_music.end())
		{
			it = s_sounds.find(tag);
			assert(it != s_sounds.end());
			it->second->release();
			s_sounds.erase(it);
		}

		else
		{
			it->second->release();
			s_music.erase(it);
		}
	}

	else
	{
		for (const auto& music : s_music)
		{
			music.second->release();
		}

		for (const auto& sound : s_sounds)
		{
			sound.second->release();
		}

		s_music.clear();
		s_sounds.clear();
	}
}
//======================================================================================================
void Audio::SetRootFolder(const std::string& rootFolder)
{
	s_rootFolder = rootFolder;
}
//======================================================================================================
Audio::Audio(Type type, const std::string& tag, const std::string& filename) 
	: m_type(type)
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
Audio::Audio(const Audio& copy)
{
	m_pan = copy.m_pan;
	m_volume = copy.m_volume;
	m_frequency = copy.m_frequency;
	m_minFrequency = copy.m_minFrequency;
	m_maxFrequency = copy.m_maxFrequency;

	m_tag = copy.m_tag;
	m_type = copy.m_type;
	m_isMuted = copy.m_isMuted;
	m_loopCount = copy.m_loopCount;

	m_channel = nullptr;
	m_channelGroup = nullptr;

	SetAudio(m_tag, m_type);
}
//======================================================================================================
float Audio::GetPan() const
{
	return m_pan;
}
//======================================================================================================
float Audio::GetVolume() const
{
	return m_volume;
}
//======================================================================================================
float Audio::GetFrequency() const
{
	return m_frequency;
}
//======================================================================================================
Fuint Audio::GetLength() const
{
	Fuint length = 0;

	if (m_audioData)
	{
		m_audioData->getLength(&length, FMOD_TIMEUNIT_MS);
	}

	return length;
}
//======================================================================================================
Fuint Audio::GetPosition() const
{
	Fuint tempPosition = 0;

	if (m_channel)
	{
		m_channel->getPosition(&tempPosition, FMOD_TIMEUNIT_MS);
	}

	return tempPosition;
}
//======================================================================================================
void Audio::SetPan(float pan)
{
	m_pan = std::clamp(pan, LEFT_PAN, RIGHT_PAN);
	assert(m_channel != nullptr);
	m_channel->setPan(m_pan);
}
//======================================================================================================
void Audio::SetVolume(float volume)
{
	m_volume = std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
	assert(m_channel != nullptr);
	m_channel->setVolume(m_volume);
}
//======================================================================================================
void Audio::SetFrequency(float frequency)
{
	m_frequency = std::clamp(frequency, m_minFrequency, m_maxFrequency);
	assert(m_channel != nullptr);
	m_channel->setFrequency(m_frequency);
}
//======================================================================================================
void Audio::IsMuted(bool flag)
{
	m_isMuted = flag;
	assert(m_channel != nullptr);
	m_channel->setMute(m_isMuted);
}
//======================================================================================================
void Audio::SetLoopCount(Loop loopType, Loop loopCount)
{
	if (loopType == Loop::Custom)
	{
		m_loopCount = loopCount;
	}

	else if (loopType == Loop::Endless || loopType == Loop::None || loopType == Loop::Single)
	{
		m_loopCount = loopType;
	}

	assert(m_channel != nullptr);
	m_channel->setLoopCount(static_cast<int>(m_loopCount));
}
//======================================================================================================
void Audio::SetAudio(const std::string& tag, Type type)
{
	auto& audioMap = (type == Type::Music) ? s_music : s_sounds;
	auto it = audioMap.find(tag);
	assert(it != audioMap.end());
	m_audioData = (*it).second;
	m_tag = tag;
}
//======================================================================================================
void Audio::SetFrequencyRange(float minFrequency, float maxFrequency)
{
	m_minFrequency = minFrequency;
	m_maxFrequency = maxFrequency;
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

	m_frequency *= pow(ratio, interval);
	assert(m_channel != nullptr);
	m_channel->setFrequency(m_frequency);
}
//======================================================================================================
bool Audio::Play()
{
	assert(!m_tag.empty());

	if (!m_channel || (GetPosition() == 0))
	{
		s_audioSystem->playSound(m_audioData, m_channelGroup, false, &m_channel);
	}

	if (!m_channel)
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
	if (m_channel)
	{
		m_channel->setPaused(true);
	}
}
//======================================================================================================
void Audio::Resume()
{
	if (m_channel)
	{
		m_channel->setPaused(false);
	}
}
//======================================================================================================
void Audio::Stop()
{
	if (m_channel)
	{
		m_channel->stop();
	}

	m_channel = nullptr;
}
//======================================================================================================
void Audio::Move(Position position, Fuint positionValue)
{
	if (m_channel)
	{
		if (position == Position::Custom)
		{
			m_channel->setPosition(positionValue, FMOD_TIMEUNIT_MS);
		}

		else if (position == Position::Start)
		{
			m_channel->setPosition(0, FMOD_TIMEUNIT_MS);
		}

		else if (position == Position::End)
		{
			m_channel->setPosition(GetLength(), FMOD_TIMEUNIT_MS);
		}
	}
}