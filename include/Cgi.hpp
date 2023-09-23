#ifndef CGI_HPP
#define CGI_HPP
#include "webserv.hpp"

#define CGI_BUFF 10001
class Cgi{

    public:
        Cgi();
        Cgi(const Cgi &obj);
        Cgi &operator=(const Cgi &rhs);
        ~Cgi();

        void SetEnv(std::map<std::string, std::string> &envMap, Response &res, Request &req);
        char **GetCharEnv();

        void HandleCgi(Response &res, Request &req);

    private:

        std::map<std::string, std::string> envMap;
        std::string scriptpath;

        void RunCgi(Response &res, Request &req);

};

#endif
