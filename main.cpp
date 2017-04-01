#include "Connection.h"
#include "Log.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <mutex>
#include <future>
using namespace std;

/// JudgeClient(Yui-Friend) --- JudgeServer(Yui-Home)
#define JUDGE_SERVER_PORT 60001

/// UserClient(Yui) --- JudgeServer(Yui-Home)
#define USER_SERVER_PORT 60002

void ShowWelcomeMessage()
{
    printf(
    "Welcome to use Programing contest controller system! I'm Yui!\n"
    "Now I'll serve the contest as soon as possible!\n"
    "Yui-Home : Yui-Project Judge Network Server.\n"
    "Yui-Project 2017 / HC TECH 2017\n"
    );
}


bool stopJudgeClientLabel;

void procJudgeClient(ServerListener& nsl)
{
    while(1)
    {
        if(stopJudgeClientLabel) break;
        Connection c=nsl.accept();
        if(c.ready())
        {

        }
    }
    return ;
}

bool stopUserClientLabel;

void procUserClient(ServerListener& usl)
{
    return ;
}

char linecommand[1024];
void ControlTerminal()
{
    while(1)
    {
        memset(linecommand,0,1024);
        gets(linecommand);
    }
}

int main()
{
    if(!log_init()) return -1;
    log("Log System Started Successfully.");

    ShowWelcomeMessage();

    log("Starting ServerListener For Yui-Friend...");
    ServerListener nsl(JUDGE_SERVER_PORT,100);
    if(!nsl.ready())
    {
        log("Failed to start ServerSocket.");
        return 0;
    }
    log("ServerListener Started.");
    log("Starting Service For Yui-Friend...");

    stopJudgeClientLabel=false;
    thread td(procJudgeClient,ref(nsl));
    td.detach();

    log("Service Started.");
    log("Starting ServerListener For Yui...");
    ServerListener usl(USER_SERVER_PORT,100);
    if(!usl.ready())
    {
        log("Failed to start ServerSocket.");
        return 0;
    }
    log("ServerListener Started.");
    log("Starting Service For Yui...");

    stopUserClientLabel=false;
    thread tdx(procUserClient,ref(usl));
    tdx.detach();

    log("Service Started.");
    log("Entering Control Terminal...");

    ControlTerminal();
    return 0;
}
