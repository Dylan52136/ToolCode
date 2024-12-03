#include "DVlc.h"

DVlc::DVlc():
    m_instance(nullptr),
    m_media(nullptr),
    m_player(nullptr)
{
    m_instance = libvlc_new(0,nullptr);
}

DVlc::~DVlc()
{
    if (m_player)   libvlc_media_player_release(m_player);
    if (m_media) libvlc_media_release(m_media);
    if (m_instance)  libvlc_release(m_instance);
}

int DVlc::SetMedia(const std::string& strUrl)
{
    if (nullptr == m_instance)   return -1;
    m_media = libvlc_media_new_location(m_instance, strUrl.c_str());
    if (nullptr != m_media)
    {
        m_player = libvlc_media_player_new_from_media(m_media);
        if (nullptr != m_player) return 0;
    }
    return -1;
}

int DVlc::SetHwin(HWND hWnd)
{
    if (!m_player)   return -1;
    if (m_player)
    {
        libvlc_media_player_set_hwnd(m_player, hWnd);
    }
    return 0;
}

int DVlc::Player()
{
    if (!m_player) return -1;
    return libvlc_media_player_play(m_player);
}

int DVlc::Pause()
{
    if (!m_player)   return -1;
    libvlc_media_player_pause(m_player);
    return 0;
}

int DVlc::Stop()
{
    if (!m_player)   return -1;
    libvlc_media_player_stop(m_player);
    return 0;
}

float DVlc::GetPosition()
{
    if (!m_player)   return 0.0f;
    return libvlc_media_player_get_position(m_player);
}

void DVlc::SetPosition(float pos)
{
    if (!m_player)   return;
    libvlc_media_player_set_position(m_player,pos);
}

int DVlc::GetVolume()
{
    if (!m_player)   return 0;
    return libvlc_audio_get_volume(m_player);
}

void DVlc::SetVolume(int volume)
{
    if (!m_player)   return;
    libvlc_audio_set_volume(m_player, volume);
}

CSize DVlc::GetMediaInfo()
{
    CSize size;
    if (m_player)
    {
        size.iWidth = libvlc_video_get_width(m_player);
        size.iHeight = libvlc_video_get_height(m_player);
    }
    return size;
}
