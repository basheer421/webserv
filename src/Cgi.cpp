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
// add all the headers to the response

void Cgi::HandleCgi(Response &res, Request &req)
{
    this->scriptpath = "." + req.getCgiUrl();

    this->RunCgi(res, req);
}

void Cgi::RunCgi(Response &res, Request &req){
    std::cerr << "----ENTERING CGI----" << std::endl;

    // create temp files for the child and parent.

    FILE *parent_input = tmpfile();
    FILE *child_output = tmpfile();

    if (parent_input == NULL || child_output == NULL)
        throw std::runtime_error("500");

    int parent_in_fd = fileno(parent_input);
    int child_out_fd = fileno(child_output);


    // set post request file.

    if (req.getReqType() == POST)
    {
       if (fputs(req.getPostBody().c_str(), parent_input) == EOF)
            throw std::runtime_error("500");
        fseek(parent_input, 0, SEEK_SET);
    }

    pid_t pid;
    // create child process

    pid = fork();
    if (pid == -1)
    {
        throw std::runtime_error("500");
    }
	// set file descriptors for child process
	// if execve fails then free and exit child process and return error.

	if (pid == 0)
	{
		char *const *env = this->GetCharEnv();
	// check for dup success
		dup2(parent_in_fd, STDIN_FILENO);
		dup2(child_out_fd, STDOUT_FILENO);

		std::cerr << this->scriptpath <<std::endl;
        char *const *argv = NULL;
		execve(this->scriptpath.c_str(), argv, env);
		std::cout << "---------FAILED--------" << std::endl;

		for (int i = 0; env[i] != NULL; i++)
			free(env[i]);
		delete[] env;

		exit(-1);
	}
	// stop child process
	//check for error
	// wait for child to end and then read from std output of the child process via pipe.
	if (pid != 0)
	{
		int outchild;
		outchild = waitpid(-1, NULL, 0);
        if (WIFEXITED(outchild) == false)
        {
            std::cerr << "TEST" << std::endl;
            throw std::runtime_error("500");
        }

		std::cout << outchild << std::endl;
		char read_buffer[101];
		std::string body;

		memset(read_buffer, 0, 101);
		if (fseek(child_output, 0, SEEK_SET) != 0)
            throw std::runtime_error("500");
		while (feof(child_output) == false)
		{
			fread(read_buffer, 100, 1, child_output);
			body += read_buffer;
			memset(read_buffer, 0, 101);
		}
		std::cerr << "---------CGI RESPONSE BODY-------- "<<std::endl;
		std::cerr << body << std::endl;

        res.setResponseHeader("200", "OK");
        res.setCgiBody(body);
	}
	fclose(parent_input);
	fclose(child_output);
	close(child_out_fd);
	close(parent_in_fd);

    // set response object.
    std::cerr << "----LEAVING CGI----" << std::endl;
}
