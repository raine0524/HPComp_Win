#ifndef COMPMANAGER_H_
#define COMPMANAGER_H_

#include "stdafx.h"
#include "NETEC/NETEC_Node.h"
#include "NETEC/NETEC_Core.h"
#include "HPComposite.h"
#include "AllCmdDefine.h"
#include "HPLiveDefine.h"

#define CHECK_ACTIVE	1000		//1s

class ActiveChecker : public KTimer
{
public:
	virtual void OnTimerEvent(unsigned int nEventID);
};

class CompManager
:public NETEC_Node
,public KThread
{
public:
    CompManager();
    virtual ~CompManager() {}

public:
    bool Start();
    void Stop();

public:
    //Callback function
    virtual void OnNETEC_NodeConnectStatusChanged(NETEC_Session::CONNECT_STATUS cs);
    virtual void OnNETEC_NodeReceivedFromAgent(const char* cszDomain, unsigned int nAgentID, const char* pData, int nLen);

private:
    void Login();
    void Logout();
    void ThreadProcMain();
    void PushBufferPool(KCmdPacket& rPacket);
    void ClearBufferPool();
    bool ProcessBufferPool();

private:
	void GetTaskInfoAndStream(KCmdPacket& t, COMP_TASKINFO& taskInfo, VEC_STREAM& vStream);
    void OnDispatchCmd(KCmdPacket& in);
    void OnDispatchLogin(KCmdPacket& t);
    void OnDispatchLogout(KCmdPacket& t);
    void OnDispatchStartTask(KCmdPacket& t);
    void OnDispatchStopTask(KCmdPacket& t);
	void OnDispatchModifyTask(KCmdPacket& t);
    void OnDispatchChangeChannel(KCmdPacket& t);

private:
    bool m_bWantToStop;
    KCritSec m_secBufferPool;
    HPComposite* m_pHPComposite;
	ActiveChecker* m_pActiveChecker;
    BUFFER_QUEUE m_lstBufferPool;
};
#endif      //COMPMANAGER_H_
