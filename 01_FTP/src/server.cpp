#include "server.hpp"

Server::Server(char* argv[])
{
	std::string json_file_name(argv[1]);
	nlohmann::json config;
	std::ifstream json_file(json_file_name, std::ifstream::binary);
	json_file >> config;

	command_port = config["commandChannelPort"];
	data_port = config["dataChannelPort"];

	for (auto user : config["users"])
		users.push_back(new User(user["user"], user["password"], user["admin"] == "true",
				std::stoi(std::string(user["size"]))));
	for (auto file : config["files"])
		private_files.push_back(file);
}

void Server::setup()
{
	command_fd = initial_socket(command_port);
	data_fd = initial_socket(data_port);
}

int Server::initial_socket(unsigned short port)
{
	int new_socket = socket_tools.creat_socket();
	socket_tools.bind_socket(new_socket, port);
	socket_tools.listen_socket(new_socket);
	socket_tools.set_socket_options(new_socket);

	return new_socket;
}

int Server::accept_new_request(int master_socket)
{
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	int* caddrlen = &len;

	int new_socket;
	if((new_socket = accept(master_socket, (struct sockaddr *) &caddr, (unsigned int *) caddrlen)) < 0){
		std::cerr << "New client connection failed." << std::endl;
		return 1;
	}
	else
		std::cout << "Client accepted" << std::endl;

	return new_socket;
}



void Server::run()
{
	fd_set master_set, working_set;
	char buf[COMMAND_BUF_SIZE];
	int max_fd = data_fd;

	FD_ZERO(&master_set);
	FD_SET(STDIN_FILENO, &master_set);
	FD_SET(command_fd, &master_set);
	FD_SET(data_fd, &master_set);

	while(true)
	{
		working_set = master_set;
		if (select(max_fd + 1, &working_set, NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select failed" << std::endl;
			exit(-1);
		}

		if (FD_ISSET(command_fd, &working_set))// New client command channel
		{
			Client* new_client = new Client();
			clients.push_back(new_client);
			int new_fd = accept_new_request(command_fd);
			clients.back()->set_command_fd(new_fd);
			max_fd = std::max(max_fd, new_fd);
			FD_SET(new_fd, &master_set);
		}
		else if (FD_ISSET(data_fd, &working_set))// New client data channel
			clients.back()->set_data_fd(accept_new_request(data_fd));
		else if(FD_ISSET(STDIN_FILENO, &working_set))// Quit
		{
			read(STDIN_FILENO, buf, sizeof(buf));
			if(strcmp(buf, "quit\n") == 0)
				break;
		}
		else// Client Request
		{
			int len = clients.size();
			for (int i = 0; i < len; i++)
			{
				if (!FD_ISSET(clients[i]->get_command_fd(), &working_set))
					continue;
				int msg_size = recv(clients[i]->get_command_fd(), &buf, sizeof(buf), 0);
				if(msg_size <= 0)
				{
					if(msg_size < 0)
						std::cerr << "message is broken" << std::endl;
					else if(msg_size == 0)
					{
						std::cout << "A client quits" << std::endl;
						FD_CLR(clients[i]->get_command_fd(), &master_set);
						close(clients[i]->get_command_fd());
						close(clients[i]->get_data_fd());
						delete(clients[i]);
						clients.erase(clients.begin() + i);
					}
				}
				else{
					buf[msg_size] = '\0';
					std::string command(buf);
					command_handler.handle(clients[i], users, command, private_files);
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	Server server(argv);
	server.setup();
	server.run();
}