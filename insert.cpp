#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

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
int validate(string sql){

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

// 修改表
void alter(string sql){

}

// 修改数据
void update(string sql){

}

int main(){
    string sql_command;
    while (cin >> sql_command){
        if(sql_command == "exit") break;
        if(sql_command.substr(0, 5) == "create")
        if(sql_command.substr(0, 5) == "insert")
        if(sql_command.substr(0, 5) == "delete")
        if(sql_command.substr(0, 5) == "select")
        if(sql_command.substr(0, 5) == "update")
        if(sql_command.substr(0, 4) == "alter")

    }
}