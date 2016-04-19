#include <bits/stdc++.h>
using namespace std;

ifstream fin ("input5.txt");

map<string,pair<string,string> > GRAMMER;
map<pair<string,string>,string> ACTION;
map<pair<string,string>,string> GOTO;


int main(){
	// taking grammer as input
	string x,y,z;
	int gr;
	fin>>gr;
	while(gr--){
		fin>>x>>y>>z;
		GRAMMER[x]=make_pair(y,z);
	}

	// action table
	int act;
	fin>>act;
	while(act--){
		fin>>x>>y>>z;
		ACTION[make_pair(x,y)]=z;
	}

	// goto table
	int got;
	fin>>got;
	while(got--){
		fin>>x>>y>>z;
		GOTO[make_pair(x,y)]=z;
	}

	stack<string> stk;
	string s;
	//cout<<"Enter input string: ";cin>>s;
	s="i+i+i";
	s += "$";

	bool flag=true;
	string action;//stores the action
	string lhgr,rhgr;
	stk.push("0");
	string p;
	//p is input pointer
	// x is stack top
	p=" ";
	int ptr=0;
	while(1){
		x=stk.top();
		p[0] = s[ptr];
		action=ACTION[make_pair(x,p)];
		if(action==""){
			cout<<"Error at: "<<x<<" "<<p<<endl;
			flag=false;
			break;
		}
		if(action[0]=='a'){
			break;
		}
		else if(action[0]=='s'){
			stk.push(p);
			stk.push(action.substr(1,action.size()-1));
			ptr++;

			cout<<"Pushing: "<<p<<" on stack"<<endl;
		}
		else{
			lhgr = GRAMMER[action.substr(1,action.size()-1)].first;
			rhgr = GRAMMER[action.substr(1,action.size()-1)].second;
			cout<<"Reducing using : "<<lhgr<<"->"<<rhgr<<endl;
			int n = 2*rhgr.size();
			while(n--){
				stk.pop();
			}
			x = stk.top();
			stk.push(lhgr);
			stk.push(GOTO[make_pair(x,lhgr)]);
		}
	}

	if(flag) cout<<"accepted..."<<endl;
	else cout<<"rejected..."<<endl;
}