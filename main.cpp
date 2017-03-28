#include "Connection.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

#define JUDGE_PORT 60001


bool TryConnectVec(Connection Conn,vector<string> vec)
{
    for(auto& str:vec)
    {
        cout<<"About to connect to "<<str<<endl;
        Connection c;
        if(c.connect(str,JUDGE_PORT)==0)
        {
            cout<<"Successfully connected to "<<str<<endl;
            Conn=c;
            return true;
        }
    }
    return false;
}

vector<string> GetDefaultServerList()
{
    vector<string> vec;
    vec.push_back("127.0.0.1");
    return vec;
}

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
    ShowWelcomeMessage();
    vector<string> vec=GetDefaultServerList();
    Connection conn;
    if(!TryConnectVec(conn,vec))
    {
        cout<<"Failed to connect to any server."<<endl;
        cout<<"Abort."<<endl;
        return 0;
    }
    return 0;
}
