#include "SearchEngine.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

SearchEngine::SearchEngine() : totaldoc(0) {
    irrelevent = {"of", "the", "is", "are", "and"};
}

int SearchEngine:: mindistance(const string &s1, const string &s2)
    {
       int m = s1.length(), n = s2.length();
    
    
    if (m < n) return mindistance(s2, s1);

    vector<int> dp(n + 1);

   
    for (int j = 0; j <= n; j++) 
    dp[j] = j;

    for (int i = 1; i <= m; i++)
     {

        int prevdiagonal = dp[0]; 
        dp[0] = i;              
        
        for (int j = 1; j <= n; j++)
         {
            int temp = dp[j];   
            
            if (s1[i - 1] == s2[j - 1])
            {
                dp[j] = prevdiagonal;
            } 
            else 
            {
                
                dp[j] = 1 + min({dp[j], dp[j - 1], prevdiagonal});
            }
            prevdiagonal = temp; 
        }
    }
    return dp[n];
}

 void SearchEngine:: scan_directory()
    {
        files.clear();
        if (!fs::exists("data"))
        {
            return;
        }
        for (auto &datafile : fs::directory_iterator("data"))
        {
            if (datafile.path().extension() == ".txt")
            {
                files.push_back(datafile.path().string());
            }
        }
    }

 bool SearchEngine:: comp(pair<string, double> a, pair<string, double> b)
    {
        if (a.second > b.second)
        {
            return true;
        }
        return false;
    }
    void SearchEngine:: indexsize(set<string> &docs)
    {
        for (auto &p : index)
        {
            for (auto &q : p.second)
            {
                docs.insert(q.first);
            }
        }
    }

    void SearchEngine::initialize()
    {
        scan_directory();

        if (fs::exists("index.txt"))
        {
            load_index();
        }

        if (fs::exists("timestamp.txt"))
        {
            load_timestamp();
        }

        bool indexchange = false;
        vector<string> filestoupdate;

        for (auto &filename : files)
        {
            long long currenttime = fs::last_write_time(filename).time_since_epoch().count();

            if (filetimestamps.find(filename) == filetimestamps.end() || currenttime != filetimestamps[filename])
            {
                cout << "updating file " << filename << endl;
                remove_oldfile(filename);
                // build_index_newfile(filename);
                filestoupdate.push_back(filename);

                filetimestamps[filename] = currenttime;
                indexchange = true;
            }
        }

        // mutithreading in batches

        if (!filestoupdate.empty())
        {
            int num_threads = thread::hardware_concurrency(); // knowing about hardware  // if it fails or oncomputable returns 0
            if (num_threads == 0)                             // safety ..if doesn't know we are using 2
                num_threads = 2;

            vector<vector<string>> batches(num_threads);
            for (int i = 0; i < filestoupdate.size(); i++)
            {
                batches[i % num_threads].push_back(filestoupdate[i]);
            }

            vector<std::thread> threads;
            for (int i = 0; i < num_threads; i++)
            {
                if (!batches[i].empty())
                {
                    threads.push_back(thread(&SearchEngine::build_index_batch, this, batches[i]));
                }
            }

            for (auto &t : threads)
            {
                if (t.joinable())
                {
                    t.join();
                }
            }
        }

        for (auto it = filetimestamps.begin(); it != filetimestamps.end();)
        {
            if (find(files.begin(), files.end(), it->first) == files.end())
            {
                remove_oldfile(it->first);
                it = filetimestamps.erase(it);
                indexchange = true;
            }
            else
            {
                it++;
            }
        }

        if (indexchange)
        {
            save_index();
            save_timestamp();
        }

        set<string> docs;
        indexsize(docs); // indexsize function will push file names from existing index

        totaldoc = docs.size();
    }

    string SearchEngine:: fuzzysearch(string &q)
    {
         string close="";
        int qlen=q.length();
         int minm=4;
        for(auto it:index)
        {
           
            int len=it.first.length();
            int diff=abs(qlen-len);
            if(diff>3)
            {
                continue;
            }
           
            int chardiff=mindistance(it.first,q);
            if(chardiff<minm)
            {
                minm=chardiff;
                close=it.first;
            }
            if(minm<=1)
            {
                break;
            }
           
        }
        return close;
    }

    void SearchEngine:: save_timestamp()
    {
        ofstream fout("timestamp.txt");
        for (auto &filename : files)
        {
            auto t = fs::last_write_time(filename);

            auto count = t.time_since_epoch().count();

            fout << filename << " " << count << "\n";
        }
    }

    void SearchEngine:: load_timestamp()
    {
        ifstream fin("timestamp.txt");
        if (!fin)
        {
            return;
        }

        filetimestamps.clear();
        string filename;
        long long timestamp;

        while (fin >> filename >> timestamp)

        {
            filetimestamps[filename] = timestamp;
        }
        fin.close();
    }

    void SearchEngine:: save_index()
    {
        ofstream fout("index.txt");

        if (!fout)
        {
            cout << "Error: Could not save index\n";
            return;
        }
        for (auto &idx : index)
        {

            fout << idx.first << " ";
            for (auto &sec : idx.second)
            {
                fout << sec.first << ":" << sec.second << " ";
            }

            fout << "\n";
        }
    }

    void SearchEngine:: remove_oldfile(string filename)
    {
        for (auto it = index.begin(); it != index.end();)
        {
            it->second.erase(filename);

            if (it->second.empty())
            {
                it = index.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    void SearchEngine:: build_index_batch(const vector<string> &filesupdate)
    {
        for (auto filename : filesupdate)
        {
            ifstream fin;
            fin.open(filename);
            if (!fin)
            {
                cout << "error opening file\n";
                return;
            }

            string line;
            while (getline(fin, line))
            {
                string clean = "";
                for (int i = 0; i < line.length(); i++)
                {
                    if (isalnum(line[i]) || line[i] == ' ')
                    {
                        clean += tolower(line[i]);
                    }
                    else
                    {
                        clean += ' ';
                    }
                }
                stringstream ss(clean);
                string word;
                while (ss >> word)
                {
                    if (irrelevent.find(word) != irrelevent.end())
                    {
                        continue;
                    }
                    lock_guard<mutex> lock(mlock);
                    index[word][filename]++;
                }
            }
            fin.close();
        }
    }

    void SearchEngine:: load_index()
    {

        ifstream fin("index.txt");
        if (!fin)
        {
            return;
        }

        index.clear();
        string l;
        while (getline(fin, l))
        {
            stringstream ss(l);
            string wd;
            ss >> wd;
            string te = wd;
            while (ss >> wd)
            {
                int pos = wd.find(':');
                if (pos == string::npos)
                {
                    continue;
                }
                string file = wd.substr(0, pos);
                int freq = stoi(wd.substr(pos + 1));
                index[te][file] = freq;
            }
        }
        fin.close();
    }

    void SearchEngine:: search(string query)
    {

        string clean_query = "";

        for (auto &q : query)
        {
            if (isalnum(q) || q == ' ')
            {
                clean_query += tolower(q);
            }
            else
            {
                clean_query += " ";
            }
        }

        // now query is converted into tokens of words

        vector<string> token;
        stringstream s(clean_query);
        string to;
        while (s >> to)
        {
            if (irrelevent.find(to) != irrelevent.end())
            {
                continue;
            }
            token.push_back(to);
        }

        if (token.empty())
        {
            cout << "no valid query \n";
            return;
        }

        // idf storage

        unordered_map<string, double> tokenidf;
        for (auto &st : token)
        {
            if (index.find(st) != index.end())
            {
                tokenidf[st] = (log(1.00 + (double)totaldoc / index[st].size()));
            }
            else
            {
                tokenidf[st] = 0;
            }
        }

        // file scores calculation

        unordered_map<string, double> file_score;

        for (auto &st : token)
        {
            if (index.find(st) != index.end())
            {
                for (auto &p : index[st])
                {

                    double tf = 1.00 + log10(p.second);
                    file_score[p.first] += tf * tokenidf[st];
                }
            }
        }

        // storing files with rank

        vector<pair<string, double>> filerank;
        for (auto &p : file_score)
        {
            filerank.push_back({p.first, p.second});
        }
        sort(filerank.begin(), filerank.end(), comp);

        cout << fixed << setprecision(2);
        int cnt = 0;
        for (auto &p : filerank)
        {
            if (cnt >= 5)
                break;
            cnt++;
            //cout << cnt << ". " << p.first << " (Rank: " << p.second << ")" << endl;
            cout << CYAN << cnt << ". " << RESET << p.first << GREEN << " (Score: " << p.second << ")" << RESET << endl;

            ifstream file(p.first);
            if (file)
            {
                string line;
                while (getline(file, line))
                {
                    string lower_line = line;
                    for (char &c : lower_line)
                        c = tolower(c);

                    if (lower_line.find(token[0]) != string::npos)
                    {
                        //cout << "   Snippet: ... " << line << " ..." << endl;
                        cout << "   Snippet: ... " << YELLOW << line << RESET << " ..." << endl;
                        break;
                    }
                }
            }
            cout << endl;
        }

        if(filerank.size()==0)
        {
            string corrected="";
            for(auto &it:token)
            {
                string closef=fuzzysearch(it);
                if(closef.length()>0)
                {
                   corrected+=closef+" ";
                }
            }
            if(corrected!=query)
            {
                //cout<<"Did you mean : " << corrected<<"?"<<endl;
                cout << YELLOW << "Did you mean: " << RESET << corrected << "?" << endl;
            }
        }
        
    }

