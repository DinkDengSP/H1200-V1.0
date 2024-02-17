/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 14:42:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 14:17:23 +0800
************************************************************************************************/ 
#include "SrvImplSocketNet.h"
#include "SrvTaskHeader.h"
#include "SocketNetState.h"

/*----------------------------------------Socket特性初始化功能--------------------------------------------*/
//获取设备IP
static uint32_t SrvImplSocketLAN8700GetIP(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t phyNetIp = 0XFFFFFFFF;
    GPIO_PinState pinState = BoardPortInReadState(BOARD_PORT_IN_M_SMBUS_TEST1);
    if(pinState == GPIO_PIN_SET)
    {
        //设置为默认IP
        phyNetIp = DEFAULT_VALUE_BOARD_PARAM_NET_PHY1_IP;
        MCU_LogPrintf("BoardPortIn Set Valid,Use Default IP,LAN8700 IP: 0X%08X\r\n",phyNetIp);
        return phyNetIp;
    }
    //获取IP
    errorSub = BoardParamReadNetPhy1IpCurrent(&phyNetIp);
    if(ERROR_SUB_OK != errorSub)
    {
        //读取失败,用默认IP
        MCU_LogPrintf("BoardParamReadNetPhyIP_Addr Failed,ErrorSub: 0X%08X\r\n",errorSub);
        phyNetIp = DEFAULT_VALUE_BOARD_PARAM_NET_PHY1_IP;
    }
    MCU_LogPrintf("LAN8700 IP: 0X%08X\r\n",phyNetIp);
    return phyNetIp;
}

//创建内存池
static void SrvImplSocketLAN8700CreateMemoryPool(void)
{
    SOCKET_STATE_MSG* lan8700NetBasePtr = SocketNetStateGetBaseStatePtr();
    //创建内存池
    SystemPrintf("SrvImplSocketLAN8700CreateMemoryPool Start\r\n");
    do
    {
        lan8700NetBasePtr->netMemPoolPtr = NetX_DriverLAN8700_CreateMemoryPool();
        if(lan8700NetBasePtr->netMemPoolPtr == NULL)
        {
            SystemPrintf("NetX_DriverLAN8700_CreateMemoryPool Failed\r\n");
            CoreDelayMs(10);
        }
    }while(lan8700NetBasePtr->netMemPoolPtr == NULL);
    SystemPrintf("SrvImplSocketLAN8700CreateMemoryPool Success Over\r\n");
}

//创建IP实例
static void SrvImplSocketLAN8700CreateInstanceIP(void)
{
    uint32_t nxResult = NX_SUCCESS;
    uint8_t ip0,ip1,ip2,ip3;
    //获取板卡IP
    uint32_t ipValue = SrvImplSocketLAN8700GetIP();
    ip0 = (uint8_t)ipValue;
    ipValue >>= 8;
    ip1 = (uint8_t)ipValue;
    ipValue >>= 8;
    ip2 = (uint8_t)ipValue;
    ipValue >>= 8;
    ip3 = (uint8_t)ipValue; 
    //IP地址计算
    ULONG ipAddress = IP_ADDRESS(ip0, ip1, ip2, ip3);
    //输出IP地址
    MCU_LogPrintf("%s Start,IP: %d.%d.%d.%d\r\n",__func__,ip0,ip1,ip2,ip3);
    //IP实例化
    SystemPrintf("%s Start,IP: %d.%d.%d.%d\r\n",__func__,ip0,ip1,ip2,ip3);
    do
    {
        //实例化IP
        nxResult = nx_ip_create(&lan8700NetX,          //IP对象
                                "SrvTaskIP_LAN8700",   //创建的线程名称            
                                ipAddress,              //IP地址
                                0xFFFFFF00UL,           //子网掩码
                                SocketNetStateGetMemPoolPtr(),   //内存池 
                                NetX_DriverLAN8700,              //网卡驱动
                                (UCHAR*)stackBufferSrvTaskPhyNetX,  //IP任务栈地址
                                STK_SIZE_SRV_TASK_NETX_IP_PHY,       //IP任务栈大小，单位字节
                                PRIO_SRV_TASK_NETX_IP_PHY_LOW);      //IP任务优先级
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_ip_create Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("%s Over\r\n",__func__);
}

//打开IP各项特性
static void SrvImplSocketLAN8700EnableFeatureIP(NX_IP* interfaceIP)
{
    uint32_t nxResult = NX_SUCCESS;
    SystemPrintf("%s Start\r\n",__func__);
    //使能ARP，并提供ARP缓存
    SystemPrintf("LAN8700 nx_arp_enable Start\r\n");
    do
    {
        //每个ARP条目均为52个字节，因此，ARP 条目总数是52字节整数倍
        nxResult =  nx_arp_enable(interfaceIP,(void *)lan8700ArpTableBufNetX,NX_LAN8700_ARP_LENGTH); 
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_arp_enable Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 nx_arp_enable Success Over\r\n");
    //使能IP分片
    SystemPrintf("LAN8700 nx_ip_fragment_enable Start\r\n");
    do
    {
        nxResult = nx_ip_fragment_enable(interfaceIP);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_ip_fragment_enable Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 nx_ip_fragment_enable Success Over\r\n");
    //使能TCP
    SystemPrintf("LAN8700 nx_tcp_enable Start\r\n");
    do
    {
        nxResult =  nx_tcp_enable(interfaceIP);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_tcp_enable Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 nx_tcp_enable Success Over\r\n");
    //使能UDP
    SystemPrintf("LAN8700 nx_udp_enable Start\r\n");
    do
    {
        nxResult =  nx_udp_enable(interfaceIP);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_udp_enable Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 nx_udp_enable Success Over\r\n");
    //使能ICMP
    SystemPrintf("LAN8700 nx_icmp_enable Start\r\n");
    do
    {
        nxResult =  nx_icmp_enable(interfaceIP);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_icmp_enable Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 nx_icmp_enable Success Over\r\n");
    SystemPrintf("%s Success Over\r\n",__func__);
}

//等待IP实例化完成
static void SrvImplSocketLAN8700WaitInitializeIP(void)
{
    SystemPrintf("%s Start\r\n",__func__);
    //初始化完成,等待IP线程初始化,然后切换任务到高优先级
    while(netx_lan8700_thread_ptr == NULL)
    {
        CoreDelayMinTick();
    }
    SystemPrintf("netx_lan8700_thread_ptr InitOver ,Change High Priority: %d\r\n",PRIO_SRV_TASK_NETX_IP_PHY_HIGH);
    uint32_t old_priority;
    //修改IP线程优先级到高优先级
    tx_thread_priority_change(netx_lan8700_thread_ptr, PRIO_SRV_TASK_NETX_IP_PHY_HIGH, &old_priority);
    SystemPrintf("netx_lan8700_thread_ptr Change High Priotity Over,Old Priority: %d\r\n",old_priority);
    SystemPrintf("%s Success Over\r\n",__func__);
}

//初始化PHY连接状态
static void SrvImplSocketLAN8700InitPhyConnectState(void)
{
    uint32_t nxResult = NX_SUCCESS;
    SOCKET_STATE_MSG* lan8700NetBasePtr = SocketNetStateGetBaseStatePtr();
    //首先检测连接状态
    nxResult = nx_ip_driver_direct_command(&lan8700NetX, NX_LINK_GET_STATUS,&(lan8700NetBasePtr->lanPhyConnectStateCurrent));
    //如果检测到网线未连接,则自动关闭Socket,重新建立Socket连接
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700 nx_ip_driver_direct_command NX_LINK_GET_STATUS Failed: %02X\r\n",nxResult);
        lan8700NetBasePtr->lanPhyConnectStateCurrent = NX_FALSE;
        lan8700NetBasePtr->lanPhyConnectStateLast = NX_FALSE;
        return;
    }
    //获取到了网线连接状态
    lan8700NetBasePtr->lanPhyConnectStateLast = lan8700NetBasePtr->lanPhyConnectStateCurrent;
    if(lan8700NetBasePtr->lanPhyConnectStateCurrent == NX_TRUE)
    {
        //打印网线连接状态
        SystemPrintf("LAN8700 Init PhyLinkState Connect\r\n");
    }
    else
    {
        //打印网线连接状态
        SystemPrintf("LAN8700 Init PhyLinkState DisConnect!!\r\n");
    }
}

//获取Socket状态
static uint32_t SrvImplSocketLAN8700PortSocketInfoGet(NX_TCP_SOCKET* socketInstance,NX_SOCKET_INFO* socketInfoPtr)
{
    uint32_t nxResult;
    NX_SOCKET_INFO socketInfoLocal;
    nxResult = nx_tcp_socket_info_get(socketInstance,//TCP Socket控制块 */
                                      &(socketInfoLocal.tcp_packets_sent),          //发送的TCP数据包总数目 */
                                      &(socketInfoLocal.tcp_bytes_sent),            //发送的TCP总字节数 */
                                      &(socketInfoLocal.tcp_packets_received),      //接收TCP数据包总数目 */
                                      &(socketInfoLocal.tcp_bytes_received),        //接收的TCP总字节数 */
                                      &(socketInfoLocal.tcp_retransmit_packets),    //重新传输的TCP数据包总数目 */
                                      &(socketInfoLocal.tcp_packets_queued),        //Socket上TCP排队的TCP数据包总数 */
                                      &(socketInfoLocal.tcp_checksum_errors),       //Socket上有校验和错误的TCP数据包总数 */
                                      &(socketInfoLocal.tcp_socket_state),          //Socket当前状态 */
                                      &(socketInfoLocal.tcp_transmit_queue_depth),  //仍在排队等待ACK的发送数据包总数 */
                                      &(socketInfoLocal.tcp_transmit_window),       //当前发送窗口大小 */
                                      &(socketInfoLocal.tcp_receive_window));       //当前接收窗口大小 */
    if(nxResult != NX_SUCCESS)
    {
        //读取失败
        return nxResult;
    }
    //后续可以考虑是否进行变化显示
    //拷贝到实际变量
    UserMemCopy((uint8_t*)socketInfoPtr,(uint8_t*)&socketInfoLocal,LENGTH_NX_SOCKET_INFO);
    return nxResult;
}

//创建主通讯Socket
void SrvImplSocketLAN8700CreateSocketMain(void)
{
    uint32_t nxResult;
    //创建TCP Socket
    SystemPrintf("LAN8700 MainPort nx_tcp_socket_create Start\r\n");
    do
    {
        nxResult = nx_tcp_socket_create(&lan8700NetX,              //IP实例控制块  
                                        SocketNetStateGetMainSocketPtr(), //TCP控制块
                                        "tcpSocketMain",            //TCP Socket名
                                        NX_IP_NORMAL|NX_IP_MIN_DELAY|NX_IP_MAX_RELIABLE,//IP服务类型 
                                        NX_FRAGMENT_OKAY,           //使能IP分段 */ 
                                        NX_IP_TIME_TO_LIVE,         //指定一个 8 位的值，用于定义此数据包在被丢弃之前可通过的路由器数目
                                        TCP_SERVICE_MAIN_SOCKET_BUF_LENGTH, //TCP Socket接收队列中允许的最大字节数
                                        NX_NULL,                    //用于在接收流中检测到紧急数据时调用的回调函数
                                        NX_NULL);                   //TCP Socket另一端发出断开连接时调用的回调函数
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_tcp_socket_create Main Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 MainPort nx_tcp_socket_create Success Over\r\n");
}

//创建日志通讯Socket
void SrvImplSocketLAN8700CreateSocketLog(void)
{
    uint32_t nxResult;
    //创建TCP Socket
    SystemPrintf("LAN8700 LogPort nx_tcp_socket_create Start\r\n");
    do
    {
        nxResult = nx_tcp_socket_create(&lan8700NetX,              //IP实例控制块 */    
                                        SocketNetStateGetLogSocketPtr(),  //TCP控制块 */ 
                                        "tcpSocketLog",             //TCP Socket名 */ 
                                        NX_IP_NORMAL|NX_IP_MIN_DELAY|NX_IP_MAX_RELIABLE,//IP服务类型 
                                        NX_FRAGMENT_OKAY,           //使能IP分段 */ 
                                        NX_IP_TIME_TO_LIVE,         //指定一个 8 位的值，用于定义此数据包在被丢弃之前可通过的路由器数目
                                        TCP_SERVICE_LOG_SOCKET_BUF_LENGTH, //TCP Socket接收队列中允许的最大字节数
                                        NX_NULL,                    //用于在接收流中检测到紧急数据时调用的回调函数
                                        NX_NULL);                   //TCP Socket另一端发出断开连接时调用的回调函数
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 nx_tcp_socket_create Log Failed,nxResult: %d\r\n",nxResult);
            CoreDelayMs(10);
        }
    }while(nxResult != NX_SUCCESS);
    SystemPrintf("LAN8700 LogPort nx_tcp_socket_create Success Over\r\n");
}

//上次检测PHY状态的时间
static uint32_t lastCheckLAN8700PhyConnectStateTimeStamp = 0;
//网卡协议栈初始化
void SrvImplSocketLAN8700StackInit(void)
{
    if(netx_lan8700_thread_ptr != NULL)
    {
        //网络堆栈已经初始化完成,不需要再次初始化了
        return;
    }
    //创建内存池
    SrvImplSocketLAN8700CreateMemoryPool();
    //IP实例化
    SrvImplSocketLAN8700CreateInstanceIP();
    //打开IP实例的各项特性
    SrvImplSocketLAN8700EnableFeatureIP(&lan8700NetX);
    //等待IP实例化完成
    SrvImplSocketLAN8700WaitInitializeIP();
    //打开ETH中断
    MCU_ETH_EnableInt();
    //初始化网线连接状态
    SrvImplSocketLAN8700InitPhyConnectState();
    //创建主通讯Socket
    SrvImplSocketLAN8700CreateSocketMain();
    //创建日志通讯Socket
    SrvImplSocketLAN8700CreateSocketLog();
    //打印初始化完成日志
    SystemPrintf("%s Success Over\r\n",__func__);
    //设置初始化完成
    SocketNetStateSetBaseInitOver();
    //设置检测时间戳
    lastCheckLAN8700PhyConnectStateTimeStamp = tx_time_get();
}

/*----------------------------------------网线插拔检测功能------------------------------------------------*/
//重建主通讯端口
static void SrvImplSocketLAN8700MainPortReCreate(void)
{
    uint32_t nxResult;
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_disconnect Start\r\n");
    //Socket断开,等待检测到网卡未连接或者正常退出
    do
    {
        nxResult = nx_tcp_socket_disconnect(SocketNetStateGetMainSocketPtr(),NX_WAIT_FOREVER);
        if(NX_NOT_CONNECTED == nxResult)
        {
            SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_disconnect NX_NOT_CONNECTED\r\n");
            break;
        }
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_disconnect Failed: 0x%02x\r\n",nxResult);
        }
        else
        {
            SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_disconnect Success Over\r\n");
            break;
        }
    }while(1);
    //解除Socket和服务器端口的绑定
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unaccept Start\r\n");
    nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetMainSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unaccept Success Over\r\n");
    //取消监听,之后就会进入CLOSE模式
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unlisten Start\r\n");
    nxResult = nx_tcp_server_socket_unlisten(&lan8700NetX, TCP_MAIN_SOCKET_PORT);
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unlisten,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_server_socket_unlisten,Success Over\r\n");
    //删除Socket
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_delete Start\r\n");
    nxResult = nx_tcp_socket_delete(SocketNetStateGetMainSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_delete,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_MainPort nx_tcp_socket_delete Success Over\r\n");
    //重建Socket
    SrvImplSocketLAN8700CreateSocketMain();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetMainSocketPtr(),SocketNetStateGetMainSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_MainPort PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return;
    }
    //状态复制
    SocketNetStateSetMainSocketStateCurrentByRecord(SocketNetStateGetMainSocketStateByInfo());
    SocketNetStateSetMainSocketStateLastByRecord(SocketNetStateGetMainSocketStateByInfo());
}

//重建日志端口
static void SrvImplSocketLAN8700LogPortReCreate(void)
{
    uint32_t nxResult;
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_disconnect Start\r\n");
    //Socket断开,等待检测到网卡未连接或者正常退出
    do
    {
        nxResult = nx_tcp_socket_disconnect(SocketNetStateGetLogSocketPtr(),NX_WAIT_FOREVER);
        if(NX_NOT_CONNECTED == nxResult)
        {
            SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_disconnect NX_NOT_CONNECTED\r\n");
            break;
        }
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_disconnect Failed: 0x%02x\r\n",nxResult);
        }
        else
        {
            SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_disconnect Success Over\r\n");
            break;
        }
    }while(1);
    //解除Socket和服务器端口的绑定
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unaccept Start\r\n");
    nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetLogSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unaccept Success Over\r\n");
    //取消监听,之后就会进入CLOSE模式
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unlisten Start\r\n");
    nxResult = nx_tcp_server_socket_unlisten(&lan8700NetX, TCP_LOG_SOCKET_PORT);
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unlisten,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_server_socket_unlisten,Success Over\r\n");
    //删除Socket
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_delete Start\r\n");
    nxResult = nx_tcp_socket_delete(SocketNetStateGetLogSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_delete,Failed: 0X%02X\r\n",nxResult);
        return;
    }
    SystemPrintf("LAN8700AllPortFSM_LogPort nx_tcp_socket_delete Success Over\r\n");
    //重建Socket
    SrvImplSocketLAN8700CreateSocketLog();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetLogSocketPtr(),SocketNetStateGetLogSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700AllPortFSM_LogPort PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return;
    }
    //状态复制
    SocketNetStateSetLogSocketStateCurrentByRecord(SocketNetStateGetLogSocketStateByInfo());
    SocketNetStateSetLogSocketStateLastByRecord(SocketNetStateGetLogSocketStateByInfo());
}

//网线拔掉连续监测到的次数
static uint16_t phyDisConnectCount = 0;
//所有通讯网口状态机
void SrvImplSocketLAN8700PhyLinkFSM(void)
{
    uint32_t nxResult;
    SOCKET_STATE_MSG* lan8700NetBasePtr = SocketNetStateGetBaseStatePtr();
    if(tx_time_get() - lastCheckLAN8700PhyConnectStateTimeStamp < TIME_MS_PHY_DISCONNECT_CHECK_PERIOD)
    {
        return;
    }
    //时间更新
    lastCheckLAN8700PhyConnectStateTimeStamp = tx_time_get();
    //周期检测,首先检测连接状态
    nxResult = nx_ip_driver_direct_command(&lan8700NetX, NX_LINK_GET_STATUS,&(lan8700NetBasePtr->lanPhyConnectStateCurrent));
    //如果检测到网线未连接,则自动关闭Socket,重新建立Socket连接
    if(nxResult != NX_SUCCESS)
    {
        //读不到网络状态,就当成断网了
        SystemPrintf("LAN8700AllPortFSM nx_ip_driver_direct_command NX_LINK_GET_STATUS Failed: %02X\r\n",nxResult);
        lan8700NetBasePtr->lanPhyConnectStateCurrent = NX_FALSE;
    }
    //网络连上,断开计数清零
    if(lan8700NetBasePtr->lanPhyConnectStateCurrent == NX_TRUE)
    {
        phyDisConnectCount = 0;
    }
    if((lan8700NetBasePtr->lanPhyConnectStateCurrent == NX_FALSE)&&(lan8700NetBasePtr->lanPhyConnectStateLast == NX_TRUE))
    {
        phyDisConnectCount++;
        //对链接状态计数
        if(phyDisConnectCount < COUNT_MAX_PHY_DISCONNECT_WAIT_STABLE)
            return;
        if(phyDisConnectCount >= COUNT_MAX_PHY_DISCONNECT_WAIT_STABLE)
        {
            //防止溢出
            phyDisConnectCount = COUNT_MAX_PHY_DISCONNECT_WAIT_STABLE;
        }
        //达到指定的次数,检测到网线断开
        SystemPrintf("LAN8700AllPortFSM PhyPort Check DisConnect,LastState Connect\r\n");
        //执行完成,状态机转移
        lan8700NetBasePtr->lanPhyConnectStateLast = lan8700NetBasePtr->lanPhyConnectStateCurrent;
        //更新主通讯网口Socket状态
        SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetMainSocketPtr(),SocketNetStateGetMainSocketInfoPtr());
        //更新日志Socket状态
        SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetLogSocketPtr(),SocketNetStateGetLogSocketInfoPtr());
        //如果主网口处于链接状态,关闭连接
        if(SocketNetStateGetMainSocketStateByInfo() == NX_TCP_ESTABLISHED)
        {
            SrvImplSocketLAN8700MainPortReCreate();
        }
        //如果调试网口处于链接状态,关闭链接
        if(SocketNetStateGetLogSocketStateByInfo() == NX_TCP_ESTABLISHED)
        {
            SrvImplSocketLAN8700LogPortReCreate();
        }
    }
    else if((lan8700NetBasePtr->lanPhyConnectStateCurrent == NX_TRUE)&&(lan8700NetBasePtr->lanPhyConnectStateLast == NX_FALSE))
    {
        //检测到插入
        SystemPrintf("LAN8700AllPortFSM PhyPort Check Connect,LastState DisConnect\r\n");
        //执行完成,状态转移
        lan8700NetBasePtr->lanPhyConnectStateLast = lan8700NetBasePtr->lanPhyConnectStateCurrent;
    }
}

/*----------------------------------------主网口通讯状态机------------------------------------------------*/
//Socket关闭状态下执行的状态机功能
static void SrvImplSocketLAN8700MainPortFSM_Closed(void)
{
    uint32_t nxResult;
    //Socket已经关闭,切换Listen
    SystemPrintf("LAN8700MainPortFSM_Closed nx_tcp_server_socket_listen Start\r\n");
    nxResult = nx_tcp_server_socket_listen(&lan8700NetX,                  //IP实例控制块 */  
                                            TCP_MAIN_SOCKET_PORT,          //监听端口 */          
                                            SocketNetStateGetMainSocketPtr(),    //TCP Socket控制块
                                            TCP_MAIN_SOCKET_PORT_MAX_CLIENT,   //可以监听的连接数
                                            NULL);                                   //监听回调函数
    if(nxResult != NX_SUCCESS)
    {
        //监听失败
        SystemPrintf("LAN8700MainPortFSM_Closed nx_tcp_server_socket_listen Failed,nxResult: 0X%02X\r\n",nxResult);
    }
    else
    {
        //监听成功
        SystemPrintf("LAN8700MainPortFSM_Closed nx_tcp_server_socket_listen Success Over\r\n");
    }
}

//监听状态下,启用新的链接
static void SrvImplSocketLAN8700MainPortFSM_Listen(uint32_t lastSocketState)
{
    uint32_t nxResult;
    //持续监听
    if((lastSocketState == NX_TCP_SYN_RECEIVED)||(lastSocketState == NX_TCP_LISTEN_STATE))
    {
        //关闭套接字接收
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept Start\r\n");
        nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetMainSocketPtr()); 
        if(nxResult == NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept,Success Over\r\n");
        }
        else
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        }
        //重新listen
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten Start\r\n");
        nxResult = nx_tcp_server_socket_relisten(&lan8700NetX, TCP_MAIN_SOCKET_PORT, SocketNetStateGetMainSocketPtr());
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten,Failed: 0X%02X\r\n",nxResult);
            return;
        }
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten Success Over\r\n");
    }
    if(lastSocketState == NX_TCP_ESTABLISHED)
    {
        //上一次是已连接,这个时候变成监听,说明可能是调试导致上位机断开了连接
        //关闭套接字接收
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept Start\r\n");
        nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetMainSocketPtr()); 
        if(nxResult == NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept,Success Over\r\n");
        }
        else
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        }
        //重新listen
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten Start\r\n");
        nxResult = nx_tcp_server_socket_relisten(&lan8700NetX, TCP_MAIN_SOCKET_PORT, SocketNetStateGetMainSocketPtr());
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten,Failed: 0X%02X\r\n",nxResult);
            return;
        }
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_relisten Success Over\r\n");
    }
    //监听状态,启用新的链接接收
    SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_accept Start\r\n");
    nxResult = nx_tcp_server_socket_accept(SocketNetStateGetMainSocketPtr(),NX_NO_WAIT);
    if(nxResult == NX_IN_PROGRESS)
    {
        //正在处理,目前不用管
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_accept,NX_IN_PROGRESS\r\n");
    }
    else if(nxResult == NX_NOT_CONNECTED)
    {
        //未连接
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_accept,NX_NOT_CONNECTED\r\n");
    }
    else if(nxResult == NX_SUCCESS)
    {
        //接收链接成功
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_accept,Success Over\r\n");
    }
    else
    {
        //其他状态
        SystemPrintf("LAN8700MainPortFSM_Listen nx_tcp_server_socket_accept,Failed: 0X%02X\r\n",nxResult);
    }
    return;
}

//链接关闭的情况下执行的操作
static void SrvImplSocketLAN8700MainPortFSM_CloseWait(void)
{
    uint32_t nxResult;
    //解除Socket和服务器端口的绑定
    SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unaccept Start\r\n");
    nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetMainSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        CoreDelayMs(500);
    }
    SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unaccept Success Over\r\n");
    //解除监听
    SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unlisten Start\r\n");
    nxResult = nx_tcp_server_socket_unlisten(&lan8700NetX, TCP_MAIN_SOCKET_PORT);
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unlisten,Failed: 0X%02X\r\n",nxResult);
        CoreDelayMs(500);
    }
    SystemPrintf("LAN8700MainPortFSM_CloseWait nx_tcp_server_socket_unlisten Success Over\r\n");
}

//Socket接收数据包指针
static NX_PACKET *tcpSocketMainPortRecvDataPackPtr = NULL;
//链接已经建立的情况下执行的操作
static void SrvImplSocketLAN8700MainPortFSM_Established(void)
{
    uint32_t nxResult;
    //读取到了数据,解析出来的接受数据的长度
    ULONG recvDataLength = 0;
    //接受的数据缓存
    uint8_t* recvDataBufferPtr = NULL;
    //远程连接的IP
    ULONG peerIpAddressMainPort = 0;
    //远程端口号
    ULONG peerPortMainPort = 0;
    //实际解包出来的数据长度
    ULONG readByteCountReal = 0;
    //网口数据包指针
    NET_RECV_PACK* netRecvPackPtr = NULL;
    while(1)
    {
        tcpSocketMainPortRecvDataPackPtr = NULL;
        //读取到了数据,解析出来的接受数据的长度
        recvDataLength = 0;
        //接受的数据缓存
        recvDataBufferPtr = NULL;
        //远程连接的IP
        peerIpAddressMainPort = 0;
        //远程端口号
        peerPortMainPort = 0;
        //实际解包出来的数据长度
        readByteCountReal = 0;
        //网口数据包指针
        netRecvPackPtr = NULL;
        //到这里,连接已经建立,接收数据包
        nxResult = nx_tcp_socket_receive(SocketNetStateGetMainSocketPtr(),&tcpSocketMainPortRecvDataPackPtr, NX_NO_WAIT);  
        if(nxResult != NX_SUCCESS)
        {
            //没有数据或者数据读取失败
            break;
        }
        //解析数据长度
        nxResult = nx_packet_length_get(tcpSocketMainPortRecvDataPackPtr,&recvDataLength);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM nx_packet_length_get Failed: 0X%02X\r\n",nxResult);
            //释放接收数据包内存
            nx_packet_release(tcpSocketMainPortRecvDataPackPtr);
            continue;
        }
        //获得的数据包为0,不用理会
        if(recvDataLength == 0)
        {
            //打印接收到0数据包
            SystemPrintf("LAN8700MainPortFSM RecvZeroPackData\r\n");
            //释放接收数据包内存
            nx_packet_release(tcpSocketMainPortRecvDataPackPtr);
            continue;
        }
        //申请内存
        recvDataBufferPtr = UserMemMalloc(NETX_LAN8700_MEM_REGION,recvDataLength);
        if(recvDataBufferPtr == NULL)
        {
            //内存申请失败
            SystemPrintf("LAN8700MainPortFSM recvDataBufferPtr MemMalloc Failed\r\n");
            //释放接收数据包内存
            nx_packet_release(tcpSocketMainPortRecvDataPackPtr);
            continue;
        }
        //内存清空
        UserMemClear((void*)recvDataBufferPtr,recvDataLength);
        //获取远程数据IP和远程数据端口号
        nx_tcp_socket_peer_info_get(SocketNetStateGetMainSocketPtr(),&peerIpAddressMainPort,&peerPortMainPort); 
        //获取客户端发来的数据
        nxResult = nx_packet_data_retrieve(tcpSocketMainPortRecvDataPackPtr,recvDataBufferPtr,&readByteCountReal); 
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700MainPortFSM nx_packet_data_retrieve Failed: 0X%02X\r\n",nxResult);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
            //释放接收数据包内存
            nx_packet_release(tcpSocketMainPortRecvDataPackPtr);
            continue;
        }
        //释放接收数据包内存
        nx_packet_release(tcpSocketMainPortRecvDataPackPtr);
        //组包创建数据包
        netRecvPackPtr = UserMemMalloc(NETX_LAN8700_MEM_REGION,LENGTH_NET_RECV_PACK);
        if(netRecvPackPtr == NULL)
        {
            SystemPrintf("LAN8700MainPortFSM netRecvPackPtr Malloc Failed: 0X%02X\r\n",nxResult);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
            continue;
        }
        //数据填充
        netRecvPackPtr->netPackSrc = NET_PACK_SRC_MAIN;
        netRecvPackPtr->peerIpAddress = peerIpAddressMainPort;
        netRecvPackPtr->peerPort = peerPortMainPort;
        netRecvPackPtr->recvDataLength = readByteCountReal;
        netRecvPackPtr->recvDatBufPtr = recvDataBufferPtr;
        //将数据发送到处理线程
        uint32_t txResult;
        txResult = tx_queue_send(&queueSrvTaskSocketNet,(void*)(&netRecvPackPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //发送失败
            SystemPrintf("LAN8700MainPortFSM QueueSendFailed: %d\r\n",txResult);
            //内存释放
            UserMemFree(NETX_LAN8700_MEM_REGION,netRecvPackPtr);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
        }
    }
}

//主通讯端口状态机
void SrvImplSocketLAN8700MainPortFSM(void)
{
    uint32_t nxResult;
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetMainSocketPtr(),SocketNetStateGetMainSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700MainPortFSM PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return;
    }
    //状态复制
    SocketNetStateSetMainSocketStateCurrentByRecord(SocketNetStateGetMainSocketStateByInfo());
    //打印状态标记
    if(SocketNetStateGetMainSocketStateCurrentByRecord() != SocketNetStateGetMainSocketStateLastByRecord())
    {
        //打印状态变化
        SystemPrintf("LAN8700 MainPortStateChange,Current: %s,Last: %s\r\n",
                            SocketNetStateGetSocketState(SocketNetStateGetMainSocketStateCurrentByRecord()),
                            SocketNetStateGetSocketState(SocketNetStateGetMainSocketStateLastByRecord()));
    }
    //打印相关Socket信息
    switch(SocketNetStateGetMainSocketStateCurrentByRecord())
    {
        case NX_TCP_CLOSED:
            //Closed状态下执行的操作
            SrvImplSocketLAN8700MainPortFSM_Closed();
            break;
        case NX_TCP_LISTEN_STATE:
            //Listen状态下执行的操作
            SrvImplSocketLAN8700MainPortFSM_Listen(SocketNetStateGetMainSocketStateLastByRecord());
            break;
        case NX_TCP_SYN_SENT:
            //同步发送状态
            break;
        case NX_TCP_SYN_RECEIVED:
            //接收同步状态
            break;
        case NX_TCP_ESTABLISHED:
            //已连接状态,数据读取
            SrvImplSocketLAN8700MainPortFSM_Established();
            break;
        case NX_TCP_CLOSE_WAIT:
            //链接断开的情况下执行的操作
            SrvImplSocketLAN8700MainPortFSM_CloseWait();
            break;
        case NX_TCP_FIN_WAIT_1:
            break;
        case NX_TCP_FIN_WAIT_2:
            break;
        case NX_TCP_CLOSING:
            break;
        case NX_TCP_TIMED_WAIT:
            break;
        case NX_TCP_LAST_ACK:
            break;
        default:
            break;
    }
    //设置上一次状态和当前状态相同
    SocketNetStateSetMainSocketStateLastByRecord(SocketNetStateGetMainSocketStateCurrentByRecord());
    return;
}

/*----------------------------------------日志网口通讯状态机------------------------------------------------*/
//Socket关闭状态下执行的状态机功能
static void SrvImplSocketLAN8700LogPortFSM_Closed(void)
{
    uint32_t nxResult;
    //Socket已经关闭,切换Listen
    SystemPrintf("LAN8700 LogPortFSM_Closed nx_tcp_server_socket_listen Start\r\n");
    nxResult = nx_tcp_server_socket_listen(&lan8700NetX,                //IP实例控制块 */  
                                            TCP_LOG_SOCKET_PORT,        //监听端口 */          
                                            SocketNetStateGetLogSocketPtr(),  //TCP Socket控制块
                                            TCP_LOG_SOCKET_PORT_MAX_CLIENT,   //可以监听的连接数
                                            NULL);                         //监听回调函数
    if(nxResult != NX_SUCCESS)
    {
        //监听失败
        SystemPrintf("LAN8700 LogPortFSM_Closed nx_tcp_server_socket_listen Failed,nxResult: 0X%02X\r\n",nxResult);
    }
    else
    {
        //监听成功
        SystemPrintf("LAN8700 LogPortFSM_Closed nx_tcp_server_socket_listen Success Over\r\n");
    }
}

//监听状态下,启用新的链接
static void SrvImplSocketLAN8700LogPortFSM_Listen(uint32_t lastSocketState)
{
    uint32_t nxResult;
    //持续监听
    if((lastSocketState == NX_TCP_SYN_RECEIVED)||(lastSocketState == NX_TCP_LISTEN_STATE))
    {
        //关闭套接字接收
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept Start\r\n");
        nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetLogSocketPtr()); 
        if(nxResult == NX_SUCCESS)
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept,Success Over\r\n");
        }
        else
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        }
        //重新listen
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten Start\r\n");
        nxResult = nx_tcp_server_socket_relisten(&lan8700NetX, TCP_LOG_SOCKET_PORT, SocketNetStateGetLogSocketPtr());
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten,Failed: 0X%02X\r\n",nxResult);
            return;
        }
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten Success Over\r\n");
    }
    if(lastSocketState == NX_TCP_ESTABLISHED)
    {
        //上一次是已连接,这个时候变成监听,说明可能是调试导致上位机断开了连接
        //关闭套接字接收
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept Start\r\n");
        nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetLogSocketPtr()); 
        if(nxResult == NX_SUCCESS)
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept,Success Over\r\n");
        }
        else
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        }
        //重新listen
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten Start\r\n");
        nxResult = nx_tcp_server_socket_relisten(&lan8700NetX, TCP_LOG_SOCKET_PORT, SocketNetStateGetLogSocketPtr());
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten,Failed: 0X%02X\r\n",nxResult);
            return;
        }
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_relisten Success Over\r\n");
    }
    //监听状态,启用新的链接接收
    SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_accept Start\r\n");
    nxResult = nx_tcp_server_socket_accept(SocketNetStateGetLogSocketPtr(),NX_NO_WAIT);
    if(nxResult == NX_IN_PROGRESS)
    {
        //正在处理,目前不用管
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_accept,NX_IN_PROGRESS\r\n");
    }
    else if(nxResult == NX_NOT_CONNECTED)
    {
        //未连接
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_accept,NX_NOT_CONNECTED\r\n");
    }
    else if(nxResult == NX_SUCCESS)
    {
        //接收链接成功
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_accept,Success Over\r\n");
    }
    else
    {
        //其他状态
        SystemPrintf("LAN8700 LogPortFSM_Listen nx_tcp_server_socket_accept,Failed: 0X%02X\r\n",nxResult);
    }
    return;
}

//链接关闭的情况下执行的操作
static void SrvImplSocketLAN8700LogPortFSM_CloseWait(void)
{
    uint32_t nxResult;
    //解除Socket和服务器端口的绑定
    SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unaccept Start\r\n");
    nxResult = nx_tcp_server_socket_unaccept(SocketNetStateGetLogSocketPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unaccept,Failed: 0X%02X\r\n",nxResult);
        CoreDelayMs(500);
    }
    SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unaccept Success Over\r\n");
    //解除监听
    SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unlisten Start\r\n");
    nxResult = nx_tcp_server_socket_unlisten(&lan8700NetX, TCP_LOG_SOCKET_PORT);
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unlisten,Failed: 0X%02X\r\n",nxResult);
        CoreDelayMs(500);
    }
    SystemPrintf("LAN8700 LogPortFSM_CloseWait nx_tcp_server_socket_unlisten Success Over\r\n");
}

//Socket接收数据包指针
static NX_PACKET *tcpSocketLogPortRecvDataPackPtr = NULL;
//链接已经建立的情况下执行的操作
static void SrvImplSocketLAN8700LogPortFSM_Established(void)
{
    uint32_t nxResult;
    //读取到了数据,解析出来的接受数据的长度
    ULONG recvDataLength = 0;
    //接受的数据缓存
    uint8_t* recvDataBufferPtr = NULL;
    //远程连接的IP
    ULONG peerIpAddressLogPort = 0;
    //远程端口号
    ULONG peerPortLogPort = 0;
    //实际解包出来的数据长度
    ULONG readByteCountReal = 0;
    //网口数据包指针
    NET_RECV_PACK* netRecvPackPtr = NULL;
    while(1)
    {
        tcpSocketLogPortRecvDataPackPtr = NULL;
        //读取到了数据,解析出来的接受数据的长度
        recvDataLength = 0;
        //接受的数据缓存
        recvDataBufferPtr = NULL;
        //远程连接的IP
        peerIpAddressLogPort = 0;
        //远程端口号
        peerPortLogPort = 0;
        //实际解包出来的数据长度
        readByteCountReal = 0;
        //网口数据包指针
        netRecvPackPtr = NULL;
        //到这里,连接已经建立,接收数据包
        nxResult = nx_tcp_socket_receive(SocketNetStateGetLogSocketPtr(),&tcpSocketLogPortRecvDataPackPtr, NX_NO_WAIT);  
        if(nxResult != NX_SUCCESS)
        {
            //没有数据或者数据读取失败
            break;
        }
        //解析数据长度
        nxResult = nx_packet_length_get(tcpSocketLogPortRecvDataPackPtr,&recvDataLength);
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700LogPortFSM nx_packet_length_get Failed: 0X%02X\r\n",nxResult);
            //释放接收数据包内存
            nx_packet_release(tcpSocketLogPortRecvDataPackPtr);
            continue;
        }
        //获得的数据包为0,不用理会
        if(recvDataLength == 0)
        {
            //打印接收到0数据包
            SystemPrintf("LAN8700LogPortFSM RecvZeroPackData\r\n");
            //释放接收数据包内存
            nx_packet_release(tcpSocketLogPortRecvDataPackPtr);
            continue;
        }
        //申请内存
        recvDataBufferPtr = UserMemMalloc(NETX_LAN8700_MEM_REGION,recvDataLength);
        if(recvDataBufferPtr == NULL)
        {
            //内存申请失败
            SystemPrintf("LAN8700LogPortFSM recvDataBufferPtr MemMalloc Failed\r\n");
            //释放接收数据包内存
            nx_packet_release(tcpSocketLogPortRecvDataPackPtr);
            continue;
        }
        //内存清空
        UserMemClear((void*)recvDataBufferPtr,recvDataLength);
        //获取远程数据IP和远程数据端口号
        nx_tcp_socket_peer_info_get(SocketNetStateGetLogSocketPtr(),&peerIpAddressLogPort,&peerPortLogPort); 
        //获取客户端发来的数据
        nxResult = nx_packet_data_retrieve(tcpSocketLogPortRecvDataPackPtr,recvDataBufferPtr,&readByteCountReal); 
        if(nxResult != NX_SUCCESS)
        {
            SystemPrintf("LAN8700LogPortFSM nx_packet_data_retrieve Failed: 0X%02X\r\n",nxResult);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
            //释放接收数据包内存
            nx_packet_release(tcpSocketLogPortRecvDataPackPtr);
            continue;
        }
        //释放接收数据包内存
        nx_packet_release(tcpSocketLogPortRecvDataPackPtr);
        //组包创建数据包
        netRecvPackPtr = UserMemMalloc(NETX_LAN8700_MEM_REGION,LENGTH_NET_RECV_PACK);
        if(netRecvPackPtr == NULL)
        {
            SystemPrintf("LAN8700LogPortFSM netRecvPackPtr Malloc Failed: 0X%02X\r\n",nxResult);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
            continue;
        }
        //数据填充
        netRecvPackPtr->netPackSrc = NET_PACK_SRC_LOG;
        netRecvPackPtr->peerIpAddress = peerIpAddressLogPort;
        netRecvPackPtr->peerPort = peerPortLogPort;
        netRecvPackPtr->recvDataLength = readByteCountReal;
        netRecvPackPtr->recvDatBufPtr = recvDataBufferPtr;
        //将数据发送到处理线程
        uint32_t txResult;
        txResult = tx_queue_send(&queueSrvTaskSocketNet,(void*)(&netRecvPackPtr),TX_NO_WAIT);
        if(txResult != TX_SUCCESS)
        {
            //发送失败
            SystemPrintf("LAN8700LogPortFSM QueueSendFailed: %d\r\n",txResult);
            //内存释放
            UserMemFree(NETX_LAN8700_MEM_REGION,netRecvPackPtr);
            //释放申请的暂时内存呢
            UserMemFree(NETX_LAN8700_MEM_REGION,recvDataBufferPtr);
        }
    }
}

//日志通讯端口状态机
void SrvImplSocketLAN8700LogPortFSM(void)
{
    uint32_t nxResult;
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetLogSocketPtr(),SocketNetStateGetLogSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        SystemPrintf("LAN8700 LogPortFSM PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return;
    }
    //状态复制
    SocketNetStateSetLogSocketStateCurrentByRecord(SocketNetStateGetLogSocketStateByInfo());
    //打印状态标记
    if(SocketNetStateGetLogSocketStateCurrentByRecord() != SocketNetStateGetLogSocketStateLastByRecord())
    {
        //打印状态变化
        SystemPrintf("LAN8700 LogPortStateChange,Current: %s,Last: %s\r\n",
                            SocketNetStateGetSocketState(SocketNetStateGetLogSocketStateCurrentByRecord()),
                            SocketNetStateGetSocketState(SocketNetStateGetLogSocketStateLastByRecord()));
    }
    //打印相关Socket信息
    switch(SocketNetStateGetLogSocketStateCurrentByRecord())
    {
        case NX_TCP_CLOSED:
            //Closed状态下执行的操作
            SrvImplSocketLAN8700LogPortFSM_Closed();
            break;
        case NX_TCP_LISTEN_STATE:
            //Listen状态下执行的操作
            SrvImplSocketLAN8700LogPortFSM_Listen(SocketNetStateGetLogSocketStateLastByRecord());
            break;
        case NX_TCP_SYN_SENT:
            //同步发送状态
            break;
        case NX_TCP_SYN_RECEIVED:
            //接收同步状态
            break;
        case NX_TCP_ESTABLISHED:
            //已连接状态,数据读取
            SrvImplSocketLAN8700LogPortFSM_Established();
            break;
        case NX_TCP_CLOSE_WAIT:
            //链接断开的情况下执行的操作
            SrvImplSocketLAN8700LogPortFSM_CloseWait();
            break;
        case NX_TCP_FIN_WAIT_1:
            break;
        case NX_TCP_FIN_WAIT_2:
            break;
        case NX_TCP_CLOSING:
            break;
        case NX_TCP_TIMED_WAIT:
            break;
        case NX_TCP_LAST_ACK:
            break;
        default:
            break;
    }
    //设置上一次状态和当前状态相同
    SocketNetStateSetLogSocketStateLastByRecord(SocketNetStateGetLogSocketStateCurrentByRecord());
    return;
}


/*----------------------------------------Socket辅助功能------------------------------------------------*/
//主通讯网口数据发送
uint32_t SrvImplSocketLAN8700MainSendBuf(uint8_t* bufferPtr,uint32_t bufLength)
{
    //首先查看网口状态
    uint32_t nxResult;
    //获取互斥信号量
    NetStateMainPortDataSendGetLock();
    NX_SOCKET_INFO* socketInfoPtr = SocketNetStateGetMainSocketInfoPtr();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetMainSocketPtr(),socketInfoPtr);
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateMainPortDataSendReleaseLock();
        //日志打印
        SystemPrintf("LAN8700 MainPortSendBuf PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return nxResult;
    }
    if(socketInfoPtr->tcp_socket_state != NX_TCP_ESTABLISHED)
    {
        //释放互斥信号量
        NetStateMainPortDataSendReleaseLock();
        //数据未连接
        return NX_NOT_CONNECTED;
    }
    //数据已经链接,开始准备发送必须的条件
    NX_PACKET* sendPackDataPtr = NULL;
    //创建数据包
    nxResult = nx_packet_allocate(SocketNetStateGetMemPoolPtr(),&sendPackDataPtr,NX_TCP_PACKET,NX_NO_WAIT);
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateMainPortDataSendReleaseLock();
        //日志打印
        SystemPrintf("LAN8700 MainPortSendBuf SendPacketMemMalloc Failed: 0X%02X\r\n",nxResult);
        return nxResult;
    }
    //数据填入发送数据包
    nxResult = nx_packet_data_append(sendPackDataPtr,bufferPtr,bufLength,SocketNetStateGetMemPoolPtr(),NX_NO_WAIT);
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateMainPortDataSendReleaseLock();
        //日志打印
        SystemPrintf("LAN8700 MainPortSendBuf PacketDataAppend Failed: 0X%02X\r\n",nxResult);
        //释放内存
        nx_packet_release(sendPackDataPtr);
        return nxResult;
    }
    //发出数据包
    nxResult =  nx_tcp_socket_send(SocketNetStateGetMainSocketPtr(),sendPackDataPtr,TRANS_TIME_OUT_MS); 
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateMainPortDataSendReleaseLock();
        SystemPrintf("LAN8700 MainPortSendBuf,DataSendFailed: 0X%02X\r\n",nxResult);
        //释放内存
        nx_packet_release(sendPackDataPtr);
        return nxResult;
    }
    //释放互斥信号量
    NetStateMainPortDataSendReleaseLock();
    return nxResult;
}

//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplSocketLAN8700LogSendBuf(uint8_t* bufferPtr,uint32_t bufLength)
{
    //首先查看网口状态
    uint32_t nxResult;
    //获取互斥信号量
    NetStateLogPortDataSendGetLock();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetLogSocketPtr(),SocketNetStateGetLogSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateLogPortDataSendReleaseLock();
        //日志打印
        MCU_LogPrintf("LogPortSendBuf PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        return nxResult;
    }
    if(SocketNetStateGetLogSocketStateByInfo() != NX_TCP_ESTABLISHED)
    {
        //释放互斥信号量
        NetStateLogPortDataSendReleaseLock();
        //数据未连接
        return NX_NOT_CONNECTED;
    }
    //数据已经链接,开始准备发送必须的条件
    NX_PACKET* sendPackDataPtr = NULL;
    //创建数据包
    nxResult = nx_packet_allocate(SocketNetStateGetMemPoolPtr(),&sendPackDataPtr,NX_TCP_PACKET,NX_NO_WAIT);
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateLogPortDataSendReleaseLock();
        //日志打印
        MCU_LogPrintf("LogPortSendBuf SendPacketMemMalloc Failed: 0X%02X\r\n",nxResult);
        return nxResult;
    }
    //数据填入发送数据包
    nxResult = nx_packet_data_append(sendPackDataPtr,bufferPtr,bufLength,SocketNetStateGetMemPoolPtr(),NX_NO_WAIT);
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateLogPortDataSendReleaseLock();
        //日志打印
        MCU_LogPrintf("LogPortSendBuf PacketDataAppend Failed: 0X%02X\r\n",nxResult);
        //释放内存
        nx_packet_release(sendPackDataPtr);
        return nxResult;
    }
    //发出数据包
    nxResult =  nx_tcp_socket_send(SocketNetStateGetLogSocketPtr(),sendPackDataPtr,TRANS_TIME_OUT_MS); 
    if(nxResult != NX_SUCCESS)
    {
        //释放互斥信号量
        NetStateLogPortDataSendReleaseLock();
        MCU_LogPrintf("LogPortSendBuf,DataSendFailed: 0X%02X\r\n",nxResult);
        //释放内存
        nx_packet_release(sendPackDataPtr);
        return nxResult;
    }
    //释放互斥信号量
    NetStateLogPortDataSendReleaseLock();
    return nxResult;
}

//获取主通讯网口状态
ULONG SrvImplSocketLAN8700MainGetState(void)
{
    //首先查看网口状态
    uint32_t nxResult;
    //获取网口互斥信号量
    NetStateMainPortDataSendGetLock();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetMainSocketPtr(),SocketNetStateGetMainSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        //释放网口互斥信号量
        NetStateMainPortDataSendReleaseLock();
        //日志打印
        MCU_LogPrintf("MainPort PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        //读不到默认未连接
        return NX_TCP_CLOSED;
    }
    nxResult = SocketNetStateGetMainSocketStateByInfo();
    //释放网口互斥信号量
    NetStateMainPortDataSendReleaseLock();
    return nxResult;
}

//获取日志通讯网口状态
ULONG SrvImplSocketLAN8700LogGetState(void)
{
    //首先查看网口状态
    uint32_t nxResult;
    //获取网口互斥信号量
    NetStateLogPortDataSendGetLock();
    //读取Socket状态
    nxResult = SrvImplSocketLAN8700PortSocketInfoGet(SocketNetStateGetLogSocketPtr(),SocketNetStateGetLogSocketInfoPtr());
    if(nxResult != NX_SUCCESS)
    {
        //释放网口互斥信号量
        NetStateLogPortDataSendReleaseLock();
        //日志打印
        MCU_LogPrintf("LogPort PortSocketInfoGet Failed: 0X%02X\r\n",nxResult);
        //读不到默认未连接
        return NX_TCP_CLOSED;
    }
    nxResult = SocketNetStateGetLogSocketStateByInfo();
    //释放网口互斥信号量
    NetStateLogPortDataSendReleaseLock();
    return nxResult;
}











