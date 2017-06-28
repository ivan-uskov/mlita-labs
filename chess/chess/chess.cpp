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

using GoCache = Matrix3D<bool, false>;

long long int iterateQueue(Data & d, queue<SearchState> & q, const KillCache & killCache)
{
    GoCache goCache(d.width(), d.height(), size_t(std::pow(size_t(2), d.enemies.size())));
    goCache(q.front().p.x, q.front().p.y, q.front().enemies) = true;
    bool process = true;
    while (process && (q.size() > 0))
    {
        auto s = q.front();
        q.pop();
        auto closest = d.getClosest(s.p);
        for (auto p : closest)
        {
            if (goCache(p.x, p.y, s.enemies))
            {
                // point already visited
                continue;
            }

            auto newSteps = s.steps + 1;
            if (isEnemy(d(p).type))
            {
                auto newEnemies = (s.enemies & (~Utils::makeBinary(d(p).id)));
                if (killCache(p.x, p.y, newEnemies))
                {
                    continue;
                }
                if (newEnemies == 0)
                {
                    return newSteps;
                }
                q.push({ p, newSteps, newEnemies });
                goCache(p.x, p.y, newEnemies) = true;
            }
            else if (!killCache(p.x, p.y, s.enemies))
            {
                q.push({ p, newSteps, s.enemies });
                goCache(p.x, p.y, s.enemies) = true;
            }
        }
    }

    return -1;
}

long long int getStepsCount(Data & d, ostream & out)
{
    if (d.enemies.size() == 0)
    {
        return 0;
    }

    KillCache cache(d);
    queue<SearchState> q;
    q.push({ d.start, 0, Utils::toBinarySet(d.enemies.size()) });
    return iterateQueue(d, q, cache);
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto d = Reader::readData(in);
        out << getStepsCount(d, out) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

