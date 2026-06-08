#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <mutex>
#include <filesystem>
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