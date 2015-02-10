#pragma once

#define RECOVER_TIME 5   //����ʼʱ������5�����򲻻ָ�
#define ENDTIME      1   //�������ʱ��С��1������ִ��
#define MAX_VIDEO_W	 1920
#define MAX_VIDEO_H	 1080
#define MIN_VIDEO_W  320
#define MIN_VIDEO_H	 240

typedef enum tm_type
{
	TM_2_PIC_IN_PIC		= 1,
	TM_3_L1_R2			= 2,
	TM_4_2X2			= 3,
	TM_4_L1_R3			= 4,
};

typedef enum protocol_type
{
	protocol_type_error = -1,
	protocol_type_ebox  = 0, //��ƽ������
	protocol_type_h323	= 1, //H323Э���豸
	protocol_type_rtsp	= 2, //RTSPЭ���豸
	protocol_type_rtmp	= 3, //RTMPЭ���豸
	//protocol_type_sip	= 4, 
};

typedef enum call_type
{
	call_type_in		= 0, //����
	call_type_out		= 1, //����
};

typedef enum link_status
{
	link_status_nolink			= 0,	//δ����         ���߶Ͽ��ɹ�
	link_status_linkprocess		= 1,	//���Ӵ�����     HPLIVEѡ��÷���������
	link_status_linking			= 2,    //������		 �����������HPLIVE����״̬ call out
	link_status_linked			= 3,	//������		 ����������ӳɹ�


	//link_status_unlinkprocess	= 5,	//�Ͽ�������     HPLIVEѡ��÷����ͶϿ�����
	//link_status_unlinking		= 6,	//�Ͽ���         �������Ͽ���HPLIVE�����Ͽ�����
	//link_status_deleting		= 7,	//ɾ����
	//link_status_failed		= 8,	//����ʧ�ܴ����������ʧ��
	//link_status_deletefail	= 9,	//ɾ��ʧ��
	//link_status_hangupfail	= 10,	//�Ҷ�ʧ��
	//link_status_relink		= 11,	//��������
};

typedef enum play_status
{
	play_status_normal			= 0,  //û�лط�
	play_status_process			= 1,  //������   HPLIVE���͸�REC
	play_status_playing			= 2,  //�ط���
	play_status_finish			= 3,  //�طŽ���
	play_status_failed			= 4,  //�ط�ʧ��
};

typedef enum rec_status
{
	rec_status_normal			= 0, //δ¼��
	rec_status_recing			= 1, //¼����
	rec_status_pause			= 2, //¼����ͣ
};

typedef enum live_status
{
	live_status_normal			= 0, //δֱ��
	live_status_living			= 1, //ֱ����
	live_status_pause			= 2, //ֱ����ͣ
};


typedef enum EnDeviceType
{
    DEV_ENCODER		= 0,	   // ������
    DEV_H323		= 1,	   //H323�ն�
	DEV_RTSP		= 2,	   //RTSP�ն�
	DEV_RTMP		= 3,	   //RTMP�ն�

	SERVER_REC		= 100, //¼��������
    SERVER_COM		= 101, //�ϳɷ�����
	SERVER_HPH323   = 102, //HPH323
	SERVER_MP		= 103, //MP������
	SERVER_HPRTSP	= 104, //HPRTSP
	SERVER_HPRTMP	= 105, //HPRTMP

	SERVER_NGINX	= 200, //ý�������
	DEV_UNKNOWN		= 999, //δ����
};

struct PlayBack_File
{
	std::string fileid;
	std::string name;
	std::string filePath;	//ȫ·����������
	std::string callid;		//����call

	PlayBack_File()
	{
		fileid		= "";
		name		= "";
		filePath	= "";
		callid		= "";
	};

	PlayBack_File(const PlayBack_File& other)
	{
		fileid		= other.fileid ;
		name		= other.name;
		filePath	= other.filePath;
		callid		= other.callid;
	};
};


struct DeviceBaseInfo
{
    std::string strDevID;
    unsigned short usDevType;
    std::string strNodeID;

    DeviceBaseInfo()
    {
        usDevType	= DEV_UNKNOWN;
		strDevID	= "";
		strNodeID	= "";
    };

    DeviceBaseInfo(const DeviceBaseInfo& other)
    {
        usDevType = other.usDevType ;
        strDevID = other.strDevID;
        strNodeID = other.strNodeID;
    };
};

//-------------------------------------
// �����豸��Ϣ
typedef struct DeviceInfo : public DeviceBaseInfo
{
    DeviceInfo()
    {
        usChannelNum    = 0;
        usMcuPort       = 0;
        usLocalPort     = 0;
        usStatus		= 0;
        nNginxPort      = 0;
    }

    DeviceInfo(const DeviceInfo& other)
        :DeviceBaseInfo(other)
    {
        usChannelNum = other.usChannelNum;
        usStatus = other.usStatus;
        usLocalPort = other.usLocalPort;
        usMcuPort = other.usMcuPort;
        strDevName = other.strDevName;
        strMcuID = other.strMcuID;
        strMcuAddr = other.strMcuAddr;
        strNatAddr = other.strNatAddr;
        strNatType = other.strNatType;
        strLocalAddr = other.strLocalAddr;
        strMacAddr = other.strMacAddr;
        strURL = other.strURL;
		strVersion = other.strVersion;
		strAbility = other.strAbility;
        strServerID = other.strServerID;
        strServerIP = other.strServerIP;
        strAppName = other.strAppName;
        nNginxPort = other.nNginxPort;
    }

    unsigned short  usChannelNum;
	unsigned short  usStatus;
    unsigned short	usLocalPort;
    unsigned short	usMcuPort;

    // �豸�Ļ�����Ϣ
    std::string     strDevName;

    // ʵʱ������Ϣ
    std::string		strMcuID;
    std::string		strMcuAddr;
    std::string		strNatAddr;
    std::string     strNatType;
    std::string		strLocalAddr;
    std::string     strMacAddr;
    std::string     strURL;
	std::string     strVersion;
	std::string     strAbility;

    // RTMP
    std::string     strServerID;
    std::string     strServerIP;
    std::string     strAppName;
    unsigned int    nNginxPort;
} *PDeviceInfo;

//-------------------------------------
// ���ͨ����Ϣ
typedef struct ChannelInfo
{
    ChannelInfo()
    {
        usChannelNo = 0;
        ulVideoCID = 0;
        ulAudioCID = 0;
    }
    std::string     strChannelID;
    std::string     strChannelName;
    std::string     strDevID;
    std::string     strNodeID;
    unsigned short  usChannelNo;
    unsigned long	ulVideoCID;
    unsigned long	ulAudioCID;
}*PChannelInfo;

template<typename T>
inline void CMDPacket( T& t, DeviceInfo& devInfo)
{
    t.SetAttrib( "USERID" , devInfo.strDevID);
    t.SetAttrib( "NAME", devInfo.strDevName);
    t.SetAttribUS( "CHANNELNUM" , devInfo.usChannelNum);
    t.SetAttribUS( "DEVIDTYPE" , devInfo.usDevType);

    t.SetAttrib( "MCUID:", devInfo.strMcuID);
    t.SetAttrib( "MCUADDR:", devInfo.strMcuAddr);
    t.SetAttribUS( "MCUPORT:", devInfo.usMcuPort);
    t.SetAttrib( "NODEID", devInfo.strNodeID);
    t.SetAttrib( "NATADDR:", devInfo.strNatAddr);
    t.SetAttrib( "NATTYPE", devInfo.strNatType);
    t.SetAttrib( "LOCALADDR:", devInfo.strLocalAddr);
    t.SetAttribUL( "LOCALPORT:", devInfo.usLocalPort);
    t.SetAttrib( "MACADDR", devInfo.strMacAddr);
    t.SetAttribUS( "STATUS", devInfo.usStatus);
    t.SetAttrib( "URL", devInfo.strURL);
    t.SetAttrib( "SERVERID",devInfo.strServerID);
    t.SetAttrib( "SERVERIP",devInfo.strServerIP);
    t.SetAttrib( "APP",devInfo.strAppName);
    t.SetAttribUN( "NGINXPORT",devInfo.nNginxPort);
}

//------------------------------------------------------------------------
template<typename T>
inline void CMDUnPacket( T& t, DeviceInfo& devInfo)
{
    devInfo.strDevID = t.GetAttrib( "USERID" ).AsString();
    devInfo.strDevName = t.GetAttrib( "NAME" ).AsString();
    devInfo.usChannelNum = t.GetAttrib( "CHANNELNUM" ).AsUnsignedShort();
    devInfo.usDevType = t.GetAttrib( "DEVICETYPE" ).AsUnsignedShort();

    devInfo.strMcuID = t.GetAttrib( "MCUID:" ).AsString();
    devInfo.strMcuAddr = t.GetAttrib( "MCUADDR:" ).AsString();
    devInfo.usMcuPort = t.GetAttrib( "MCUPORT:" ).AsUnsignedShort();
    devInfo.strNodeID = t.GetAttrib( "NODEID" ).AsString();
    devInfo.strNatAddr = t.GetAttrib( "NATADDR:" ).AsString();
    devInfo.strNatType = t.GetAttrib( "NATTYPE" ).AsString();
    devInfo.strLocalAddr = t.GetAttrib( "LOCALADDR:" ).AsString();
    devInfo.usLocalPort = t.GetAttrib( "LOCALPORT:" ).AsUnsignedShort();
    devInfo.strMacAddr= t.GetAttrib("MACADDR").AsString();
	devInfo.usStatus = t.GetAttrib("STATUS").AsUnsignedShort();
    devInfo.strURL = t.GetAttrib("URL").AsString();

    devInfo.strServerID = t.GetAttrib("SERVERID").AsString();
    devInfo.strServerIP = t.GetAttrib("SERVERIP").AsString();
    devInfo.strAppName  = t.GetAttrib("APP").AsString();
    devInfo.nNginxPort  = t.GetAttrib("NGINXPORT").AsUnsignedInt();
	devInfo.strVersion  = t.GetAttrib("VERSION").AsString();
    devInfo.strAbility  = t.GetAttrib("SKILL").AsString();
}

/************************************************************************/
/* ��������ͨ����Ϣ                                                   */
/************************************************************************/
template<typename T>
inline void CMDPacket( T& t, PChannelInfo pChannelInfo)
{
    t.SetAttrib( "CHANNELID", pChannelInfo->strChannelID);
    t.SetAttrib( "CHANNELNAME:", pChannelInfo->strChannelName);
    t.SetAttrib( "DEVID", pChannelInfo->strDevID);
    t.SetAttrib( "NODEID", pChannelInfo->strNodeID);
    t.SetAttribUS( "CHNUM", pChannelInfo->usChannelNo);
    t.SetAttribUL( "VIDEOCID:", pChannelInfo->ulVideoCID);
    t.SetAttribUL( "AUDIOCID:", pChannelInfo->ulAudioCID);
}

//------------------------------------------------------------------------
template<typename T>
inline void CMDUnPacket( T& t, PChannelInfo pChannelInfo)
{
    pChannelInfo->strChannelID = t.GetAttrib( "CHANNELID" ).AsString();
    pChannelInfo->strDevID = t.GetAttrib( "DEVID" ).AsString();
    pChannelInfo->strChannelName = t.GetAttrib( "CHANNELNAME:" ).AsString();
    pChannelInfo->strNodeID = t.GetAttrib( "NODEID" ).AsString();
    pChannelInfo->usChannelNo = t.GetAttrib( "CHNUM" ).AsUnsignedShort();
    pChannelInfo->ulVideoCID = t.GetAttrib( "VIDEOCID:" ).AsUnsignedLong();
    pChannelInfo->ulAudioCID = t.GetAttrib( "AUDIOCID:" ).AsUnsignedLong();
    if(pChannelInfo->strChannelID.empty())
    {
        if(pChannelInfo->usChannelNo<10)
        {
            pChannelInfo->strChannelID = pChannelInfo->strDevID + "_0" + USHORT2STR(pChannelInfo->usChannelNo);
        }
        else
        {
            pChannelInfo->strChannelID = pChannelInfo->strDevID + '_' + USHORT2STR(pChannelInfo->usChannelNo);
        }
    }
}

typedef std::map<std::string,DeviceInfo> DEV_MAP;

typedef std::map<const std::string,DeviceBaseInfo*>	MapDevInfo;

typedef std::list<DeviceBaseInfo> ListDevInfo;

struct TaskInfo
{
    TaskInfo()
    {
        usIsRecord = 0;
        usIsLive   = 0;
        usIsPublic = 0;
		usIsMobile = 0;
    };

    std::string strTaskID;
    unsigned short usIsRecord;
    unsigned short usIsLive;
    unsigned short usIsPublic;
	unsigned short usIsMobile;
};

typedef std::map<const std::string,TaskInfo> MapTaskInfo;

#define MAXVIEWERNUM 50
#define BALLANCEVIEWERNUM 5
#define URLITEM 5
#define IPADDR_POS 2
#define APP_POS 3
#define STREAMID_POS 4
#define RTMPHEADER "rtmp://"

#define ERR_RTMPAVAILABLE 1    //�ҵ����õ�RTMP������
#define ERR_NORTMPAVAILABLE 2  //û�п��õ�RTMP������
