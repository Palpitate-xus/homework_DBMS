#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Token{
	string cifadanyuan;
	string cisu;
	Token(){
	}
	Token(string cifadanyuan,string cisu){
		this->cifadanyuan = cifadanyuan;
		this->cisu = cisu;
	}
	string toString(){
		return this->cifadanyuan+"  "+this->cisu;
	}
	//toString方法，方便我们后面把它展示到控制台上
};

class Lexical{
	const string operators = "+-*/%=&|<>!";
	const string singleword = "(),.;";
	//这里使用一个哈希表来存储sql里面的保留字，这里只是一部分，有耐心可以的话，可以全加进去。
	unordered_map<string,int> keyword{
					{"select",1},{"insert",2},{"delete",3},{"update",4},
					{"from",5},{"create",6},{"database",7},{"as",8},
					{"drop",9},{"alter",10},{"into",11},{"where",12},
					{"join",13},{"set",14},{"use",15},{"table",16},
					{"index",17},{"primary",18},{"in",19},{"between",20},
					{"like",21},{"and",22},{"or",23},{"key",24},
					{"by",25},{"character",26},{"varchar",25},{"boolean",26},
					{"int",25},{"decimal",26},{"null",25},{"date",26},
					{"time",25},{"not",26},{"values",27},{"order",28}
				};
	char temp[40];//这里存放我们分析到的token，先放在一个临时的数组里
	char *ch;//这是用来指向待分析程序的指针。
	
	Token lexOperators(){ 
		Token token;
		if(*ch == '/'){
			switch(*(++ch)){
				case '*':
					while((*ch != '/' || *(ch - 1) != '*') && *ch != '\0') ch++;
					break;
				default:
					temp[0] = *(--ch);
					temp[1] = '\0';
					token.cifadanyuan = temp;
					token.cisu = temp;
			}
		}else if(*ch == '<' || *ch == '>' || *ch == '!'){
			if(*(ch + 1) == '='){
				temp[0] = *ch;
				temp[1] = *(++ch);
				temp[2] = '\0';
			}else{
				temp[0] = *ch;
				temp[1] = '\0';
			}
			token.cifadanyuan = temp;
			token.cisu = temp;
		}else{
			temp[0] = *ch;
			temp[1] = '\0';
			token.cifadanyuan = temp;
			token.cisu = temp;
		}
		ch++;
		return token;
	}//分析运算符的函数，顺便吞掉注释
	Token lexIdentifier(){
		Token token;
		int i;
		for(i = 0;isalpha(*ch);ch++) {
			temp[i++] = *ch;
		}
		temp[i] = '\0';
		string s = (string)temp;
		transform(s.begin(), s.end(), s.begin(),::tolower);
		if(keyword.find(s) != keyword.end()){
			token.cifadanyuan = s;
			token.cisu = s;
		}else{
			token.cifadanyuan = "ID";
			token.cisu = temp;
		}
		return token;
	}//分析保留字和用户自定义标识符的函数
	Token lexDigit(){
		int i;
		int flag = 1; 
		Token token;
		for(i = 0; isdigit(*ch) || *ch == '.'; ch++){
			if(*ch == '.'){
				if(flag){ flag = 0;	} else { break; }	
			}
			temp[i++] = *ch;
		}
		temp[i] = '\0';
		token.cifadanyuan = "NUM";
		token.cisu = temp;
		return token;
	}//分析整数和小数的函数
	Token lexString(){
		Token token;
		int i;
		for(i = 0,ch++; *ch != '\'' && *ch != '\0'; ch++){
			temp[i++] = *ch;
		}
		temp[i] = '\0';
		token.cifadanyuan = "STRING";
		token.cisu = temp;
		ch++;
		return token;
	}//分析字符串的函数
	Token lexSingleword(){
		temp[0] = *(ch++);
		temp[1] = '\0';
		Token token(temp,temp);
		return token;
	}//分析括号，分号等等的函数
	Lexical(char* ch){ this->ch = ch; } //分析器的构造函数
	vector<Token> lexSource(){
		vector<Token> result;
		while(*ch != '\0'){
			while(isspace(*ch)) ch++;
			if(isalpha(*ch)) {
				result.push_back(lexIdentifier());
			}else if(isdigit(*ch)) {
				result.push_back(lexDigit());
			}else if(operators.find(*ch) != string::npos) {
				result.push_back(lexOperators());
			}else if(*ch =='\'') {
				result.push_back(lexString());
			}else if(singleword.find(*ch) != string::npos) {
				result.push_back(lexSingleword());
			}
		}
		return result;
	}//利用上述函数分析整个sql语句
}

int main(){
	char* ch = "select * from test";
	Lexical lex(ch);
	vector<Token> s = lex.lexSource();
	Token t;
	for(int i = 0; i < s.size(); i++){
		t = s[i];
		cout<<t.toString()<<"\n";
	}
	return 0;
}
