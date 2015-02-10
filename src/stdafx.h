// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#ifdef _WIN32

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0400		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0400	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0400	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��
#define USE_MPS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <string>
#include <direct.h>
#include <io.h>
#include <MMSystem.h>
#include "KBASE.h"

#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(lib,"../lib/KBASE.lib")
#pragma comment(lib,"../lib/NETEC.lib")
#pragma comment(lib, "../lib/AVCONMP.lib")
#pragma comment(lib,"../lib/AUDEC.lib")
#pragma comment(lib,"../lib/MPVideo.lib")
#else
#pragma comment(lib,"../lib/KBASE.lib")
#pragma comment(lib,"../lib/NETEC.lib")
#pragma comment(lib, "../lib/AVCONMP.lib")
#pragma comment(lib,"../lib/AUDEC.lib")
#pragma comment(lib,"../lib/MPVideo.lib")
#endif

#else
/*linuxϵͳͷ�ļ�*/
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/un.h>
#include <semaphore.h>
#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <stdarg.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/vfs.h>

/*����FIFO·��*/
#define FIFOPATH "/tmp/myfifo"

#ifdef AFX_EXT_CLASS
#undef AFX_EXT_CLASS
#endif
#define AFX_EXT_CLASS

#ifdef AFX_EXT_API
#undef AFX_EXT_API
#endif
#define AFX_EXT_API

#ifdef AVCON_API
#undef AVCON_API
#endif
#define AVCON_API

#ifdef CALLBACK
#undef CALLBACK
#endif
#define CALLBACK

typedef  unsigned char	BYTE ;
typedef  int			BOOL;
typedef  unsigned int	UINT;
typedef  void* 			LPVOID;
typedef  void* 			HANDLE;
typedef unsigned long   DWORD;
typedef unsigned short  WORD;
typedef long			LONG;
typedef char			CHAR;
typedef long long		__int64;


#define TRACE printf
#ifndef		TRUE
#define 	TRUE	1
#endif

#ifndef		FALSE
#define		FALSE	0
#endif

#define __stdcall
#define CALLBACK
#ifndef __HWND_defined
#define __HWND_defined
#endif
typedef long HWND;


#include "KBASE.h"
#endif

#define MAXBUF 4096
#define MAXLEN(a) (sizeof(a)-1)
#define MINLEN(a,b) (((a)>(b)) ? (b) : (a))

#include <time.h>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
using namespace std;
/*�ܿ�*/
#include "NETEC/XThreadBase.h"
#include <NETEC/NETEC.h>
#include <NETEC/NETEC_Client.h>
#include <NETEC/NETEC_Core.h>
#include <NETEC/NETEC_Node.h>
#include <NETEC/NETEC_Setting.h>
#include <NETEC/NETEC.h>
#include <NETEC/XCritSec.h>
#include <NETEC/XAutoLock.h>

////�ܿ�
#ifdef AFX_EXT_CLASS
#undef AFX_EXT_CLASS
#endif
#define AFX_EXT_CLASS

#ifdef AFX_EXT_API
#undef AFX_EXT_API
#endif
#define AFX_EXT_API

#include "Util.h"
#include "AllCmdDefine.h"

#define REQ_STREAM_NONE     0x00
#define REQ_STREAM_AUDIO    0x01
#define REQ_STREAM_VIDEO    0x02
#define REQ_STREAM_VSUB1    0x04
#define REQ_STREAM_VSUB2    0x08

typedef struct tagCONFIG
{    
    std::string local_serverid;
	unsigned short local_ability;
	unsigned short local_fill;
	unsigned short local_loglevel;

    std::string server_host;
    unsigned short server_port;	
} CONFIG;

extern CONFIG g_config;

#define MAX_PATH 260
#define Gsize (1024.00*1024.00*1024.00)
#define Msize (1024.00*1024.00)
unsigned long	get_file_len(const char *filename);
unsigned long	GetFileLen(const char* lpszFileName, int nMode=0);
unsigned long   get_disk_free( const string& strPath);
CString GetIniName();
CString GetCompVersion();
