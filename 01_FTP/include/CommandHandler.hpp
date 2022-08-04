#ifndef _CommandHandler_hpp_
#define _CommandHandler_hpp_

#include "defines_and_libs.hpp"
#include "User.hpp"
#include "ServerClient.hpp"

class CommandHandler
{
	public:
		CommandHandler();
        int handle(Client* client, std::vector<User*> users, std::string command,
				std::vector<std::string> forbiden_files);
		static void log(std::string username, std::string action);
	private:
		std::string get_part(int index, std::string command);
		void handle_user(Client* client, std::vector<User*> users, std::string command);
		void handle_pass(Client* client, std::string command);
		void handle_pwd(Client* client);
		void handle_mkd(Client* client, std::string command);
		void handle_dele(Client* client, std::string command);
		void handle_ls(Client* client);
		void handle_cwd(Client* client, std::string command);
		void handle_rename(Client* client, std::string command);
		void handle_retr(Client* client, std::string command);
		void handle_quit(Client* client);
		void handle_help(Client* client);
		bool is_working_with_file(std::string type_command);
		bool handle_bad_scenario(Client* client, std::string type_command, std::string command,
std::vector<std::string>files_forbiden);
		std::string execute(const char* command);
		std::string execute(std::string command);
		std::string get_dir(std::string client_dir, std::string command_dir);
		std::string clean_dir(std::string dir);
		std::string root_path;
};


#endif