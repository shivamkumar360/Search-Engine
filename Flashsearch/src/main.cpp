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
        engine.search(q);
    }
    return 0;
}