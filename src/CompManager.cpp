#include "HPCompositeDefine.h"
#include "CompManager.h"

CONFIG g_config;

void ActiveChecker::OnTimerEvent(unsigned int nEventID)
{
	if (CHECK_ACTIVE == nEventID)
		WritePrivateProfileString(_T("active"), _T("value"), _T("1"), GetIniName());
}

CompManager::CompManager()
:m_bWantToStop(false)
,m_pHPComposite(NULL)
,m_pActiveChecker(NULL)
{
}

bool CompManager::Start()
{
    NETEC_Core::Start();
#ifndef WIN32
    NETEC_Setting::SetVideoProtocolType(NETEC_Setting::PT_TCP);
    NETEC_Setting::SetAudioProtocolType(NETEC_Setting::PT_TCP);
#endif
    NETEC_Node::SetServerIP(g_config.server_host.c_str());
    NETEC_Node::SetServerPort(g_config.server_port);
    NETEC_Node::Start();

    m_pHPComposite = new HPComposite();
    if (!m_pHPComposite)
    {
		LOG::ERR("[HPCOMP]Create HPComposite failure.\n");
        return false;
    }
    StartThread();
#ifdef USE_MPS
	WritePrivateProfileString(_T("version"), _T("value"), GetCompVersion(), GetIniName());
#endif			//USE_MPS
    return true;
}

void CompManager::Stop()
{
	Logout();
	NETEC_Node::Stop();
	NETEC_Core::Stop();

	m_bWantToStop = true;
	WaitForStop();
	ClearBufferPool();    
	if (m_pHPComposite)
	{		
		delete m_pHPComposite;
		m_pHPComposite = NULL;
	}
}

void CompManager::OnNETEC_NodeConnectStatusChanged(NETEC_Session::CONNECT_STATUS cs)
{
    switch(cs)
    {
    case NETEC_Session::CS_CONNECTING:
		{
			LOG::INF("[HPCOMP]Connecting to router.\n");
#ifdef USE_MPS
			WritePrivateProfileString(_T("CompStatus"), _T("Login"), _T("未激活"), GetIniName());
#endif
		}
        break;
    case NETEC_Session::CS_FAILED:
		{
			LOG::INF("[HPCOMP]Connect router failed.\n");
#ifdef USE_MPS
			WritePrivateProfileString(_T("CompStatus"), _T("Login"), _T("未激活"), GetIniName());
			WritePrivateProfileString(_T("Error"), _T("Text"), _T("连接失败，请检查配置是否正确"), GetIniName());
#endif
		}
        break;
    case NETEC_Session::CS_RESTARTED:
    case NETEC_Session::CS_CONNECTED:
        {
            LOG::INF("[HPCOMP]Connect router connected.\n");
            Logout();
            Login();
        }
        break;
    case NETEC_Session::CS_DISCONNECTED:
		{
			LOG::INF("[HPCOMP]Disconnected from router.\n");
			m_pHPComposite->StopAllTask();
#ifdef USE_MPS
			WritePrivateProfileString(_T("CompStatus"), _T("Login"), _T("未激活"), GetIniName());
			WritePrivateProfileString(_T("Error"), _T("Text"), _T("连接中断，请检查MCU是否在线"), GetIniName());
#endif
		}
        break;
    case NETEC_Session::CS_BUSY:
		{
			LOG::INF("[HPCOMP]Session is busy.\n");
#ifdef USE_MPS
			WritePrivateProfileString(_T("CompStatus"), _T("Login"), _T("未激活"), GetIniName());
#endif
		}
        break;
    case NETEC_Session::CS_RECONNECTED:
        LOG::INF("[HPCOMP]HPComp reconnected.\n");
        break;
    case NETEC_Session::CS_IDLE:
        LOG::INF("[HPCOMP]Session is idle.\n");
        break;
    default:
        LOG::INF("[HPCOMP]Unknown comp server status(%d).\n", (int)cs);
    }
}

void CompManager::OnNETEC_NodeReceivedFromAgent(const char* cszDomain, unsigned int nAgentID, const char* pData, int nLen)
{
    if (nLen <= 11 || pData == NULL || *(pData+nLen-1) != '\0')
        return;
    LOG::DBG("[HPCOMP]From Agent MON: %s", pData);
    KCmdPacket rPacket(pData, nLen);
    PushBufferPool(rPacket);
}

void CompManager::Login()
{
    KCmdPacket out("VIDEOCOMP", MON_LOGIN, g_config.local_serverid);
    out.SetAttrib("MCUID:", NETEC_Node::GetMCUID());
    out.SetAttrib("MCUADDR:", NETEC_Node::GetMCUIP());
    out.SetAttribUS("MCUPORT:", NETEC_Node::GetServerPort());
    out.SetAttrib("NATADDR:", NETEC_Node::GetNATIP());
    out.SetAttrib("LOCALADDR:", NETEC_Node::GetLocalIP());
    out.SetAttribUS("LOCALPORT:", NETEC_Node::GetLocalPort());
    out.SetAttrib("NODEID", NETEC_Node::GetNodeID());
	out.SetAttrib("MACADDR", UTIL::GetMacAddr());
    out.SetAttribUS("DEVICETYPE", SERVER_COM);
	out.SetAttrib("SERVERIP", NETEC_Node::GetLocalIP());
	out.SetAttrib("VERSION",AVCON_SERVER_VERSION);
	out.SetAttrib("SKILL", INT2STR(g_config.local_ability));
    UTIL::SendToDevAgent(out, DEV_AGENT_ID);
}

void CompManager::Logout()
{
    LOG::INF("[HPCOMP]logout stop all composite task.\n");
    KCmdPacket out("VIDEOCOMP", MON_LOGOUT, g_config.local_serverid);
    out.SetAttrib("STATUS", "offline");
    UTIL::SendToDevAgent(out, DEV_AGENT_ID);
#ifdef USE_MPS
	if (m_pActiveChecker)
	{
		m_pActiveChecker->StopTimer();
		delete m_pActiveChecker;
		m_pActiveChecker = NULL;
	}
	WritePrivateProfileString(_T("active"), _T("value"), _T("0"), GetIniName());
#endif
}

void CompManager::PushBufferPool(KCmdPacket& rPacket)
{
    KAutoLock l(m_secBufferPool);
    KCmdPacket* pPacket = new KCmdPacket(rPacket);
    m_lstBufferPool.push(pPacket);
}

void CompManager::ClearBufferPool()
{
    KAutoLock l(m_secBufferPool);
    while (m_lstBufferPool.size() > 0)
    {
        KCmdPacket* pCmdPacket = (KCmdPacket*)m_lstBufferPool.front();
        m_lstBufferPool.pop();
        if (pCmdPacket)
        {
            delete pCmdPacket;
            pCmdPacket = NULL;
        }
    }
}

void CompManager::ThreadProcMain()
{
    while (!m_bWantToStop)
    {
        ProcessBufferPool();
        Pause(1);
    }
	m_pHPComposite->StopAllTask();
}

bool CompManager::ProcessBufferPool()
{
    KCmdPacket* pPacket = NULL;
    {
        KAutoLock l(m_secBufferPool);
        if (m_lstBufferPool.empty())
            return false;
        pPacket = (KCmdPacket*)m_lstBufferPool.front();
        m_lstBufferPool.pop();
    }
    if (pPacket)
    {
        OnDispatchCmd(*pPacket);
        delete pPacket;
        pPacket = NULL;
    }
    return true;
}

void CompManager::OnDispatchCmd(KCmdPacket& in)
{
    int nCMD = in.GetCMD();
    switch(nCMD)
    {
    case MON_LOGIN:				OnDispatchLogin(in);				break;
    case REC_HPRECLOGOUT:	OnDispatchLogout(in);					break;
    case REC_STARTTASK:		OnDispatchStartTask(in);				break;
    case REC_STOPTASK:			OnDispatchStopTask(in);				break;
	case REC_MODIFYTASK:		OnDispatchModifyTask(in);			break;
    case MON_CHLONLINE:		OnDispatchChangeChannel(in);	        break;
    default: LOG::ERR("[HPCOMP]Received an unknown command(%d).\n", nCMD);
    }
}

void CompManager::OnDispatchLogin(KCmdPacket& t)
{
    //login successfully
    unsigned int unErrCode = t.GetAttrib("ERRORCODE").AsUnsignedInt();
    if (unErrCode != ERR_NO)
    {
        LOG::ERR("[HPCOMP]Login Error Code: %d.\n", unErrCode);
        exit(EXIT_FAILURE);
    }
    LOG::INF("[HPCOMP]HPComp server login.\n");
#ifdef USE_MPS
	WritePrivateProfileString(_T("CompStatus"), _T("Login"), _T("已激活"), GetIniName());
	WritePrivateProfileString(_T("Error"), _T("Text"), _T(""), GetIniName());
	WritePrivateProfileString(_T("active"), _T("value"), _T("1"), GetIniName());
	if (!m_pActiveChecker)
	{
		m_pActiveChecker = new ActiveChecker();
		m_pActiveChecker->SetTimerEvent(CHECK_ACTIVE, CHECK_ACTIVE);
		m_pActiveChecker->StartTimer();
	}
#endif
}

void CompManager::OnDispatchLogout(KCmdPacket& t)
{
}

void CompManager::GetTaskInfoAndStream(KCmdPacket& t, COMP_TASKINFO& taskInfo, VEC_STREAM& vStream)
{
	std::string strSize;
	taskInfo.strTaskID				= t.GetAttrib("TASKID").AsString();
	taskInfo.strTaskName		= t.GetAttrib("TASKNAME").AsString();
	taskInfo.bUseHigh			= (t.GetAttrib("BHIGHSTREAM").AsUnsignedShort() != 0?true:false);
	strSize	= t.GetAttrib("HIGHSIZE").AsString();
	taskInfo.uHighWidth		= STR2UINT(strSize.substr(0, strSize.find("X")));
	taskInfo.uHighHeight		= STR2UINT(strSize.substr(strSize.find("X")+1, std::string::npos));
	taskInfo.uHighBitrate		= t.GetAttrib("HIGHWIDTH").AsUnsignedInt();
	taskInfo.bUseMid				= (t.GetAttrib("BMIDSTREAM").AsUnsignedShort() != 0?true:false);
	strSize	= t.GetAttrib("MIDSIZE").AsString();
	taskInfo.uMidWidth			= STR2UINT(strSize.substr(0, strSize.find("X")));
	taskInfo.uMidHeight		= STR2UINT(strSize.substr(strSize.find("X")+1, std::string::npos));
	taskInfo.uMidBitrate		= t.GetAttrib("MIDWIDTH").AsUnsignedInt();
	taskInfo.bUseLow			= (t.GetAttrib("BLOWSTREAM").AsUnsignedShort() != 0?true:false);
	strSize	= t.GetAttrib("LOWSIZE").AsString();
	taskInfo.uLowWidth		= STR2UINT(strSize.substr(0, strSize.find("X")));
	taskInfo.uLowHeight		= STR2UINT(strSize.substr(strSize.find("X")+1, std::string::npos));
	taskInfo.uLowBitrate		= t.GetAttrib("LOWWIDTH").AsUnsignedInt();
	taskInfo.uTMID					= t.GetAttrib("MIXTMID").AsUnsignedInt();

	CMD_ITEM_LST lstItems = t.GetItemList();
	for (CMD_ITEM_LST::const_iterator it = lstItems.begin(); it != lstItems.end(); it++)
	{
		KCmdItem item((std::string)*it);
		TASK_STREAM taskStream;
		taskStream.strNodeID		= item.GetAttrib("NODEID").AsString();
		taskStream.strDevID			= item.GetAttrib("DEVID").AsString();
		taskStream.strCHLID			= item.GetAttrib("CHLID").AsString();
		taskStream.strMCUID			= item.GetAttrib("MCUID").AsString();
		taskStream.strMCUAddr	= item.GetAttrib("MCUADDR").AsString();
		taskStream.usMCUPort		= item.GetAttrib("MCUPORT").AsUnsignedShort();
		taskStream.strNATAddr		= item.GetAttrib("NATADDR").AsString();
		taskStream.strLocalAddr	= item.GetAttrib("LOCALADDR").AsString();
		taskStream.usLocalPort		= item.GetAttrib("LOCALPORT").AsUnsignedShort();
		taskStream.ulAudioID			= item.GetAttrib("AUDIOCID").AsUnsignedLong();
		taskStream.ulVideoID			= item.GetAttrib("VIDEOCID").AsUnsignedLong();
		taskStream.ulWndIndex		= item.GetAttrib("WNDINDEX").AsUnsignedLong();
		vStream.push_back(taskStream);
	}
}

void CompManager::OnDispatchStartTask(KCmdPacket& t)
{
	COMP_TASKINFO taskInfo;
	VEC_STREAM vStream;
	GetTaskInfoAndStream(t, taskInfo, vStream);
	m_pHPComposite->StartTask(taskInfo, vStream);
}

void CompManager::OnDispatchStopTask(KCmdPacket& t)
{
    std::string strTaskID = t.GetAttrib("TASKID").AsString();
    m_pHPComposite->StopTask(strTaskID);
}

void CompManager::OnDispatchModifyTask(KCmdPacket& t)
{
	COMP_TASKINFO taskInfo;
	VEC_STREAM vStream;
	GetTaskInfoAndStream(t, taskInfo, vStream);
	if (!m_pHPComposite->ModifyTask(taskInfo, vStream))
		LOG::ERR("[HPCOMP]Modify task failed, since the task does not exist!\n");
}

void CompManager::OnDispatchChangeChannel(KCmdPacket& t)
{
    TASK_STREAM taskStream;
    taskStream.strNodeID		= t.GetAttrib("NODEID").AsString();
	taskStream.strDevID			= t.GetAttrib("DEVID").AsString();
    taskStream.strCHLID			= t.GetAttrib("CHANNELID").AsString();
    taskStream.strMCUID			= t.GetAttrib("MCUID").AsString();
    taskStream.strMCUAddr		= t.GetAttrib("MCUADDR").AsString();
    taskStream.usMCUPort		= t.GetAttrib("MCUPORT").AsUnsignedShort();
    taskStream.strNATAddr		= t.GetAttrib("NATADDR").AsString();
    taskStream.strLocalAddr		= t.GetAttrib("LOCALADDR").AsString();
    taskStream.usLocalPort		= t.GetAttrib("LOCALPORT").AsUnsignedShort();
    taskStream.ulAudioID			= t.GetAttrib("AUDIOCID").AsUnsignedLong();
    taskStream.ulVideoID			= t.GetAttrib("VIDEOCID").AsUnsignedLong();
    m_pHPComposite->OnCHLChanged(taskStream);
}
