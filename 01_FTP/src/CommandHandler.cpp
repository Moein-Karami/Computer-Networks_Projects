#include "CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    root_path = execute("pwd");
}

void CommandHandler::log(std::string username, std::string action)
{
	std::ofstream log_file;
	log_file.open("server.log", std::ios::app);

	time_t date = time(0);
	char* dt = ctime(&date);
	dt[strlen(dt) - 1] = '\0';
	log_file << '[' << dt << " User: " << username << "] " << action << std::endl;

	log_file.close();
}

int CommandHandler::handle(Client* client, std::vector<User*> users, std::string command, std::vector<std::string> forbiden_files)
{
    std::string type_command = get_part(0, command);
    std::cout << "Enter command handler for parsing command" << std::endl;
    if (handle_bad_scenario(client, type_command, command, forbiden_files) == BAD_CASE)
    {
        return BAD_CASE;
    }
    if(type_command == "user")
    {
        handle_user(client, users, command);
    }
    else if(type_command == "pass")
    {
        handle_pass(client, command);
    }
    else if(type_command == "pwd")
    {
        handle_pwd(client);
    }
    else if(type_command == "mkd")
    {
        handle_mkd(client, command);
    }
    else if(type_command == "dele")
    {
        handle_dele(client, command);
    }
    else if(type_command == "cwd")
    {
        handle_cwd(client, command);
    }
    else if(type_command == "ls")
    {
        handle_ls(client);
    }
    else if(type_command == "rename")
    {
        handle_rename(client, command);
    }
    else if(type_command == "retr")
    {
        handle_retr(client, command);
    }
    else if(type_command == "help")
    {
        handle_help(client);
    }
    else if(type_command == "quit")
    {
        handle_quit(client);
        return QUITE_TYPE;
    }
    return OK_TYPE;
}

std::string CommandHandler::get_part(int index_part, std::string command)
{
    std::stringstream stream_command(command);
    int cnt = -1;
    std::string ans;
    while(cnt < index_part)
    {
        stream_command >> ans;
        cnt++;
    }   
    return ans;
}

void CommandHandler::handle_user(Client* client, std::vector<User*> users, std::string command)
{
    char command_buf[DATA_BUF_SIZE] = MSG_430;
    int command_fd = client->get_command_fd();
    if(client->check_login_state())
        strcpy(command_buf, MSG_500);
    else
    {
        for(int i = 0; i < users.size(); i++)
        {   
            if(users[i]->check_username(get_part(1, command)))
            {
                client->set_user(users[i]);
                strcpy(command_buf, MSG_331);
                break;
            }
        }
    }
    if (strcmp(command_buf, MSG_331) == 0)
        log(client->get_username(), "enters username");

    send(command_fd, &command_buf, strlen(command_buf), 0);
}

void CommandHandler::handle_pass(Client* client, std::string command)
{
    if(!client->check_wait_for_pass())
    {
        send(client->get_command_fd(), MSG_503, strlen(MSG_503), 0);
        log("unknown", "enters password");
        return;
    }
    if(client->correct_pass(get_part(1, command)))
    {
        client->login();
        log(client->get_username(), "enters correct password");
        send(client->get_command_fd(), MSG_230, strlen(MSG_230), 0);
    }
    else 
    {
        log(client->get_username(), "enters wrong password");
        send(client->get_command_fd(), MSG_430, strlen(MSG_430), 0);
    }
}

void CommandHandler::handle_pwd(Client* client)
{
    log(client->get_username(), "asks for pwd");
    std::string response = "257: " + client->get_dir() + '\n';
    send(client->get_command_fd(), response.c_str(), response.size(), 0);
}

void CommandHandler::handle_mkd(Client* client, std::string command)
{
    std::string mkdir_str = "mkdir ";
    std::string path = get_part(1, command);

    mkdir_str += get_dir(client->get_dir(), path);

    std::string response = execute(mkdir_str);
    if (response != "")
    {
        log(client->get_username(), "failes to make directory");
        send(client->get_command_fd(), MSG_501, strlen(MSG_501), 0);
        return;
    }
    else
    {
        log(client->get_username(), "makes directory");
        response = "257: " + command + " created.";
        send(client->get_command_fd(), response.c_str(), response.size(), 0);
        return;
    }
}

void CommandHandler::handle_dele(Client* client, std::string command)
{
    std::string mode = get_part(1, command);
    std::string path_deleted = get_part(2, command);
    std::string exec_str = "rm ";
    if (mode == D_MODE)
        exec_str += "-r ";

    exec_str += get_dir(client->get_dir(), path_deleted);
    
    std::string response;
    response = execute(exec_str);
    
    if (response != "")
    {
        log(client->get_username(), "fails to remove");
        send(client->get_command_fd(), MSG_501, strlen(MSG_501), 0);
    }
    else
    {
        log(client->get_username(), "removes " + path_deleted);
        response = "257: " + path_deleted + " deleted."; 
        send(client->get_command_fd(), response.c_str(), response.size(), 0);
    }
}

void CommandHandler::handle_ls(Client* client)
{
    std::string command = "ls " + client->get_dir();
    std::string response = execute(command);
    send(client->get_command_fd(), MSG_226_2, strlen(MSG_226_2), 0);
    send(client->get_data_fd(), response.c_str(), response.size(), 0);
    log(client->get_username(), "asks for ls");
}

void CommandHandler::handle_cwd(Client* client, std::string command)
{
    // TODO : check if get out of root
    std::string path = get_part(1, command);
    std::string new_dir = get_dir(client->get_dir(), path);

    std::string exec_str = "test -d " + new_dir + " && echo \"Directory Exists\"";
    std::string response = execute(exec_str);
    new_dir = clean_dir(new_dir);

    if (response == "" || new_dir.size() < root_path.size() - 1)
    {
        log(client->get_username(), "fails to cwd");
        send(client->get_command_fd(), MSG_501, strlen(MSG_501), 0);
    }
    else
    {
        log(client->get_username(), "changes dir");
        send(client->get_command_fd(), MSG_250, strlen(MSG_250), 0);
        client->set_dir(new_dir);
    }
}

void CommandHandler::handle_rename(Client* client, std::string command)
{
    std::string from_dir = get_part(1, command);
    std::string to_dir = get_part(2, command);
    std::string exec_str = "mv ";
    
    exec_str += get_dir(client->get_dir(), from_dir);
    exec_str += " ";
    exec_str += get_dir(client->get_dir(), to_dir);
    std::string responce = execute(exec_str);

    if (responce == "")
    {
        log(client->get_username(), "renames " + from_dir + " to " + to_dir);
        send(client->get_command_fd(), MSG_250, strlen(MSG_250), 0);
    }
    else
    {
        log(client->get_username(), "fails to renames " + from_dir + " to " + to_dir);
        send(client->get_command_fd(), MSG_501, strlen(MSG_501), 0);
    }
}

void CommandHandler::handle_retr(Client* client, std::string command)
{
    std::string path = get_dir(client->get_dir(), get_part(1, command));
    std::string response = execute("test -f " + path + " && echo \"File Exists\"");
    if (response == "")
    {
        log(client->get_username(), "fails to download " + path);
        send(client->get_command_fd(), MSG_501, strlen(MSG_501), 0);
    }
    else
    {
        response = execute("ls -l " + path);
        int space = stoi(get_part(4, response));
        if (client->check_space(space))
        {
            log(client->get_username(), "downloads " + path);
            send(client->get_command_fd(), MSG_226, strlen(MSG_226), 0);
            std::ifstream file(path);
            std::string result;
            while (file >> response)
                result += response;
            send(client->get_data_fd(), result.c_str(), result.size(), 0);
        }
        else
        {
            log(client->get_username(), "attempts to download large file: " + path);
            send(client->get_command_fd(), MSG_425, strlen(MSG_425), 0);
        }
    }
}

void CommandHandler::handle_help(Client* client)
{
    std::string msg = "214\nUSER[name], Its argument is used to specify the user string. It is used foruser authentication.\nPASS[pass], Its password of account.\nPWD, Show you your path.\nMKD[path], Make directory with path name.\nDELE[option, file/directory], Delete file/directory with specific options.\nLS, Show you files and directories in your path.\nCWD[path], Change directory to path.\nRENAME[name, new_name], Change name to new_name.\nRETR[name], Download filename.\nHELP, Show help of commands.\nQUIT, Quit from server.\n";
    send(client->get_command_fd(), msg.c_str(), msg.size(), 0);
    log(client->get_username(), "asks for help");
}

void CommandHandler::handle_quit(Client* client)
{
    log(client->get_username(), "quits");
    send(client->get_command_fd(), MSG_221, strlen(MSG_221), 0);
}

std::string CommandHandler::execute(const char* command)
{
    char buffer[DATA_BUF_SIZE];
    std::string result = "";
    FILE* pipe = popen(command, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try 
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }

    pclose(pipe);
    result[result.size() - 1] = '\0';
    return result;
}

std::string CommandHandler::execute (std::string command)
{
    return execute(command.c_str());
}

bool CommandHandler::handle_bad_scenario(Client* client, std::string type_command, std::string command,
std::vector<std::string>files_forbiden)
{
    if(type_command != "user"  && type_command != "pass" && type_command != "dele" && type_command != "rename" &&
    type_command != "cwd" && type_command != "pwd" && type_command != "help" && type_command != "mkd" && type_command != "retr"
    && type_command != "quit" && type_command != "ls")
    {
        std::string response = MSG_500;
        send(client->get_command_fd(), response.c_str(), response.size(), 0);
        return BAD_CASE;
    }
    if(type_command != "user" && type_command != "pass" && !client->is_logged())
    {
        std::string response = MSG_332;
        send(client->get_command_fd(), response.c_str(), response.size(), 0);
        return BAD_CASE;
    }
    if(is_working_with_file(type_command) && !client->is_admin())
    {
        std::string file_name = get_part(1, command);
        for(int i = 0; i < files_forbiden.size(); i++)
        {
            if(files_forbiden[i] == file_name)
            {
                std::string response = MSG_550;
                send(client->get_command_fd(), response.c_str(), response.size(), 0);
                return BAD_CASE;
            }
        }
    }
    return OK_TYPE;
}

bool CommandHandler::is_working_with_file(std::string type_command)
{
    if(type_command == "dele" || type_command == "retr" || type_command == "rename")
    {
        return 1;
    }
    return 0;
}

std::string CommandHandler::get_dir(std::string client_dir, std::string command_dir)
{
    std::string dir;
    if (command_dir[0] != '/')
        dir = client_dir + "/" + command_dir;
    else
        dir = root_path + command_dir;
    return dir;
}

std::string CommandHandler::clean_dir(std::string dir)
{
    std::vector<std::string> vec;
    std::string word;
    std::string tmp = "/";
    for (auto chr : dir)
    {
        if (chr == '/')
        {
            if (word.size())
                vec.push_back(word), word.clear();
            vec.push_back(tmp);
        }
        else
            word += chr;
    }
    if (word.size())
        vec.push_back(word);

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == "..")
        {
            if (i < 3)
            {
                vec.clear();
                break;
            }
            vec.erase(vec.begin() + i - 3, vec.begin() + i + 1), i -= 4;
        }
    }
    dir.clear();
    for (auto i : vec)
        dir += i;
    return dir;
}