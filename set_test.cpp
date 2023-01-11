#include <iostream>
#include <set>
#include <cstdio>
#include <ctime>
#include <vector>
#include <stack>
using namespace std;
struct strlist{
    string str[30];
    int len=0;
    void print(){for(int i=0;i<len;i++)cout<<str[i]<<endl;}
    void append(string nstr){str[len++]=nstr;}
};
vector<strlist> BreakDown(vector<string> logic){
    vector<strlist>lgc;stack<string>con,log;
    for(auto it=logic.cbegin();it!=logic.end();it++){
        string tstr=*it;
        if(tstr=="and"){
            if(con.empty()||con.top()=="("||con.top()=="or"){con.push(tstr);continue;}
            string nstr=log.top();log.pop();
            if(lgc.empty()){
                string nstr1=log.top();log.pop();
                lgc.push_back({{nstr,nstr1},2});
                continue;
            }
            for(auto it1=lgc.begin();it1!=lgc.end();it1++)(*it1).append(nstr);
        }
        else if(tstr=="or"){
            if(con.empty()||con.top()=="("){con.push(tstr);continue;}
            string tem=con.top();con.pop();con.push(tstr);
            string nstr=log.top();log.pop();
            if(lgc.empty()){
                string nstr1=log.top();log.pop();
                if(tem=="or")lgc.push_back({{nstr},1}),lgc.push_back({{nstr1},1});
                else lgc.push_back({{nstr,nstr1},2});
                continue;
            }
            if(tem=="or")lgc.push_back({{nstr},1});
            else for(auto it1=lgc.begin();it1!=lgc.end();it1++)(*it1).append(nstr);
        }
        else if(tstr==")"){
            while(con.top()!="("){
                string tem=con.top();con.pop();
                string nstr=log.top();log.pop();
                if(lgc.empty()){
                    string nstr1=log.top();log.pop();
                    if(tem=="or")lgc.push_back({{nstr},1}),lgc.push_back({{nstr1},1});
                    else lgc.push_back({{nstr,nstr1},2});
                    continue;
                }
                if(tem=="or")lgc.push_back({{nstr},1});
                else for(auto it1=lgc.begin();it1!=lgc.end();it1++)(*it1).append(nstr);
            }
            con.pop();
        }
        else if(tstr=="(")con.push("(");
        else log.push(tstr);
    }
    return lgc;
}
int main()
{
    vector<string>tem;vector<strlist>t;
    string str[23]={"(","a","and","(","b","or","(","c","and","(","d","or","e",")",
                    "and","f","or","g",")","and","h",")",")"};
    for(int i=0;i<23;i++)tem.push_back(str[i]);
    t=BreakDown(tem);
    for(auto it=t.begin();it!=t.end();it++)(*it).print(),puts("");
    system("pause");
    return 0;
}