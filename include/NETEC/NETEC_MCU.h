//NETEC_MCU.h
#ifndef __NETEC_MCU_H__
#define __NETEC_MCU_H__

#include "NETEC_Session.h"

class NETEC_API NETEC_MCU
{
public:
	NETEC_MCU(void);
	virtual~NETEC_MCU(void);
public:
	/******************************************************************************
	* SetServerIP
	*描述：设置服务器IP
	*输入：	cszServerIP			-服务器IP
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetServerIP(const char*cszServerIP);

	/******************************************************************************
	* SetServerPort
	*描述：设置服务器端口
	*输入：	usServerPort		-服务器端口
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetServerPort(unsigned short usServerPort);

	/******************************************************************************
	* SetLocalIP
	*描述：设置本地IP，用于连接服务器
	*输入：	cszLocalIP			-服本地IP
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetLocalIP(const char*cszLocalIP);

	/******************************************************************************
	* SetID
	*描述：设置到服务器上论证的标识
	*输入：	cszID				-标识
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetID(const char*cszID);

	/******************************************************************************
	* SetPassword
	*描述：设置到服务器上论证的密码
	*输入：	cszPassword			-密码
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetPassword(const char*cszPassword);

	/******************************************************************************
	* SetVersion
	*描述：设置版本
	*输入：	cszVersion			-版本
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetVersion(const char*cszVersion);

	/******************************************************************************
	* SetUserData
	*描述：设置用户数据
	*输入：	cszUserData			-版本
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetUserData(const char*cszUserData);

	/******************************************************************************
	* SetType
	*描述：设置到服务器上论证的类型
	*输入：	nType			-类型
	*输出： 无
	*返回值：无
	*****************************************************************************/
	static void SetType(unsigned int nType);

	/******************************************************************************
	* SetIP
	*描述：设置IP项，可以设置多个，但同一个IP只能设置一次
	*输入：	cszIP			-MCU的IP
			cszIPType			-IP类型
			nCheck				-是否需要反向验证，0不需要，非0需要
	*输出： 无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	static int SetIP(const char*cszIP,const char*cszIPType,int nCheck);

	/******************************************************************************
	* GetIPActive
	*描述：IP是否验证通过
	*输入：	无
	*输出： 无
	*返回值：如果验证通过，那么返回非0，否则返回0
	*****************************************************************************/
	static int GetIPActive(const char*cszIP,const char*cszIPType);

	/******************************************************************************
	* SetMCURoute
	*描述：设置MCU的路由【最好指定内网连接内网，电信连接电信，网通连接网通，即指定相同地址相连】
	*输入：	cszPeerMCUID			-对方MCU标识
			cszPeerMCUIP		-对方MCU的IP地址【本机到对方MCU最快的地址】
			usPeerMCUPort		-对方MCU的端口
			cszLocalIP			-本机的IP地址【本机的哪个IP到对方MCU的IP地址最快】
	*输出： 无
	*返回值：如果参数无效，那么返回非0，否则返回0
	*****************************************************************************/
	static int SetMCURoute(const char*cszPeerMCUID,const char*cszPeerMCUIP,unsigned short usPeerMCUPort,const char*cszLocalIP);

	/******************************************************************************
	* Start
	*描述：启动
	*输入：	无
	*输出： 无
	*返回值：成功返回0，失败返回-1
	*****************************************************************************/
	virtual int Start(void);

	/******************************************************************************
	* IsStarted
	*描述：是否已经启动
	*输入：	无
	*输出： 无
	*返回值：如果已经启动，那么返回非0，否则返回0
	*****************************************************************************/
	static int IsStarted(void);

	/******************************************************************************
	* Stop
	*描述：停止
	*输入：	无
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void Stop(void);

	/******************************************************************************
	* GetDomain
	*描述：获取所在的域
	*输入：	无
	*输出： 无
	*返回值：所在的域
	*****************************************************************************/
	static const char*GetDomain(void);

	/******************************************************************************
	* GetConnectStatus
	*描述：获取连接状态
	*输入：	无
	*输出： 无
	*返回值：连接状态
	*****************************************************************************/
	static NETEC_Session::CONNECT_STATUS GetConnectStatus(void);

	/******************************************************************************
	* GetLoginSuccessful
	*描述：获取是否登录成功
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int GetLoginSuccessful(void);
	
	/******************************************************************************
	* GetLoginError
	*描述：获取是否登录失败
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int GetLoginError(void);

	/******************************************************************************
	* GetLoginErrorID
	*描述：获取是否由于帐号错误导致登录失败
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int GetLoginErrorID(void);

	/******************************************************************************
	* GetLoginErrorPassword
	*描述：获取是否由于密码错误导致登录失败
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int GetLoginErrorPassword(void);

	/******************************************************************************
	* GetLoginErrorDuplication
	*描述：获取是否由于相同帐号已登录导致登录失败
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int GetLoginErrorDuplication(void);

	/******************************************************************************
	* GetLoginErrorUserCode
	*描述：获取登录错误用户代码
	*输入：	无
	*输出： 无
	*返回值：返回登录错误用户代码
	*****************************************************************************/
	static const char* GetLoginErrorUserCode(void);

	/******************************************************************************
	* SendDataToRouter
	*描述：发送数据给指定的网络路由
	*输入：	cszData				-数据指针
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int SendDataToRouter(const char*cszData);

	/******************************************************************************
	* OnNETEC_MCUConnectStatusChanged
	*描述：连接状态改变的回调
	*输入：	无
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUConnectStatusChanged(void){};

	/******************************************************************************
	* OnNETEC_MCULoginSuccessful
	*描述：登录成功的回调
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	virtual void OnNETEC_MCULoginSuccessful(void){};
	
	/******************************************************************************
	* OnNETEC_MCULoginError
	*描述：登录失败的回调
	*输入：	无
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	virtual void OnNETEC_MCULoginError(void){};


	/******************************************************************************
	* SetMulticastAddress
	*描述：设置组播地址
	*输入：	cszMulticastIP			组播IP地址【224.0.1.0～238.255.255.255】
			nMulticastPort			组播端口【建议从2000-65535】
			cszLocalInterface		本地地址【从本地哪个地址发送组播】
			nEnable					是否启用组播【0不启用，非0启用】
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	static int SetMulticastAddress(const char*cszMulticastIP,unsigned short nMulticastPort,const char*cszLocalInterface,int nEnable);

	//网状桥
	static void SetEnableNetBridge(int nEnable);
	static int GetEnableNetBridge(void);
	//树状桥
	static void SetEnableTreeBridge(int nEnable);
	static int GetEnableTreeBridge(void);
	//自动路由
	static void SetEnableAutoRoute(int nEnable);
	static int GetEnableAutoRoute(void);

	/******************************************************************************
	* OnNETEC_MCUReceivedData
	*描述：收到本层服务器发来的数据
	*输入：	
			pData				-要送的数据
			nLen				-要送的数据的长度
	*输出： 无
	*返回值：返回非0为是，否则为否
	*****************************************************************************/
	virtual void OnNETEC_MCUReceivedData(const char*pData,int nLen){};

	/******************************************************************************
	* SendDataToNode
	*描述：发送数据给节点
	*输入：	cszNodeID			-节点标识
			pData				-要送的数据
			nLen				-要送的数据的长度
	*输出： 无
	*返回值：成功返回0；失败返回-1，表示网络未连通
	*****************************************************************************/
	static int SendDataToNode(unsigned int nFromAgentID,unsigned int nToAgentID,const char*cszNodeID,const char*pData,int nLen);

	/******************************************************************************
	* SendDataToAgent
	*描述：发送数据给网络应用
	*输入：	cszDomain			-网络应用所在的域
			nAgentID			-网络应用标识
			pData				-要送的数据
			nLen				-要送的数据的长度
	*输出： 无
	*返回值：成功返回0；失败返回-1，表示网络未连通
	*****************************************************************************/
	static int SendDataToAgent(const char*cszDomain,unsigned int nAgentID,const char*pData,int nLen);

	/******************************************************************************
	* SendDataToRouter
	*描述：发送数据给网络路由
	*输入：	cszDomain			-网络路由所在的域
			pData				-要送的数据
			nLen				-要送的数据的长度
	*输出： 无
	*返回值：成功返回0；失败返回-1，表示网络未连通
	*****************************************************************************/
	static int SendDataToRouter(const char*cszDomain,const char*pData,int nLen);


	/******************************************************************************
	* OnNETEC_MCUReceivedFromAgent
	*描述：收到网络应用数据包的回调
	*输入：	cszDomain				-网络应用所在的域
			nAgentID				-网络应用标识
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUReceivedFromAgent(const char*cszDomain,unsigned int nAgentID,const char* pData,int nLen){};

	/******************************************************************************
	* OnNETEC_MCUReceivedFromRouter
	*描述：收到网络路由数据包的回调
	*输入：	cszDomain				-网络路由所在的域
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUReceivedFromRouter(const char*cszDomain,const char* pData,int nLen){};

	/******************************************************************************
	* OnNETEC_MCUReceivedFromNode
	*描述：收到网络节点数据包的回调
	*输入：	cszNodeID				-网络节点标识
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUReceivedFromNode(const char*cszNodeID,unsigned int nFromAgentID,unsigned int nToAgentID,const char* pData,int nLen){};

	/******************************************************************************
	* OnNETEC_MCUNodeDisconnected
	*描述：网络节点不在线的回调
	*输入：	cszNodeID				-网络节点标识
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUNodeDisconnected(const char*cszNodeID,unsigned int nFromAgentID,unsigned int nToAgentID,const char* pData,int nLen){};

	/******************************************************************************
	* OnNETEC_MCUAgentDisconnected
	*描述：网络应用不在线的回调
	*输入：	cszDomain				-网络应用所在的域
			nAgentID				-网络应用标识
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCUAgentDisconnected(const char*cszDomain,unsigned int nAgentID,const char* pData,int nLen){};

	/******************************************************************************
	* OnNETEC_MCURouterDisconnected
	*描述：网络路由不在线的回调
	*输入：	cszDomain				-网络路由所在的域
			pData					-数据
			nLen					-数据长度
	*输出： 无
	*返回值：无
	*****************************************************************************/
	virtual void OnNETEC_MCURouterDisconnected(const char*cszDomain,const char* pData,int nLen){};

	static const char*GetMCULocalNodeID(void);

};

#endif