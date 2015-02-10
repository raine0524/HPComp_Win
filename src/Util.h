 #pragma once

#include <string>
#include <vector>
using namespace std;

#include <KBASE/CritSec.h>
#include <KBASE/Utils.h>
#include "KXNCP.h"

#ifdef WIN32
#include <WinSock2.h>
#include <IPHlpApi.h>
#pragma comment(lib, "IPHlpApi.lib")
#endif

class UTIL
{
public:
	static string GetTime(long second=0);
	static string UTF8toGB2312(const char*utf8);
	static string GB2312toUTF8(const char*gb2312);
	static string GetIpByDomainName(const char*domain);
	static string GetRunPath(string file);
	static string GetInstallPath(string file);
	static bool COMBIN2DVRID(const char*ip, const int port, string&dvrid);
	static KCritSec&GetDBMutex(void);
	static unsigned long GetFrameNumberPerSecond(void);
	static void SplitToVector(const string& strCombin, const string& szKey, vector<string>& vecData);
	static string splitDvrid(string &dvrid, int &port);
	static std::string generic_recKey(const std::string& strRoomID,const std::string& strDevID,const std::string strChanID);
	static bool SplitRecKey(const std::string& strKey,std::string strRoomID,std::string strDevid,std::string strChanID);
	static std::string	GetStreamID(const std::string& strDevID,const std::string& strChanid, int nStreamType);
	static bool GetDevChanID(const std::string strStreamID,std::string strDevID,std::string strChanid);
	static unsigned long GenerateSSRC();
	static unsigned long GetTimestamp();
	static std::string GetMacAddr();
	static void SendToDevAgent(KCmdPacket& t, unsigned int nAgentID);
};
