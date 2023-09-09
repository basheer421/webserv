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
    return (charenv);
}
// add all the headers to the response

void Cgi::HandleCgi(Response &res, Request &req)
{
    this->scriptpath = "." + req.getCgiUrl();

    this->RunCgi(res, req);
}

void Cgi::RunCgi(Response &res, Request &req){
    std::cout << "---------CGI--------" << std::endl;

    FILE *parent_input = tmpfile();
    FILE *child_output = tmpfile();

    if (parent_input == NULL || child_output == NULL)
        throw std::runtime_error("500");

    int parent_in_fd = fileno(parent_input);
    int child_out_fd = fileno(child_output);

    if (req.getReqType() == POST)
    {
       if (fputs(req.getPostBody().c_str(), parent_input) == EOF)
            throw std::runtime_error("500");
       if  (fseek(parent_input, 0, SEEK_SET) == -1)
            throw std::runtime_error("500");
    }

    pid_t pid;

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
	
    	if (dup2(parent_in_fd, STDIN_FILENO) == -1)
            throw std::runtime_error("500");
        if (dup2(child_out_fd, STDOUT_FILENO) == -1)
            throw std::runtime_error("500");

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
	// check for error
	// wait for child to end and then read from std output of the child process via pipe.

	if (pid != 0)
	{
		int     outchild = 0;
        double  time = 0;
        int status = 0;

        clock_t start = clock();
        while (outchild == 0)
        {
            // check for if kill fails
		    outchild = waitpid(-1, &status, WNOHANG);
            clock_t end = clock();
            time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            // std::cerr << "outchild: " << outchild << "time :" << time << std::endl;
            
            if (time > 6)
            {
                kill(pid, SIGTERM);
                throw std::runtime_error("500");
            }
        }

        if (WIFEXITED(status) != true)
        {
            throw std::runtime_error("500");
        }

		std::cout << outchild << std::endl;
		char read_buffer[CGI_BUFF];
        std::string head = "";
		std::string body = "";

		memset(read_buffer, 0, CGI_BUFF);
		if (fseek(child_output, 0, SEEK_SET) != 0)
            throw std::runtime_error("500");
		while (feof(child_output) == false)
		{
			fread(read_buffer, (CGI_BUFF - 1), 1, child_output);
			body += read_buffer;
			memset(read_buffer, 0, CGI_BUFF);
		}
		// std::cerr << body << std::endl;
        // parse response.
    
        if (body.find("\r\n\r\n") != std::string::npos)
        {
            head = body.substr(0, body.find("\r\n\r\n"));
            body.erase(0, (body.find("\r\n\r\n") + 4));

            res.setResponseHeader("200", "OK");
            res.appendHeader(head);
            res.setCgiBody(body);

            // std::cout << "-------S------" << std::endl;

            // std::cout << "HEAD: " << res.getHeader() << std::endl;
            // std::cout << "BODY: " << res.getbody() << std::endl;
        }
        else
        {
            res.setResponseHeader("200", "OK");
            res.setCgiBody(body);
        }
	}

	fclose(parent_input);
	fclose(child_output);
	close(child_out_fd);
	close(parent_in_fd);

    // set response object.
}
