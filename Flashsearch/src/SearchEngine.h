#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <mutex>
#include <filesystem>

#define RESET   "\033[0m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define GRAY    "\033[1;30m"

using namespace std;

class SearchEngine {

private:
    unordered_map<string, unordered_map<string, int>> index;
    unordered_map<string, long long> filetimestamps;
    set<string> irrelevent ;
    vector<string> files;
    int totaldoc = 0;
    mutex mlock;
    

    int mindistance(const string &s1, const string &s2);
    void scan_directory();
    static bool comp(pair<string, double> a, pair<string, double> b);
    void indexsize(set<string> &docs);

public:
    
    SearchEngine();
    void initialize();
    string fuzzysearch(string &q);
    void save_timestamp();
    void load_timestamp();
    void save_index();
    void remove_oldfile(string filename);
    void build_index_batch(const vector<string> &filesupdate);
    void load_index();
    void search(string query);

};