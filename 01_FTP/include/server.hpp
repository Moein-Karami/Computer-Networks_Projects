#ifndef _Server_hpp
#define _Server_hpp

#include "defines_and_libs.hpp"
#include "socket_tool.hpp"
#include "ServerClient.hpp"
#include "User.hpp"
#include "json.hpp"
#include "CommandHandler.hpp"

class Server
{
	public:
		Server(char* argv[]);
		void setup();
		void run();
		int initial_socket(unsigned short port);
		int accept_new_request(int master_socket);
		
	private:  
		std::vector<User*> users;
		std::vector<Client*> clients;
		int command_fd;
		int data_fd;
		int command_port;
		int data_port;
		std::string root;
		std::vector<std::string> private_files;
		SocketTools socket_tools;
		CommandHandler command_handler;
		
};

#endif