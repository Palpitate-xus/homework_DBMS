#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include "set.h"
using namespace std;

struct strlist{
    string str[30];
    int len = 0;
    void print()
    {
        for(int i = 0; i < len; i++)
        cout << str[i] << endl;
    }
    void append(string nstr)
    {
        str[len++] = nstr;
    }
};

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
    cout << "processed" << endl;
    return raw_sql;
}

// sql语句解析
string parser(string sql)
{
    cout << sql << endl;
    if(sql.substr(0, 3) == "use") {
        cout << "use" << endl;
        if(sql.substr(4, 8) == "database") {
            cout << "database" << endl;
            cout << sql.substr(13, sql.size() - 13) << endl;
            return sql.substr(13, sql.size() - 13);  // 返回数据库名称
        }
    }
    else if(sql.substr(0, 4) == "view") {
        cout << "view" << endl;
        cout << sql.substr(5, sql.size() - 5) << endl;
        return sql.substr(5, sql.size() - 5);  // 返回数据表名称
    }
    else if(sql.substr(0, 6) == "create") {
        // create标准格式: create database [databasename]
        cout << "create" << endl;
        if (sql.substr(7, 8) == "database")
        {
            cout << "database" << endl;
            cout << sql.substr(16, sql.size() - 16) << endl;
            return sql.substr(16, sql.size() - 16);  // 返回数据库名称
        }
        // create标准格式：create table [tablename] {col: type, col: type……}
        if (sql.substr(7, 5) == "table")
        {
            cout << "table" << endl;
            string left_sql = sql.substr(13, sql.size()-13);
            int space_pos = 0;
            for (int i = 0; i < left_sql.size(); i++)
            {
                if (left_sql[i] == ' ')
                {
                    space_pos = i;
                    break;
                }
            }
            cout << left_sql.substr(0, space_pos) << endl;
            int comma_pos[1000];
            int comma_count = 0;
            for (int i = space_pos+1; i < left_sql.size()-1; i++)
            {
                if (left_sql[i] == ',')
                {
                    comma_pos[comma_count] = i;
                    comma_count++;
                }
            }
            string cols[999];
            cols[0] = left_sql.substr(space_pos + 2, comma_pos[0] - space_pos - 2);
            for (int i = 0; i < comma_count && comma_count > 1; i++)
            {
                cols[i+1] = left_sql.substr(comma_pos[i] + 1, comma_pos[i+1] - comma_pos[i] - 1);
            }
            cols[comma_count] = left_sql.substr(comma_pos[comma_count-1] + 1, left_sql.size() - comma_pos[comma_count-1] - 2);
            cout << cols[0] << endl;
        }
        // 可能会有索引 create index on [tablename]([cols])
        if (sql.substr(6, 13) == "index")
        {
            return sql.substr(14, sql.size());  // 这个待完成
        }
    }
    else if(sql.substr(0, 11) == "insert into") {
        cout << "insert into" << endl;
        int pos[20];
        int count = 0;
        for (int i = 11; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
                cout << pos[count-1] << endl;
            }
            // 扫描空格之间的字符
            // [tablename] [column] values [values]
        }
        string tokens[5];
        for (int i = 0; i < count; i++)
        {
            tokens[i] = sql.substr(pos[i] + 1, pos[i+1] - pos[i] - 1);
        }
        tokens[count-1] = sql.substr(pos[count-1] + 1, sql.size() - pos[count-1] - 1);
        string tablename = tokens[0];
        string raw_column = tokens[1].substr(1, tokens[1].size() - 2);  // 去除首尾空格
        string raw_values = tokens[3].substr(1, tokens[3].size() - 2);  // 去除首尾空格
        cout << raw_column << endl << raw_values << endl;
        int colcount = 0;  // 标记列数
        int col_pos[999];  // 记录列中','的位置
        int valuescount = 0;  // 标记给定数值的个数
        int val_pos[999];  // 记录数值中','的位置
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
        cout << colcount << endl << valuescount << endl;
        if (colcount != valuescount)
        {
            return "cols is not equal to values";
        }

        string cols[999];
        string values[999];
        cols[0] = raw_column.substr(0, col_pos[0] - 1);
        values[0] = raw_values.substr(0, val_pos[0] - 1);
        for (int i = 0; i < colcount; i++)
        {
            cols[i+1] = raw_column.substr(col_pos[i] + 1, col_pos[i+1] - col_pos[i] - 1);  // 提取列名
            values[i+1] = raw_values.substr(val_pos[i] + 1, val_pos[i+1] - val_pos[i] - 1);  // 提取数值
        }
    }
    else if(sql.substr(0, 11) == "delete from") {
        // 标准delete语句：delete from [tablename] where [xxx] and ...
        cout << "delete from" << endl;
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
        string tokens[999];
        tokens[0] = sql.substr(12, pos[0] - 12);
        int temp = 1;
        for (int i = 0; i < count; i++)
        {
            tokens[i+1] = sql.substr(pos[i] + 1, pos[i+1] - pos[i] - 1);
        }
        string tablename = tokens[0];  // 提取表名
        cout << tablename << endl;
        int andnum = count - 2;  // 条件的个数
        cout << andnum << endl;
        string condition[999];
        int condition_count = 0;
        for (int i = 2; i < count + 1; i+=2)
        {
            condition[condition_count] = tokens[i];
            condition_count++;
        }
        cout << condition[0] << endl << condition[1] << endl;
    }
    else if (sql.substr(0, 6) == "delete")
    {
        cout << "delete" << endl;
        if (sql.substr(7, 8) == "database")
        {
            cout << "database" << endl;
            cout << sql.substr(16, sql.size() - 16) << endl;
            return sql.substr(16, sql.size());
        }
        else if (sql.substr(7, 5) == "table")
        {
            cout << "table" << endl;
            cout << sql.substr(13, sql.size()) << endl;
            return sql.substr(13, sql.size());
        }
        else
        {
            return "invalid command";
        }
    }
    
    else if(sql.substr(0, 6) == "select") {
        // 标准的select语句：select [*]/[column] from [tablename] where [conditions]
        cout << "select" << endl;
        int pos[20];
        int count = 0;
        for (int i = 7; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
            }
        }
        string tokens[999];
        tokens[0] = sql.substr(7, pos[0] - 7);
        for (int i = 0; i < count; i++)
        {
            tokens[i+1] = sql.substr(pos[i] + 1, pos[i+1] - pos[i]);
        }
        strlist conditions[999];
        int conditions_pos = 0;
        cout << "count:  " << count << endl;
        for (int i = 4; i < count + 1; i++)
        {
            if (tokens[i] == "or ")
            {
                conditions_pos++;
            }
            else if (tokens[i] != "and ")
            {
                conditions[conditions_pos].append(tokens[i]);
            }
        }
        string raw_result = tokens[0];  // select出的列
        cout << raw_result << endl;
        string raw_tablename = tokens[2];  // 表名
        cout << raw_tablename << endl;
        string result[999];
        // 如果select非全部列
        if (raw_result != "*")
        {
            int col_count = 0;
            int result_pos[999];
            for (int i = 0; i < raw_result.size(); i++)
            {
                if (raw_result[i] == ',')
                {
                    result_pos[col_count] = i;
                    col_count++;
                }
            }
            for (int i = 0; i < col_count; i++)
            {
                result[i] = raw_result.substr(result_pos[i], result_pos[i+1] - result_pos[i]);
                cout << result[i] << endl;
            }  
        }
        
        // string conditions[999];
        // int con_count = 0;
        // for (int i = 5; i < count; i++)
        // {
        //     conditions[con_count] = tokens[i];
        //     con_count++;
        // }
    }
    else if(sql.substr(0, 6) == "update") {
        // 标准的update语句：update [tablename] set [updates] where [conditions]
        cout << "update" << endl;
        int pos[20];
        int count = 0;
        for (int i = 7; i < sql.size(); i++)
        {
            if (sql[i] == ' ')
            {
                pos[count] = i; // 记录空格的位置
                count++;
                cout << pos[count-1] << endl;
            }
        }
        string tokens[999];
        tokens[0] = sql.substr(7, pos[0] - 7);
        for (int i = 0; i < count; i++)
        {
            tokens[i+1] = sql.substr(pos[i] + 1, pos[i+1] - pos[i]);
            cout << tokens[i] << endl;
        }
        cout << tokens[count] << endl;
        string tablename = tokens[0];
    }
    else if(sql.substr(0, 4) == "alter") {
        // 或许会有这个功能
    }
    else
    {
        return "SQL syntax error";
    }
    
}

int main()
{
    string sql_input;
    // cin >> sql_input;
    sql_input = "create table test {abc:efg,edf:gdsf,dsfsdf:dsfs}";
    // sql_input = "use database test";
    // sql_input = "create database test";
    // sql_input = "insert into test (column1,column2,column3,column4) values (value1,value2,value3,value4)";
    // sql_input = "delete from test where column1=value1 and column2=value2";
    // sql_input = "delete database test";
    // sql_input = "delete table test";
    // sql_input = "select * from test where abc=efg or efg=def and opq=xyz";
    // sql_input = "update test set abc=efg where efg=def or opq=xyz and xyz=abc";
    // getline(cin, sql_input);
    sql_input = sql_processor(sql_input);
    parser(sql_input);
    system("pause");
}