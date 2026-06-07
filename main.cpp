#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <set>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class SearchEngine
{
private:
    unordered_map<string, unordered_map<string, int>> index;
    unordered_map<string, long long> filetimestamps;
    set<string> irrelevent = {"of", "the", "is", "are", "and"};
    vector<string> files;
    int totaldoc = 0;
    bool build = true;

    void scan_directory()
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

    static bool comp(pair<string, double> a, pair<string, double> b)
    {
        if (a.second > b.second)
        {
            return true;
        }
        return false;
    }
    void indexsize(set<string> &docs)
    {
        for (auto &p : index)
        {
            for (auto &q : p.second)
            {
                docs.insert(q.first);
            }
        }
    }

public:
    void initialize()
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

        for (auto &filename : files)
        {
            long long currenttime = fs::last_write_time(filename).time_since_epoch().count();

            if (filetimestamps.find(filename) == filetimestamps.end() || currenttime != filetimestamps[filename])
            {
                cout << "updating file " << filename << endl;
                remove_oldfile(filename);
                build_index_newfile(filename);
                filetimestamps[filename] = currenttime;
                indexchange = true;
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

    void save_timestamp()
    {
        ofstream fout("timestamp.txt");
        for (auto &filename : files)
        {
            auto t = fs::last_write_time(filename);

            auto count = t.time_since_epoch().count();

            fout << filename << " " << count << "\n";
        }
    }

    void load_timestamp()
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

    void save_index()
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
    void remove_oldfile(string filename)
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

    void build_index_newfile(string filename)
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
                index[word][filename]++;
            }
        }
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

    void search(string query)
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
            cout << cnt << ". " << p.first << " (Rank: " << p.second << ")" << endl;

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
                        cout << "   Snippet: ... " << line << " ..." << endl;
                        break;
                    }
                }
            }
            cout << endl;
        }
    }
};

int main()
{
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