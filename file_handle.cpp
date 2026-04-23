#include<iostream>
#include<fstream>
#include<sstream>
#include <unordered_map>
#include <set>
#include<cctype>
#include <vector>
#include<algorithm>
#include <iomanip>
#include<cmath>
using namespace std;

bool comp(pair<string,double>a,pair<string,double>b)
{
    if(a.second>b.second)
    {
        return true;
    }
    return false;
}
int main()
{
    set<string>irrelevent={"of","the","is","and"};
    string query;
    cout<<"enter the query\n";
    getline(cin, query);
    string clean_query="";
    for(auto& q:query)
    {
        if(isalnum(q) || q==' ')
        {
            clean_query+=tolower(q);
        }
        else
        {
            clean_query+=" ";
        }
    }
    vector<string>token;
    stringstream s(clean_query);
    string to;
    while(s>>to)
    {
        if(irrelevent.find(to)!=irrelevent.end())
        {
            continue;
        }
        token.push_back(to);
    }

    if(token.empty())
{
    cout << "no valid query \n";
    return 0;
}


    vector<string>files={"data.txt","data2.txt"};
    int totaldoc=files.size();
    unordered_map<string,unordered_map<string,int>>index;
    for(auto &c:files)
    {
    ifstream fin;
    fin.open(c);
    if(!fin)
    {
        cout<<"error opening file\n";
        return 1;
    }
    string line;
    while(getline(fin,line))
    {
        string clean="";
        for(int i=0;i<line.length();i++)
        {
            if(isalnum(line[i]) || line[i]==' ')
            {
                clean+=tolower(line[i]);
            }
            else
            {
                clean+=' ';
            }
        }
        stringstream ss(clean);
        string word;
        while(ss>>word)
        {
            if(irrelevent.find(word)!=irrelevent.end())
        {
            continue;
        }
            index[word][c]++;
        }
    }
    
    fin.close();
}
unordered_map<string, double>tokenidf;
for(auto& st:token)
{
    if(index.find(st)!=index.end())
    {
        tokenidf[st]=(log(1.00+(double)totaldoc/index[st].size()));
    }
    else
    {
         tokenidf[st]=0;
    }
}

unordered_map<string,double>file_score;

for(auto& st:token)
{
    if(index.find(st)!=index.end())
    {
    for(auto& p:index[st])
    {
        file_score[p.first]+=p.second*tokenidf[st];
}
}
}

vector<pair<string,double>>filerank;
for(auto &p : file_score)
{
    filerank.push_back({p.first,p.second});
}
sort(filerank.begin(),filerank.end(),comp);
cout << fixed << setprecision(2);
int cnt=0;
for(auto &p : filerank)
{
    if(cnt>=5)
    {
        break;
    }
    cnt++;
    cout<<p.first<<"-> "<<p.second<<endl;
}

    return 0;
}