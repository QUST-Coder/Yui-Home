#include "Connection.h"
#include "Log.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

#define JUDGE_SERVER_PORT 60001

void ShowWelcomeMessage()
{
    cout<<"Welcome to use Programing contest controller system! I'm Yui!"<<endl;
    cout<<"Now I'll serve the contest as soon as possible!"<<endl;
    cout<<endl;
    cout<<"Yui-Home : Yui-Project Judge Network Server."<<endl;
    cout<<"Yui-Project 2017 / HC TECH 2017"<<endl;
    cout<<endl;
}

int main()
{
    if(!log_init()) return -1;
    log("Log System Started Successfully.");

    ShowWelcomeMessage();

    log("Starting Server Socket...");
    ServerListener nsl(JUDGE_SERVER_PORT,100);
    if(!nsl.ready())
    {
        log("Failed to start ServerSocket.");
    }

    return 0;
}
