#ifndef __PROGTEST__
#include <iostream>
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        char * m_Value;
        const char * Value () const{
            return m_Value;
        }
        CNode(const char * value, CNode * next){
            m_Value = new char[strlen(value) + 1];
            strncpy (m_Value, value, strlen(value) + 1);
            m_Next = next;
        }
    };

    CNode * m_Begin = nullptr;
    size_t m_Size = 0;

public:
    // default constructor
    CLinkedSet () = default;

    // copy constructor
    CLinkedSet ( const CLinkedSet & other ){
        for (CNode * node = other.m_Begin; node; node = node->m_Next){
            this->Insert(node->Value());
        }
    }

    // operator=
    CLinkedSet & operator = (CLinkedSet other){
        if (this == &other) {
            return *this;
        }
        while (m_Begin != nullptr) {
            CNode * tmp = m_Begin->m_Next;
            delete [] m_Begin->m_Value;
            delete m_Begin;
            m_Begin = tmp;
        }
        m_Begin = nullptr;
        m_Size = 0;
        for (CNode * node = other.m_Begin; node; node = node->m_Next){
            this->Insert(node->Value());
        }
        return *this;
    }

    // destructor
    ~CLinkedSet (){
        while (m_Begin != nullptr) {
            CNode * tmp = m_Begin->m_Next;
            delete [] m_Begin->m_Value;
            delete m_Begin;
            m_Begin = tmp;
        }
    }

    bool Insert ( const char * value ){
        if(Contains(value)){
            return false;
        }
        CNode * tmp = m_Begin;
        CNode * prev = nullptr;
        while(tmp != nullptr){
            if(strcmp(tmp->Value(), value) > 0){
                break;
            }
            prev = tmp;
            tmp = tmp->m_Next;
        }
        if(prev == nullptr){
            m_Begin = new CNode(value, m_Begin);
        }else{
            prev->m_Next = new CNode(value, tmp);
        }
        m_Size++;
        return true;
    }

    bool Remove ( const char * value ){
        CNode * tmp = m_Begin;
        CNode * prev = nullptr;
        while(tmp != nullptr){
            if(strcmp(tmp->Value(), value) == 0){
                if(prev == nullptr){
                    m_Begin = tmp->m_Next;
                }else{
                    prev->m_Next = tmp->m_Next;
                }
                delete [] tmp->m_Value;
                delete tmp;
                m_Size--;
                return true;
            }
            prev = tmp;
            tmp = tmp->m_Next;
        }
        return false;
    }

    bool Empty () const {
        return m_Begin == nullptr;
    }

    size_t Size () const{
        return m_Size;
    }

    bool Contains ( const char * value ) const{
        CNode * tmp = m_Begin;
        while(tmp != nullptr){
            if(strcmp(tmp->Value(), value) == 0){
                return true;
            }
            tmp = tmp->m_Next;
        }
        return false;
    }

    // print in format  < null | Jerabek Michal | Pavlik Ales >
    void Print () const
    {
        CNode * current = m_Begin;
        cout << "< ";
        while (current != nullptr)
        {
            cout << current->m_Value << " | ";
            current = current->m_Next;
        }
        cout << "null >" << endl;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        x0 . Print();
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );
    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
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
        CLinkedSet x0;
        CLinkedSet x1;
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

};

int main ()
{
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */

