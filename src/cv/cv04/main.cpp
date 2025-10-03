#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

struct CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    // constructor, destructor
    CTime() = default;
    CTime(int hour, int minute, int second = 0) : m_Hour(hour), m_Minute(minute), m_Second(second) {
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            throw invalid_argument("Invalid time");
        }
    }

    friend CTime operator + ( const CTime & lhs, int secondsAdd ) {
        int totalSeconds = secondsAdd;
        if (totalSeconds < 0 ) return lhs - abs(totalSeconds);
        int time = (lhs.m_Hour * 3600 + lhs.m_Minute * 60 + lhs.m_Second + secondsAdd) % 86400;
        return {time / 3600, (time % 3600) / 60, time % 60 };
    }
    friend CTime operator + ( int secondsAdd, const CTime & lhs ) {
        int totalSeconds = secondsAdd;
        if (totalSeconds < 0 ) return lhs - abs(totalSeconds);
        int time = (lhs.m_Hour * 3600 + lhs.m_Minute * 60 + lhs.m_Second + secondsAdd) % 86400;
        return {time / 3600, (time % 3600) / 60, time % 60 };
    }
    friend CTime operator - (const CTime & a, const int seconds) {
        int totalSeconds = seconds;
        if (totalSeconds < 0 ) return a + abs(totalSeconds);
        int time = (a.m_Hour * 3600 + a.m_Minute * 60 + a.m_Second - seconds) % 86400;
        if (time < 0) time += 86400;
        return {time / 3600, (time % 3600) / 60, time % 60 };
    }

    CTime & operator += (const int seconds){
        return *this = *this + seconds;
    }
    CTime & operator -= (const int seconds){
        return *this = *this - seconds;
    }
    friend int operator - (const CTime & lhs, const CTime & rhs) {
        if (lhs == rhs) return 0;
        int timeToAdd, tmpTime;
        int time1 = lhs.m_Second + 60 * lhs.m_Minute + 3600 * lhs.m_Hour;
        int time2 = rhs.m_Second + 60 * rhs.m_Minute + 3600 * rhs.m_Hour;
        timeToAdd = abs(time1 - time2);
        tmpTime = 86400 - timeToAdd;
        return timeToAdd < tmpTime ? timeToAdd : tmpTime;
    }
    friend bool operator == (const CTime & lhs, const CTime & rhs){
        return tie(lhs.m_Hour, lhs.m_Minute, lhs.m_Second) == tie(rhs.m_Hour, rhs.m_Minute, rhs.m_Second);
    }
    friend bool operator < (const CTime & lhs, const CTime & rhs){
        return  tie(lhs.m_Hour, lhs.m_Minute, lhs.m_Second) < tie(rhs.m_Hour, rhs.m_Minute, rhs.m_Second);
    }
    friend bool operator <= (const CTime & lhs, const CTime & rhs){
        return  lhs < rhs || lhs == rhs;
    }
    friend bool operator > (const CTime & lhs, const CTime & rhs){
        return  tie(lhs.m_Hour, lhs.m_Minute, lhs.m_Second) > tie(rhs.m_Hour, rhs.m_Minute, rhs.m_Second);
    }
    friend bool operator >= (const CTime & lhs, const CTime & rhs){
        return  lhs > rhs || lhs == rhs;
    }
    friend bool operator != (const CTime & lhs, const CTime & rhs){
        return !(lhs == rhs);
    }

    CTime operator ++ (int n){
        CTime result = *this;
        *this += 1;
        return result;
    }
    CTime operator -- (int n){
        CTime result = *this;
        *this -= 1;
        return result;
    }
    CTime & operator ++ (){
        return *this += 1;
    }
    CTime & operator -- (){
        return *this -= 1;
    }

    // output operator
    friend ostream & operator << (ostream & os, const CTime & t){
        return os << setfill(' ') << setw(2) << t.m_Hour << ":" << setfill('0') << setw(2) << t.m_Minute << ":" << setw(2) << t.m_Second;
    }

    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour
               && time.m_Minute == minute
               && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    /*
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );
    */
    return 0;
}
#endif /* __PROGTEST__ */
