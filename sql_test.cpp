#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;

// sql语句预处理
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

// sql语句解析
string parser(string sql)
{
    if(sql.substr(0, 2) == "use") {
        if(sql.substr(4, 11) == "database") {
            return sql.substr(12, sql.size());  // 返回数据库名称
        }
    }
    if(sql.substr(0, 5) == "create") {
        // create标准格式: create database [databasename]
        if (sql.substr(6, 13) == "database")
        {
            return sql.substr(14, sql.size());  // 返回数据库名称
            // 扫描空格
            // int pos[20];
            // int count = 0;
            // for (int i = 12; i < sql.size(); i++)
            // {
            //     if (sql[i] == ' ')
            //     {
            //         pos[count] = i; // 记录空格的位置
            //         count++;
            //     }
            // }
            // 扫描空格之间的字符
            // for (int i = 0; i < count; i++)
            // {
            //     for (int j = pos[i]; j < pos[i+1]; j++)
            //     {
            //         /* code */
            //     }
                
            // }
            
        }
        // create标准格式：create table [tablename]
        if (sql.substr(6, 13) == "table")
        {
            return sql.substr(14, sql.size());  // 返回表名称
        }
        // 可能会有索引
        if (sql.substr(6, 13) == "index")
        {
            return sql.substr(14, sql.size());  // 这个待完成
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
            // 扫描空格之间的字符
            // [tablename], [column], values, [values]
            string tokens[5];
            for (int i = 0; i < count; i++)
            {
                tokens[i] = sql.substr(pos[i], pos[i+1]);
            }
            string tablename = tokens[0];
            string raw_column = tokens[1].substr(1, tokens[1].size()-1);  // 去除首尾空格
            string raw_values = tokens[3].substr(1, tokens[3].size()-1);  // 去除首尾空格

            int colcount = 0;  // 标记列数
            int col_pos[99999];  // 记录列中','的位置
            int valuescount = 0;  // 标记给定数值的个数
            int val_pos[99999];  // 记录数值中','的位置
            for (int i = 0; i < raw_column.size(); i++)
            {
                if (raw_column[i] == ',') {
                    col_pos[colcount] = i;
                    colcount++;
                }
            }
            for (int i = 0; i < raw_values.size(); i++)
            {
                if (raw_values[i] == ',')
                {
                    val_pos[valuescount] = i;
                    valuescount++;
                }
            }
            if (colcount != valuescount)
            {
                return "cols is not equal to values";
            }

            string cols[99999];
            string values[99999];
            for (int i = 0; i < colcount; i++)
            {
                cols[i] = raw_column.substr(col_pos[i], col_pos[i+1]);  // 提取列名
                values[i] = raw_values.substr(val_pos[i], val_pos[i+1]);  // 提取数值
            }
        }
    }
    if(sql.substr(0, 10) == "delete from") {
        // 标准delete语句：delete from [tablename] where [xxx]
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