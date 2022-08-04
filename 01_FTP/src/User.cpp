#include "User.hpp"

User::User(std::string username, std::string password, bool is_admin, int space)
: username(username),
password(password),
is_admin(is_admin),
space(space)
{
}

bool User::check_username(std::string name)
{
	return username == name;
}

bool User::check_password(std::string pass)
{
	return pass == password;
}

bool User::check_space(int need_space)
{
	if (need_space <= space)
	{
		space -= need_space;
		return true;
	}
	return false;
}

bool User::has_access()
{
	return is_admin;
}

std::string User::get_user_name()
{
	return username;
}