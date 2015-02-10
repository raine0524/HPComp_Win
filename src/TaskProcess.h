#ifndef TASKPROCESS_H_
#define TASKPROCESS_H_

#include "stdafx.h"
#include "TMInfo.h"
#include "PlayChan.h"
#include "HPCompositeDefine.h"
#include "H264FrameParser.h"
#include "VIDEC/VIDEC_Header.h"
#include "VIDEC/VIDEC_CodecDef.h"
#include "AVCONMP/IMXEC_MediaChannel.h"
#include "MPVideo/MPVideo.h"

#ifdef WIN32
#define MAX_INPUT_SOURCES	4
#endif
#define MAX_OUTPUT_NUM	3

class TaskProcessNotify
{
public:
	virtual void OnAddSingleStream(const std::string& strStreamID, CPlayChan* pPlayer) = 0;
	virtual void OnDestroySingleStream(const std::string& strStreamID, CPlayChan* pPlayer) = 0;
};

class TaskProcess
:public AVCONMP_CapChanCallback
,public PlayChanNotify
{
public:
    TaskProcess(TaskProcessNotify& rNotify);
    ~TaskProcess() {}

public:
    bool StartTask(int iScreenIndex, const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream);
    int StopTask();
	void ModifyTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream);
	virtual void OnResolutionChanged(unsigned int nWndIndex, unsigned int nWidth, unsigned int nHeight);
	
private:
	HPTM_TM_SUBTYPE GetTemplateType(unsigned int uTMID);
	void OnSetTemplate(int width, int height, HPTM_TM_SUBTYPE type);
	void AdjustPlayerRect(int width, int height, int& wnd_x, int& wnd_y, int& wnd_cx, int& wnd_cy);
	void OnStreamChanged(const VEC_STREAM& vStream);
	CPlayChan* GetPlayChan(const std::string& strStreamID) const;
	bool AddSingleStream(const TASK_STREAM& taskStream);
	bool AddAllStream(const VEC_STREAM& vTaskStream);
	void DestroySingleStream(const std::string& strStreamID);
	void DestroyAllStream();

private:
	TaskProcessNotify& m_rNotify;
    COMP_TASKINFO m_taskInfo;
	std::map<std::string, CPlayChan*> m_mapPlayer;
	AVCONMP_CapChanDev* m_pCapChanDev;
	int m_iScreenIndex, m_width, m_height;
};

#endif  //TASKPROCESS_H_
