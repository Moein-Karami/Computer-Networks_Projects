#ifndef _ServerClient_hpp_
#define _ServerClient_hpp_

#include "defines_and_libs.hpp"
#include "User.hpp"

class Client
{
	public:
		Client(User* user = NULL, int command_fd = 0, int data_fd = 0, std::string dir = ".");
		int get_command_fd();
		int get_data_fd();
		bool check_space(int need_space);
		std::string get_dir();
		void set_command_fd(int fd);
		void set_data_fd(int fd);
		bool check_login_state();
		void set_user(User* user_input);
		bool check_wait_for_pass();
		std::string get_username();
		bool is_connected();
		bool is_logged();
		bool correct_pass(std::string pass_input);
		void login();
		bool is_admin();
		void set_dir(std::string new_dir);
	private:  
		User* user;
		int command_fd;
		int data_fd;
		std::string dir;
		ClientState log_state;
};


#endif