#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;

string sql_processor(string raw_sql)
{
    // 大写转小写
    for(auto &i : raw_sql){
        i = tolower(i);
    }
    raw_sql.erase(remove(raw_sql.begin(), raw_sql.end(), '\n'),raw_sql.end());  // 删除换行符
    raw_sql.erase(remove(raw_sql.begin(), raw_sql.end(), '\t'),raw_sql.end());  // 删除制表符
    return raw_sql;
}

string parser(string sql)
{
    if(sql.substr(0, 2) == "use") {
        if(sql.substr(4, 11) == "database") {
            return sql.substr(12, sql.size());  // 返回数据库名称
        }
    }
    if(sql.substr(0, 5) == "create") {
        if (sql.substr(6, 13) == "database")
        {
            // 扫描空格
            int pos[20];
            int count = 0;
            for (int i = 12; i < sql.size(); i++)
            {
                if (sql[i] == ' ')
                {
                pos[count] = i; // 记录空格的位置
                count++;
                }
            }
            // 扫描空格之间的字符
            for (int i = 0; i < count; i++)
            {
                for (int j = pos[i]; j < pos[i+1]; j++)
                {
                    /* code */
                }
                
            }
            
        }
        if (sql.substr(6, 13) == "table")
        {
            /* code */
        }
    }
    if(sql.substr(0, 10) == "insert into") {
        int pos[20];
        int count = 0;
        for (int i = 12; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
            }
        }
    }
    if(sql.substr(0, 10) == "delete from") {
        int pos[20];
        int count = 0;
        for (int i = 12; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
            }
            
        }
    }
    if(sql.substr(0, 5) == "select") {

    }
    if(sql.substr(0, 5) == "update") {

    }
    if(sql.substr(0, 4) == "alter") {

    }
}




int main()
{
    string sql_input;
    cin >> sql_input;
    sql_input = sql_processor(sql_input);
    parser(sql_input);
    system("pause");
}