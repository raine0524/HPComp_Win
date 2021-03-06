//IMXEC_Call.h
#ifndef __IMXEC_CALL_H__
#define __IMXEC_CALL_H__

#include <AVCONMP/IMXEC_Export.h>
#include <MPVideo/MPVideo.h>
#include <AUDEC/AUDEC_CodecID.h>
#include <VIDEC/VIDEC_CodecDef.h>
#include <winsock2.h>
#include <windows.h>


//呼叫接收原因
typedef enum
{
	IMXEC_CALL_END_BY_TIMEOUT=0,		//呼叫超时
	IMXEC_CALL_END_BY_REMOTE_HANGUP,	//远端结束呼叫
	IMXEC_CALL_END_BY_LOCAL_HANGUP		//本机结束呼叫
}IMXEC_CALL_END_REASON;

typedef enum
{
	IMXEC_CALL_DIR_UNKOWN=0,
	IMXEC_CALL_DIR_IN,
	IMXEC_CALL_DIR_OUT,
}IMXEC_CALL_DIRECTION;


//H235 Media use

typedef enum
{
	IMXEC_CALL_H235AUTH_UNKOWN=0,
	IMXEC_CALL_H235AUTH_SUCCESS,         /*H235验证通过状态*/
	IMXEC_CALL_H235AUTH_FAILURE,         /*H235验证失败状态*/
	IMXEC_CALL_H235MEDIA_FAILURE         /*H235媒体流共享密钥协商失败状态*/
}IMXEC_CALL_H235AUTH_STATE;


typedef enum
{
	IMXEC_CALL_MEDIA_UNKOWN=0,
	IMXEC_CALL_MEDIA_AUDIO,              /*音频类型*/
	IMXEC_CALL_MEDIA_MAINVIDEO,          /*视频类型*/
	IMXEC_CALL_MEDIA_H239VIDEO,          /*辅流类型*/
	IMXEC_CALL_MEDIA_H224DATA            /*H224数据类型*/

}IMXEC_CALL_MEDIA_TYPE;





//PTZ[摄像机遥控命令]
typedef enum 
{
	PTZ_START_PAN_UP=0,		//开始向上
	PTZ_STOP_PAN_UP,		//停止向上
	PTZ_START_PAN_DOWN,		//开始向下
	PTZ_STOP_PAN_DOWN,		//停止向下
	PTZ_START_TILT_LEFT,	//开始向左
	PTZ_STOP_TILT_LEFT,		//停止向左
	PTZ_START_TILT_RIGHT,	//开始向右
	PTZ_STOP_TILT_RIGHT,	//停止向右
	PTZ_START_ZOOM_IN,		//开始放大
	PTZ_STOP_ZOOM_IN,		//停止放大
	PTZ_START_ZOOM_OUT,		//开始缩小
	PTZ_STOP_ZOOM_OUT,		//停止缩小
	PTZ_START_FOCUS_IN,		//开始向近处聚焦
	PTZ_STOP_FOCUS_IN,		//停止向近处聚焦
	PTZ_START_FOCUS_OUT,	//开始向远处聚焦
	PTZ_STOP_FOCUS_OUT,		//停止向远处聚焦
	PTZ_START_FOCUS_AUTO,	//开始自动聚焦
	PTZ_STOP_FOCUS_AUTO,	//停止自动聚焦
	PTZ_START_IRIS_MINUS,	//开始光圈变小
	PTZ_STOP_IRIS_MINUS,	//停止光圈变小
	PTZ_START_IRIS_PLUS,	//开始光圈变大
	PTZ_STTOPIRIS_PLUS,		//停止光圈变大
}PTZ_ACTION;



class IMXEC_H323Callback
{
public:
	IMXEC_H323Callback(void){};
	virtual~IMXEC_H323Callback(void){};
public:
	//视频数据回调函数nKeyFrame,0:非关键帧 1:关键帧
	virtual void OnIMXEC_H323CallbackMainVideoData(unsigned char*pData,int nLen,int nKeyFrame,VIDEC_CODEC_TYPE videoCodecType)=0;

	//音频数据回调函数
	virtual void OnIMXEC_H323CallbackMainAudioData(unsigned char*pData,int nLen,X_AUDIO_CODEC_ID audioCodecType)=0;

	//视频数据回调函数nKeyFrame,0:非关键帧 1:关键帧
	virtual void OnIMXEC_H323CallbackSecondVideoData(unsigned char*pData,int nLen,int nKeyFrame,VIDEC_CODEC_TYPE videoCodecType)=0;
};

//呼叫类
class IMXEC_API IMXEC_Call
{
public:
	IMXEC_Call(void);
	virtual~ IMXEC_Call(void);
protected:
	int		m_nExclusive;
public:
	int GetExclusive(void);
public:
	/******************************************************************************
	* Start
	*描述：启动呼叫
	*输入：nExclusive		-是否是排他的
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	virtual int Start(int nExclusive=0);

	/******************************************************************************
	* Stop
	*描述：停止呼叫
	*输入：无
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	virtual void Stop(void);

	/******************************************************************************
	* OnIMXEC_CallRingIn
	*描述：呼入事件，有终端呼叫本机的时候系统会调用此事件
	*输入：	cszCallID		-呼叫标识
			cszUserID		-用户标识或用户IP
			cszUserName		-用户名称
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallRingIn(const char*cszCallID,const char*cszUserID,const char*cszUserName)=0;

	/******************************************************************************
	* OnIMXEC_CallRingOut
	*描述：呼出事件，本机发起呼叫成功后系统会调用此事件
	*输入：	cszCallID		-呼叫标识
			cszUserID		-用户标识或用户IP
			cszUserName		-用户名称
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallRingOut(const char*cszCallID,const char*cszUserID,const char*cszUserName)=0;

	/******************************************************************************
	* OnIMXEC_CallEstablished
	*描述：呼叫建立事件，当双方建立通话后【本机接受了远端的呼叫请求或远端接受了本机的呼叫请求】系统会调用此事件
	*输入：	cszCallID		-呼叫标识
			cszUserID		-用户标识或用户IP
			cszUserName		-用户名称
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallEstablished(const char*cszCallID,const char*cszUserID,const char*cszUserName)=0;

	/******************************************************************************
	* OnIMXEC_CallDestroyed
	*描述：呼叫销毁事件，当通话结束后【本机挂断或远端挂断】系统会调用此事件
	*输入：	cszCallID		-呼叫标识
			cszUserID		-用户标识或用户IP
			cszUserName		-用户名称
			nReason			-结束原因
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallDestroyed(const char*cszCallID,const char*cszUserID,const char*cszUserName,IMXEC_CALL_END_REASON nReason)=0;

	/******************************************************************************
	* OnIMXEC_CallRecvdPTZ
	*描述：接收到PTZ事件，当对方发送云台控制命令时，系统会调用此事件
	*输入：	cszCallID		-呼叫标识
			nPTZAction		-云台动作
			nSpeed			-动作的速度
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallRecvdPTZ(const char*cszCallID,PTZ_ACTION nPTZAction,int nSpeed)=0;

	/******************************************************************************
	* OnIMXEC_CallSecondVideoStart
	*描述：辅流开始事件，当辅流开始时，系统会调用此事件
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallSecondVideoStart(const char*cszCallID)=0;

	/******************************************************************************
	* OnIMXEC_CallSecondVideoStop
	*描述：辅流停止事件，当辅流停止时，系统会调用此事件
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_CallSecondVideoStop(const char*cszCallID)=0;

	/******************************************************************************
	* OnIMXEC_SIPVirtualLineRegistrationSuccess
	*描述：SIP注册成功事件
	*输入：	nLineID		-线路标识
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_SIPVirtualLineRegistrationSuccess(int nLineID){};

	/******************************************************************************
	* OnIMXEC_SIPVirtualLineRegistrationError
	*描述：SIP注册失败事件
	*输入：	nLineID			-线路标识
			cszErrorCode	-错误码
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_SIPVirtualLineRegistrationError(int nLineID,const char*cszErrorCode){};

	/******************************************************************************
	* OnIMXEC_H323RegistrationSuccess
	*描述：H323注册成功事件
	*输入：无
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_H323RegistrationSuccess(void){};

	/******************************************************************************
	* OnIMXEC_H323RegistrationError
	*描述：H323注册失败事件
	*输入：无
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnIMXEC_H323RegistrationError(void){};

	/******************************************************************************
	* MakeCall
	*描述：发起呼叫
	*输入：	cszUserID		-对方的用户ID或IP
			cszProtocol		-呼叫协议，"H323","SIP","IMXEC"
	*输出：无
	*返回值：呼叫标识
	*****************************************************************************/
	static const char*MakeCall(const char*cszUserID,const char*cszProtocol);

	/******************************************************************************
	* AcceptCall
	*描述：接受呼叫请求
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int AcceptCall(const char*cszCallID);

	/******************************************************************************
	* HangupCall
	*描述：挂断呼叫
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int HangupCall(const char*cszCallID);

	/******************************************************************************
	* SendPTZ
	*描述：发送PTZ【发送云台控制命令】
	*输入：	cszCallID		-呼叫标识
			nPTZAction		-云台动作
			nSpeed			-动作的速度
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SendPTZ (const char*cszCallID,PTZ_ACTION nPTZAction,int nSpeed);

	/******************************************************************************
	* StartSecondVideo
	*描述：开始辅流
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int StartSecondVideo (const char*cszCallID);

	/******************************************************************************
	* StopSecondVideo
	*描述：停止辅流
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int StopSecondVideo (const char*cszCallID);


	/******************************************************************************
	* GetLocalMainVideoInfo
	*描述：获取本地主流视频信息
	*输入：	cszCallID		-呼叫标识
	*输出：	nWidth			-图像宽度
			nHeight			-图像高度
			nVideoCodecType	-编码类型
			hVideoWnd		-窗口句柄
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetLocalMainVideoInfo(const char*cszCallID, int&nWidth, int&nHeight,VIDEC_CODEC_TYPE&nVideoCodecType,HWND&hVideoWnd);

	/******************************************************************************
	* GetLocalMainVideoPacketStatistics
	*描述：获取本地主视频的包的统计数据
	*输入：	cszCallID			-呼叫标识
	*输出： ulTotalPackets		-总共发送了多少个包
			ulLostPackets		-总共丢弃了多少个包
			AvgPacketLossRate	-平均丢包率
			CurPacketLossRate	-当前丢包率
			nBitrate			-码流【bps】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetLocalMainVideoPacketStatistics(const char*cszCallID,unsigned long&ulTotalPackets,unsigned long&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate,int&nBitrate);


	/******************************************************************************
	* GetRemoteMainVideoInfo
	*描述：获取远程主流视频信息
	*输入：	cszCallID		-呼叫标识
	*输出：	nWidth			-图像宽度
			nHeight			-图像高度
			nVideoCodecType	-编码类型
			hVideoWnd		-窗口句柄
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetRemoteMainVideoInfo(const char*cszCallID, int&nWidth, int&nHeight,VIDEC_CODEC_TYPE&nVideoCodecType,HWND&hVideoWnd);

	/******************************************************************************
	* GetRemoteMainVideoPacketStatistics
	*描述：获取远端主视频的包的统计数据
	*输入：	cszCallID			-呼叫标识
	*输出： ulTotalPackets		-总共发送了多少个包
			ulLostPackets		-总共丢弃了多少个包
			AvgPacketLossRate	-平均丢包率
			CurPacketLossRate	-当前丢包率
			nBitrate			-码流【bps】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetRemoteMainVideoPacketStatistics(const char*cszCallID,unsigned long&ulTotalPackets,unsigned long&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate,int&nBitrate);


	/******************************************************************************
	* GetSecondVideoInfo
	*描述：获取辅流视频信息
	*输入：	cszCallID		-呼叫标识
	*输出：	nWidth			-图像宽度
			nHeight			-图像高度
			nVideoCodecType	-编码类型
			hVideoWnd		-窗口句柄
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetSecondVideoInfo(const char*cszCallID, int&nWidth, int&nHeight,VIDEC_CODEC_TYPE&nVideoCodecType,HWND&hVideoWnd);
	
	/******************************************************************************
	* GetSecondVideoPacketStatistics
	*描述：获取辅流视频的包的统计数据
	*输入：	cszCallID			-呼叫标识
	*输出： ulTotalPackets		-总共发送了多少个包
			ulLostPackets		-总共丢弃了多少个包
			AvgPacketLossRate	-平均丢包率
			CurPacketLossRate	-当前丢包率
			nBitrate			-码流【bps】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetSecondVideoPacketStatistics(const char*cszCallID,unsigned long&ulTotalPackets,unsigned long&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate,int&nBitrate);

	/******************************************************************************
	* GetLocalAudioLevel
	*描述：获取本地音频Level【实际声音大小】
	*输入：	cszCallID		-呼叫标识
	*输出：	nLevel			-实际声音大小【0-10】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetLocalAudioLevel(const char*cszCallID,int&nLevel);

	/******************************************************************************
	* SetLocalAudioVolume
	*描述：获取本地音频音量
	*输入：	cszCallID		-呼叫标识
			nVolume			-音量
	*输出：	无
	*返回值：无
	*****************************************************************************/
	static void SetLocalAudioVolume(const char*cszCallID,int nVolume);

	/******************************************************************************
	* SetLocalAudioMute
	*描述：获取本地音频是否静音
	*输入：	cszCallID		-呼叫标识
			nMute			-是否静音
	*输出：	无
	*返回值：无
	*****************************************************************************/
	static void SetLocalAudioMute(const char*cszCallID,int nMute);

	/******************************************************************************
	* GetLocalAudioCodecID
	*描述：获取本地音频编码信息
	*输入：	cszCallID		-呼叫标识
	*输出：	nAudioCodecID	-编码ID
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetLocalAudioCodecID(const char*cszCallID, X_AUDIO_CODEC_ID &nAudioCodecID);

	/******************************************************************************
	* GetLocalAudioPacketStatistics
	*描述：获取本机音频的包的统计数据
	*输入：	cszCallID			-呼叫标识
	*输出： ulTotalPackets		-总共发送了多少个包
			ulLostPackets		-总共丢弃了多少个包
			AvgPacketLossRate	-平均丢包率
			CurPacketLossRate	-当前丢包率
			nBitrate			-码流【bps】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetLocalAudioPacketStatistics(const char*cszCallID,unsigned long&ulTotalPackets,unsigned long&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate,int&nBitrate);

	/******************************************************************************
	* GetRemoteAudioLevel
	*描述：获取远程音频Level【实际声音大小】
	*输入：	cszCallID		-呼叫标识
	*输出：	nLevel			-实际声音大小【0-10】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetRemoteAudioLevel(const char*cszCallID,int&nLevel);

	/******************************************************************************
	* SetRemoteAudioVolume
	*描述：获取远程音频音量
	*输入：	cszCallID		-呼叫标识
			nVolume			-音量
	*输出：	无
	*返回值：无
	*****************************************************************************/
	static void SetRemoteAudioVolume(const char*cszCallID,int nVolume);

	/******************************************************************************
	* SetRemoteAudioMute
	*描述：获取远程音频是否静音
	*输入：	cszCallID		-呼叫标识
			nMute			-是否静音
	*输出：	无
	*返回值：无
	*****************************************************************************/
	static void SetRemoteAudioMute(const char*cszCallID,int nMute);

	/******************************************************************************
	* GetRemoteAudioCodecID
	*描述：获取远端音频编码
	*输入：	cszCallID		-呼叫标识
	*输出：	nAudioCodecID	-编码ID
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetRemoteAudioCodecID (const char*cszCallID, X_AUDIO_CODEC_ID &nAudioCodecID);

	/******************************************************************************
	* GetRemoteAudioPacketStatistics
	*描述：获取远端音频的包的统计数据
	*输入：	cszCallID			-呼叫标识
	*输出： ulTotalPackets		-总共发送了多少个包
			ulLostPackets		-总共丢弃了多少个包
			AvgPacketLossRate	-平均丢包率
			CurPacketLossRate	-当前丢包率
			nBitrate			-码流【bps】
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetRemoteAudioPacketStatistics(const char*cszCallID,unsigned long&ulTotalPackets,unsigned long&ulLostPackets,double&AvgPacketLossRate,double&CurPacketLossRate,int&nBitrate);

	/******************************************************************************
	* GetCallSupportSecondVideo
	*描述：获取呼叫是否支持辅流【H239】
	*输入：	cszCallID		-呼叫标识
	*输出：	无
	*返回值：支持返回非0，不支持返回0
	*****************************************************************************/
	static int GetCallSupportSecondVideo(const char*cszCallID);

	/******************************************************************************
	* GetMainAudioID
	*描述：获取主流音频标识
	*输入： cszCallID		-呼叫标识
	*输出： nMainAudioID	-主流音频标识
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetMainAudioID(const char*cszCallID,unsigned long&nMainAudioID);

	/******************************************************************************
	* GetMainVideoID
	*描述：获取主流视频标识
	*输入： cszCallID		-呼叫标识
	*输出： nMainVideoID	-主流视频标识
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetMainVideoID(const char*cszCallID,unsigned long&nMainVideoID);

	/******************************************************************************
	* GetSecondVideoID
	*描述：获取辅流视频标识
	*输入： cszCallID		-呼叫标识
	*输出： nSecondVideoID	-辅流视频标识
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetSecondVideoID(const char*cszCallID,unsigned long&nSecondVideoID);

	/******************************************************************************
	* SetMainVideoScreenIndex
	*描述：设置主流视频图像桌面序号
	*输入： cszCallID		-呼叫标识
			nScreenIndex	-桌面序号
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SetMainVideoScreenIndex(const char*cszCallID,int nScreenIndex);

	/******************************************************************************
	* SetSecondVideoScreenIndex
	*描述：设置辅流视频图像桌面序号
	*输入： cszCallID		-呼叫标识
			nScreenIndex	-桌面序号
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SetSecondVideoScreenIndex(const char*cszCallID,int nScreenIndex);

	/******************************************************************************
	* SetSecondVideoScreenWnd
	*描述：设置辅流视频所在屏幕窗口位置
	*输入：	cszCallID		-呼叫标识
			nScreenIndex	-屏幕号
			nWndIndex		-窗口号
			nFullScreen		-是否全屏显示【0：普通显示，非0：全屏显示】
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetSecondVideoScreenWnd(const char*cszCallID,int nScreenIndex,int nWndIndex,int nFullScreen);

	/******************************************************************************
	* SetRemoteVideoScreenWnd
	*描述：设置远端视频所在屏幕窗口位置
	*输入：	cszCallID		-呼叫标识
			nScreenIndex	-屏幕号
			nWndIndex		-窗口号
			nFullScreen		-是否全屏显示【0：普通显示，非0：全屏显示】
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetRemoteVideoScreenWnd(const char*cszCallID,int nScreenIndex,int nWndIndex,int nFullScreen);


	/******************************************************************************
	* SendDTMF
	*描述：发送DTMF
	*输入：	cszCallID		-呼叫标识
			nDTMF			-DTMF
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SendDTMF(const char*cszCallID,const char nDTMF);

	/******************************************************************************
	* StartMixingWithWaveFile
	*描述：开始对呼叫进行WAVE文件混音，对方会听到WAVE文件播放的声音
	*输入：	cszCallID		-呼叫标识
			cszPathName		-WAVE文件全路径
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int StartMixingWithWaveFile(const char*cszCallID,const char*cszPathName);

	/******************************************************************************
	* StopMixingWithWaveFile
	*描述：停止对呼叫进行WAVE文件混音
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int StopMixingWithWaveFile(const char*cszCallID);

	/******************************************************************************
	* SetAudioOnly
	*描述：设置是否只支持音频
	*输入：	nAudioOnly		-是否只支持音频【0：支持视频音频，非0：只支持音频】
	*输出：无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SetAudioOnly(int nAudioOnly);

	/******************************************************************************
	* GetCallProtocol
	*描述：获取呼叫协议
	*输入：	cszCallID		-呼叫标识
	*输出：无
	*返回值：成功返回呼叫协议，失败返回空串""
	*****************************************************************************/
	static const char*GetCallProtocol(const char*cszCallID);

	/******************************************************************************
	* GetHasVideo
	*描述：获取是否只有视频
	*输入：cszCallID		-呼叫标识
	*输出：无
	*返回值：如果有视频，那么返回非0，否则返回0
	*****************************************************************************/
	static int GetHasVideo(const char*cszCallID);

	/******************************************************************************
	* GetSecondVideoDirection
	*描述：获取辅流的方向
	*输入：	cszCallID		-呼叫标识
	*输出： nDir			-辅流的方向
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int GetSecondVideoDirection(const char*cszCallID,IMXEC_CALL_DIRECTION&nDir);




	//H235 Media use

	/******************************************************************************
	* GetH235AuthCheckState
	*描述：获取H235验证检查状态 用于取呼叫验证及为网守时作验证检查
	*输入：	cszCallID		  -呼叫标识
	*输出： nState  -验证检查状态
			nLength -返回验证失败原因字符串长度
	*返回值：成功返回验证失败原因，失败返回空
	*****************************************************************************/
	const char* GetH235AuthCheckState(const char*cszCallID,IMXEC_CALL_H235AUTH_STATE &nState,int &nLength);

	/******************************************************************************
	* OnChannelRecvMediaEncryption
	*描述：接收媒体通道已加密状态通知，当打开加密的接收媒体通道时，系统会调用此事件
	*输入：	cszCallID		-呼叫标识
	nMediaType		-媒体类型
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnChannelRecvMediaEncryption(const char*cszCallID,IMXEC_CALL_MEDIA_TYPE nMediaType){};

	/******************************************************************************
	* OnChannelSendMediaEncryption
	*描述：发送媒体通道已加密状态通知，当打开加密的发送媒体通道时，系统会调用此事件
	*输入：	cszCallID		-呼叫标识
	nMediaType		-媒体类型
	*输出：无
	*返回值：无
	*****************************************************************************/
	virtual void OnChannelSendMediaEncryption(const char*cszCallID,IMXEC_CALL_MEDIA_TYPE nMediaType){};

	/******************************************************************************
	* SetExternalIP
	*描述：设置外网映射IP
	*输入：cszExternalIP		-外网映射IP
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetExternalIP(const char*cszExternalIP);

	/******************************************************************************
	* SetMaxCallNumber
	*描述：设置最大呼叫个数
	*输入：nMaxCallNumber		-最大呼叫个数
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetMaxCallNumber(unsigned short nMaxCallNumber);

	/******************************************************************************
	* SetMaxVideoTypeNumber
	*描述：设置视频最大合成格式数
	*输入：nMaxVideoTypeNumber		-视频最大合成格式数
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetMaxVideoTypeNumber(unsigned short nMaxVideoTypeNumber);

	/******************************************************************************
	* SetMaxTranscodeNumber
	*描述：设置视频最大转码个数IP
	*输入：nMaxTranscodeNumber		-视频最大转码个数
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetMaxTranscodeNumber(unsigned short nMaxTranscodeNumber);

	/******************************************************************************
	* SetMaxTranscodeNumber
	*描述：设置视频转码降帧模式
	*输入：nTranscodeSkipFrameCount		-(0:不降帧，1：降1半帧，2：降1/4)
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetTranscodeSkipFrameCount(unsigned short nTranscodeSkipFrameCount);

	/******************************************************************************
	* SetTranscodeBitrate
	*描述：设置视频转码码率
	*输入：nTranscodeBitrate		-(转码码率)
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetTranscodeBitrate(unsigned int nTranscodeBitrate);

	/******************************************************************************
	* SetH323Callback
	*描述：设置H323回调
	*输入：	cszCallID		-呼叫标识
			pCallback		-回调指针
	*输出：无
	*返回值：无
	*****************************************************************************/
	static void SetH323Callback(const char*cszCallID,IMXEC_H323Callback* pCallback);
};



#endif