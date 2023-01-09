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

int login(string username, string password)
{
    string data;
    ifstream infile; 
    infile.open("user.dat"); 
    infile >> data;
    cout << data;
    infile.close();
    return 0;
}

int permissionQuery(string username)
{
    return 0;
}

int main ()
{
    
   string data;
 
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
   ifstream infile; 
   infile.open("user.dat"); 
 
   cout << "Reading from the file" << endl; 
   infile >> data; 
 
   // 在屏幕上写入数据
   cout << data << endl;
   
   // 关闭打开的文件
   infile.close();
    system("pause");
   return 0;
}