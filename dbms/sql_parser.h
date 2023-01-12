#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include "TableManage.h"
#include "logs.h"
#include "permission.h"
using namespace std;

int now_permission = 1;
string modifylogic(string logic){
    if(logic == "(" || logic == ")"|| logic == "and" || logic == "or" ) return logic;
    for(int i = 0; i < logic.size(); i++){
        if(logic[i] == '>' || logic[i] == '<'|| logic[i] == '='){
            //cout<<logic<<' '<<i<<endl;
            logic = logic[i] + logic;
            logic[i+1] = ' ';
            return logic;
        }
    }
    return "";
}
// sql语句预处理
string sql_processor(string raw_sql){ 
    // 大写转小写
    for(auto &i : raw_sql){
        i = tolower(i);
        if (i > 127) return "error";
    }
    raw_sql.erase(remove(raw_sql.begin(), raw_sql.end(), '\n'),raw_sql.end());  // 删除换行符
    raw_sql.erase(remove(raw_sql.begin(), raw_sql.end(), '\t'),raw_sql.end());  // 删除制表符
    cout << "processed" << endl;
    return raw_sql;
}
// sql语句解析
string DBname = "info";
string execute(string sql){
    // cout << sql << '\n' << endl;
    if(sql.substr(0, 3) == "use"){
        if(sql.substr(4, 8) == "database"){
            DBname = sql.substr(13, sql.size() - 13);
            if(_access(DBname.c_str(),0)){
                cout<<"Database not found"<<endl;///////////////////////////////////////////////////////////////////
                DBname="";
                return "database not exist";//设置访问失败，不存在该数据库
                logs("xus","Database not exist",getTime());
            }
            cout<<"set Database to "<<DBname<<endl;/////////////////////////////////////////////////////////////////
            logs("xus","set Database name",getTime());
            return "successfully";//修改成功
        }
    }
    else if(sql.substr(0, 6) == "create") {
        // create user username password permission
        if (sql.substr(7, 4) == "user"){
            if (!now_permission) {
                cout << "permission denied" << endl;
                return "permission denied";
            }
            int space_pos[12];
            int count = 0;
            for (int i = 12; i < sql.size(); i++)
            {
                if (sql[i] == ' ')
                {
                    space_pos[count] = i;
                    count++;
                }
            }
            string tokens[3];
            tokens[0] = sql.substr(12, space_pos[0] - 12);
            for (int i = 0; i < count; i++)
            {
                tokens[i+1] = sql.substr(space_pos[i] + 1, space_pos[i+1] - space_pos[i] - 1);
            }
            string username = tokens[0];
            string password = tokens[1];
            string permission = tokens[2];
            if (permissionQuery(username) != -1) {
                // 如果已经存在该用户
                cout << "error: user already exist" << endl;
                return "error: user already exist";
            }
            createUser(username, password, permission);
            logs("xus","create User",getTime());
            cout << "create user  " << username << "  succeeded" << endl;
        }
        // create标准格式: create database [databasename]
        if (sql.substr(7, 8) == "database"){
            if (!now_permission) {
                cout << "permission denied" << endl;
                logs("xus","permission denied",getTime());
                return "permission denied";
            } 
            int tem=createDatabase(sql.substr(16, sql.size() - 16));
            if(tem){
                cout<<"Failed:Database "<<sql.substr(16, sql.size() - 16)<<" already exists"<<endl;////////////////
                logs("xus","Database already exists",getTime());
                return "database already exists";
            }
            else {
                cout<<"Create Database succeeded"<<endl;////////////////////////////////////////////////////////////
                return "success";
            }
        }
        // create标准格式：create table [tablename] {col: type, col: type……}
        if (sql.substr(7, 5) == "table"){
            if (!now_permission) {
                cout << "permission denied" << endl;
                return "permission denied";
            }
            if(_access(DBname.c_str(),0)){
                cout<<"Invalid Database Name:"+DBname<<endl;///////////////////////////////////////////////////////////
                return "Invalid Database Name";
            }
            string left_sql = sql.substr(13, sql.size()-13);
            int space_pos = 0;
            for (; space_pos < left_sql.size()&&left_sql[space_pos]!=' '; space_pos++);
            table tbl={left_sql.substr(0, space_pos)};
            for(int pos=space_pos+2;;){
                int commapos=0,cutpos=0;
                for(int i=pos;i<left_sql.size();i++){
                    if(left_sql[i]==':')cutpos=i;
                    else if(left_sql[i]==','||left_sql[i]=='}'){commapos=i;break;}
                }
                if(!commapos||!cutpos)break;
                string cname=left_sql.substr(pos,cutpos-pos),ctype=left_sql.substr(cutpos+1,commapos-cutpos-1);
                if(ctype.substr(0,3)=="int"){
                    int t=3;while(ctype[t]!=' '&&t<ctype.size())t++;
                    if(ctype[t+1]=='0'&&t<ctype.size())tbl.append(setint(cname,0,2));
                    else tbl.append(setint(cname,1,2));
                }
                else if(ctype.substr(0,4)=="tiny"){
                    int t=4;while(ctype[t]!=' '&&t<ctype.size())t++;
                    if(ctype[t+1]=='0'&&t<ctype.size())tbl.append(setint(cname,0,1));
                    else tbl.append(setint(cname,1,1));
                }
                else if(ctype.substr(0,4)=="long"){
                    int t=4;while(ctype[t]!=' '&&t<ctype.size())t++;
                    if(ctype[t+1]=='0'&&t<ctype.size())tbl.append(setint(cname,0,3));
                    else tbl.append(setint(cname,1,3));
                }
                else if(ctype.substr(0,4)=="date"){
                    int t=4;while(ctype[t]!=' '&&t<ctype.size())t++;
                    if(ctype[t+1]=='0'&&t<ctype.size())tbl.append(setdate(cname,0));
                    else tbl.append(setdate(cname,1));
                }
                else if(ctype.substr(0,4)=="char"){
                    int len=0,t=4;
                    for(;ctype[t]!=' '&&t<ctype.size();t++)len=len*10+ctype[t]-48;
                    if(!len)len=1;
                    if(ctype[t+1]=='0'&&t<ctype.size())tbl.append(setstring(cname,0,len));
                    else tbl.append(setstring(cname,1,len));
                }
                pos=commapos+1;
            }
            if(createTable(DBname,tbl)){
                cout<<"Table "+tbl.tablename+" already exists"<<endl;//////////////////////////////////////////////////
                return "already exists";
            }
            else{
                cout<<"Table create succeded"<<endl;//////////////////////////////////////////////////////////////////
                return "success";
            }
        }
    }
    else if(sql.substr(0, 12) == "insert into "){
        if(_access(DBname.c_str(),0)){
            cout<<"Invalid Database name:"+DBname<<endl;///////////////////////////////////////////////////////////
            return "invalid database name";
        }
        vector<string>subs;
        for(int st=12;;){
            while(sql[st]==' '&&st<sql.size())st++;
            if(st>=sql.size())break;
            if(sql[st]=='('){
                int ed=++st;while(sql[ed]!=')'&&ed<sql.size())ed++;
                if(ed>=sql.size()){
                    cout<<"SQL syntax error"<<endl;/////////////////////////////////////////////////////////////
                    return "SQL syntax error";
                }
                subs.push_back(sql.substr(st,ed-st));
                st=ed+1;
                continue;
            }
            int ed=st;
            while(sql[ed]!=' '&&sql[ed]!=')'&&ed<sql.size())ed++;
            subs.push_back(sql.substr(st,ed-st));
            st=ed+1;
        }
        string tablename=subs.front();
        if(_access((DBname+"\\"+tablename+".stc").c_str(),0)){
            cout<<"Table "+tablename+" not exist"<<endl;///////////////////////////////////////////////////////////
            return "table not exist";
        }
        subs.erase(subs.begin());
        string columns=*subs.begin(),vals=*(subs.begin()+2);strlist values;
        for(int stc=0,stv=0;;){
            int edc=stc,edv=stv;
            while(columns[edc]!=','&&edc<columns.size())edc++;
            while(vals[edv]!=','&&edv<vals.size())edv++;
            values.append(columns.substr(stc,edc-stc)+' '+vals.substr(stv,edv-stv));
            stc=edc+1,stv=edv+1;
            if(stc>=columns.size()||stv>=vals.size())break;
        }
        if(insert(DBname,tablename,values)){
            cout<<"Invalid data, please check"<<endl;/////////////////////////////////////////////////////
            return "invalid data";
        }
        else{
            cout<<"Data inserted"<<endl;/////////////////////////////////////////////////////////////////////
            return "success";
        }
    }
    else if(sql.substr(0, 12) == "delete from "){
        // 标准delete语句：delete from [tablename] where [xxx] and ...
        if(_access(DBname.c_str(),0)){
            cout<<"Invalid Database name:"+DBname<<endl;///////////////////////////////////////////////////////////
            return "invalid database name";
        }
        vector<string>subs;
        for(int st=12;;){
            while(sql[st]==' '&&st<sql.size())st++;
            if(st>=sql.size())break;
            if(sql[st]=='('){subs.push_back("("),st++;continue;}
            if(sql[st]==')'){subs.push_back(")"),st++;continue;}
            int ed=st;
            while(sql[ed]!=' '&&sql[ed]!=')'&&ed<sql.size())ed++;
            subs.push_back(sql.substr(st,ed-st));
            st=ed;
        }
        string tablename=subs.front();
        if(_access((DBname+"\\"+tablename+".stc").c_str(),0)){
            cout<<"Table "+tablename+" not exist"<<endl;///////////////////////////////////////////////////////////
            return "table not exist";
        }
        subs.erase(subs.begin());
        if(!subs.size()){
            strlist aint={{""},0};
            delet(DBname,tablename,aint);
            cout<<"Delete done"<<endl;//////////////////////////////////////////////////////////////////////////////////
            return "delete done";
        }
        if(subs.size()==1){
            cout<<"SQL syntax error"<<endl;/////////////////////////////////////////////////////////////////////
            return "SQL syntax error";
        }
        *subs.begin()="(";subs.push_back(")");
        for(auto it=subs.begin();it!=subs.end();it++)*it=modifylogic(*it);
        vector<strlist>lgcs=BreakDown(subs);
        for(auto it=lgcs.begin();it!=lgcs.end();it++)delet(DBname,tablename,*it);
        cout<<"Delete done"<<endl;//////////////////////////////////////////////////////////////////////////////////
        return "delete done";
    }
    else if (sql.substr(0, 4) == "drop"){
        if (!now_permission) {
            cout << "permission denied" << endl;
            return "permission denied";
        }
        if(_access(DBname.c_str(),0)){
            cout<<"Invalid Database name:"+DBname<<endl;///////////////////////////////////////////////////////////
            return "invalid database name";
        }
        int st=4;
        while(sql[st]==' '&&st<sql.size()) st++;
        if(st>=sql.size()){
            cout<<"SQL syntax error"<<endl;//////////////////////////////////////////////////////////
            return "SQL syntax error";
        }
        int ed=st;
        while(sql[ed]!=' '&&ed<sql.size()) ed++;
        string op=sql.substr(st,ed-st);
        if(op=="table"){
            st=ed;
            while(sql[st]==' '&&st<sql.size()) st++;
            ed=st;
            while(sql[ed]!=' '&&ed<sql.size()) ed++;
            string tablename=sql.substr(st,ed-st);
            if(dropTable(DBname,tablename)){
                cout<<"Table "+tablename+" not exist"<<endl;//////////////////////////////////////////////////////////////
                return "table not exist";
            }
            else{
                cout<<"Table dropped"<<endl;/////////////////////////////////////////////////////////////////
                return "table dropped";
            }
        }
        else if(op=="database"){
            st=ed;while(sql[st]==' '&&st<sql.size())st++;
            ed=st;while(sql[ed]!=' '&&ed<sql.size())ed++;
            string dbname=sql.substr(st,ed-st);
            if(dropDatabase(dbname)){
                cout<<"Database "+dbname+" not exist"<<endl;///////////////////////////////////////////////////
                return "database not exist";
            }
            else{
                cout<<"Database dropped"<<endl;/////////////////////////////////////////////////////////////////
                return "database dropped";
            }
        }
        cout<<"SQL syntax error"<<endl;//////////////////////////////////////////////////////////
        return "SQL syntax error";
    }
    else if(sql.substr(0, 6) == "select") {
        // 标准的select语句：select [*]/[column] from [tablename] where [conditions]
        if(_access(DBname.c_str(),0)){
            cout<<"Invalid Database name:"+DBname<<endl;///////////////////////////////////////////////////////////
            return "invalid database name";
        }
        vector<string>subs;
        for(int st=6;;){
            while(sql[st]==' '&&st<sql.size())st++;
            if(st>=sql.size())break;
            if(sql[st]=='('){subs.push_back("("),st++;continue;}
            if(sql[st]==')'){subs.push_back(")"),st++;continue;}
            int ed=st;
            while(sql[ed]!=' '&&sql[ed]!=')'&&ed<sql.size())ed++;
            subs.push_back(sql.substr(st,ed-st));
            st=ed;
        }
        string tablename=*(subs.begin()+2),columns=*subs.begin();
        if(_access((DBname+"\\"+tablename+".stc").c_str(),0)){
            cout<<"Table "+tablename+" not exist"<<endl;///////////////////////////////////////////////////////////
            return "table not exist";
        }
        table tbl=getTablestruction(DBname,tablename);
        set<string>nonslc;
        if(columns=="*")goto skip;
        for(int i=0;i<tbl.len;i++)nonslc.insert(tbl.cols[i].dataName);
        for(int st=0;;){
            int ed=st;while(columns[ed]!=','&&ed<columns.size())ed++;
            string col=columns.substr(st,ed-st);
            if(!nonslc.count(col)){
                cout<<"Invalid column name "+col<<endl;////////////////////////////////////////////////
                return "invalid column name";
            }
            nonslc.erase(col);
            st=ed+1;if(st>columns.size())break;
        }
        skip:subs.erase(subs.begin());subs.erase(subs.begin());subs.erase(subs.begin());
        if(subs.size()==1){
            cout<<"SQL syntax error"<<endl;/////////////////////////////////////////////////////////////////////
            return "sql syntax error";
        }
        for(int i=0;i<tbl.len;i++)if(!nonslc.count(tbl.cols[i].dataName))cout<<tbl.cols[i].dataName+' '<<ends;///////////////////////////////////////////////////////////////////
        cout<<'\n'<<ends;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(!subs.size()){
            strlist aint={{""},0};vector<string>ans;
            ans=lookup(DBname,tablename,aint,nonslc);
            for(auto it=ans.cbegin();it!=ans.cend();it++)cout<<*it<<endl;/////////////////////////////////////////////////////////////////
            return 0;
        }
        *subs.begin()="(";subs.push_back(")");
        for(auto it=subs.begin();it!=subs.end();it++)*it=modifylogic(*it);
        vector<strlist>lgcs=BreakDown(subs);
        for(auto it=lgcs.begin();it!=lgcs.end();it++){
            vector<string>ans=lookup(DBname,tablename,*it,nonslc);
            for(auto it=ans.cbegin();it!=ans.cend();it++)cout<<*it<<endl;/////////////////////////////////////////////////////////////////
        }
        return 0;
    }
    /*else if(sql.substr(0, 6) == "update") {
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
    }*/
    else if(sql.substr(0, 4) == "view") {
        int st=4;while(sql[st]==' '&&st<sql.size())st++;
        if(st>=sql.size()){
            cout<<"SQL syntax error"<<endl;//////////////////////////////////////////////////////////
            return "SQL syntax error";
        }
        int ed=st;while(sql[ed]!=' '&&ed<sql.size())ed++;
        string op=sql.substr(st,ed-st);
        if(op=="table"){
            st=ed;while(sql[st]==' '&&st<sql.size())st++;
            ed=st;while(sql[ed]!=' '&&ed<sql.size())ed++;
            string tablename=sql.substr(st,ed-st);
            table tbl=getTablestruction(DBname,tablename);
            if(!tbl.len){
                cout<<"Table "+tablename+" not exist"<<endl;////////////////////////////////////////////
                return "table not exist";
            }
            else{
                tbl.print();/////////////////////////////////////////////////////////////////////////////////////////
                return "success";
            }
        }
        else if(op=="database"){
            strlist tbln=getTablenames(DBname);
            tbln.print();///////////////////////////////////////////////////////////////////////
            return "success";
        }
        cout<<"SQL syntax error"<<endl;//////////////////////////////////////////////////////////
        return "SQL syntax error";
    }
    else
    {
        cout<<"SQL syntax error"<<endl;///////////////////////////////////////////////////
        return "SQL syntax error";
    }
    return "success";
}

// int main()
// {
//     string sql_input;
//     // cin >> sql_input;
//     // sql_input = "create table test {column1:char20 0,column2:char,column3:char1 1,column4:date 0,column5:long}";
//     // sql_input = "use database test";
//     // sql_input = "create database test";
//     // sql_input = "insert into test (column1,column2,column3,column4) values (value1,value2,value3,value4)";
//     // sql_input = "delete from test <where column1=value1 and (column2>value2 or column3<value3)>";
//     // sql_input = "drop database test";
//     // sql_input = "drop table test";
//     // sql_input = "select * from test <where column1=value1 and (column2>value2 or column3<value3)>";
//     // sql_input = "update test set abc=efg where efg=def or opq=xyz and xyz=abc";not done yet
//     //<> means it's not necessary
//     string username, password;
//     cout << "login" << endl;
//     cin >> username >> password;
//     if (login(username, password))
//     {
//         now_permission = permissionQuery(username);
//         while (1)
//         {
//             fflush(stdin);
//             getline(cin, sql_input);
//             if (sql_input == "exit") return 0;
//             sql_input = sql_processor(sql_input);
//             execute(sql_input);
//         }
//     }
//     else
//     {
//         cout << "wrong username or password" << endl;
//     }
//     //system("pause");
// }
