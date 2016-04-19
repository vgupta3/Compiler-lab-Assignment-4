#include <bits/stdc++.h>
using namespace std;

/*
grammer
8
E=Te
e=+Te
e=@
T=Ft
t=*Ft
t=@
F=(E)
F=i
*/
ifstream fin ("input123.txt");
map<char,set<char> > first;
map<char,set<char> > follow;
map<char,vector<string> > CFG;
map<char,bool> nonTerminal;
map<pair<char,char>,string> PT;
char start;

void FIRST(){
	bool flag=true;
	while(flag){
		flag=false;
		for(map<char,vector<string> >::iterator it=CFG.begin();it!=CFG.end();it++){
			char V=it->first;
			string s;
			for(int i=0;i< it->second.size();i++){
				s=it->second[i];
				//null production
				if(s=="@"){
					if(first[V].find('@') == first[V].end()){
						first[V].insert('@');
						flag=true;
					}
					continue;
				}
				int j=0;
				bool epsilon;
				while(j<s.size()){
					if(nonTerminal[s[j]]){
						//if non terminal
						epsilon=false;
						set<char>::iterator it1=first[s[j]].begin();
						while(it1 != first[s[j]].end()){
							//has null production
							if(*it1=='@'){
								epsilon=true;
								it1++;
								continue;
							}
							if(first[V].find(*it1) == first[V].end()){
								flag=true;
								first[V].insert(*it1);
							}
							it1++;
						}
						if(epsilon) j++;
						else break;
					}
					else{
						//if s[j] terminal then insert s[j]
						if(first[V].find(s[j]) == first[V].end()){
							first[V].insert(s[j]);
							flag=true;
						}
						epsilon=false;
						break;
					}
				}
				if(epsilon){
					if(first[V].find('@') == first[V].end()){
						first[V].insert('@');
						flag=true;
					}
				}
			}
		}
	}
	cout<<endl<<"First set"<<endl;
	for(map<char,set<char> >::iterator it=first.begin();it!=first.end();it++){
		cout<<it->first<<" = ";
		for(set<char>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
			cout<<*it2<<" ";
		cout<<endl;
	}
}

void FOLLOW(){
	follow[start].insert('$');
	bool flag=true;
	while(flag){
		flag=false;
		for(map<char,vector<string> >::iterator it=CFG.begin();it!=CFG.end();it++){
			char V=it->first;
			string s;
			for(int i=0;i<it->second.size();i++){
				s=it->second[i];
				if(s=="@") continue;
				int j=0;
				while(j<s.size()){
 					if(nonTerminal[s[j]]){
 						//s[j] is non terminal
						bool epsilon=false;
						if(j+1<s.size()){
							//s[j+1] is terminal   follow of s[j] is not calculated or if calculated s[j+1] is not in it
							if(!nonTerminal[s[j+1]] && (follow.find(s[j]) == follow.end() ||
									follow[s[j]].find(s[j+1]) == follow[s[j]].end())){
								flag=true;
								follow[s[j]].insert(s[j+1]);
								j++;
								continue;
							}
							//s[j+1] is non terminal
							for(set<char>::iterator it2=first[s[j+1]].begin();it2!=first[s[j+1]].end();it2++){
								// s[j+1] ka first set s[j] mei add karna hai except epsilon
								if(*it2=='@'){
									epsilon=true;
									continue;
								}
								if(follow.find(s[j]) == follow.end() || follow[s[j]].find(*it2) == follow[s[j]].end()){
									flag=true;
									follow[s[j]].insert(*it2);
								}
							}
						}
						if(j == s.length()-1 || epsilon){
							for(set<char>::iterator it3=follow[V].begin();it3!=follow[V].end();it3++){
							//s[j] ka calculate nahi kiya hai  or  agar kiya hai toh *it3 nahi hai
								if(follow.find(s[j]) == follow.end() ||
									 follow[s[j]].find(*it3) == follow[s[j]].end()){
									flag=true;
									follow[s[j]].insert(*it3);
								}
							}
						}
						j++;
					}
 					else{
						j++;
						continue;
					}
				}
			}
		}
	}
	cout<<endl<<"Follow set"<<endl;
	for(map<char,set<char> >::iterator it=follow.begin();it!=follow.end();it++){
		cout<<it->first<<" = ";
		for(set<char>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
			cout<<*it2<<" ";
		cout<<endl;
	}
}

set<char> forstring(string s){
	set<char> x;
	int j=0;
	bool epsilon;
	while(j<s.size()){
		if(nonTerminal[s[j]]){
			//if non terminal
			epsilon=false;
			set<char>::iterator it=first[s[j]].begin();
			while(it != first[s[j]].end()){
				//has null production
				if(*it == '@'){
					epsilon=true;
					it++;
					continue;
				}
				x.insert(*it);
				it++;
			}
			if(epsilon) j++;
			else break;
		}
		else{
			x.insert(s[j]);
			epsilon=false;
			break;
		}
	}
	if(epsilon){
		x.insert('@');
	}

	/*cout<<"First set of string:"<<s<<endl;
	for(set<char>::iterator it=x.begin();it!=x.end();it++) cout<<*it<<" ";
	cout<<endl;*/
	return x;
}

void PARSING_TABLE(){
	set<char> first_temp;
	for(map<char,vector<string> >::iterator it=CFG.begin();it!=CFG.end();it++){
		char V=it->first;
		string s;
		bool epsilon,endmarker;
		for(int i=0;i< it->second.size();i++){
			s=it->second[i];
			first_temp = forstring(s);
			epsilon=false;
			endmarker=false;
			for(set<char>::iterator it2=first_temp.begin();it2!=first_temp.end();it2++){
				if(*it2 == '@'){
					epsilon=true;
					continue;
				}
				else{
					PT[make_pair(V,*it2)]=s;
				}
			}
			if(epsilon){
				for(set<char>::iterator it2=follow[V].begin();it2!=follow[V].end();it2++){
					if(*it2 == '$'){
						endmarker=true;
						continue;
					}
					else{
						PT[make_pair(V,*it2)]=s;
					}
				}
			}
			if(endmarker && epsilon) PT[make_pair(V,'$')]=s;
		}
	}
	cout<<endl<<"Parsing table"<<endl;
	for(map<pair<char,char>,string>::iterator it=PT.begin();it!=PT.end();it++)
		cout<<it->first.first<<" "<<it->first.second<<" : "<<it->second<<endl;
}

void PARSE(string s){
	s=s+"$";
	stack<char> stk;
	stk.push('$');
	stk.push(start);
	bool ans=true;
	int ptr=0;
	char x,a;
	do{
		x=stk.top();
		a=s[ptr];
		if(nonTerminal[x]){
			if(PT.find(make_pair(x,a))!=PT.end()){
				stk.pop();
				string prod = PT[make_pair(x,a)];
				if(prod == "@") continue;
				cout<<"push...."<<x<<"->"<<prod<<endl;
				int j=prod.length()-1;
				while(j>=0){
					stk.push(prod[j]);
					j--;
				}
			}
			else{
				ans=false;
				cout<<"Error..... production not found"<<endl;
				break;
			}
		}
		else{
			if(x==a){
				stk.pop();
				ptr++;
			}
			else{
				ans=false;
				cout<<"Error..... terminals not matched"<<endl;
				break;
			}
		}
	}while(stk.top()!='$');
	if(ans) cout<<"Parsed..."<<endl;
}

int main(){
	int production;//number of production
	//cin>>production;
	fin>>production;
	string s;
	while(production--){
		//cin>>s;
		fin>>s;
		cout<<s<<endl;
		nonTerminal[s[0]]=true;
		CFG[s[0]].push_back(s.substr(2,s.size()-2));
	}
	cout<<endl<<"Enter starting symbol: ";
	cin>>start;
    FIRST();
    FOLLOW();
    PARSING_TABLE();
    cout<<"String to be parsed: "<<endl;
    cin>>s;
    PARSE(s);
}
