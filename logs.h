#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

// 获取当前时间
string getTime()
{
   time_t now = time(0);
   string now_dt = ctime(&now);
   cout << now_dt << endl;
   return now_dt;
}

// 写入日志文件
int log(string user, string operation, string time)
{
    fstream oFile("dbms.log", ios::binary | ios::out | ios::app);
    if (!oFile)
        cout << "error 1" << endl;
    oFile << time << "    " << user << "    " << operation << endl;
    oFile.close();
    return 0;
}