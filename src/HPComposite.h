#ifndef HPCOMPOSITE_H_
#define HPCOMPOSITE_H_

#include <set>
#include <map>
#include "stdafx.h"
#include "TaskProcess.h"

#define MAX_TASK_NUM	1000

typedef std::map<std::string, TaskProcess*> MAP_TASKPROC;
typedef std::multimap<std::string, CPlayChan*> MMAP_PLAYCHAN;

class HPComposite : public TaskProcessNotify
{
public:
    HPComposite() {}
    virtual ~HPComposite() {}

public:
    bool StartTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream);
    void StopTask(const std::string& strTaskID);
	bool ModifyTask(const COMP_TASKINFO& stTaskInfo, const VEC_STREAM& vTaskStream);
    void StopAllTask();
	bool OnCHLChanged(const TASK_STREAM& stTaskStream);

public:
	virtual void OnAddSingleStream(const std::string& strStreamID, CPlayChan* pPlayer);
	virtual void OnDestroySingleStream(const std::string& strStreamID, CPlayChan* pPlayer);

private:
	void FindStream(const std::string& strStreamID, std::set<CPlayChan*>& setStream);
	int CreateScreenIndex();
	void DestroyScreenIndex(int index);

private:
    MAP_TASKPROC m_mapTask;
    MMAP_PLAYCHAN m_mmapStream;
	std::set<int> m_setIndex;
};

#endif  //HPCOMPOSITE_H_
