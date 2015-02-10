#include "PlayChan.h"

CPlayChan::CPlayChan(PlayChanNotify& rNotify)
:m_pPlayer(NULL)
,m_nWidth(0)
,m_nHeight(0)
,m_rNotify(rNotify)
{
}

int CPlayChan::Open(const TASK_STREAM& stTaskStream)
{
	if (NULL == m_pPlayer)
		m_pPlayer = AVCONMP_PlayChan::Create(*this);
	if (0 != m_pPlayer->Open(stTaskStream.ulAudioID, stTaskStream.ulVideoID, stTaskStream.strNodeID.c_str(), stTaskStream.strNATAddr.c_str(), stTaskStream.usLocalPort, stTaskStream.strLocalAddr.c_str(), stTaskStream.usLocalPort, stTaskStream.strMCUID.c_str(), stTaskStream.strMCUAddr.c_str(), stTaskStream.usMCUPort, 0))
	{
		m_pPlayer->Close();
		delete m_pPlayer;
		m_pPlayer = NULL;
		return -1;
	}
	else
	{
		m_pPlayer->SetVideoCast(true);
		m_pPlayer->SetAudioCast(true);
		m_pPlayer->EnableAudio(1);
		m_pPlayer->StartAudio();
		m_pPlayer->EnableVideo(1);
		m_pPlayer->StartVideo();
	}
	return 0;
}

void CPlayChan::Close()
{
	if (m_pPlayer)
	{
		m_pPlayer->Close();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
}

void CPlayChan::SetScreenWnd(int iScreenIndex, unsigned long ulWndIndex, int iFullScreen)
{
	m_nWndIndex = ulWndIndex;
	LOG::DBG("[HPCOMP] AVCONMP_PlayChan::SetScreenWnd(iScreenIndex = %d, iWndIndex = %d, iFullScreen = %d)\n", iScreenIndex, ulWndIndex, iFullScreen);
	if (m_pPlayer)
		m_pPlayer->SetScreenWnd(iScreenIndex, ulWndIndex, iFullScreen);
}

void CPlayChan::OnAVCONMP_PlayChanCallbackVidChanData(unsigned char*pData,int nLen,int nKeyFrame,int& nFlagDontDecode)
{
	if (0 == nKeyFrame)
		return;
	if (VIDEC_HEADER_EXT_IS_VALID(pData))
	{
		int width = VIDEC_HEADER_EXT_GET_VIRTUAL_WIDTH(pData);
		int height = VIDEC_HEADER_EXT_GET_VIRTUAL_HEIGHT(pData);
		if (width != m_nWidth || height != m_nHeight)
		{
			m_nWidth = width;
			m_nHeight = height;
			LOG::INF("[PlayChan]Get key frame and it prompt the width=%d, height=%d\n", m_nWidth, m_nHeight);
			m_rNotify.OnResolutionChanged(m_nWndIndex, width, height);
		}
	}
}