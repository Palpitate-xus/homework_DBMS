#ifndef LOGIN_H
#define LOGIN_H
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
int userLogin(string username, string password)
{
    struct user{
        string username;
        string password;
        string permission;
    };
    user users[999];
    int user_count = 0;
    // 读取users数据
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
            if (password == users[i].password)
            {
                infile.close();
                cout << "successfully login" << endl;
                return 1;
            }
            else
            {
                infile.close();
                cout << "wrong password" << endl;
                return -1;
            }
        }
    }
    infile.close();
    cout << "this user is not exist" << endl;
    return 0;
}

#endif // LOGIN_H
