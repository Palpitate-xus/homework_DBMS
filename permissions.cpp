#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

struct user{
    string username;
    string password;
    string permission;
};

user users[99999];
int user_count = 0;

// 登录函数，返回值1表示登陆成功，0表示不存在该用户，-1表示密码错误。
int login(string username, string password)
{
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

// 用户权限查询函数
int permissionQuery(string username)
{
    for (int i = 0; i < user_count; i++)
    {
        if (username == users[i].username)
        {
            if (users[i].permission == "admin")
            {
                cout << "permission: admin" << endl;
                return 1;
            }
            else if (users[i].permission == "user")
            {
                cout << "permission: user" << endl;
                return 0;
            }
        }
    }
}

// 创建用户
int createUser(user new_user)
{
    fstream fs("user.dat", ios::binary | ios::out | ios::app);
    fs << new_user.username << " " << new_user.password << " " << new_user.permission << endl;
    fs.close();
    return 0;
}

int main ()
{
//    ofstream outfile;
//    outfile.open("user.dat");
//    cin.getline(data, 100);

//    outfile << data << endl;
 
//    cout << "Enter your age: "; 
//    cin >> data;
//    cin.ignore();
   
//    outfile << data << endl;
 
//    outfile.close();
 
   // 以读模式打开文件
//    ifstream infile; 
//    infile.open("user.dat"); 
 
//    cout << "Reading from the file" << endl; 
//    infile >> data; 
 
   // 在屏幕上写入数据
//    cout << data << endl;
   
   // 关闭打开的文件
//    infile.close();
    if (login("xus", "xus"))
    {
        permissionQuery("xus");
    }
    system("pause");
   return 0;
}