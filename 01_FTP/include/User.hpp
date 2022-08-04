#ifndef _User_hpp_
#define _User_hpp_

#include "defines_and_libs.hpp"

class User
{
	public:
		User(std::string username, std::string password, bool is_admin, int space);
		bool check_username(std::string name);
		std::string get_user_name();
		bool check_password(std::string pass);
		bool check_space(int need_space);
		bool has_access();

	private:
		std::string username;
		std::string password;
		int space;
		bool is_admin;
};

#endif