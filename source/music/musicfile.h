#ifndef _MUSICFILE_H
#define _MUSICFILE_H

#include <string>
#include <stdio.h>
#include "memory/smartptr.hpp"

enum MusicType
{
	UNKNOWN,
	MP3,
	OGG,
	MOD,
	XM,
	S3M
};

enum PlayStatus
{
	MUSIC_UNKNOWN,
	MUSIC_PLAYING,
	MUSIC_PAUSED,
	MUSIC_STOPPED
};

class MusicFile
{
public:
	~MusicFile();

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual PlayStatus Status() = 0;
	virtual void SetVolume(int volume) = 0;
	
	virtual bool IsValid();

	static MusicFile *Load(std::string filename);
	static MusicType GetMusicType(std::string filename);
protected:
	MusicFile(std::string filename, MusicType music_type);
	MusicFile(MusicType music_type);

	SmartBuf m_music;
	MusicType m_music_type;
	int m_music_fileSize;
	bool isValid;
};

class Mp3File : public MusicFile
{
public:
	Mp3File(std::string filename);
	~Mp3File();

	void Play();
	void Pause();
	void Stop();
	PlayStatus Status();	
	void SetVolume(int volume);
private:
	FILE *fp;
	bool isPaused;
};

class OggFile : public MusicFile
{
public:
	OggFile(std::string filename);
	~OggFile();

	void Play();
	void Pause();
	void Stop();
	PlayStatus Status();	
	void SetVolume(int volume);
private:
	FILE *fp;
};

class BaseModFile : public MusicFile
{
public:
	BaseModFile(std::string filename, MusicType musicType);
	~BaseModFile();
	
	void Play();
	void Pause();
	void Stop();
	PlayStatus Status();	
	void SetVolume(int volume);
	
	bool IsValid();
private:
	std::string filename;
};

class ModFile : public BaseModFile
{
public:
	ModFile(std::string filename) : BaseModFile(filename, MOD) {};
};

class S3MFile : public BaseModFile
{
public:
	S3MFile(std::string filename) : BaseModFile(filename, S3M) {};
};

class XMFile : public BaseModFile
{
public:
	XMFile(std::string filename) : BaseModFile(filename, XM) {};
};

#endif