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

class SearchEngine
{
    private:
    unordered_map<string,unordered_map<string,int>>index;
    set<string>irrelevent={"of","the","is","are","and"};
    vector<string>files;
    int totaldoc=0;
    bool build=true;


    void scan_directory() {
        files.clear();
        if (!fs::exists("data")) 
        {
        return;
        }
        for (auto& datafile : fs::directory_iterator("data"))
        {
            if (datafile.path().extension() == ".txt")
            {
                files.push_back(datafile.path().string());
            }
        }
    }




    static bool comp(pair<string,double>a,pair<string,double>b)
{
    if(a.second>b.second)
    {
        return true;
    }
    return false;
}
void indexsize(set<string>& docs)
{
    for(auto &p : index)
{
    for(auto &q : p.second)
    {
        docs.insert(q.first);
    }
}
}

public:

void initialize() 
{
        scan_directory(); 
        
        if (fs::exists("index.txt")) {
            load_index();
        }

        // If  files changed, rebuilding of  index will take place

        set<string> docs;
        indexsize(docs);    // indexsize function will push file names from existing index 
        if (docs.size() != files.size()) // if no of files not  matched rebuilding will take place 
        {
            cout << "Index outdated or missing. Building now...\n";
            build_index();
        } 
        else 
        {
            cout << "Index loaded successfully from file.\n";
            totaldoc = docs.size();
        }
    }




    void build_index()
    {

        totaldoc = files.size();
        ofstream fout("index.txt");

        for(auto &c:files)
    {
    ifstream fin;
    fin.open(c);
    if(!fin)
    {
        cout<<"error opening file\n";
        return ;
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
    
    
}

for (auto& idx : index)
{
    fout << idx.first << " ";
    for (auto& sec : idx.second) 
    {
        fout << sec.first << ":" << sec.second << " ";
    }

            
    fout << "\n";

    }
    fout.close();

}

    void load_index()
    {

        ifstream fin("index.txt");
        if (!fin)
        {
         return;
        }

        index.clear();
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
        fin.close();
    }
      



    void search(string query)
    {

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
    return ;
}


    // idf storage


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

// file scores calculation


unordered_map<string,double>file_score;

for(auto& st:token)
{
    if(index.find(st)!=index.end())
    {
    for(auto& p:index[st])
    {

        double tf = 1.00 + log10(p.second); 
        file_score[p.first] += tf * tokenidf[st];
       
}
}
}



// storing files with rank


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







}

};

int main() {
    SearchEngine engine;
    engine.initialize(); 

    string q;
    while (true) 
    {
        cout << "\nEnter Query (or 'exit'): ";
        getline(cin, q);
        if (q == "exit") 
        {
        break;
        }
        engine.search(q);
    }
    return 0;
}