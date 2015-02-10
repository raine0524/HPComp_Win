#include "HPComposite.h"

bool HPComposite::StartTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream)
{
	MAP_TASKPROC::iterator it = m_mapTask.find(stTaskInfo.strTaskID);
	if (it != m_mapTask.end())
		return false;

	bool bRet;
    TaskProcess* pProcess = new TaskProcess(*this);
    if (pProcess)
    {
        bRet = pProcess->StartTask(CreateScreenIndex(), stTaskInfo, vTaskStream);
		if (bRet)
			m_mapTask[stTaskInfo.strTaskID] = pProcess;
		else
		{
			delete pProcess;
			pProcess = NULL;
		}
    }

	KCmdPacket out("DEVAGENT", MON_PORTINFO, g_config.local_serverid);
	out.SetAttribUN("USERPORT", m_mapTask.size());
	UTIL::SendToDevAgent(out,DEV_AGENT_ID);
    return bRet;
}

bool HPComposite::ModifyTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream)
{
	MAP_TASKPROC::iterator it = m_mapTask.find(stTaskInfo.strTaskID);
	if (it != m_mapTask.end())
	{
		it->second->ModifyTask(stTaskInfo, vTaskStream);
		return true;
	}
	return false;
}

void HPComposite::StopTask(const std::string& strTaskID)
{
	MAP_TASKPROC::iterator it = m_mapTask.find(strTaskID);
	if (it != m_mapTask.end())
	{
		int index = it->second->StopTask();
		delete it->second;
		DestroyScreenIndex(index);
		m_mapTask.erase(it);
	}

	KCmdPacket out("DEVAGENT", MON_PORTINFO, g_config.local_serverid);
	out.SetAttribUN("USERPORT", m_mapTask.size());
	UTIL::SendToDevAgent(out,DEV_AGENT_ID);
}

void HPComposite::StopAllTask()
{
    while (m_mapTask.size() > 0)
        StopTask(m_mapTask.begin()->first);
}

bool HPComposite::OnCHLChanged(const TASK_STREAM& stTaskStream)
{
    std::string strStreamID = stTaskStream.strDevID+stTaskStream.strCHLID;
    LOG::INF("[HPCOMP]OnCHLChanged: strStreamID = %s\n", strStreamID.c_str());
	std::set<CPlayChan*> setPlayer;
	FindStream(strStreamID, setPlayer);
	if (setPlayer.empty())
	{
		LOG::ERR("[HPCOMP]In process of changing the channel that does not exist.\n");
		return false;
	}

	bool bConn = true;
	for (std::set<CPlayChan*>::iterator it = setPlayer.begin(); it != setPlayer.end(); it++)
	{
		CPlayChan* pPlayer = *it;
		if (-1 == pPlayer->Open(stTaskStream))
			bConn = false;
	}

    if (!bConn)
        LOG::ERR("[HPCOMP]Cannot connect to devices that is used to receive real stream\n");
	return bConn;
}

void HPComposite::FindStream(const std::string& strStreamID, std::set<CPlayChan*>& setPlayer)
{
	std::pair<MMAP_PLAYCHAN::iterator, MMAP_PLAYCHAN::iterator> ret = m_mmapStream.equal_range(strStreamID);
	for (MMAP_PLAYCHAN::iterator it = ret.first; it != ret.second; ++it)
		setPlayer.insert(it->second);
}

int HPComposite::CreateScreenIndex()
{
	int index;
	for (index = 0; index < MAX_TASK_NUM; index++)
		if (m_setIndex.find(index) == m_setIndex.end())
			break;
	m_setIndex.insert(index);
	return index;
}

void HPComposite::DestroyScreenIndex(int index)
{
	std::set<int>::iterator it = m_setIndex.find(index);
	if (it != m_setIndex.end())
		m_setIndex.erase(it);
}

void HPComposite::OnAddSingleStream(const std::string& strStreamID, CPlayChan* pPlayer)
{
	m_mmapStream.insert(std::pair<std::string, CPlayChan*>(strStreamID, pPlayer));
}

void HPComposite::OnDestroySingleStream(const std::string& strStreamID, CPlayChan* pPlayer)
{
	std::pair<MMAP_PLAYCHAN::iterator, MMAP_PLAYCHAN::iterator> ret = m_mmapStream.equal_range(strStreamID);
	for (MMAP_PLAYCHAN::iterator iter = ret.first; iter != ret.second; ++iter)
		if (iter->second == pPlayer)
		{
			m_mmapStream.erase(iter);
			break;
		}
}
