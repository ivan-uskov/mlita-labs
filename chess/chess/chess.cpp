#include "stdafx.h"
#include "Reader.h"
#include "Data.h"
#include "KillCache.h"

using namespace std;

struct SearchState
{
    Point p;
    size_t steps = 0;
    size_t enemies;
};

void iterateQueue(Data & d, queue<SearchState> & q)
{
    //bool process = true;
    //while (process)
    //{
        //auto currElt = q.front();
        //q.pop();
        //auto closest = d.getClosest(currElt.p);
        //for (auto p : closest)
        //{

        //}
    //}
}

void printStepsCount(Data & d, ostream & out)
{
    KillCache cache(d);
    queue<SearchState> q;
    q.push({ d.start, 0, Utils::toBinarySet(d.enemies.size()) });
    iterateQueue(d, q);
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto d = Reader::readData(in);
        printStepsCount(d, out);
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

