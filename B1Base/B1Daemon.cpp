//
// B1Daemon.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Daemon.h"
#include "B1DaemonDelegate.h"
#include "B1String.h"
#include "B1Thread.h"

#if !defined(_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#endif

#include <stdio.h>

using namespace BnD;

void B1Daemon::writeError(const char* file, const char* msg, int code)
{
    if (auto fp = fopen(file, "w")) {
        fprintf(fp, "[%d]%s\n", code, msg);
        fclose(fp);
    }
}

bool B1Daemon::startDaemon(B1DaemonDelegate* delegate)
{
#if defined(_WIN32)
    int pid = 0;
    while (_stopDaemon != true) {
        if ((pid = delegate->startWorkingProcess()) < 0) {
            B1Thread::sleep(1000);
        }
        else if (pid == 0) {
            return true;
        }
    }
    return true;
#else
    int pid = fork();
    if (pid < 0)
        return false;

    if (pid > 0) {
        printf("child process: [%d] - parent process: [%d]\n", pid, getpid());
        exit(0);
    }
    else if (pid == 0) {
        printf("startDaemon: pid[%d]\n", getpid());
    }

    //  To prevent [signal]s from being affected when the terminal is terminated.
    signal(SIGHUP, SIG_IGN);
#if defined(__linux__)
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    stdin = freopen("/dev/null", "r", stdin);
    stdout = freopen("/dev/null", "w", stdout);
    stderr = freopen("/dev/null", "w", stderr);
#endif
    if (chdir("/") != 0) {  //  Change the execution location to Root.
        printf("chdir failed\n");
    }
    setsid();   //  Grant a new session.

    while (_stopDaemon != true) {
        if ((pid = delegate->startWorkingProcess()) < 0) {
            B1Thread::sleep(1000);
        }
        else if (pid == 0) {
            return true;
        }
        else if (pid > 0) {
            int ret = 0;
            ::wait(&ret);
            if (_restartInterval > 0)
                B1Thread::sleep(_restartInterval);
        }
    }
    exit(0);
    return false;
#endif
}
