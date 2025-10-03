#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect
{
public:
    CRect ( double x, double y, double w, double h )
            : m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h )
    {}
    friend ostream & operator << ( ostream & os, const CRect & x ) {
        return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};
#endif /* __PROGTEST__ */

class CElement
{
public:
    virtual ostream & print ( ostream & out, bool isLast = true, bool isInWindow = false ) const = 0;
    [[nodiscard]] virtual CElement * clone () const = 0;
    virtual ~CElement() noexcept = default;
    virtual void setPosition( const CRect & absPos ) = 0;
    virtual int getId () const = 0;
};

ostream & operator << ( ostream & out, const CElement & self )
{
    return self.print(out );
}

class CElementId : public CElement
{
protected:
    int m_Id;
    CRect m_AbsPosition = CRect(0,0,0,0);
    CRect m_RelPosition;
public:
    explicit CElementId (int id, const CRect & pos )
            : m_Id( id ), m_RelPosition( pos )
    {
        m_AbsPosition = pos;
    };
    int getId () const override {
        return m_Id;
    }
    void setPosition( const CRect & absPos ) override {
        m_AbsPosition.m_X = absPos.m_X + m_RelPosition.m_X * absPos.m_W;
        m_AbsPosition.m_Y = absPos.m_Y + m_RelPosition.m_Y * absPos.m_H;
        m_AbsPosition.m_W = m_RelPosition.m_W * absPos.m_W;
        m_AbsPosition.m_H = m_RelPosition.m_H * absPos.m_H;
    }
};

class CWindow : public CElementId
{
private:
    string m_Title;
    vector<CElement *> m_Elements;
public:
    CWindow ( int id, const string & title, const CRect & absPos )
            : CElementId( id, absPos ), m_Title( title )
    {};
    // add
    CWindow & add ( const CElement & element ){
        // Clone the element and set its position
        CElement * newElement = element.clone();
        newElement->setPosition(m_RelPosition);
        // Add the new element to the vector
        m_Elements.emplace_back(newElement);
        return *this;
    }

    CWindow(const CWindow& other)
        : CElementId(other), m_Title(other.m_Title)
    {
        for (const auto& elem : other.m_Elements) {
            m_Elements.push_back(elem->clone());
        }
    }

    CWindow& operator=(const CWindow& other)
    {
        if (this != &other) {
            CElementId::operator=(other);
            m_Title = other.m_Title;
            for (auto& elem : m_Elements) {
                delete elem;
            }
            m_Elements.clear();
            for (const auto& elem : other.m_Elements) {
                m_Elements.push_back(elem->clone());
            }
        }
        return *this;
    }

    ~CWindow()
    {
        for (auto& elem : m_Elements) {
            delete elem;
        }
    }
    // search
    CElement * search ( int id ) const {
        for ( const auto & x : m_Elements )
            if ( x->getId() == id )
                return x;
        return nullptr;
    }
    // setPosition
    void setPosition( const CRect & absPos ) override {
        m_AbsPosition = absPos;
        for ( auto & x : m_Elements )
            x->setPosition( absPos );
    }
    // print
    ostream & print ( ostream & out, bool isLast, bool isInWindow ) const override {
        out << "[" << m_Id << "] Window \"" << m_Title << "\" " << m_AbsPosition << endl;
        for ( auto it = m_Elements.begin(); it != m_Elements.end(); ++it ) {
            out << "+- ";
            if ( it == m_Elements.end() - 1 )
                (*it)->print(out, true, true);
            else
                (*it)->print(out, false, true);
        }
        return out;
    }
    // clone
    CElement * clone () const override {
        return new CWindow( *this );
    }
};

class CButton : public CElementId
{
private:
    string m_Name;
public:
    explicit CButton ( int id, const CRect & relPos, const string & name )
            : CElementId( id, relPos ), m_Name( name )
    {};
    ostream & print ( ostream & out, bool isLast, bool isInWindow ) const override {
        return out << "[" << m_Id << "] Button \"" << m_Name << "\" " << m_AbsPosition << endl;
    }
    CElement * clone () const override {
        return new CButton( *this );
    }
};

class CInput : public CElementId
{
private:
    string m_Value;
public:
    CInput ( int id, const CRect & relPos, const string & value )
            : CElementId( id, relPos ), m_Value( value )
    {};
    ostream & print ( ostream & out, bool islast, bool isInWindow ) const override {
        return out << "[" << m_Id << "] Input \"" << m_Value << "\" " << m_AbsPosition << endl;
    }
    CElement * clone () const override {
        return new CInput( *this );
    }
    // setValue
    void setValue ( const string & value ){
        m_Value = value;
    }
    // getValue
    string getValue() const {
        return m_Value;
    }
};
class CLabel : public CElementId
{
private:
    string m_Label;
public:
    explicit CLabel ( int id, const CRect & relPos, const string & label )
            : CElementId( id, relPos ), m_Label( label )
    {};
    ostream & print ( ostream & out, bool isLast, bool isInWindow ) const override {
        return out << "[" << m_Id << "] Label \"" << m_Label << "\" " << m_AbsPosition << endl;
    }
    CElement * clone () const override {
        return new CLabel( *this );
    }
};

class CComboBox : public CElementId
{
private:
    vector<string> m_BoxElems;
    size_t m_Selected = 0;
public:
    CComboBox ( int id, const CRect & relPos )
            : CElementId( id, relPos )
    {}

    // Copy constructor
    CComboBox(const CComboBox& other)
            : CElementId(other)
    {
        m_BoxElems = other.m_BoxElems;
        m_Selected = other.m_Selected;
    }

    // Copy assignment operator
    CComboBox& operator=(const CComboBox& other) {
        if (this != &other) {
            CElementId::operator=(other);
            m_BoxElems = other.m_BoxElems;
            m_Selected = other.m_Selected;
        }
        return *this;
    }
    // add
    CComboBox & add ( const string & boxElement ){
        m_BoxElems.push_back(boxElement);
        return *this;
    }
    ostream & print ( ostream & out, bool isLast, bool isInWindow ) const override {
        out << "[" << m_Id << "] ComboBox " << m_AbsPosition << endl;
        for ( size_t i = 0; i < m_BoxElems.size(); i++ ) {
            if (isLast && isInWindow)
                out << "   ";
            if (!isLast && isInWindow)
                out << "|  ";
            if ( i == m_Selected )
                out << "+->" << m_BoxElems[i] << "<" << endl;
            else
                out << "+- " << m_BoxElems[i] << endl;
        }
        return out;
    }
    CElement * clone () const override {
        return new CComboBox( *this );
    }
    // setSelected
    void setSelected ( int selected ){
        m_Selected = selected;
    }
    // getSelected
    int getSelected () const {
        return m_Selected;
    }
};

// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    CWindow b = a;
    assert ( toString ( *b . search ( 20 ) ) ==
             "[20] ComboBox (70,154,480,48)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
    b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (70,250,480,48)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [20] ComboBox (84,186,512,52)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (84,290,512,52)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
