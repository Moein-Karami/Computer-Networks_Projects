#ifndef _Socket_tool_hpp_
#define _Socket_tool_hpp_

#include "defines_and_libs.hpp"

class SocketTools
{
	public:
		int creat_socket();
		void bind_socket(int new_socket, unsigned short port);
		void listen_socket(int new_socket);
		void set_socket_options(int new_socket);
		struct sockaddr_in init_socaddr_in(unsigned short port);
		int connect_to_server(unsigned short port);
};

#endif