#ifndef	PLAYCHAN_H_
#define	PLAYCHAN_H_

#include "stdafx.h"
#include "HPCompositeDefine.h"
#include "VIDEC/VIDEC_Header.h"
#include "AVCONMP/IMXEC_MediaChannel.h"

class PlayChanNotify
{
public:
	virtual void OnResolutionChanged(unsigned int nWndIndex, unsigned int nWidth, unsigned int nHeight) = 0;
};

class CPlayChan : public AVCONMP_PlayChanCallback
{
public:
	CPlayChan(PlayChanNotify& rNotify);
	~CPlayChan() {}
public:
	int Open(const TASK_STREAM& stTaskStream);
	void Close();
	void SetScreenWnd(int iScreenIndex, unsigned long ulWndIndex, int iFullScreen);

	virtual void OnAVCONMP_PlayChanCallbackAudChanData(unsigned char*pData,int nLen,bool&bLoop) {}
	virtual void OnAVCONMP_PlayChanCallbackAudioSamples(const short*pSamples,int nSamples,int nSampleRate,int nTimestamp) {}
	virtual void OnAVCONMP_PlayChanCallbackVidChanData(unsigned char*pData,int nLen,int nKeyFrame,int& nFlagDontDecode);

private:
	AVCONMP_PlayChan* m_pPlayer;
	unsigned int m_nWndIndex, m_nWidth, m_nHeight;
	PlayChanNotify& m_rNotify;
};

#endif		//PLAYCHAN_H_