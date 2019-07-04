/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Network.h"

CL_UDPSocket* gSocket;
CL_Thread* gThread;

void Network::OnConnected()
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnConnected"
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::OnDisconnected()
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnDisconnected"
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::OnClientJoin(CL_NetGameConnection* connection)
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnClientJoin", connection
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::OnClientLeave(CL_NetGameConnection* connection)
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnClientLeave", connection
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::OnClientEventReceived(const CL_NetGameEvent& gameEvent)
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnClientEventReceived", gameEvent.get_name().c_str(),
		gameEvent.get_argument(0).to_string().c_str()
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::OnServerEventReceived(CL_NetGameConnection* connection, const CL_NetGameEvent& gameEvent)
{
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnServerEventReceived", connection,
		gameEvent.get_name().c_str(), gameEvent.get_argument(0).to_string().c_str()
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Network::StartServer(const std::string& port)
{
	if (!IsNull()) return;
	
	m_server.start(port);
	m_netType = SERVER;
}

void Network::StartClient(const std::string& ip, const std::string& port)
{
	if (!IsNull()) return;

	m_client.connect(ip, port);
	m_netType = CLIENT;
}

int Network::GetNetType()
{
	return m_netType;
}

void Network::SendEvent(CL_NetGameConnection& connection, const std::string& name, const std::string& data)
{
	if (!IsServer()) return;

	connection.send_event(
		CL_NetGameEvent(name.c_str(), data.c_str())
	);
}

void Network::SendEvent(const std::string& name, const std::string& data)
{
	if (IsNull()) return;

	CL_NetGameEvent gameEvent(name.c_str(), data.c_str());
	
	if (IsServer())
		m_server.send_event(gameEvent);
	else
		m_client.send_event(gameEvent);
}

bool Network::IsServer()
{
	return (m_netType == SERVER);
}

bool Network::IsClient()
{
	return (m_netType == CLIENT);
}

void Network::ThreadLoop()
{
	CL_Mutex mutex;

	//while (gSocket->get_read_event().wait(60 * 10000))
	while (gSocket->get_read_event().wait(-1))
	{
		CL_SocketName senderName;
		NetMessage netMessage;

		char buffer[16*1024];
		int received = gSocket->receive(buffer, 16*1024, senderName);
		if (received <= 0) { break; }

		netMessage.ip = std::string(senderName.get_address().c_str());
		netMessage.port = std::string(senderName.get_port().c_str());
		netMessage.data = std::string(buffer);
		
		if (netMessage.data != "")
		{
			mutex.lock();
				m_netMsgs.push_back(netMessage);
			mutex.unlock();
		}
	}
}

CL_UDPSocket* Network::udpBind(const std::string& port)
{
	if (gSocket != NULL)
		delete gSocket;

	if (gThread != NULL)
	{
		gThread->kill();
		delete gThread;
	}

	gSocket = new CL_UDPSocket;
	gThread = new CL_Thread;

	if (port != "")
		gSocket->bind(CL_SocketName(port));

	gThread->start(this, &Network::ThreadLoop);
	
	return gSocket;
}

std::string Network::udpGetPort()
{
	if (gSocket != NULL)
		return gSocket->get_local_name().get_port();
	else
		return "";
}

void Network::udpSend(const std::string& ip, const std::string& port, const std::string& data)
{
	if (gSocket != NULL)
	{
		gSocket->send(data.data(), data.length(), CL_SocketName(ip, port));
	}
}

void Network::Update()
{
	for (unsigned int i = 0; i < m_netMsgs.size(); i++)
	{
		//try
		//{

		luabind::call_function<void>(
			g_Lua->State(), "OnDatagramReady", m_netMsgs[i].data.c_str(), m_netMsgs[i].ip.c_str(), m_netMsgs[i].port.c_str()
		);

		//}
		//catch (const luabind::error &e)
		//{
		//	g_Lua->Print( lua_tostring(e.state(), -1) );
		//}
	}

	m_netMsgs.clear();

	if (IsNull()) return;

	if (IsServer())
		m_server.process_events();
	else
		m_client.process_events();
}

bool Network::IsNull()
{
	return (m_netType == NONE);
}

void Network::LuaBind(luabind::object& globals)
{

	luabind::module( g_Lua->State() )
	[
		luabind::class_<CL_NetGameConnection>("GameConnection")
	];

	luabind::module( g_Lua->State() )
	[
		luabind::class_<CL_UDPSocket>("UDPSocket")
	];

	luabind::module( g_Lua->State() )
	[
		luabind::class_<Network>("Network")
		.def("udpGetPort", &Network::udpGetPort)
		.def("GetNetType", &Network::GetNetType)
		.def("StartServer", &Network::StartServer)
		.def("StartClient", &Network::StartClient)
		.def("SendEvent", (void(Network::*)(CL_NetGameConnection&, const std::string&, const std::string&)) &Network::SendEvent)
		.def("SendEvent", (void(Network::*)(const std::string&, const std::string&)) &Network::SendEvent)
		.def("IsClient", &Network::IsClient)
		.def("IsServer", &Network::IsServer)
		.def("udpBind", &Network::udpBind)
		.def("udpSend", &Network::udpSend)
		.def("IsNull", &Network::IsNull)
		.def("Update", &Network::Update)
	];

	// Add the network enums to the global table.
	globals["NET_SERVER"] = SERVER;
	globals["NET_CLIENT"] = CLIENT;
	globals["NET_NONE"] = NONE;

	globals["NetworkInstance"] = g_Network;
}

Network::Network()
{
	m_slots.connect(m_server.sig_client_connected(), this, &Network::OnClientJoin);
	m_slots.connect(m_server.sig_client_disconnected(), this, &Network::OnClientLeave);
	m_slots.connect(m_server.sig_event_received(), this, &Network::OnServerEventReceived);

	m_slots.connect(m_client.sig_connected(), this, &Network::OnConnected);
	m_slots.connect(m_client.sig_disconnected(), this, &Network::OnDisconnected);
	m_slots.connect(m_client.sig_event_received(), this, &Network::OnClientEventReceived);

	m_netType = NONE;
	gSocket = NULL;
	gThread = NULL;
}

Network::~Network()
{
	if (gThread != NULL)
	{
		gThread->kill();
		delete gThread;
	}

	if (gSocket != NULL)
		delete gSocket;

	gSocket = NULL;
	gThread = NULL;
}
