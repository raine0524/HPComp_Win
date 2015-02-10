#include "stdafx.h"
#include "KBASE.h"
#include "Util.h"

#ifdef WIN32
//#include "KRTL32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#define REC_KEY_SEPTOR ^

string UTIL::GetTime(long second)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	timep -= second;
	char buffer[32]={0};
	p=localtime(&timep);
	snprintf(buffer, MAXLEN(buffer),"%04d%02d%02d%02d%02d%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	return buffer;
}

#ifdef _WIN32
#include <KBASE.h>
#else
#include <iconv.h>
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	size_t in_len = (size_t)inlen;
	size_t out_len = (size_t)outlen;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0)
	{
		iconv_close(cd);
		return -1;
	}
	memset(outbuf,0,out_len);
	if (iconv(cd,pin,&in_len,pout,&out_len)==-1)
	{
		iconv_close(cd);
		return -1;
	}
	iconv_close(cd);
	return 0;
}
#endif

string UTIL::UTF8toGB2312(const char*utf8)
{
#ifdef WIN32
	USES_CONVERSION;
	return W2A(A2U(utf8));
	return "";
#else
	string gb2312 = utf8;
	int utf_len = strlen(utf8);
	int gb_len = utf_len*2 + 1;
	char* gb_buf = new char[gb_len];
	if (gb_buf)
	{
		code_convert("utf-8","gb2312",(char*)utf8, utf_len, gb_buf, gb_len);
		gb2312 = gb_buf;
		delete [] gb_buf;
	}
	return gb2312;
#endif
}

string UTIL::GB2312toUTF8(const char*gb2312)
{
#ifdef WIN32
	USES_CONVERSION;
	return U2A(A2W(gb2312));
	return "";
#else
	string utf = gb2312;
	int gb_len = strlen(gb2312);
	int utf_len = gb_len * 2 + 1;
	char * utf_buf = new char[utf_len];
	if (utf_buf)
	{
		code_convert("gb2312","utf-8",(char*)gb2312,gb_len,utf_buf,utf_len);
		utf = utf_buf;
		delete [] utf_buf;
	}
	return utf;
#endif
}

string UTIL::GetIpByDomainName(const char*domain)
{
#ifdef _WIN32
    struct hostent * phost;
	phost = gethostbyname(domain);
	if (phost==NULL)
	{
		return domain;
	}
	return inet_ntoa(*((struct in_addr *)phost->h_addr));
#else
	return "";
#endif
}

string UTIL::GetRunPath(string file)
{
	string path = "";
#ifdef WIN32
	char buf[MAXBUF] = {0};
	GetModuleFileNameA(NULL, buf, MAXBUF-1);
	*(strrchr(buf,'\\')) = NULL;
	snprintf(buf, MAXLEN(buf), "%s\\%s", buf, file.c_str());
	path = buf;
#else
	#ifdef AVCONMTSR_ENABLE
		path = "/usr/local/avconmtsr/" + file;
	#else
		path = "/usr/local/avcond/bin/" + file;
	#endif
#endif
	return path;
}

string UTIL::GetInstallPath(string file)
{
	string path = "";
#ifdef WIN32

#ifdef _DEBUG
	char buf[MAXBUF] = {0};
	GetModuleFileNameA(NULL, buf, MAXBUF-1);
	*(strrchr(buf,'\\')) = NULL;//debug
	*(strrchr(buf,'\\')) = NULL;//bin
	*(strrchr(buf,'\\')) = NULL;
	snprintf(buf, MAXLEN(buf), "%s\\%s", buf, file.c_str());
	path = buf;
#else
	char buf[MAXBUF] = {0};
	GetPrivateProfileStringA("AVCON6_MGW", "APP_Path", "", buf, sizeof(buf)-1, "D:\\AVCONXPE\\HpVersion.ini");
	path = buf;
#endif

#else
	#ifdef AVCONMTSR_ENABLE
		path = "/usr/local/avconmtsr/" + file;
	#else
		path = "/usr/local/avcond/" + file;
	#endif
#endif

	return path;
}

bool UTIL::COMBIN2DVRID(const char*ip, const int port, string&dvrid)
{
	char buf[256] = {0};
	sprintf(buf,"%s:%d", ip, port);
	dvrid = buf;
	return true;
}

KCritSec g_MutexForDB;
KCritSec& UTIL::GetDBMutex(void)
{
	return g_MutexForDB;
}

static unsigned long s_ulDbgFrameCount = 0;
static unsigned long s_ulDbgFrameTimeStamp = 0;
unsigned long UTIL::GetFrameNumberPerSecond(void)
{
	unsigned long ulFrameNumberPerSecond = 0;

	if (s_ulDbgFrameTimeStamp==0)
	{
		s_ulDbgFrameTimeStamp = GetTimestamp();
	}

	unsigned long ulCurrentTimeStamp = GetTimestamp();
	if ((ulCurrentTimeStamp-s_ulDbgFrameTimeStamp)>=1000)
	{
		s_ulDbgFrameTimeStamp = GetTimestamp();

		ulFrameNumberPerSecond = s_ulDbgFrameCount;
		s_ulDbgFrameCount=0;
	}

	s_ulDbgFrameCount++;

    return ulFrameNumberPerSecond;
}

void UTIL::SplitToVector(const string& strCombin, const string& szKey, vector<string>& vecData)
{
	vecData.clear();
	string strItem, strTemp = strCombin;

	size_t nFound = strTemp.find(szKey);
	while (nFound>=0)
	{
		strItem = strTemp.substr(0, nFound);

		if (!strItem.empty())
		{
			vecData.push_back(strItem);
		}

		strTemp = strTemp.substr(nFound+1, strTemp.length()-nFound-1);
		nFound = (int)strTemp.find(szKey);
	}
	if (!strTemp.empty())
	{
		vecData.push_back(strTemp);
	}
}

string UTIL::splitDvrid(string &dvrid, int &port)
{
	string sPort="",sIP="";
	size_t pos = dvrid.find(":");
	if (pos!=-1)
	{
		sIP = dvrid.substr(0,pos);
		sPort = dvrid.substr(pos+1);
		port = atoi(sPort.c_str());
		return sIP;
	}
	port = 0;
	return "";
}

std::string UTIL::generic_recKey(const std::string& strRoomID,const std::string& strDevID,const std::string strChanID)
{
	char buffer[1024]={0};
	sprintf(buffer,"%s^%s^%s",strRoomID.c_str(),strDevID.c_str(),strChanID.c_str());
	return buffer;
}

bool UTIL::SplitRecKey(const std::string& strKey,std::string strRoomID,std::string strDevid,std::string strChanID)
{
	size_t nLpos = strKey.find_first_of("^");
	size_t nRpos = strKey.find_last_of("^");
	if (nLpos !=-1 && nRpos !=-1 && nLpos !=nRpos)
	{
		strRoomID	= strKey.substr(0,nLpos);
		strDevid	= strKey.substr(nLpos+1,nRpos-nLpos-1);
		strChanID	= strKey.substr(nRpos+1,strKey.size()-nRpos-1);
		return true;
	}
	return false;
}


std::string	UTIL::GetStreamID(const std::string& strDevID,const std::string& strChanid, int nStreamType)
{
	char buffer[1024]={0};
	if (1 != nStreamType && 2 != nStreamType)
	{
		nStreamType = 0;
	}
	sprintf(buffer,"%s^%s^%d",strDevID.c_str(),strChanid.c_str(), nStreamType);
	return buffer;
}

bool UTIL::GetDevChanID(const std::string strStreamID,std::string strDevID,std::string strChanid)
{
	size_t nLpos = strStreamID.find_first_of("^");
	if (nLpos !=-1)
	{
		strDevID	= strStreamID.substr(0,nLpos);
		strChanid	= strStreamID.substr(nLpos+1,strStreamID.size()-nLpos-1);
		return true;
	}
	return false;
}

unsigned long UTIL::GenerateSSRC()
{
#ifdef WIN32
	LARGE_INTEGER frequence, privious;
	if(!QueryPerformanceFrequency( &frequence))//取高精度运行计数器的频率
	{
		return timeGetTime();
	}

	if (!QueryPerformanceCounter( &privious ))
	{
		return timeGetTime();
	}

	DWORD dwRet = (DWORD)(1000000 * privious.QuadPart / frequence.QuadPart ); //换算到微秒数

	return dwRet;//微秒
#else
	struct timeval now;
	gettimeofday(&now,NULL);
	return now.tv_sec*1000+now.tv_usec;
#endif
}

unsigned long UTIL::GetTimestamp()
{
#ifdef WIN32
    return ::timeGetTime();     //millisec
#else
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec*1000+now.tv_usec/1000;
#endif
}

std::string UTIL::GetMacAddr()
{
	char buf[32];
#ifdef	WIN32
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (NULL == pAdapterInfo)
	{
		LOG::ERR("[HPCOMP]UTIL::GetMacAddr allocate memory failed.\n");
		exit(EXIT_FAILURE);
	}
	if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if (NO_ERROR == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			//Be sure the type of net is ethernet
			if (MIB_IF_TYPE_ETHERNET != pAdapter->Type)
				continue;
			//Confirm the mac is like 00-00-00-00-00-00
			if (6 != pAdapter->AddressLength)
				continue;
			sprintf(buf, "%02x%02x%02x%02x%02x%02x",
				(int)pAdapter->Address[0],
				(int)pAdapter->Address[1],
				(int)pAdapter->Address[2],
				(int)pAdapter->Address[3],
				(int)pAdapter->Address[4],
				(int)pAdapter->Address[5]);
			break;
		}
	}
	free(pAdapterInfo);
#else
	int sock;
	struct ifreq ifr;
	memset(&buf, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name)-1);
	if (-1 == (sock = socket(AF_INET, SOCK_STREAM, 0)))
	{
		LOG::ERR("[HPCOMP]Create socket file failed.\n");
		exit(EXIT_FAILURE);
	}
	if ((ioctl(sock, SIOCGIFHWADDR, &ifr)) < 0)
	{
		LOG::ERR("[HPCOMP]GetMacAddr through sdk failed.\n");
		exit(EXIT_FAILURE);
	}
	sprintf(buf, "%02x%02x%0x2%02x%02x%02x",
		(unsigned char)ifr.ifr_hwaddr.sa_data[0],
		(unsigned char)ifr.ifr_hwaddr.sa_data[1],
		(unsigned char)ifr.ifr_hwaddr.sa_data[2],
		(unsigned char)ifr.ifr_hwaddr.sa_data[3],
		(unsigned char)ifr.ifr_hwaddr.sa_data[4],
		(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	LOG::INF("[HPCOMP]Local Mac Address is %s.\n", buf);
	close(sock);    
#endif
	return std::string(buf);
}

void UTIL::SendToDevAgent(KCmdPacket& t, unsigned int nAgentID)
{
    std::string data = t.GetString();
    NETEC_Node::SendDataToAgent("", nAgentID, data.c_str(), (int)data.length()+1);
    LOG::INF("[HPCOMP]send dev-agent: %s", data.c_str());
}
