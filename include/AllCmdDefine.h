#pragma once

//=======================================================================================
// ���е�ͨ������
//=======================================================================================

typedef enum _ENUM_ALL_PROTOCAL
{
	//ȱʡ�ģ���ʼ��KCMDPacketʱ������CMD���ܲ�ȷ������ʱʹ�� CMD_DEFAULT��
	CMD_DEFAULT						= 9999,

 	//MON ��ͨ������
	//MON(��ʼ)---------------------------------------------------
	MON_DISCONNECT					= 3000,
	MON_LOGIN						= 3001,
	MON_LOGOUT						= 3002,
	MON_SENDSMS						= 3003,
	MON_DEVONLINE					= 3004,
	MON_DEVOFFLINE					= 3005,
	MON_DEVCHECK					= 3006,
	MON_DEVNAME						= 3007,
	MON_CHLONLINE					= 3008,
	MON_CHLOFFLINE					= 3009,

	MON_CHLNAME						= 3010,
	MON_DEVBASINF					= 3011,
	MON_DEVCLK						= 3012,
	MON_A2AINVITE					= 3013,
	MON_A2ABYE						= 3014,
	MON_YTCTRL						= 3015,
	MON_A2ARSP						= 3016,
	MON_GETYTCTRLPARAMS				= 3017,
	MON_GETYTCONNPARAMS				= 3018,
	MON_SETYTCTRLPARAMS				= 3019,

	MON_SETYTCONNPARAMS				= 3020,
	MON_ALERT						= 3021,
	MON_GETREGION					= 3022,
	MON_GETSUBREGION				= 3023,
	MON_VRSSVR						= 3024,
	MON_MGSRPT						= 3025,
	MON_CHLRPT						= 3026,
	MON_DEVCHANNELCHANGE			= 3027,
	MON_GETMONRECTASK				= 3028,
	MON_GETMONRECCHNLTASK			= 3029,

	MON_GETMONRECSTRATEGY			= 3030,
	MON_DOMAIN_MCU_DISCONNECT		= 3031,
	MON_DOMAIN_GATEWAY_ONLINE		= 3032,
	MON_DOMAIN_DEVICE_ONLINE		= 3033,
	MON_DOMAIN_CHANNEL_ONLINE		= 3034,
	MON_DOMAIN_NODE_DISCONNECT		= 3035,
	MON_DOMAIN_CHANNEL_OFFLINE		= 3036,
	MON_DOMAIN_DEVNAME				= 3037,
	MON_DOMAIN_CHLNAME				= 3038,
	MON_GETDOMAINMONDEVINF			= 3039,

	MON_UDPRCV_ALARM				= 3040,
	MON_UDPRCV_ALARM_SPD			= 3041,
	MON_NVRSTG						= 3042,
	MON_NVRSKD						= 3043,
	MON_MON_SET_YT_CTRL				= 3044, //û�� ������
	MON_UNSUBSCRIBE_NODEID			= 3045,
	MON_MONGPS						= 3046,
	MON_DOMAIN_CARDNAME				= 3047,
	MON_FETCHDEVINFO                = 3048,
	MON_RTMPSERVER_CONNECTED        = 3049,
	MON_RTMPSERVER_DISCONNECTED     = 3050,
	MON_PORTINFO                    = 3051,
	H323_CALLIN_START				= 3052,//H323�ն˺��з���
	ON_H323_CALLIN_START			= 3053,//��ӦH323�ն˺��з���

	H323_CALLIN_FIND_MP				= 3054,//HPLIVE���͸�HPH323����MP��Ϣ
	ON_CALL_STATUS					= 3055,//HP323���͸�HPLIVE���е���Ϣ
	H323_CALL_START_REC				= 3056,//MP���͸�HPLIVE��ʼ¼����Ϣ
	H323_CALL_STOP_REC				= 3057,//MP���͸�HPLIVEֹͣ¼����Ϣ
	H323_CALL_REC_STATUS			= 3058,//HPLIVE���͸�MP��¼��״̬��Ϣ
	H323_CALL_GETPLAYBACK			= 3059,//��ȡ�ļ��б�
	H323_CALL_PLAY_FILE				= 3060,//������Ƶ�ļ�
	H323_CALL_PLAY_FILE_STATUS		= 3061,//������Ƶ�ļ�״̬
	H323_CALL_STOP_PLAY_FILE		= 3062,//ֹͣ�ط�
	MP_SERVER_DISCONNECTED			= 3063,//MP����Ͽ�
	HPH323_SERVER_DISCONNECTED		= 3064,//HPH323�Ͽ�

	//MON(����)---------------------------------------------------

	//REC ��ͨ������
	//REC(��ʼ)---------------------------------------------------
	REC_RECSTATUS			= 6000,
	REC_RECCONFSTATUS		= 6001,
	REC_RECCONFAVSTATUS		= 6002,
	REC_RECCONFFULL			= 6003,
	REC_DISCONNECT			= 6004, //¼���������Ͽ�
	REC_HPRECLOGIN			= 6005, //¼�������¼
	REC_HPRECLOGOUT			= 6006, //¼������ǳ�
	REC_GETPOLICY			= 6007,
	REC_DISKINFO			= 6008,
	REC_PORTINFO			= 6009,

	REC_STARTEDREC			= 6010,
	REC_STOPEDREC			= 6011,
	REC_GETDELFILE			= 6012,
	REC_DELFILE				= 6013,
	REC_STARTEDCONFREC		= 6015,
	REC_STARTEDCONFAVREC	= 6016,
	REC_CONFRECFULL			= 6017,
	REC_HPRECSTOPED			= 6018,
	REC_RECSTART			= 6019,

	REC_RECSTOP				= 6020,
	REC_RECSTOPALL			= 6021,
	REC_RECCONFSTART		= 6022,
	REC_RECCONFSTOP			= 6023,
	REC_RECCONFAVSTART		= 6024,
	REC_RECCONFAVSTOP		= 6025,
	REC_RECCONFDOSSTART		= 6026,
	REC_RECCONFDOSSTOP		= 6027,
	REC_POLICY				= 6028,
	REC_STARTREC			= 6029,

	REC_STOPREC				= 6030,
	REC_STARTCONFREC		= 6031,
	REC_STOPCONFREC			= 6032,
	REC_STARTCONFAVREC		= 6033,
	REC_STOPCONFAVREC		= 6034,
	REC_STARTCONFDOSREC		= 6035,
	REC_STOPCONFDOSREC		= 6036,
	REC_ERRRECFILE			= 6037,
	REC_SENDPICTURE			= 6038, //ץ��ͼƬ���͵�¼��������
	REC_SENDPIC				= 6039,
	REC_CHANGESTREAM		= 6040,
	REC_LIVEUSERCHAGE		= 6041,//ֱ�������仯
	REC_CHANGEFILEPATH      = 6042,
	REC_LIVESTREAM			= 6043,
	REC_STARTTASK			= 6044,	  //��ʼִ������ֱ�Ӵ�������
	REC_STOPTASK			= 6045,   //ִֹͣ������
	REC_CMD_TASK_STATUS		= 6046,   //����״̬����
	REC_FILE_STAUS			= 6047,	  //¼���ļ�״̬
	REC_MODIFYTASK          = 6048,   //�޸��������
	REC_RECOVER				= 6049,	  //��ȡ�޸���Ƶ
	REC_COMINFO             = 6050,   //�ϳ���Ƶ����Ϣ
	REC_CHANGETASK          = 6051,   //�ı�������Ϣ
	REC_CALLOUT_START		= 6052,   //WEB����ĺ���
	REC_CALL_STOP			= 6053,	  //WEB����ֹͣ����
	TASK_REC_STATUS			= 6054,	  //¼��״̬
	TASK_LIVE_STATUS		= 6055,	  //ֱ��״̬

	//REC(����)---------------------------------------------------

	//ROUTER ��ͨ������
	//ROUTER(��ʼ)---------------------------------------------------
	ROUTER_MCU_REGISTER			    = 7000,
	ROUTER_MCU_UNREGISTER		    = 7001,
	ROUTER_MCU_ADDR				    = 7002,
	ROUTER_MCU_USRCNT			    = 7003,
	ROUTER_MCU_UPDATE			    = 7004,
	ROUTER_MCU_ADDR_UPDATE		    = 7005,
	ROUTER_SVRLST				    = 7006,
	ROUTER_SVRTIME				    = 7007,
	ROUTER_TRAFFIC				    = 7008,
	ROUTER_AGENT_REGISTER		    = 7009,

	ROUTER_AGENT_UNREGISTER		    = 7010,
	ROUTER_DOMAIN_CONNECTED		    = 7011,
	ROUTER_MCU_CONNECT			    = 7012,
	ROUTER_DOMAIN_REGISTER		    = 7013,
	ROUTER_DOMAIN_UNREGISTER	    = 7014,
	ROUTER_DOMAIN_WEBSVR		    = 7015, //û�� ������
	ROUTER_DOMAIN_LICENSE		    = 7016,
	ROUTER_CHKLIC				    = 7017,
	ROUTER_ACTRTR				    = 7018,
	ROUTER_ACTMCU				    = 7019,

	ROUTER_ACCNT				    = 7020,
	ROUTER_CKACCNT				    = 7021,
	ROUTER_DOMAIN_ONLINE            = 7022,
	ROUTER_CHILDDOMAIN_CONNECTED    = 7023,
	ROUTER_DOMAIN_DISCONNECT        = 7024,
	ROUTER_RTMP_REGISTER            = 7025,
	ROUTER_RTMP_UNREGISTER          = 7026,
	ROUTER_NODE_DISCONNECTED        = 7027,
	//ROUTER(����)---------------------------------------------------

    //WEB BEGIN------------------------------------------------------
    WEB_START_PUBLISH               = 8000,
    WEB_STOP_PUBLISH                = 8001,
    WEB_TASK_START                  = 8002,
    WEB_TASK_STOP                   = 8003,
    WEB_MAKECALL                    = 8004,
    WEB_HANGUP                      = 8005,
    WEB_PLAY                        = 8006,
    WEB_PLAY_DONE                   = 8007,
    WEB_TASK_MODIFY                 = 8008,
    WEB_PUBLISH                     = 8009,
    WEB_PUBLISH_DONE                = 8010,
	WEB_START_H323                  = 8020,
	WEB_STOP_H323	                = 8021,
	WEB_DRAG_WINDOW                 = 8022,
	START_RTSP						= 8023,
	STOP_RTSP						= 8024,
	WEB_REC_STATUS					= 8025,		//WEB����¼��״̬
	WEB_LIVE_STATUS					= 8026,		//WEB����ֱ��״̬

    //WEB END--------------------------------------------------------

    //DEV BEGIN-----------------------------------------------------]
    DEV_REGISTER                   = 9000,
    DEV_UNREGISTER                 = 9001,
    DEV_DISCONNECTED               = 9002,
    DEV_DEVICE_ONLINE              = 9003,
    DEV_DEVICE_OFFLINE             = 9004,
    DEV_CHANNEL_ONLINE             = 9005,
    DEV_CHANNEL_OFFLINE            = 9006,
    //DEV END-------------------------------------------------------


	//HPH323 BEGIN
	HPH323_DISCONNECT			   = 10000, //¼���������Ͽ�

	//HPH323 END

}ENUM_ALL_PROTOCAL;


//=======================================================================================
// ���е�ͨ�ŵİ�ͷKEY
//=======================================================================================
#define SOURCE_NODE			"SOURCE_NODE"		//���Ͷ˵�NODEID
#define DEVID				"DEVID"				//�豸ID
#define ERRORCODE			"ERRORCODE"			//��½���������
#define	DEVICE_TYPE			"DEVICE_TYPE"       //��½�豸����
#define H323_IP				"H323_IP"			//H323IP
#define H323_CALLID			"H323_CALLID"		//H323 CallID
#define MP_ID				"MP_ID"				//MP ID
#define NODE_ID				"NODE_ID"			//����ڵ�ID�������ݴ����ã�����ռ��NODEID
#define STATUS				"STATUS"			//״ֵ̬
#define BINDDEVID			"BINDDEVID"			//H323��Ӧ���豸ID
#define H323IPADDR			"H323IPADDR"		//H323IP��ַ
#define AUDIOFORMAT			"AUDIOFORMAT"		//��Ƶ��������
#define VIDEOFORMAT			"VIDEOFORMAT"		//��Ƶ��������
#define VIDEORESOLUTION		"VIDEORESOLUTION"	//�����ֱ���
#define VIDEOFRAMERATE		"VIDEOFRAMERATE"	//����֡��
#define BITRATE				"BITRATE"			//��������
#define DUALVIDEOFORMAT		"DUALVIDEOFORMAT"	//������Ƶ��������
#define DUALVIDEORESOLUTION "DUALVIDEORESOLUTION" //�����ֱ���
#define DUALVIDEOFRAMERATE	"DUALVIDEOFRAMERATE"//������Ƶ����֡��
#define DUALBITRATE			"DUALBITRATE"		//��������
#define DUALVIDEO			"DUALVIDEO"			//�Ƿ�����H239
#define PROTOCOL_TYPE		"PROTOCOL_TYPE"     //����Э������
#define SOURCE_IP			"SOURCE_IP"			//H323IP��������Э���URL
#define CALL_FROM_LINK		"CALL_FROM_LINK"    //������֮���ӹ���
#define VIDEO_W				"VIDEO_W"			//��Ƶ�Ŀ��
#define VIDEO_H				"VIDEO_H"			//��Ƶ�ĸ߶�
#define REC_STATUS			"REC_STATUS"		//¼��״̬
#define PAGE_INDEX			"PAGE_INDEX"		//ҳ�����
#define PAGE_COUNT			"PAGE_COUNT"        //һҳ������
#define GET_ALL				"GET_ALL"			//��ȡ����
#define F_ID				"F_ID"				//¼���ļ�ID
#define NAME				"NAME"				//����
#define S_T					"S_T"				//��ʼʱ��
#define E_T					"E_T"				//����ʱ��
#define F_PATH				"F_PATH"			//�ļ�·��
#define PLAY_STATUS			"PLAY_STATUS"		//�ط�״̬
#define HPH323_ID			"HPH323_ID"			//HPH323ID
#define A_ID				"A_ID"				//��ƵID
#define V_ID				"V_ID"				//��ƵID
#define MCU_ID				"MCU_ID"			//MCUID
#define MCU_IP				"MCU_IP"			//MCUIP
#define MCU_PORT			"MCU_PORT"			//MCU�˿�
#define NATADDR				"NATADDR"			//NATADDR
#define LOCALADDR			"LOCALADDR"			//����IP
#define LOCALPORT			"LOCALPORT"			//���ض˿�