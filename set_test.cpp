#include <iostream>
#include <set>
#include <cstdio>
#include <ctime>
using namespace std;
int main()
{
    set<int> t;
    double a=clock();
    for(int i=100000;i;i--)t.insert(i);
    double b=clock();
    for(int i=100000;i;i--)t.erase(i);
    double c=clock();
    printf("%lf,%lf\n", (b-a)/CLK_TCK,(c-b)/CLK_TCK);
    system("pause");
    return 0;
}