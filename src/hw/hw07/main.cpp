#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
class CContest
{
public:
    // default constructor
    CContest() = default;
    // destructor
    ~CContest() = default;
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch ( const string & contestant1, const string & contestant2, const M_ & result )
    {
        auto it = find_if ( m_Matchups.begin(), m_Matchups.end(), [ & ] ( const CMatchup & x )
        {
            return ( x . m_contestant1 == contestant1 && x . m_contestant2 == contestant2 ) ||
                   ( x . m_contestant1 == contestant2 && x . m_contestant2 == contestant1 );
        } );
        if ( it != m_Matchups.end() )
            throw logic_error ( "Match already exists!" );
        m_Matchups.emplace_back ( contestant1, contestant2, result );
        return * this;
    }

    vector<pair<string,int>> findWins ( const function<int ( const M_ & )> & comparator ) const{
        map<string, set<string>> winnersCount;
        for ( const auto & x : m_Matchups ){
            winnersCount[x.m_contestant1];
            winnersCount[x.m_contestant2];
        }

        set<string> visited;
        deque<string> q;

        q.push_back(m_Matchups[0].m_contestant1);
        visited.insert(m_Matchups[0].m_contestant1);

        while (!q.empty()) {
            string pos = q.front();
            q.pop_front();

            for (const auto &x: m_Matchups) {
                if (x.m_contestant1 == pos || x.m_contestant2 == pos) {
                    string other = (x.m_contestant1 == pos) ? x.m_contestant2 : x.m_contestant1;
                    int res = (x.m_contestant1 == pos) ? comparator(x.m_Result) : -comparator(x.m_Result);
                    if (visited.find(other) == visited.end()) {
                        visited.insert(other);
                        q.push_front(other);
                    }
                    if (res == 0)
                        throw logic_error("Result can not be determined!");
                    if (res > 0) {
                        winnersCount[pos].insert(other);
                        for (const auto &y: winnersCount) {
                            if (y.second.find(pos) != y.second.end()) {
                                winnersCount[y.first].insert(other);
                            }
                        }
                    }
                }
            }
        }

        vector<pair<string,int>> winsByContestant;
        set<int> checkDuplicates;
        int wins = 0;
        for (const auto& entry : winnersCount) {
            wins = (int)entry.second.size();
            if ( checkDuplicates.count ( wins ) == 1)
                throw logic_error ( "Result can not be determined!" );
            checkDuplicates.insert ( wins );
            auto it = lower_bound(winsByContestant.begin(), winsByContestant.end(), make_pair(entry.first, wins), [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second > b.second;
            });
            winsByContestant.insert(it, make_pair(entry.first, wins));
        }
        return winsByContestant;
    }

    bool isOrdered ( const function<int ( const M_ & )> & comparator ) const
    {
        try {
            findWins(comparator);
        } catch (const logic_error& e) {
            return false;
        }
        vector<pair<string,int>> winsByContestant = findWins(comparator);
        return true;
    }
    // results ( comparator )
    list<string> results ( const function<int ( const M_ & )> & comparator ) const
    {
        vector<pair<string,int>> winsByContestant = findWins(comparator);
        list<string> result;
        for ( const auto & x : winsByContestant ){
            result.push_back( x.first );
        }
        return result;
    }
private:
    struct CMatchup
    {
    public:
        // default constructor
        CMatchup() = default;
        // constructor
        CMatchup ( const string & contestant1, const string & contestant2, M_ result )
                : m_contestant1 ( contestant1 ), m_contestant2 ( contestant2 ), m_Result ( result )
        {}
        string m_contestant1;
        string m_contestant2;
        M_ m_Result;
    };
    vector<CMatchup> m_Matchups;
};

#ifndef __PROGTEST__
struct CMatch
{
public:
    CMatch ( int a, int b )
    : m_A ( a ), m_B ( b )
    {}
    int m_A;
    int m_B;
};

class HigherScoreThreshold
{
public:
    HigherScoreThreshold ( int diffAtLeast )
    : m_DiffAtLeast ( diffAtLeast )
    {}
    int operator () ( const CMatch & x ) const {
        return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
private:
    int m_DiffAtLeast;
};

int HigherScore ( const CMatch & x ) {
    return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int main ( void )
{
    /*
    CContest<CMatch> x;

    x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
            . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
            . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
            . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
            . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
            . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );


    assert ( ! x . isOrdered ( HigherScore ) );
    try
    {
        list<string> res = x . results ( HigherScore );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }

    x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) );

    assert ( x . isOrdered ( HigherScore ) );
    try
    {
        list<string> res = x . results ( HigherScore );
        assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == nullptr );
    }


    assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
    try
    {
        list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }

    assert ( x . isOrdered ( [] ( const CMatch & x )
                             {
                                 return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
                             } ) );
    try
    {
        list<string> res = x . results ( [] ( const CMatch & x )
                                         {
                                             return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
                                         } );
        assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == nullptr );
    }

    CContest<bool>  y;

    y . addMatch ( "Python", "PHP", true )
            . addMatch ( "PHP", "Perl", true )
            . addMatch ( "Perl", "Bash", true )
            . addMatch ( "Bash", "JavaScript", true )
            . addMatch ( "JavaScript", "VBScript", true );

    assert ( y . isOrdered ( [] ( bool v )
                             {
                                 return v ? 10 : - 10;
                             } ) );
    try
    {
        list<string> res = y . results ( [] ( bool v )
                                         {
                                             return v ? 10 : - 10;
                                         });
        assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
    }
    catch ( ... )
    {
        assert ( "Unexpected exception!" == nullptr );
    }

    y . addMatch ( "PHP", "JavaScript", false );
    assert ( !y . isOrdered ( [] ( bool v )
                              {
                                  return v ? 10 : - 10;
                              } ) );
    try
    {
        list<string> res = y . results ( [] ( bool v )
                                         {
                                             return v ? 10 : - 10;
                                         } );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }

    try
    {
        y . addMatch ( "PHP", "JavaScript", false );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }

    try
    {
        y . addMatch ( "JavaScript", "PHP", true );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }

    CContest<CMatch> a;
    a . addMatch ( "A", "B", CMatch ( 10, 3 ) )
            . addMatch ( "B", "C", CMatch ( 8, 1 ) )
            . addMatch ( "C", "A", CMatch ( 40, 0 ) );
    assert ( ! a . isOrdered ( HigherScore ) );

    CContest<CMatch> b;
    b . addMatch ( "A", "B", CMatch ( 10, 3 ) )
            . addMatch ( "A", "C", CMatch ( 8, 1 ) );
    assert ( ! b . isOrdered ( HigherScore ) );

    CContest<CMatch> c;
    c . addMatch ( "A", "B", CMatch ( 10, 3 ) )
            . addMatch ( "B", "C", CMatch ( 8, 8 ) );
    assert ( ! c . isOrdered ( HigherScore ) );

    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
