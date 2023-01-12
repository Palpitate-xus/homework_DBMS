#pragma once
#ifndef PERMISSION_H
#define PERMISSION_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
// 用户权限查询函数
int permissionQuery(string username)
{
    struct user{
        string username;
        string password;
        string permission;
    };
    user users[999];
    int user_count = 0;
    string data;
    ifstream infile;
    infile.open("user.dat");
    while (!infile.eof())
    {
        infile >> data;
        users[user_count].username = data;
        infile >> data;
        users[user_count].password = data;
        infile >> data;
        users[user_count].permission = data;
        user_count++;
    }
    for (int i = 0; i < user_count; i++)
    {
        if (username == users[i].username)
        {
            if (users[i].permission == "admin")
            {
                return 1;
            }
            else if (users[i].permission == "user")
            {
                return 0;
            }
        }
    }
    return -1;
}

// 创建用户
int createUser(string username, string password, string permission)
{
    fstream fs("user.dat", ios::binary | ios::out | ios::app);
    fs << '\n' << username << " " << password << " " << permission << endl;
    fs.close();
    return 0;
}
#endif // PERMISSION_H
