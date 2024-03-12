#include <fcntl.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <unistd.h>

#include "Logger.hpp"
#include "HTTPConstants.hpp"
#include "DynamicContentHandler.hpp"
#include "InternalErrorException.hpp"

const int READ_END = 0;
const int WRITE_END = 1;

void setEnvironment(Request& req, const ConfigSpec& cfg);
std::string runCGI(Request& req, const ConfigSpec& cfg);

DynamicContentHandler::DynamicContentHandler()
{
}

void DynamicContentHandler::handle(Request& req, Response& res, const ConfigSpec& cfg)
{
    if (!cfg.hasCGI())
    {
        _next->handle(req, res, cfg);
        return;
    }
    try
    {
        std::string response = runCGI(req, cfg);
        res.setStatus(OK);
        res.setBody(response);
    }
    catch (const std::runtime_error& e)
    {
        Logger::log(e.what());
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
    }
}

std::string runCGI(Request& req, const ConfigSpec& cfg)
{
    int fd[2];
    int status;

    if (pipe(fd) == -1)
    {
        throw std::runtime_error("pipe");
    }

    switch (fork())
    {
    case -1:
        throw std::runtime_error("fork");
    case 0:
    {
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        dup2(req.body(), STDIN_FILENO);

        setEnvironment(req, cfg);
        execl(req.realPath().c_str(), "", NULL);

        close(fd[WRITE_END]);
        throw InternalErrorException(req.realPath());
    }
    default:
        close(fd[WRITE_END]);

        int numRead;
        char buffer[PIPE_BUF];
        std::string response;
        while ((numRead = read(fd[READ_END], buffer, PIPE_BUF)))
        {
            response.append(buffer, numRead);
        }

        close(fd[READ_END]);

        waitpid(0, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            return response;
        }
        throw std::runtime_error("CGI process exited with non-zero status");
    }
}

void setEnvironment(Request& req, const ConfigSpec& cfg)
{
    setenv("CONTENT_TYPE", req.getHeader("Content-Type").c_str(), 1);
    setenv("CONTENT_LENGTH", req.getHeader("Content-Length").c_str(), 1);
    setenv("HTTP_USER_AGENT", req.getHeader("User-Agent").c_str(), 1);
    setenv("PATH_INFO", "", 1);
    setenv("QUERY_STRING", req.query().c_str(), 1);
    setenv("REMOTE_HOST", req.getHeader("Host").c_str(), 1);
    setenv("REQUEST_METHOD", req.methodText().c_str(), 1);
    setenv("SCRIPT_FILENAME", req.realPath().c_str(), 1);
    setenv("SCRIPT_NAME", req.path().c_str(), 1);
    setenv("SERVER_NAME", cfg.getServerName().c_str(), 1);
}
