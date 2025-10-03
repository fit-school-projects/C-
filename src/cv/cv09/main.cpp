#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <algorithm>
#include <functional>

#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cctype>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T>
class CSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        T m_Value;

        CNode(const T & value, CNode * next)
                : m_Next(next), m_Value(value)
        { }
    };
    CNode * m_Begin = nullptr;
    size_t m_Size = 0;
public:
    // default constructor
    CSet () = default;
    // copy constructor
    CSet( const CSet & other ){
        for ( CNode * node = other.m_Begin; node; node = node->m_Next ){
            this->Insert( node->m_Value);
        }
    }
    // operator=
    CSet & operator = ( CSet other ){
        swap(m_Begin, other.m_Begin);
        swap(m_Size, other.m_Size);
        return *this;
    }

    // destructor
    ~CSet (){
        for ( CNode * node = m_Begin; node; ){
            CNode * tmp = node->m_Next;
            delete node;
            node = tmp;
        }

        m_Begin = nullptr;
        m_Size = 0;
    }
    // Insert
    bool Insert ( const T & value ){
        if ( Contains( value )){
            return false;
        }
        m_Begin = new CNode( value, m_Begin );
        m_Size++;
        return true;
    }
    // Remove
    bool Remove ( const T & value ){
        if ( !Contains( value )){
            return false;
        }
        CNode * node = m_Begin;
        CNode * prev = nullptr;
        while ( !(!(node->m_Value < value) && !(value < node->m_Value)) ){
            prev = node;
            node = node->m_Next;
        }
        if ( prev == nullptr ){
            m_Begin = node->m_Next;
        } else {
            prev->m_Next = node->m_Next;
        }
        delete node;
        m_Size--;
        return true;
    }


    // Contains
    bool Contains ( const T & value ) const{
        for ( CNode * node = m_Begin; node; node = node->m_Next ){
            if ( !(node->m_Value < value) && !(value < node->m_Value) ){
                return true;
            }
        }
        return false;
    }

    // Size
    size_t Size () const{
        return m_Size;
    }
};

#ifndef __PROGTEST__
#include <cassert>

struct CSetTester
{
    static void test0()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
    }

    static void test1()
    {
        CSet<string> x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CSet<string> x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CSet<string> x0;
        CSet<string> x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test4()
    {
        CSet<int> x0;
        assert( x0 . Insert( 4 ) );
        assert( x0 . Insert( 8 ) );
        assert( x0 . Insert( 1 ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( 4 ) );
        assert( !x0 . Contains( 5 ) );
        assert( !x0 . Remove( 5 ) );
        assert( x0 . Remove( 4 ) );
    }
    static void test5()
    {
        CSet<const char *> x0;
        CSet<const char *> x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( !x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Size() == 1);
        x1 = x0;
        assert ( x1 .Contains( "Jerabek Michal" ) );
        assert ( x1 . Remove( "Jerabek Michal" ) );
        assert ( !x1 . Contains( "Jerabek Michal" ) );
        assert ( x0 . Contains( "Jerabek Michal" ) );
        assert( x0 . Size() == 1);
        assert ( x1 . Size() == 0 );
    }

};

int main ()
{
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
    CSetTester::test5();
    return 0;
}
#endif /* __PROGTEST__ */

