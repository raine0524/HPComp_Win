#include "TaskProcess.h"

TaskProcess::TaskProcess(TaskProcessNotify& rNotify)
:m_rNotify(rNotify)
,m_pCapChanDev(NULL)
,m_iScreenIndex(-1)
,m_width(-1)
,m_height(-1)
{
}

bool TaskProcess::StartTask(int iScreenIndex, const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream)
{
	m_iScreenIndex = iScreenIndex;
	m_taskInfo = stTaskInfo;

	int nCount = 0, width[MAX_OUTPUT_NUM], height[MAX_OUTPUT_NUM], bitrate[MAX_OUTPUT_NUM];
	int nSub, nQSub = nSub = 0;
	//Use high resolution & bitrate
	if (stTaskInfo.bUseHigh)
	{
		width[nCount] = stTaskInfo.uHighWidth;
		height[nCount] = stTaskInfo.uHighHeight;
		bitrate[nCount] = stTaskInfo.uHighBitrate;
		nCount++;
	}
	//Use middle resolution & bitrate
	if (stTaskInfo.bUseMid)
	{
		width[nCount] = stTaskInfo.uMidWidth;
		height[nCount] = stTaskInfo.uMidHeight;
		bitrate[nCount] = stTaskInfo.uMidBitrate;
		nCount++;
	}
	//Use low resolution & bitrate
	if (stTaskInfo.bUseLow)
	{
		width[nCount] = stTaskInfo.uLowWidth;
		height[nCount] = stTaskInfo.uLowHeight;
		bitrate[nCount] = stTaskInfo.uLowBitrate;
		nCount++;
	}
	if (nCount > 1)
		nSub = 1;
	if (nCount > 2)
		nQSub = 1;

	HPTM_TM_SUBTYPE type = GetTemplateType(m_taskInfo.uTMID);
	if (TM_SUBTYPE_0_NONE == type)
	{
		printf("[HPCOMP]Unvalid uTMID value, Stop executing this task.\n");
		return false;
	}
	OnSetTemplate(width[0], height[0], type);
	m_width = width[0];
	m_height = height[0];

	if (NULL == m_pCapChanDev)
		m_pCapChanDev = AVCONMP_CapChanDev::Create(*this);
	m_pCapChanDev->Open(m_iScreenIndex);
	m_pCapChanDev->OpenAudio(X_AUDIO_CODEC_G729);
	m_pCapChanDev->SetOtherCodec(width[1], height[1], bitrate[1], width[2], height[2], bitrate[2]);
	m_pCapChanDev->OpenVideoDev(width[0], height[0], 30, bitrate[0], 0, VIDEC_CODEC_H264, nSub, nQSub);
	if (!AddAllStream(vTaskStream))
		return false;

	//Send comp info to REC_AGENT
	KCmdPacket out("VIDEOCOMP", REC_COMINFO, g_config.local_serverid);
    out.SetAttrib("MCUID", NETEC_Node::GetMCUID());
    out.SetAttrib("MCUADDR", NETEC_Node::GetMCUIP());
    out.SetAttribUS("MCUPORT", NETEC_Node::GetServerPort());
    out.SetAttrib("NATADDR", NETEC_Node::GetNATIP());
    out.SetAttrib("LOCALADDR", NETEC_Node::GetLocalIP());
    out.SetAttribUS("LOCALPORT", NETEC_Node::GetLocalPort());
    out.SetAttrib("NODEID", NETEC_Node::GetNodeID());
    out.SetAttrib("TASKID", m_taskInfo.strTaskID);
	out.SetAttrib("TASKNAME", m_taskInfo.strTaskName);
	if (stTaskInfo.bUseHigh)
		out.SetAttribUS("BHIGHSTREAM", 1);
	else
		out.SetAttribUS("BHIGHSTREAM", 0);
	if (stTaskInfo.bUseMid)
		out.SetAttribUS("BMIDSTREAM", 1);
	else
		out.SetAttribUS("BMIDSTREAM", 0);
	if (stTaskInfo.bUseLow)
		out.SetAttribUS("BLOWSTREAM", 1);
	else
		out.SetAttribUS("BLOWSTREAM", 0);
	out.SetAttribUL("AUDIOCID_H", m_pCapChanDev->GetAudioID());
	out.SetAttribUL("VIDEOCID_H", m_pCapChanDev->GetVideoID());
    UTIL::SendToDevAgent(out, REC_AGENT_ID);
	return true;
}

void TaskProcess::ModifyTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream)
{
	if (stTaskInfo.uTMID != m_taskInfo.uTMID)
	{
		m_taskInfo.uTMID = stTaskInfo.uTMID;
		OnSetTemplate(m_width, m_height, GetTemplateType(stTaskInfo.uTMID));
	}
	OnStreamChanged(vTaskStream);
}

int TaskProcess::StopTask()
{
	if (m_pCapChanDev)
	{
		m_pCapChanDev->Close();
		delete m_pCapChanDev;
		m_pCapChanDev = NULL;
	}
	DestroyAllStream();
	return m_iScreenIndex;
}

void TaskProcess::OnResolutionChanged(unsigned int nWndIndex, unsigned int nWidth, unsigned int nHeight)
{
	int x, y, cx, cy;
	TMInfo::GetTMWndPos(m_width, m_height, TM_TYPE_16X9, GetTemplateType(m_taskInfo.uTMID), nWndIndex, x, y, cx, cy);
	if (!g_config.local_fill)
		AdjustPlayerRect(nWidth, nHeight, x, y, cx, cy);
	MPVideo_SetScreenWndPos(m_iScreenIndex, nWndIndex, x, y, cx, cy);
}

HPTM_TM_SUBTYPE TaskProcess::GetTemplateType(unsigned int uTMID)
{
	switch (uTMID)
	{
	case 1: return TM_SUBTYPE_2_PIC_IN_PIC;
	case 2: return TM_SUBTYPE_3_L1_R2;
	case 3: return TM_SUBTYPE_4_2X2;
	case 4: return TM_SUBTYPE_4_L1_R3;
	default: return TM_SUBTYPE_0_NONE;
	}
}

void TaskProcess::OnSetTemplate(int width, int height, HPTM_TM_SUBTYPE type)
{
	int x, y, cx, cy;
	int nWndCnt = TMInfo::GetTMWndCount(type);
	MPVideo_SetScreenPos(m_iScreenIndex,0,0,width,height);
	MPVideo_SetScreenWndCount(m_iScreenIndex, nWndCnt);
	for (int iWndIndex = 0; iWndIndex < nWndCnt; iWndIndex++)
	{
		TMInfo::GetTMWndPos(width, height, TM_TYPE_16X9, type, iWndIndex, x, y, cx, cy);
		MPVideo_SetScreenWndPos(m_iScreenIndex, iWndIndex, x, y, cx, cy);
	}
}

void TaskProcess::AdjustPlayerRect(int width, int height, int& wnd_x, int& wnd_y, int& wnd_cx, int& wnd_cy)
{
	if (width <= 0 || height <= 0 || wnd_cx <= 0 || wnd_cy <= 0)
		return;

	if (1.0f*width/height > 1.0f*wnd_cx/wnd_cy)
	{
		//reduce height
		int cy = static_cast<int>(1.0f*wnd_cx*height/width);
		int diff = (wnd_cy-cy)/2;
		wnd_y += diff;
		wnd_cy -= diff*2;
	}
	else
	{
		//reduce width
		int cx = static_cast<int>(1.0f*wnd_cy*width/height);
		int diff = (wnd_cx-cx)/2;
		wnd_x += diff;
		wnd_cx -= diff*2;
	}
	LOG::INF("[HPCOMP]Adjust player rect as %f while the real ratio is %f\n", 1.0f*wnd_cx/wnd_cy, 1.0f*width/height);
}

void TaskProcess::OnStreamChanged(const VEC_STREAM& vStream)
{
	DestroyAllStream();
	AddAllStream(vStream);
}

CPlayChan* TaskProcess::GetPlayChan(const std::string& strStreamID) const
{
	CPlayChan* pPlayer = NULL;
	std::map<std::string, CPlayChan*>::const_iterator it = m_mapPlayer.find(strStreamID);
	if (it != m_mapPlayer.end())
		pPlayer = it->second;
	return pPlayer;
}

bool TaskProcess::AddSingleStream(const TASK_STREAM& taskStream)
{
	std::string strStreamID = taskStream.strDevID+taskStream.strCHLID;
	std::map<std::string, CPlayChan*>::iterator it = m_mapPlayer.find(strStreamID);
	if (it == m_mapPlayer.end())
	{
		CPlayChan* pPlayer = new CPlayChan(*this);
		if (0 != pPlayer->Open(taskStream))
		{
			delete pPlayer;
			return false;
		}
		pPlayer->SetScreenWnd(m_iScreenIndex, taskStream.ulWndIndex, 0);
		m_mapPlayer[strStreamID] = pPlayer;
		m_rNotify.OnAddSingleStream(strStreamID, pPlayer);
	}
	return true;
}

bool TaskProcess::AddAllStream(const VEC_STREAM& vTaskStream)
{
	for (VEC_STREAM::const_iterator it = vTaskStream.begin(); it != vTaskStream.end(); it++)
		if (!AddSingleStream(*it))
			return false;
	return true;
}

void TaskProcess::DestroySingleStream(const std::string& strStreamID)
{
	std::map<std::string, CPlayChan*>::iterator it = m_mapPlayer.find(strStreamID);
	if (it != m_mapPlayer.end())
	{
		CPlayChan* pPlayer = it->second;
		if (pPlayer)
		{
			m_rNotify.OnDestroySingleStream(strStreamID, pPlayer);
			pPlayer->Close();
			delete pPlayer;
			pPlayer = NULL;
			m_mapPlayer.erase(it);
		}
	}
}

void TaskProcess::DestroyAllStream()
{
	while (m_mapPlayer.size() > 0)
		DestroySingleStream(m_mapPlayer.begin()->first);
}