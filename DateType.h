#include<iostream>
typedef int ll;
const ll days[14]={0,365,334,306,275,245,214,184,153,122,92,61,31,0};
struct Date{
    Date(ll y,ll m,ll d):
        year(y),month(m),day(d){if(month>12||day>days[month]-days[month+1]+(month==2)*isleap())month=0,year=0,day=0;}
    Date(char*s){
        ll pos[2]={0,0};
        for(ll i=0,k=0;*(s+i)&&k<2;i++)if(*(s+i)=='-')pos[k++]=i;
        if(!pos[0]||!pos[1]||pos[1]==pos[0]+1||pos[1]-pos[0]>3||strlen(s)-pos[1]>3||pos[0]>4)return;
        for(ll i=0;i<pos[0];i++)year=year*10+*(s+i)-48;
        for(ll i=pos[0]+1;i<pos[1];i++)month=month*10+*(s+i)-48;
        for(ll i=pos[1]+1;*(s+i);i++)day=day*10+*(s+i)-48;
        if(month>12||day>days[month]-days[month+1]+(month==2)*isleap())month=0,year=0,day=0;
    }
    ll year=0,month=0,day=0;
    bool isleap(){return (!(year%4)&&year%100)||!(year%400);}
    void print(){std::cout<<year<<'-'<<month<<'-'<<day<<std::endl;}
    ll convert(){return year*365+year/4-year/100+year/400-days[month]-(month<=2)*isleap()+day;}
};
Date DISCONV(ll num){//turn a day count into Date object
    Date t={0,12,31};
    if(num<=0||num>3652059)return t;
    if(num>=146097)t.year+=num/146097*400;
    num%=146097;
    while(num>0)t.year++,num-=(365+t.isleap());
    while(num+days[t.month]<=0)t.month--;
    t.day=num+days[t.month]+(t.month<=2)*t.isleap();
    return t;
}
ll CONVERT(Date t){
    return t.year*365+t.year/4-t.year/100+t.year/400-days[t.month]-(t.month<=2)*t.isleap()+t.day;
}
bool operator>(Date a,Date b){
    if(a.year!=b.year)return a.year>b.year;
    if(a.month!=b.month)return a.month>b.month;
    return a.day>b.day;
}
bool operator<(Date a,Date b){
    if(a.year!=b.year)return a.year<b.year;
    if(a.month!=b.month)return a.month<b.month;
    return a.day<b.day;
}
bool operator==(Date a,Date b){
    return a.year==b.year&&a.month==b.month&&a.day==b.day;
}
bool operator>=(Date a,Date b){
    if(a.year!=b.year)return a.year>b.year;
    if(a.month!=b.month)return a.month>b.month;
    return a.day>=b.day;
}
bool operator<=(Date a,Date b){
    if(a.year!=b.year)return a.year<b.year;
    if(a.month!=b.month)return a.month<b.month;
    return a.day<=b.day;
}
Date operator+(Date a,ll b){
    return DISCONV(a.convert()+b);
}
Date operator-(Date a,ll b){
    return DISCONV(a.convert()-b);
}
ll operator-(Date a,Date b){
    return a.convert()-b.convert();
}