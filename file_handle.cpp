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
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

bool comp(pair<string,double>a,pair<string,double>b)
{
    if(a.second>b.second)
    {
        return true;
    }
    return false;
}
void indexsize(set<string>& docs,const unordered_map<string,unordered_map<string,int>>&index)
{
    for(auto &p : index)
{
    for(auto &q : p.second)
    {
        docs.insert(q.first);
    }
}

}
int main()
{
    // irrelevent words
    set<string>irrelevent={"of","the","is","are","and"};

    // index storage part

    vector<string>files;
    int totaldoc=0;
    if(!fs::exists("data"))
    {
        cout << "data folder not found\n";
        return 0;
    }
    for(auto& datafile:fs::directory_iterator("data"))
    {
        if(datafile.path().extension()==".txt")
        {
            files.push_back(datafile.path().string());
        }
    }
   
    unordered_map<string,unordered_map<string,int>>index;
    bool build=true;

    // loading or storing index
    ifstream fin("index.txt");
    if(fin)
    {
        build=false;
        string l;
        while(getline(fin,l))
        {
        stringstream ss(l);
        string wd;
        ss>>wd;
        string te=wd;
        while(ss>>wd)
        {
           int pos=wd.find(':');
           if(pos == string::npos)
           {
            continue;
           }
           string file=wd.substr(0,pos);
           int freq=stoi(wd.substr(pos + 1));
           index[te][file]=freq;
        }

        }
        set<string> docs;
        indexsize(docs,index);

if(docs.size()!=files.size())
{
    build=true;
    index.clear();
}
else
    {
        totaldoc = docs.size();
    }
        fin.close();
    }
    if(build)
    {
        totaldoc=files.size();
        ofstream fout("index.txt");
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

    for(auto& idx:index)
    {
        fout<<idx.first<<" ";
        for(auto& sec:idx.second)
        {
            fout << sec.first << ":" << sec.second << " ";
        }
        fout<<"\n";
    }
    fout.close();
    }

if(totaldoc == 0)
{
    cout << "No documents indexed\n";
    return 0;
}


    // query part ahead

    // query string 
    string query;
    cout<<"enter the query\n";
    getline(cin, query);
    string clean_query="";


    // query is cleaned here
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

    // now query is converted into tokens of words 
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