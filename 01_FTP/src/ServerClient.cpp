#include "ServerClient.hpp"

Client::Client(User* user, int command_fd, int data_fd, std::string dir_inp)
: user(user),
command_fd(command_fd),
data_fd(data_fd),
dir(dir_inp),
log_state(ClientState::connected)
{
	char temp[1000];
 	getcwd(temp, 256);
	dir = temp;
}

int Client::get_command_fd()
{
	return command_fd;
}

int Client::get_data_fd()
{
	return data_fd;
}

std::string Client::get_dir()
{
	return dir;
}

bool Client::check_space(int need_space)
{
	return user->check_space(need_space);
}

void Client::set_command_fd(int fd)
{
	command_fd = fd;
}

void Client::set_data_fd(int fd)
{
	data_fd = fd;
}

void Client::set_user(User* user_input)
{
	user = user_input;
	log_state = ClientState::waiting_for_password;
}

bool Client::check_wait_for_pass()
{
	return (log_state == ClientState::waiting_for_password);
}

bool Client::is_logged()
{
	return (log_state == ClientState::logged_in);
}

bool Client::is_connected()
{
	return (log_state);
}

std::string Client::get_username()
{
	return user->get_user_name();
}

bool Client::correct_pass(std::string pass_input)
{
	return user->check_password(pass_input);
}

void Client::login()
{
	log_state = logged_in;
}

bool Client::is_admin()
{
	return user->has_access();
}

void Client::set_dir(std::string new_dir)
{
	dir = new_dir;
}

bool Client::check_login_state()
{
	return log_state == ClientState::logged_in;
}