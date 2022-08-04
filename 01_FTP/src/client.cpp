#include "defines_and_libs.hpp"
#include "socket_tool.hpp"
#include "json.hpp"

void handle_data(std::string response, int data_fd)
{
	char buf[DATA_BUF_SIZE];
	int len;
	if (response == MSG_226 || response == MSG_226_2)
	{
		len = recv(data_fd, buf, DATA_BUF_SIZE, 0);
		buf[len] = '\n';
		write(STDOUT_FILENO, buf, len + 1);
	}
}

int main(int argc, char* argv[])
{
	std::string json_file_name(argv[1]);
	nlohmann::json config;
	std::ifstream json_file(json_file_name, std::ifstream::binary);
	json_file >> config;

	int command_port = config["commandChannelPort"];
	int data_port = config["dataChannelPort"];

	SocketTools socket_tools;
	int command_fd = socket_tools.connect_to_server(command_port);
	std::cout << "Command channel connected" << std::endl;
	
	int data_fd = socket_tools.connect_to_server(data_port);
	std::cout << "Data channel connected" << std::endl;

	char command_buf[COMMAND_BUF_SIZE];
	char command_buf2[COMMAND_BUF_SIZE];

	std::string command, response;
	int msg_size;
	while (true)
	{
		bool flag = false;
		msg_size = read(STDIN_FILENO, command_buf, sizeof(command_buf));
		command_buf[msg_size] = '\0';
		command = std::string(command_buf);
		
		send(command_fd, &command_buf, strlen(command_buf), 0);
		
		msg_size = recv(command_fd, command_buf, COMMAND_BUF_SIZE, 0);
		int msg_size2;
		command_buf[msg_size] = '\0';
		
		response = std::string(command_buf);

		if(command_buf[0] == '2' && command_buf[1] == '1' && command_buf[2] == '4')
		{
					
			msg_size2 = recv(command_fd, command_buf2, COMMAND_BUF_SIZE, 0);
			command_buf2[msg_size2] = '\0';
			command_buf2[msg_size2] = '\n';
			flag = true;
		}
		else 
			command_buf[msg_size] = '\n';

		
		write(STDOUT_FILENO, command_buf, msg_size + 1);
		if(flag)
			write(STDOUT_FILENO, command_buf2, msg_size2 + 1);

		if (command == "quit\n")
			break;
		if (command[0] != 'l' && command[0] != 'r')
			continue;
		handle_data(response, data_fd);
	}
	close(command_fd);
	close(data_fd);
}