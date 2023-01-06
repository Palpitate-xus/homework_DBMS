#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <stack>
using namespace std;

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
    // 判断括号是否匹配
    stack<char> st;
    for (auto x : sql)
    {
        if (x == '[' || x == '(' || x == '{')
            st.push(x);
        else if (st.empty())
            return false; 
        else if ( (st.top() == '(' && x == ')') || (st.top () == '[' && x == ']') || (st.top() == '{' && x == '}') )
            st.pop();
        else
            return false;
    }
    string sql_command_list = {
        "create",
        "select",
        "insert",
        "update",
        "alter",
        "grant",
        "delete"
        };  // 合法的命令集合
    return st.empty();
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

void info(){
    cout << "请选择命令：" << endl;
    cout << "1. 新建数据库" << endl;
    cout << "2. 选择数据库" << endl;
    cout << "3. 删除数据库" << endl;
    cout << "exit. 退出" << endl;
}

int main(){
    string command;
    info();
    while (cin >> command)
    {
        /* code */
    }
    
}