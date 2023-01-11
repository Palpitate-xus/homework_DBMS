#include<bits/stdc++.h>
#include<direct.h>
#include"DateType.h"
#include"set.h"
using namespace std;
typedef int ll;
const long long INF=0x8000000000000000;
struct column{
    bool isNull=0;
    string dataType="",dataName="";
    ll dsize=0;
    void print(){
        cout<<"ColumnName: "<<dataName<<'\n';
        cout<<"hasNull: "<<isNull<<'\n';
        cout<<"DataType: "<<dataType<<'\n';
        cout<<"DataSize: "<<dsize<<'\n'<<endl;
    }
};
struct table{
    string tablename="";
    column cols[30];
    ll len=0;
    void append(column ncol){cols[len++]=ncol;}
    void print(){cout<<tablename<<'\n'<<endl;for(ll i=0;i<len;i++)cols[i].print();}
    ll size(){ll otc=0;for(ll i=0;i<len;i++)otc+=cols[i].dsize;return otc;}
};
struct strlist{
    string str[30];
    ll len=0;
    void print(){for(ll i=0;i<len;i++)cout<<str[i]<<endl;}
    void append(string nstr){str[len++]=nstr;}
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
    string filepath=DBname+"\\"+Tablename+".stc";table t={Tablename};
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
long long transint(string t){
    if(t.length()>19||!t.length())return INF;
    long long otc=0;
    for(ll i=0;i<t.length();i++){
        if(t[i]<48||t[i]>57)return INF;
        otc=otc*10+t[i]-48;
    }
    return otc;
}
ll insert(string DBname,string Tablename,strlist values){
    string filepath=DBname+"\\"+Tablename;
    if(_access((filepath+".stc").c_str(),0))return 1;//table not exist
    table tbl=getTablestruction(DBname,Tablename);
    map<string,string>key;
    for(ll i=0;i<values.len;i++){
        string tem=values.str[i];ll pos=tem.find(' ',0);
        if(pos==-1)return 1;
        key[tem.substr(0,pos)]=tem.substr(pos+1,tem.length()-pos);
    }
    FILE*xx=fopen((filepath+".dt").c_str(),"ab");
    fseek(xx,0,2);ll size=ftell(xx);
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
            ll len=str.length();long long t=transint(str);
            if(t==INF)goto l;
            fwrite(&t,tem.dsize,1,xx);
        }
    }
    fclose(xx);return 0;
    l:rewind(xx);
    _chsize(_fileno(xx),size);//clear written data
    fclose(xx);return 1;
}
Chris*fetch(string DBname,string Tablename,strlist cstr,Chris*id){
    string filepath=DBname+"\\"+Tablename;
    if(_access((filepath+".stc").c_str(),0))return id;//table not exist
    table tbl=getTablestruction(DBname,Tablename);ll size=tbl.size();
    map<string,string>key;
    for(ll i=0;i<cstr.len;i++){
        string tem=cstr.str[i];ll pos=tem.find(' ',0);
        if(pos==-1)return id;
        key[tem.substr(0,pos)]=tem.substr(pos+1,tem.length()-pos);
    }
    FILE*xx=fopen((filepath+".dt").c_str(),"rb");
    for(ll i=0,pos=0;i<tbl.len;pos+=tbl.cols[i++].dsize){
        column tem=tbl.cols[i];if(!(id->size()))goto ends;
        if(key['<'+tem.dataName]==""&&key["="+tem.dataName]==""&&key[">"+tem.dataName]=="")continue;
        if(tem.dataType=="char"){
            for(ll idx=id->kth(1);idx!=0x7FFFFFFF;idx=id->next(idx)){
                fseek(xx,pos+idx*size,0);char t[tem.dsize+1]="";
                fread(t,tem.dsize,1,xx);
                string m=key['<'+tem.dataName],tstr=string(t);
                if(m!=""&&tstr>=m)goto l1;
                m=key['>'+tem.dataName];if(m!=""&&tstr>=m)goto l1;
                m=key['='+tem.dataName];if(m!=""&&tstr!=m)goto l1;
                continue;
                l1:id->delet(idx);
            }
        }
        else if(tem.dataType=="date"){
            for(ll idx=id->kth(1);idx!=0x7FFFFFFF;idx=id->next(idx)){
                fseek(xx,pos+idx*size,0);Date tdate;
                fread(&tdate,12,1,xx);
                Date m=Date(key['<'+tem.dataName].c_str());
                if(m.year&&tdate>=m)goto l2;
                m=Date(key['>'+tem.dataName].c_str());if(m.year&&tdate<=m)goto l2;
                m=Date(key['='+tem.dataName].c_str());if(m.year&&tdate!=m)goto l2;
                continue;
                l2:id->delet(idx);
            }
        }
        else{
            for(ll idx=id->kth(1);idx!=0x7FFFFFFF;idx=id->next(idx)){
                fseek(xx,pos+idx*size,0);long long tint=0;
                fread(&tint,tem.dsize,1,xx);
                long long m=transint(key['<'+tem.dataName]);
                if(m!=INF&&tint>=m)goto l3;
                m=transint(key['>'+tem.dataName]);if(m!=INF&&tint<=m)goto l3;
                m=transint(key['='+tem.dataName]);
                if(m!=INF&&tint!=m)goto l3;
                continue;
                l3:id->delet(idx);
            }
        }
        if(!id->size())break;
    }
    ends:fclose(xx);
    return id;
}
ll delet(string DBname,string Tablename,strlist cstr){
    string filepath=DBname+"\\"+Tablename;
    if(_access((filepath+".stc").c_str(),0))return 1;//table not exist
    FILE*xx=fopen((filepath+".dt").c_str(),"rb");
    FILE*yy=fopen((filepath+".temp").c_str(),"wb");
    fseek(xx,0,2);
    table tbl=getTablestruction(DBname,Tablename);ll size=tbl.size(),rowct=ftell(xx)/size;
    rewind(xx);
    Chris*id=(Chris*)calloc(1,sizeof(Chris));
    for(ll i=0;i<rowct;i++)id->insert(i);
    id=fetch(DBname,Tablename,cstr,id);if(!(id->size()))goto end;//nothing to delete
    for(ll i=0;i<rowct;i++){
        if(id->has(i)){fseek(xx,size,1);continue;}
        for(ll i=0;i<tbl.len;i++){
            column tem=tbl.cols[i];
            if(tem.dataType=="char"){
                char tstr[tem.dsize+1]="";
                fread(tstr,tem.dsize,1,xx);
                fwrite(tstr,tem.dsize,1,yy);
            }
            else if(tem.dataType=="Date"){
                Date tdate;
                fread(&tdate,12,1,xx);
                fwrite(&tdate,12,1,yy);
            }
            else{
                long long tint=0;
                fread(&tint,tem.dsize,1,xx);
                fwrite(&tint,tem.dsize,1,yy);
            }
        }
    }
    end:fclose(xx);fclose(yy);free(id);
    remove((filepath+".dt").c_str());
    rename((filepath+".temp").c_str(),(filepath+".dt").c_str());
    return 0;
}
vector<string> lookup(string DBname,string Tablename,strlist cstr){
    string filepath=DBname+"\\"+Tablename;vector<string>otc;
    if(_access((filepath+".stc").c_str(),0))return otc;//table not exist
    FILE*xx=fopen((filepath+".dt").c_str(),"rb");
    fseek(xx,0,2);
    table tbl=getTablestruction(DBname,Tablename);ll size=tbl.size(),rowct=ftell(xx)/size;
    rewind(xx);
    Chris*id=(Chris*)calloc(1,sizeof(Chris));
    for(ll i=0;i<rowct;i++)id->insert(i);
    id=fetch(DBname,Tablename,cstr,id);if(!(id->size()))goto end;//nothing to delete
    for(ll idx=id->kth(1);idx!=0x7FFFFFFF;idx=id->next(idx)){
        fseek(xx,idx*size,0);string ans="";
        for(ll i=0;i<tbl.len;i++){
            column tem=tbl.cols[i];
            if(tem.dataType=="char"){
                char tstr[tem.dsize+1]="";
                fread(tstr,tem.dsize,1,xx);
                ans+=string(tstr)+' ';
            }
            else if(tem.dataType=="date"){
                Date tdate;
                fread(&tdate,12,1,xx);
                ans+=str(tdate)+' ';
            }
            else{
                long long tint=0;
                fread(&tint,tem.dsize,1,xx);
                ans+=transstr(tint)+' ';
            }
        }
        otc.push_back(ans);
    }
    end:fclose(xx);free(id);
    return otc;
}
int main(){
    /*createDatabase("info");
    table newt;
    newt.append(setint("goodsCount",0,2));
    newt.append(setstring("goodsName",0,15));
    newt.append(setdate("saleDate",0));
    createTable("info","goods",newt);*/
    /*strlist nstr1,nstr2;
    nstr1.append("goodsCount 15");
    nstr1.append("goodsName earphones");
    nstr1.append("saleDate 2020-08-19");
    nstr2.append("goodsCount 24");
    nstr2.append("goodsName kungpow chicken");
    nstr2.append("saleDate 2023-01-10");
    insert("info","goods",nstr1);insert("info","goods",nstr2);*/
    /*FILE*xx=fopen("info\\goods.dt","rb");
    while(1){
        ll ct=0;char gn[16]="";Date sd;
        fread(&ct,4,1,xx);
        fread(gn,15,1,xx);
        fread(&sd,12,1,xx);
        if(feof(xx))break;
        cout<<ct<<','<<gn<<','<<sd<<endl;
    }
    fclose(xx);*/
    /*strlist tem;
    tem.append("=goodsName earphones");
    tem.append("<goodsCount 30");
    delet("info","goods",tem);*/
    strlist t={{""},0};t.append("=goodsCount 24");
    vector<string>tem=lookup("info","goods",t);
    for(auto it=tem.cbegin();it!=tem.cend();it++)cout<<*it<<endl;
}