#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp
{
public:

    CTimeStamp ( int year, int month, int day, int hour, int minute, int second )
    {
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = hour;
        this->minute = minute;
        this->second = second;
    }

private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    friend class CEFaceMask;
};
class CContact
{
public:
    CContact ( CTimeStamp time, int id1, int id2 )
            : time(time), id1(id1), id2(id2)
    {}

private:
    CTimeStamp time;
    int id1;
    int id2;
    friend class CEFaceMask;
};
class CEFaceMask
{
public:
    // default constructor
    CEFaceMask () = default;
    // addContact ( contact )
    CEFaceMask & addContact ( const CContact & contact ){
        contacts.push_back(contact);
        return *this;
    }
    // getSuperSpreaders ( from, to )
    vector<int> getSuperSpreaders ( const CTimeStamp & from, const CTimeStamp & to ){
        size_t maxCount = 0;
        for (auto contact : contacts){
            if (tie(contact.time.year, contact.time.month, contact.time.day, contact.time.hour, contact.time.minute, contact.time.second) >= tie(from.year, from.month, from.day, from.hour, from.minute, from.second) &&
                tie(contact.time.year, contact.time.month, contact.time.day, contact.time.hour, contact.time.minute, contact.time.second) <= tie(to.year, to.month, to.day, to.hour, to.minute, to.second )){
                if ( contact.id1 == contact.id2 )
                    continue;
                // if id1 is in superSpreaders map, search if id2 is in set
                if (superSpreaders.find(contact.id1) != superSpreaders.end()){
                    if (find(superSpreaders[contact.id1].begin(), superSpreaders[contact.id1].end(), contact.id2) == superSpreaders[contact.id1].end())
                        superSpreaders[contact.id1].insert(contact.id2);
                } else {
                    superSpreaders[contact.id1] = set<int>{contact.id2};
                }
                // if id2 is in superSpreaders map, search if id1 is in set
                if (superSpreaders.find(contact.id2) != superSpreaders.end()){
                    if (find(superSpreaders[contact.id2].begin(), superSpreaders[contact.id2].end(), contact.id1) == superSpreaders[contact.id2].end())
                        superSpreaders[contact.id2].insert(contact.id1);
                } else {
                    superSpreaders[contact.id2] = set<int>{contact.id1};
                }
            }
        }
        vector<int> result;
        for (auto superSpreader : superSpreaders){
            if (superSpreader.second.size() > maxCount){
                maxCount = superSpreader.second.size();
            }
        }
        for (auto superSpreader : superSpreaders){
            if (superSpreader.second.size() == maxCount){
                result.push_back(superSpreader.first);
            }
        }
        superSpreaders.clear();
        return result;
    }
private:
    map<int, set<int>> superSpreaders;
    vector<CContact> contacts;
};

#ifndef __PROGTEST__
int main ()
{
    CEFaceMask test;

    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
            . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
    assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222}) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 3, 20, 18, 0, 0 ), 444444444, 666666666 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 3, 25, 0, 0, 0 ), 111111111, 666666666 ) );
    assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222, 444444444}) );
    assert ( test .getSuperSpreaders( CTimeStamp ( 2010, 1, 1, 0, 0, 0 ), CTimeStamp ( 2010, 1, 1, 0, 0, 1 ) ) == (vector<int> {}) );
    test . addContact ( CContact ( CTimeStamp ( 2015, 1, 10, 12, 40, 10 ), 111111111,111111111  ) );
    test . addContact ( CContact ( CTimeStamp ( 2015, 1, 12, 12, 40, 10 ), 333333333, 111111111 ) );
    assert ( test . getSuperSpreaders ( CTimeStamp ( 2015, 1, 1, 0, 0, 0 ), CTimeStamp ( 2015, 1, 31, 0, 0, 0 ) ) == (vector<int> {111111111, 333333333 }) );


    CEFaceMask test2;
    test2 . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
    test2 . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 222222222, 111111111 ) );
    test2 . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 555555555 ) );
    test2 . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 666666666 ) );
    test2 . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 111111111, 111111111 ) );

    assert ( test2 . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> { 333333333 }) );

    return 0;
}
#endif /* __PROGTEST__ */

