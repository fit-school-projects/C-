#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

class CRange
{
public:
    CRange ( long long lower, long long upper )
    {
        m_Lower = lower;
        m_Upper = upper;
        if (lower > upper)
            throw invalid_argument("Invalid range");
    }
    friend vector <CRange> operator + ( const CRange & lhs, const CRange & rhs )
    {
        vector <CRange> tmp;
        if (max(lhs.m_Lower, rhs.m_Lower) <= min(lhs.m_Upper, rhs.m_Upper))
        {
            tmp.push_back({min(lhs.m_Lower, rhs.m_Lower), max(lhs.m_Upper, rhs.m_Upper)});
        } else if (lhs.m_Upper != LLONG_MAX && lhs.m_Upper + 1 == rhs.m_Lower){
            tmp.push_back({lhs.m_Lower, rhs.m_Upper});
        } else if (rhs.m_Upper != LLONG_MAX && rhs.m_Upper + 1 == lhs.m_Lower){
            tmp.push_back({rhs.m_Lower, lhs.m_Upper});
        } else {
            tmp.push_back(lhs);
            tmp.push_back(rhs);
        }
        return tmp;
    }
    friend vector <CRange> operator + ( const vector <CRange> & lhs, const CRange & rhs )
    {
        vector <CRange> tmp;
        bool added = false;
        long long lower = rhs.m_Lower;
        long long upper = rhs.m_Upper;
        for (auto & i : lhs){
            if (added){
                tmp.push_back(i);
                continue;
            }
            if (max(i.m_Lower, lower) <= min(i.m_Upper, upper)){
                lower = min(i.m_Lower, lower);
                upper = max(i.m_Upper, upper);
            } else if (i.m_Upper != LLONG_MAX && i.m_Upper + 1 == lower){
                lower = i.m_Lower;
            } else if (upper != LLONG_MAX && upper + 1 == i.m_Lower){
                upper = i.m_Upper;
            } else {
                if (lower > i.m_Upper){
                    tmp.push_back(i);
                } else {
                    tmp.push_back({lower,upper});
                    tmp.push_back(i);
                    added = true;
                }
            }
        }
        if (!added) tmp.push_back({lower,upper});
        return tmp;
    }
    friend vector <CRange> operator - ( const CRange & lhs, const CRange & rhs )
    {
        vector <CRange> tmp;
        removeInterval(tmp, {lhs}, rhs);
        return tmp;
    }
    friend vector <CRange> operator - ( const vector <CRange> & lhs, const CRange & rhs )
    {
        vector <CRange> tmp;
        removeInterval(tmp, lhs, rhs);
        return tmp;
    }

    // print interval in dec and then restore the original format flags of the stream

    friend ostream & operator << ( ostream & os, const CRange & x )
    {
        ios::fmtflags f ( os . flags ());
        if ( x . m_Lower == x . m_Upper )
            os << dec << x . m_Lower;
        else
        os << dec << "<" << x . m_Lower << ".." << x . m_Upper << ">";
        os . flags ( f );
        return os;
    }

    long long getLower () const
    {
        return m_Lower;
    }
    long long getUpper () const
    {
        return m_Upper;
    }

private:
    friend class CRangeList;
    long long m_Lower;
    long long m_Upper;

    static void removeInterval (vector <CRange> & tmp, const vector <CRange> & x, const CRange & y)
    {
        for (auto & i : x){
            if (max(i.m_Lower, y.m_Lower) <= min(i.m_Upper, y.m_Upper)){
                if (i.m_Lower < y.m_Lower){
                    tmp.push_back({i.m_Lower, y.m_Lower - 1});
                }
                if (i.m_Upper > y.m_Upper){
                    tmp.push_back({y.m_Upper + 1, i.m_Upper});
                }
            } else {
                tmp.push_back(i);
            }
        }
    }
};
class CRangeList
{
public:
    // constructor
    CRangeList () = default;
    // initializer list constructor
    CRangeList ( const initializer_list <CRange> & x )
    {
        for (auto & i : x){
            *this += i;
        }
    }
    // += range / range list
    vector<CRange> & operator += (const CRange & x)
    {
        m_Ranges = m_Ranges + x;
        return m_Ranges;
    }
    vector<CRange> & operator += (const vector<CRange> & x)
    {
        for (auto & i : x){
            *this += i;
        }
        return m_Ranges;
    }

    CRangeList & operator += (const CRangeList & x)
    {
        for (auto & i : x.m_Ranges){
            *this += i;
        }
        return *this;
    }
    // includes long long / range

    auto begin () const
    {
        return m_Ranges . begin ();
    }
    auto end () const
    {
        return m_Ranges . end ();
    }
    friend CRangeList operator + ( const CRangeList & lhs, const CRange & rhs )
    {
        CRangeList tmp;
        tmp += lhs;
        tmp += rhs;
        return tmp;
    }

    // -= range / range list
    CRangeList & operator -= (const CRange & x)
    {
        m_Ranges = m_Ranges - x;
        return *this;
    }
    CRangeList & operator -= (const CRangeList & x)
    {
        for (auto & i : x.m_Ranges){
            *this -= i;
        }
        return *this;
    }
    CRangeList & operator -= (const vector<CRange> & x)
    {
        for (auto & i : x){
            *this -= i;
        }
        return *this;
    }
    // = range / range list
    CRangeList & operator = (const CRange & x){
        m_Ranges.clear();
        m_Ranges.push_back(x);
        return *this;
    }
    vector <CRange> & operator = (const CRangeList & x)
    {
        m_Ranges = x.m_Ranges;
        return m_Ranges;
    }
    vector <CRange> & operator = (const vector<CRange> & x)
    {
        m_Ranges = x;
        return m_Ranges;
    }
    // operator ==
    friend bool operator == ( const CRangeList & lhs, const CRangeList & rhs )
    {
        if (lhs.m_Ranges.size() != rhs.m_Ranges.size())
            return false;
        for (size_t i = 0; i < lhs.m_Ranges.size(); i++){
            if (lhs.m_Ranges[i].getLower() != rhs.m_Ranges[i].getLower() || lhs.m_Ranges[i].getUpper() != rhs.m_Ranges[i].getUpper())
                return false;
        }
        return true;
    }
    // operator !=
    friend bool operator != ( const CRangeList & lhs, const CRangeList & rhs )
    {
        return !(lhs == rhs);
    }
    // operator <<
    friend ostream & operator << ( ostream & os, const CRangeList & x )
    {
        os << "{";
        for ( auto it = x.m_Ranges.begin (); it != x.m_Ranges.end (); it++ )
        {
            if ( it != x.m_Ranges.begin ())
                os << ",";
            os << *it;
        }
        os << "}";
        return os;
    }

    bool includes ( long long x ) const
    {
        for (auto & i : m_Ranges){
            if (i.m_Lower <= x && i.m_Upper >= x)
                return true;
        }
        return false;
    }

    bool includes ( const CRange & x ) const
    {
        for (auto & i : m_Ranges){
            if (i.m_Lower <= x.m_Lower && i.m_Upper >= x.m_Upper)
                return true;
        }
        return false;
    }

private:
    vector<CRange> m_Ranges;
};



#ifndef __PROGTEST__
string toString ( const CRangeList& x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int                main                                    ( void )
{
    CRangeList a, b;

    assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
    a = CRange ( 5, 10 );
    a += CRange ( 25, 100 );
    assert ( toString ( a ) == "{<5..10>,<25..100>}" );
    a += CRange ( -5, 0 );
    a += CRange ( 8, 50 );
    assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
    a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
    assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
    a += CRange ( 106, 119 ) + CRange ( 152, 158 );
    assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
    a += CRange ( -3, 170 );
    a += CRange ( -30, 1000 );
    assert ( toString ( a ) == "{<-30..1000>}" );
    b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
    a += b;
    assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
    a -= CRange ( -400, -400 );
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
    a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    try
    {
        a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
        assert ( "Exception not thrown" == nullptr );
    }
    catch ( const std::logic_error & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown" == nullptr );
    }
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    b = a;
    assert ( a == b );
    assert ( !( a != b ) );
    b += CRange ( 2600, 2700 );
    assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    assert ( a == b );
    assert ( !( a != b ) );
    b += CRange ( 15, 15 );
    assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
    assert ( !( a == b ) );
    assert ( a != b );
    assert ( b . includes ( 15 ) );
    assert ( b . includes ( 2900 ) );
    assert ( b . includes ( CRange ( 15, 15 ) ) );
    assert ( b . includes ( CRange ( -350, -350 ) ) );
    assert ( b . includes ( CRange ( 100, 200 ) ) );
    assert ( !b . includes ( CRange ( 800, 900 ) ) );
    assert ( !b . includes ( CRange ( -1000, -450 ) ) );
    assert ( !b . includes ( CRange ( 0, 500 ) ) );
    a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
    assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b += a;
    assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b -= a;
    assert ( toString ( b ) == "{}" );
    b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
    assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
    b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
    assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    /*
    CRangeList c;
    c = CRange (0,0) + CRange (1,1) + CRange (2,2) - CRange (1,1);
    assert (!c.includes(1));
    c += CRange (LLONG_MAX, LLONG_MAX) + CRange (LLONG_MIN, LLONG_MIN) + CRange (LLONG_MAX, LLONG_MAX) - CRange (LLONG_MIN, LLONG_MIN);
    cout << toString(c) << endl;
    CRangeList d;
    d = CRange (LLONG_MIN, LLONG_MAX);
    cout << toString(d) << endl;
    d -= CRange (LLONG_MIN + 1, LLONG_MAX - 1);
    cout << toString(d) << endl;
     */

#ifdef EXTENDED_SYNTAX

    CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
    assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
    ostringstream oss;
    oss << setfill ( '=' ) << hex << left;
    for ( const auto & v : x + CRange ( -100, -100 ) )
        oss << v << endl;
    oss << setw ( 10 ) << 1024;
    assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );

#endif /* EXTENDED_SYNTAX */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */



