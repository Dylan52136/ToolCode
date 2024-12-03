#pragma once
#include "vlc\vlc.h"
#include <string>
#include <wtypes.h>
struct CSize
{
	int iWidth;
	int iHeight;
	CSize() :
		iWidth(0),
		iHeight(0)
	{

	}
};

class DVlc
{
public:
	DVlc();
	virtual ~DVlc();
	int SetMedia(const std::string& strUrl);
	int SetHwin(HWND hWnd);
	int Player();
	int Pause();
	int Stop();
	float GetPosition();
	void SetPosition(float pos);
	int GetVolume();
	void SetVolume(int volume);
	CSize GetMediaInfo();

protected:
	libvlc_instance_t* m_instance;
	libvlc_media_t* m_media;
	libvlc_media_player_t* m_player;
};

