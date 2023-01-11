#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;


// 本文件用于SQL语句的解析

// sql语句预处理
string sql_processor(string raw_sql)
{
    // 大写转小写
    for(auto &i : raw_sql){
        i = tolower(i);
        if (i > 127) {
            return "error";
        }
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
        // 标准delete语句：delete from [tablename] where [xxx] and ...
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
        string tokens[99999];
        for (int i = 0; i < count; i++)
        {
            tokens[i] = sql.substr(pos[i], pos[i+1]);
        }
        
        string tablename = tokens[0];  // 提取表名
        int andnum = count - 3;  // 条件的个数
        string condition[99999];
        int condition_count = 0;
        for (int i = 2; i < count; i+=2)
        {
            condition[condition_count] = tokens[i];
        }
    }
    if (sql.substr(0, 5) == "delete")
    {
        if (sql.substr(7, 14) == "database")
        {
            return sql.substr(16, sql.size());
        }
        else if (sql.substr(7, 11) == "table")
        {
            return sql.substr(13, sql.size());
        }
        else
        {
            return "invalid command";
        }
    }
    
    if(sql.substr(0, 5) == "select") {
        // 标准的select语句：select [*]/[column] from [tablename] where [conditions]
        int pos[20];
        int count = 0;
        for (int i = 6; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
            }
        }
        string tokens[99999];
        for (int i = 0; i < count; i++)
        {
            tokens[i] = sql.substr(pos[i], pos[i+1]);
        }
        string raw_result = tokens[0];  // select出的列
        string raw_tablename = tokens[2];  // 表名
        string raw_conditions = tokens[4];  // select的条件
        string result[99999];
        // 如果select非全部列
        if (raw_result != "*")
        {
            int col_count = 0;
            int result_pos[99999];
            for (int i = 0; i < raw_result.size(); i++)
            {
                if (raw_result[i] == '*')
                {
                    result_pos[col_count] = i;
                    col_count++;
                }
            }
            for (int i = 0; i < col_count; i++)
            {
                result[i] = raw_result.substr(result_pos[i], result_pos[i+1]);
            }  
        }
        
        string conditions[99999];
        int con_count = 0;
        for (int i = 5; i < count; i++)
        {
            conditions[con_count] = tokens[i];
            con_count++;
        }
    }
    if(sql.substr(0, 5) == "update") {
        // 标准的update语句：update [tablename] set [updates] where [conditions]
        int pos[20];
        int count = 0;
        for (int i = 6; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
            }
        }
        string tokens[99999];
        for (int i = 0; i < count; i++)
        {
            tokens[i] = sql.substr(pos[i], pos[i+1]);
        }

        string tablename = tokens[0];
        string raw_updates = tokens[2];
        string raw_conditions = tokens[4];

        string updates[99999];
        string conditions[99999];
        int updates_pos[99999];  // update中","的位置
        int conditions_pos[99999];  // conditions中","的位置
        int updates_count = 0;
        int conditions_count = 0;
        for (int i = 0; i < raw_updates.size(); i++)
        {
            if (raw_updates[i] == ',')
            {
                updates_pos[updates_count] = i;
                updates_count++;
            }
        }
        for (int i = 0; i < raw_conditions.size(); i++)
        {
            if (raw_conditions[i] == ',')
            {
                conditions_pos[conditions_count] = i;
                conditions_count++;
            }
        }
        updates[0] = raw_updates.substr(0, updates_pos[0]);
        for (int i = 0; i < updates_count; i++)
        {
            updates[i+1] = raw_updates.substr(updates_pos[i], updates_pos[i+1]);
        }
        conditions[0] = raw_conditions.substr(0, conditions_pos[0]);
        for (int i = 0; i < count; i++)
        {
            conditions[i+1] = raw_conditions.substr(conditions_pos[i], conditions_pos[i+1]);
        }
    }
    if(sql.substr(0, 4) == "alter") {
        // 或许会有这个功能
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
