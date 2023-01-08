#include <iostream>
#include <string>
#include <regex>
using namespace std;

// 本文件将使用正则表达式的方法进行SQL语句解析

const int inf = 0x3f3f3f3f;

// 表
struct Tables {
	string name;
	string pathName;
	vector<string>colName;
	vector<string>type;
	vector<int>size;
	FILE* fp;
};

string defaultPath;  // 默认保存路径

// 验证sql语句是否正确
bool validate(string sql){
    // 定义标准sql语句
    string sqlupdate = "update\.+set\.+(where\.+)?END";
    string sqldelete = "delete\.+from\.+where\.+END";
    string sqlinsert="insert\.+into\.+(\(\.+/)\.*)?values\(.+\)END";
    string sqlcreate="create table\.+\\(\.+\\)END";
    string sqlselect="select\.+from\.+(where\.+)?((group by)?|(order by)?|(having)?)END";
    string sqldrop  ="drop table\.+END";
    if(sql.substr(0, 5) == "create") {

    }
    if(sql.substr(0, 5) == "insert") {

    }
    if(sql.substr(0, 5) == "delete") {

    }
    if(sql.substr(0, 5) == "select") {

    }
    if(sql.substr(0, 5) == "update") {

    }
    if(sql.substr(0, 4) == "alter") {

    }
}

// 创建
void create(string sql){

}

// 插入
void insert(string sql){

}

// 删除
void sql_delete(string sql){

}

// 查询
void select(string sql){

}

// 修改数据
void update(string sql){

}

// 修改表
void alter(string sql){

}

int sql_processer(){
    string sql_command;
    while (cin >> sql_command){
        // 转为小写
        for (int i = 0; i < sql_command.size(); i++)
        {
            sql_command[i] = tolower(sql_command[i]);
        }
        
        if(sql_command == "exit") break;
        if(!validate(sql_command)) {
            cout << "SQL语法错误, 请重新输入! ";
            continue;
        }
        if(sql_command.substr(0, 5) == "create") create(sql_command);
        if(sql_command.substr(0, 5) == "insert") insert(sql_command);
        if(sql_command.substr(0, 5) == "delete") sql_delete(sql_command);
        if(sql_command.substr(0, 5) == "select") select(sql_command);
        if(sql_command.substr(0, 5) == "update") update(sql_command);
        if(sql_command.substr(0, 4) == "alter") alter(sql_command);
    }
}

void info()
{
    cout << "input what you want to do:" << endl;
}

void help()
{
    cout << "list [database]/[table]" << endl;
    cout << "use [database]" << endl;
    cout << "delete [database]" << endl;
    cout << "" << endl;
    cout << "input what you want to do:" << endl;
    cout << "input what you want to do:" << endl;
    cout << "input what you want to do:" << endl;
}

int main(){
    string command;
    info();
    while (cin >> command)
    {
        /* code */
    }
    return 0;
}