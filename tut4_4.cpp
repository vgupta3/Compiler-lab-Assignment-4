#include <bits/stdc++.h>
using namespace std;

ifstream fin ("input4.txt");
map<pair<char,char>,char> OPM;

int main(){
	int op;
	fin>>op;
	char x,y,z;
	while(op--){
		fin>>x>>y>>z;
		OPM[make_pair(x,y)]=z;
	}
	string s;
	cout<<"Enter string to be parsed: ";
	cin>>s;
	s+="$";

	bool flag=true;
	stack<char> stk;
	char a,sym;
	stk.push('$');
	int ptr=0;
	while(1){
		x=stk.top();
		a=s[ptr];
		// cout<<x<<" "<<a<<endl;
		if(x=='$' && a=='$'){
			break;
		}
		else{
			sym=OPM[make_pair(x,a)];
			cout<<sym<<endl;
			if(sym=='e'){
				flag=false;
				break;
			}
			else if(sym=='>'){
				do{
					if(!stk.empty()) stk.pop();
					else flag=false;
				}while(stk.top()!='<' && flag);
				stk.pop();
				cout<<"reduce"<<endl;
			}
			else{
				ptr++;
				stk.push(sym);
				stk.push(a);
				cout<<"shift"<<endl;
			}
		}
	}
	if(flag) cout<<"accepted...";
	else cout<<"rejected..";
}
