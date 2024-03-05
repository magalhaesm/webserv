#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include "HTTPConstants.hpp"
#include "DynamicContentHandler.hpp"
#include "InternalErrorException.hpp"

const int READ_END = 0;
const int WRITE_END = 1;

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

    switch (req.method())
    {
    case GET:
        handleGet(req, res, cfg);
        break;
    case POST:
        handlePost(req, res, cfg);
        break;
    default:
        sendStatusPage(METHOD_NOT_ALLOWED, res, cfg);
    }
}

void DynamicContentHandler::handleGet(Request& req, Response& res, const ConfigSpec& cfg)
{
    int fd[2];
    int status;

    if (pipe(fd) == -1)
    {
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
        return;
    }

    switch (fork())
    {
    case -1:
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
        return;
    case 0:
    {
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);

        execl(req.realPath().c_str(), "", NULL);
        close(fd[WRITE_END]);
        throw InternalErrorException(req.realPath());
    }
    default:
        close(fd[WRITE_END]);

        char buffer[PIPE_BUF];
        std::string cgiContent;

        int bytesRead;
        while ((bytesRead = read(fd[READ_END], buffer, PIPE_BUF)))
        {
            cgiContent.append(buffer, bytesRead);
        }
        close(fd[READ_END]);

        waitpid(0, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            res.setStatus(OK);
            res.setBody(cgiContent);
            return;
        }
        sendStatusPage(INTERNAL_SERVER_ERROR, res, cfg);
    }
}

void DynamicContentHandler::handlePost(Request&, Response& res, const ConfigSpec& cfg)
{
    sendStatusPage(NOT_IMPLEMENTED, res, cfg);
}
