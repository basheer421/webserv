#include "Cgi.hpp"

Cgi::Cgi(){

}

Cgi::Cgi(const Cgi &obj)
{
    *this = obj;
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}

Cgi::~Cgi(){

}

void Cgi::SetEnv(std::map<std::string, std::string> &envMap){
    this->envMap = envMap;
}

char **Cgi::GetCharEnv(){

    size_t size = this->envMap.size();
    char **charenv = new char*[size + 1];
    charenv[size] = NULL;

    std::map<std::string, std::string>::iterator b = this->envMap.begin();
    std::string temp;

    for (int i = 0; (size_t)i < this->envMap.size(); i++)
    {
        temp = b->first + "=" + b->second;
        charenv[i] = strdup(temp.c_str());
        b++;
    }
    // for (int i = 0; (size_t)i < size; i++)
    // {
    //     std::cout << charenv[i] << std::endl;
    // }
    return (charenv);
}

void Cgi::HandleCgi(Response &res, Request &req)
{
    this->scriptpath = ("." + req.getCgiUrl()).c_str();

    this->RunCgi(res, req);
}

void Cgi::RunCgi(Response &res, Request &req){
    std::cerr << "----ENTERING CGI----" << std::endl;

    // create temp files for the child and parent.

    FILE *parent_input = tmpfile();
    FILE *child_output = tmpfile();

    int parent_in_fd = fileno(parent_input);
    int child_out_fd = fileno(child_output);

    if (parent_input == NULL || child_output == NULL)
    {
        throw std::runtime_error("500");
    }

    // set post request file.

    if (req.getReqType() == POST)
    {
       if (fputs(req.getPostBody().c_str(), parent_input) == EOF)
       {
            throw std::runtime_error("500");
       }
        fseek(parent_input, 0, SEEK_SET);
    }

    pid_t pid;
    (void) res;
    // create child process 

    pid = fork();
    if (pid == -1)
    {
        throw std::runtime_error("500");
    }
    else
    {
        // set file descriptors for child process
        // if execve fails then free and exit child process and return error.

        if (pid == 0) 
        {
            char *const *env = this->GetCharEnv();
        // check for dup success
            dup2(parent_in_fd, STDIN_FILENO);
            dup2(child_out_fd, STDOUT_FILENO);
 
            std::cerr << this->scriptpath <<std::endl;
            execve(this->scriptpath, NULL, env);
            std::cout << "---------FAILED--------" << std::endl;

            for (int i = 0; env[i] != NULL; i++)
                delete[] env[i];
            delete[] env;

            exit(-1);
        }

        // wait for child to end and then read from std output of the child process via pipe.
        if (pid != 0)
        {
            int outchild;
            outchild = waitpid(-1, NULL, 0); 

            std::cout << outchild << std::endl;
            char read_buffer[100];
            std::string body;

            memset(read_buffer, 0, 100);
            fseek(child_output, 0, SEEK_SET);
            while (feof(child_output) == false)
            {
                fread(read_buffer, 100, 1, child_output);
                body += read_buffer;
                memset(read_buffer, 0, 100);
            }
            std::cerr << "---------TEST-------- "<<std::endl; 
            std::cerr << body << std::endl;
        }
        fclose(parent_input); 
        fclose(child_output);
        close(child_out_fd);
        close(parent_in_fd);
    }
    std::cerr << "----LEAVING CGI----" << std::endl;
    // set response object.
}