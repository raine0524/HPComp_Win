#include "CompManager.h"
#include <MPVideo/MPVideo.h>
#include <AUDEC/AUDEC_Engine.h>
#include <AUDEC/AUDEC_Conference.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <ShellAPI.h>
#include <atlbase.h>
#include <atlconv.h>
#include <TlHelp32.h>

const char* g_pXMLConf = "..\\conf\\hpcomp.xml";
bool ParseConfigFile(const std::string& strConfigFile);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	pid_t pid = GetCurrentProcessId();		//Only run one instance in current system
	TCHAR szTemp[MAX_PATH] = _T("");
	CString strAppPath;
	GetModuleFileName(NULL, szTemp, MAX_PATH);
	strAppPath = szTemp;
	int nPos = strAppPath.ReverseFind(_T('\\'))+1;
	CString strAppName = strAppPath.Right(strAppPath.GetLength()-nPos);
	if (!strAppName.IsEmpty())
	{
		bool flag = false;
		PROCESSENTRY32 pe;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe))
		{
			do 
			{
				pe.dwSize = sizeof(PROCESSENTRY32);
				if (!Process32Next(hSnapshot, &pe))
					break;
				if (pe.szExeFile == strAppName && pe.th32ProcessID != pid)
				{
					flag = true;
					break;
				}
			} while (true);
		}
		CloseHandle(hSnapshot);
		if (flag)
			return EXIT_SUCCESS;
	}

	AUDEC_GetEngine().AUDEC_SetSpeakerDev(-2);
	AUDEC_GetEngine().AUDEC_SetMicDev(-2);
	AUDEC_Engine::AUDEC_Init(0);
	AUDEC_Mixer::SetDefaultMixingPlayChan(1);

	MPVideo_Init();
	MPVideo_SetHardwareEncoderMaxCount(5);
	MPVideo_SetHardwareDecoderMaxCount(10);

	//Parse arguments and XML config file
	std::string strConfigFile(g_pXMLConf);
    if (strConfigFile.empty())
    {
        printf("[HPCOMP]Cannot find config file.\n");
        return 1;
    }
    if (!ParseConfigFile(strConfigFile))
    {
        printf("[HPCOMP]Config error: <avcond-comp>.\n");
        return 2;
    }

    //Setup log level
    if (!LOG::START((LOG_LEVEL)g_config.local_loglevel))
    {
        LOG::STOP();
        printf("[HPCOMP]Logger start failed.\n");
        return 3;
    }

    //Startup composite service
    CompManager compMgr;
    if (!compMgr.Start())
    {
        compMgr.Stop();
        printf("[HPCOMP]Local server start failed.\n");
        return 4;
    }

    while (true)
    {
		//TODO: 
        Pause(100);
    }
    compMgr.Stop();
	MPVideo_Terminate();
    LOG::STOP();
    return EXIT_SUCCESS;
}

//--------------------------------------------------------
//Parse XML config file
bool ParseConfigFile(const std::string& strConfigFile)
{
    //config
    XMLNode xConfigNode = XMLNode::OpenFile(strConfigFile.c_str(), "config");
    if (xConfigNode.IsEmpty())
    {
        printf("[HPCOMP]Config error <config>.\n");
        return false;
    }

    //config/local
    XMLNode xLocalNode = xConfigNode.GetChildNode("local");
    if (xLocalNode.IsEmpty())
    {
        printf("[HPCOMP]Config error <config/local>.\n");
        return false;
    }
    g_config.local_serverid	= NONULLSTR(xLocalNode.GetAttribute("id"));
	g_config.local_ability		= STR2USHORT(NONULLSTR(xLocalNode.GetAttribute("ability")));
	g_config.local_fill				= STR2USHORT(NONULLSTR(xLocalNode.GetAttribute("fill")));

    //config/local/log
    XMLNode xLocalLogNode = xLocalNode.GetChildNode("log");
    if (xLocalLogNode.IsEmpty())
    {
        printf("[HPCOMP]Config error <config/local/log>.\n");
        return false;
    }
    g_config.local_loglevel = STR2USHORT(NONULLSTR(xLocalLogNode.GetAttribute("level")));

    //config/server
    XMLNode xServerNode = xConfigNode.GetChildNode("server");
    if (xServerNode.IsEmpty())
    {
        printf("[HPCOMP]Config error <config/server>.\n");
        return false;
    }
    g_config.server_host = NONULLSTR(xServerNode.GetAttribute("host"));
    g_config.server_port = STR2USHORT(NONULLSTR(xServerNode.GetAttribute("port")));

    return true;
}
