#include <iostream>
#include <regex>
using namespace std;

// 本文件存放一些SQL语句的正则表达式模板

// 标准sql语句
// string sqlupdate = "update\.+set\.+(where\.+)?END";
// string sqldelete = "delete\.+from\.+where\.+END";
// string sqlinsert="insert\.+into\.+(\(\.+/)\.*)?values\(.+\)END";
// string sqlcreate="create table\.+\\(\.+\\)END";
// string sqlselect="select\.+from\.+(where\.+)?((group by)?|(order by)?|(having)?)END";
// string sqldrop  ="drop table\.+END";

int main() {
    string fnames[] = {"update test set abc=bef", "update set", "test", "a0.txt", "AAA.txt"};
    // 在 C++ 中 \ 会被作为字符串内的转义符，
    // 为使 \. 作为正则表达式传递进去生效，需要对 \ 进行二次转义，从而有 \\.
    regex txt_regex("update [a-zA-Z0-9]+ set [a-zA-Z0-9]+\=[a-zA-Z0-9]+(where [a-zA-Z0-9] +\= [a-zA-Z0-9]+)?");
    for (const auto &fname: fnames)
        cout << fname << ": " << regex_match(fname, txt_regex) << endl;
    system("pause");
}

// update_regex("update [a-zA-Z0-9]+ set [a-zA-Z0-9]+\=[a-zA-Z0-9]+(where [a-zA-Z0-9] +\= [a-zA-Z0-9]+)?");
// insert_regex("insert into [a-zA-Z0-9]+(\(([a-zA-Z0-9]+\,)+[a-zA-Z0-9]+\))? values\(([a-zA-Z0-9]+,)+[a-zA-Z0-9]+\)");
// select_regex("select ([a-zA-Z0-9]+,)+[a-zA-z0-9]+ from [a-zA-Z0-9]+ (where( [a-zA-Z0-9]+=[a-zA-Z0-9]+ and)+ [a-zA-Z0-9]+=[a-zA-Z0-9]+)?");  单表查询
