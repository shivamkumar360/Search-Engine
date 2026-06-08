#include "SearchEngine.h"

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
        auto start = chrono::high_resolution_clock::now();
        engine.search(q);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> diff = end - start;
        cout << "SEARCH LATENCY: " << diff.count() << " ms" << endl;
    }
    return 0;
}