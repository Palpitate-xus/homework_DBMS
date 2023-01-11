#include <iostream>
#include <map>
#include <string>
#include <direct.h>
#include <io.h>
#include "DateType.h"
using namespace std;
#define maxn 100010
typedef int ll;
struct column{
    bool isNull;
    string dataType,dataName;
    ll dsize;
    void print(){
        cout<<"ColumnName: "<<dataName<<'\n';
        cout<<"hasNull: "<<isNull<<'\n';
        cout<<"DataType: "<<dataType<<'\n';
        cout<<"DataSize: "<<dsize<<'\n'<<endl;
    }
};
struct table{
    column cols[30];
    ll len=0;
    string tablename="";
    void append(column ncol){cols[len++]=ncol;}
    void print(){cout<<tablename<<'\n'<<endl;for(ll i=0;i<len;i++)cols[i].print();}
    ll size(){ll otc=0;for(ll i=0;i<len;i++)otc+=cols[i].dsize;return otc;}
};
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
struct intlist{
    //
};
ll createDatabase(string DBname){
    if(!_access(DBname.c_str(),0))return 1;//database already exist
    _mkdir(DBname.c_str());
    FILE*xx=fopen((DBname+"\\"+"carve.log").c_str(),"wb");
    fclose(xx);
    xx=fopen((DBname+"\\"+"tlist.lst").c_str(),"wb");
    fclose(xx);
    return 0;//success
}
ll dropDatabase(string DBname){
    if(_access(DBname.c_str(),0))return 1;//database not exist
    system(("del "+DBname+" /q").c_str());
    _rmdir(DBname.c_str());
    return 0;//success
}
ll createTable(string DBname,string Tablename,table tbl){
    string filepath=DBname+"\\"+Tablename;
    if(!_access((filepath+".stc").c_str(),0)||_access(DBname.c_str(),0))return 1;//table already exist or database not exist
    FILE*xx=fopen((filepath+".stc").c_str(),"wb");
    fwrite(&tbl.len,4,1,xx);
    for(ll i=0;i<tbl.len;i++){
        fwrite(&tbl.cols[i].isNull,1,1,xx);
        fwrite(tbl.cols[i].dataType.c_str(),4,1,xx);
        fwrite(tbl.cols[i].dataName.c_str(),15,1,xx);
        fwrite(&tbl.cols[i].dsize,4,1,xx);
    }
    fclose(xx);
    xx=fopen((filepath+".dt").c_str(),"wb");
    fclose(xx);
    xx=fopen((DBname+"\\tlist.lst").c_str(),"ab");
    fwrite(Tablename.c_str(),15,1,xx);
    fclose(xx);
    return 0;//success
}
ll dropTable(string DBname,string Tablename){
    string filepath=DBname+"\\"+Tablename;strlist tbls;char t[15];
    if(_access((filepath+".stc").c_str(),0))return 1;//table not exist
    system(("del "+filepath+".stc,"+filepath+".dt /q").c_str());
    FILE*xx=fopen((DBname+"\\tlist.lst").c_str(),"rb");
    while(fread(t,15,1,xx)){
        string tem=string(t);
        if(tem!=Tablename)tbls.append(tem);
    }
    fclose(xx);
    xx=fopen((DBname+"\\tlist.lst").c_str(),"wb");
    for(ll i=0;i<tbls.len;i++)fwrite(tbls.str[i].c_str(),15,1,xx);
    fclose(xx);
    return 0;//success
}
strlist getTablenames(string DBname){
    FILE*xx=fopen((DBname+"\\tlist.lst").c_str(),"rb");
    char t[16];strlist tbls;
    while(fread(t,15,1,xx))tbls.append(string(t));
    fclose(xx);
    return tbls;
}
table getTablestruction(string DBname,string Tablename){
    string filepath=DBname+"\\"+Tablename+".stc";
    table t;
    t.tablename=Tablename;
    if(_access(filepath.c_str(),0))return t;//table not exist
    FILE*xx=fopen(filepath.c_str(),"rb");
    fread(&t.len,4,1,xx);
    for(ll i=0;i<t.len;i++){
        char type[5]="",name[16]="";
        fread(&t.cols[i].isNull,1,1,xx);
        fread(type,4,1,xx);
        fread(name,15,1,xx);
        fread(&t.cols[i].dsize,4,1,xx);
        t.cols[i].dataType=string(type);
        t.cols[i].dataName=string(name);
    }
    fclose(xx);
    return t;
}
column setint(string columnName,bool isNull,ll scale){
    string tem;
    if(scale<=1)tem="tiny";
    else if(scale==2)tem="int",scale=4;
    else if(scale>=3)tem="long",scale=8;
    return {isNull,tem,columnName,scale};
}
column setstring(string columnName,bool isNull,ll length){
    return {isNull,"char",columnName,max(1,min(length,1005))};
}
column setdate(string columnName,bool isNull){
    return {isNull,"date",columnName,12};
}
ll insert(string DBname,string Tablename,strlist values){
    string filepath=DBname+"\\"+Tablename;
    if(_access((filepath+".stc").c_str(),0))return 1;//table not exist
    table tbl=getTablestruction(DBname,Tablename);
    FILE*xx=fopen((filepath+".dt").c_str(),"ab");
    fseek(xx,0,2);ll size=ftell(xx);
    map<string,string>key;
    for(ll i=0;i<values.len;i++){
        string tem=values.str[i];ll pos=tem.find(' ',0);
        if(pos==-1)return 1;
        key[tem.substr(0,pos)]=tem.substr(pos+1,tem.length()-pos);
    }
    for(ll i=0;i<tbl.len;i++){
        column tem=tbl.cols[i];string str=key[tem.dataName];
        if(!tem.isNull&&str=="")goto l;//unexpected NULL value
        if(tem.dataType=="char")fwrite(str.c_str(),tem.dsize,1,xx);
        else if(tem.dataType=="date"){
            Date t=Date(str.c_str());
            if(!t.year)goto l;//wrong date form
            fwrite(&t,12,1,xx);
        }
        else{
            ll len=str.length(),t=0;
            if(len>19)goto l;//value string too long
            for(ll i=0;i<len;i++){
                if(str[i]<48||str[i]>57)goto l;//has not number characters
                t=t*10+str[i]-48;
            }
            fwrite(&t,tem.dsize,1,xx);
        }
    }
    fclose(xx);return 0;
    l:rewind(xx);
    _chsize(_fileno(xx),size);//clear written data
    fclose(xx);return 1;
}
ll delet(string DBname,string Tablename,strlist cstr){
    string filepath=DBname+"\\"+Tablename;
    if(_access((filepath+".stc").c_str(),0))return 1;//table not exist
    table tbl=getTablestruction(DBname,Tablename);ll size=tbl.size();
    FILE*xx=fopen((filepath+".dt").c_str(),"rb");
    map<string,string>key;
    while(1){
        //
    }
    fclose(xx);
    return 0;
}
