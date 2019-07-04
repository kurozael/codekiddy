/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef NETWORK_H
#define NETWORK_H

#include <ClanLib/network.h>
#include <ClanLib/core.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Lua.h"

enum NetType
{
	CLIENT,
	SERVER,
	NONE
};

struct NetMessage
{
public:
	std::string ip;
	std::string port;
	std::string data;
};

class Network : public CNonCopyable
{
public:
	void OnConnected();
	void OnDisconnected();
	void OnClientJoin(CL_NetGameConnection* connection);
	void OnClientLeave(CL_NetGameConnection* connection);
	void OnClientEventReceived(const CL_NetGameEvent& gameEvent);
	void OnServerEventReceived(CL_NetGameConnection* connection, const CL_NetGameEvent& gameEvent);
public:
	void LuaBind(luabind::object& globals);
	void StartServer(const std::string& port);
	void StartClient(const std::string& ip, const std::string& port);
	CL_UDPSocket* udpBind(const std::string& port);
	void udpSend(const std::string& ip, const std::string& port, const std::string& data);
	bool IsServer();
	bool IsClient();
	void SendEvent(CL_NetGameConnection& connection, const std::string& name, const std::string& data);
	void SendEvent(const std::string& name, const std::string& data);
	std::string udpGetPort();
	void ThreadLoop();
	int GetNetType();
	bool IsNull();
	void Update();
	~Network();
	Network();
private:
	CL_NetGameClient m_client;
	CL_NetGameServer m_server;
	CL_SlotContainer m_slots;
	std::vector<NetMessage> m_netMsgs;
	int m_netType;
};

#define g_Network CSingleton<Network>::Instance()

#endif