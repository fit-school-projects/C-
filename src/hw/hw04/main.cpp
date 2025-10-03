#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Class which represents the C type of string.
 */
class CStr {
private:
    size_t m_Len;
    char * m_Data;
public:
    /**
     * @brief Constructs a new string from a C-style string.
     *
     * @param str The C-style string to initialize the new string with.
     */
    explicit CStr ( const char * str );
    CStr ( const CStr & src);
    CStr ( CStr && src ) noexcept;
    CStr & operator = ( CStr src ) noexcept;
    ~CStr () noexcept;
    /**
     * @brief Equality comparison operator.
     *
     * @param x The string to compare to this string.
     * @return True if the strings are equal, false otherwise.
     */
    bool operator == ( const CStr & x ) const;
    bool operator == ( const char * x ) const;
    /**
     * @brief Inequality comparison operator.
     *
     * @param x The string to compare to this string.
     * @return True if the strings are not equal, false otherwise.
     */
    bool operator != ( const CStr & x ) const;
    /**
     * @brief Less than comparison operator.
     *
     * @param x The string to compare to this string.
     * @return True if this string is less than the other string, false otherwise.
     */
    bool operator < ( const CStr & x ) const;
    /**
     * @brief Output stream operator.
     *
     * @param os The output stream to write to.
     * @param s The string to write to the output stream.
     * @return The output stream.
     */
    friend ostream & operator << ( ostream & os, const CStr & s );
};
/*----------------------------------------------------------------------*/
CStr::CStr ( const char * str )
    : m_Len ( strlen ( str ) ), m_Data ( new char [m_Len + 1] ) {
    strcpy ( m_Data, str );
}
/*----------------------------------------------------------------------*/
CStr::CStr ( const CStr & src)
    : m_Len ( src . m_Len ), m_Data ( new char [m_Len + 1] ) {
    strcpy ( m_Data, src . m_Data );
}
/*----------------------------------------------------------------------*/
CStr::CStr ( CStr && src ) noexcept
    : m_Len ( src . m_Len ), m_Data ( src . m_Data ) {
    src . m_Data = nullptr;
    src . m_Len = 0;
}
/*----------------------------------------------------------------------*/
CStr & CStr::operator= ( CStr src ) noexcept {
    std::swap(m_Len, src . m_Len);
    std::swap(m_Data, src . m_Data);
    return * this;
}
/*----------------------------------------------------------------------*/
CStr::~CStr () noexcept {
    delete [] m_Data;
}
/*----------------------------------------------------------------------*/
bool CStr::operator == ( const CStr & x ) const{
    if (m_Len != x.m_Len)
        return false;
    return !strcmp ( m_Data, x . m_Data );
}
/*----------------------------------------------------------------------*/
bool CStr::operator == ( const char * x ) const{
    if (m_Len != strlen(x))
        return false;
    return !strcmp ( m_Data, x );
}
/*----------------------------------------------------------------------*/
bool CStr::operator != ( const CStr & x ) const{
    return strcmp ( m_Data, x . m_Data );
}
/*----------------------------------------------------------------------*/
bool CStr::operator < ( const CStr & x ) const{
    return strcmp ( m_Data, x . m_Data ) < 0;
}
/*----------------------------------------------------------------------*/
ostream & operator << ( ostream & os, const CStr & s ){
    return os << s . m_Data;
}

/**
 * @brief Class which represents the shared string.
 */
class CSharedStr {
private:
    CStr * m_Str;
    size_t * m_RefCount; // pointer to the number of references to the string m_Str
public:
    /**
     * @brief Constructs a new shared string from a C-style string.
     * m_RefCount parameter is used to count the number of references to the string m_Str.
     *
     * @param str The C-style string to initialize the new shared string with.
     *
     */
    explicit CSharedStr ( const char * str );
    /**
     * @brief Copy constructor.
     * Copies the string m_Str and the number of references m_RefCount.
     *
     * @param src The shared string to copy.
     */
    CSharedStr ( const CSharedStr & src );
    /**
     * @brief Move constructor.
     * Moves the string m_Str and the number of references m_RefCount.
     *
     * @param src The shared string to move.
     */
    CSharedStr ( CSharedStr && src ) noexcept;
    /**
     * @brief Copy assignment operator.
     * Copies the string m_Str and the number of references m_RefCount.
     *
     * @param src The shared string to copy.
     * @return  The shared string.
     */
    CSharedStr & operator = ( CSharedStr src ) noexcept;
    /**
     * @brief Destructor.
     *
     * If the number of references to the string m_Str is 1, the destructor deletes the string m_Str and the number of references m_RefCount.
     * Otherwise, the destructor decrements the number of references m_RefCount.
     */
    ~CSharedStr () noexcept;
    /**
     * @brief Equality comparison operator.
     *
     * @param x The shared string to compare to this shared string.
     * @return True if the shared strings are equal, false otherwise.
     */
    bool operator == ( const CSharedStr & x ) const;
    bool operator == ( const char * x ) const;
    /**
     * @brief Inequality comparison operator.
     *
     * @param x The shared string to compare to this shared string.
     * @return True if the shared strings are not equal, false otherwise.
     */
    bool operator != ( const CSharedStr & x ) const;
    /**
     * @brief Less than comparison operator.
     *
     * @param x The shared string to compare to this shared string.
     * @return True if this shared string is less than the other shared string, false otherwise.
     */
    bool operator < ( const CSharedStr & x ) const;
    /**
     * @brief Output stream operator.
     *
     * @param os The output stream to write to.
     * @param str The shared string to write to the output stream.
     * @return The output stream.
     */
    friend ostream & operator << ( ostream & os, const CSharedStr & str );
};
/*----------------------------------------------------------------------*/
CSharedStr::CSharedStr ( const char * str )
    : m_Str ( new CStr ( str ) ), m_RefCount ( new size_t ( 1 ) )
{}
/*----------------------------------------------------------------------*/
CSharedStr::CSharedStr ( const CSharedStr & src )
    : m_Str (src.m_Str), m_RefCount( src.m_RefCount ) {
    *m_RefCount += 1;
}
/*----------------------------------------------------------------------*/
CSharedStr::CSharedStr ( CSharedStr && src ) noexcept
    : m_Str ( src . m_Str ), m_RefCount ( src . m_RefCount ) {
    src . m_Str = nullptr;
    src . m_RefCount = nullptr;
}
/*----------------------------------------------------------------------*/
CSharedStr & CSharedStr::operator = ( CSharedStr src ) noexcept {
    std::swap(m_Str, src.m_Str);
    std::swap(m_RefCount, src.m_RefCount);
    return * this;
}
/*----------------------------------------------------------------------*/
CSharedStr::~CSharedStr () noexcept {
    if ( m_RefCount && (*m_RefCount -= 1) == 0 ) {
        delete m_Str;
        delete m_RefCount;
    }
}
/*----------------------------------------------------------------------*/
bool CSharedStr::operator == ( const CSharedStr & x ) const{
    return *m_Str == *x.m_Str;
}
/*----------------------------------------------------------------------*/
bool CSharedStr::operator == ( const char * x ) const{
    return *m_Str == x;
}
/*----------------------------------------------------------------------*/
bool CSharedStr::operator != ( const CSharedStr & x ) const{
    return *m_Str != *x.m_Str;
}
/*----------------------------------------------------------------------*/
bool CSharedStr::operator < ( const CSharedStr & x ) const{
    return *m_Str < *x.m_Str;
}
/*----------------------------------------------------------------------*/
ostream & operator << ( ostream & os, const CSharedStr & str ){
    return os << *str.m_Str;
}

/**
 * @brief Class which represents dynamic array of elements of type TType.
 * @tparam TType The type of the elements stored in the vector.
 */
template <typename TType>
class CVector {
private:
    size_t m_Size;
    size_t m_Capacity;
    TType * m_Data;
    /**
     * @brief Reallocates the vector to the new capacity.
     *
     * @param newCapacity The new capacity of the vector.
     */
    void realloc( size_t newCapacity );
public:
    /**
     * @brief Constructs a new vector with the given size.
     *
     * @param size The size of the vector.
     */
    explicit CVector(size_t size = 1);
    CVector( const CVector & src );
    CVector( CVector && vec ) noexcept;
    CVector & operator = ( CVector src ) noexcept;
    ~CVector() noexcept;
    /**
     * @brief Inserts the element to the given position.
     * If the vector is full, the vector is reallocated to the double capacity.
     * If the position is greater than the size of the vector, the element is inserted to the end of the vector.
     * Otherwise, the element is inserted to the given position and the elements from the given position are shifted to the right.
     * The size of the vector is incremented by one.
     *
     * @param value The element to insert.
     * @param position The position to insert the element to.
     */
    void insert ( const TType & value, size_t position );
    /**
     * @brief Finds the first element that is not less than the given value.
     *
     *
     * @param value The value to compare to.
     * @return The index of the first element that is not less than the given value.
     */
    [[nodiscard]] size_t lower_bound( const TType & value ) const;
    /**
    * @brief Adds an element to the end of the vector.
    * If the vector is full, the vector is reallocated to the double capacity.
    *
    * @param value The element to add to the vector.
    */
    void push_back( const TType & value );
    /**
     * @brief Returns a reference to the element at the given index.
     *
     * @param index The index of the element to return.
     * @return A reference to the element at the given index.
     */
    TType & operator [] ( size_t index );
    /**
     * @brief Returns a const reference to the element at the given index.
     *
     * @param index The index of the element to return.
     * @return A const reference to the element at the given index.
     */
    const TType & operator [] ( size_t index ) const;
    [[nodiscard]] size_t size() const;
    void swap ( CVector & src );
};
/*----------------------------------------------------------------------*/
template<typename TType>
CVector<TType>::CVector(size_t size)
    : m_Size(size), m_Capacity(size), m_Data(new TType[size])
{}
/*----------------------------------------------------------------------*/
template<typename TType>
CVector<TType>::CVector( const CVector & src )
    : m_Size ( src . m_Size ), m_Capacity ( src . m_Capacity ), m_Data ( new TType [m_Capacity] )
{
    for ( size_t i = 0; i < m_Size; ++i )
        m_Data [i] = src . m_Data [i];
}
/*----------------------------------------------------------------------*/
template<typename TType>
CVector<TType>::CVector( CVector && vec ) noexcept
    : m_Size (vec.m_Size), m_Capacity (vec.m_Capacity), m_Data (vec.m_Data)
{
    vec.m_Data = nullptr;
    vec.m_Size = 0;
    vec.m_Capacity = 0;
}
/*----------------------------------------------------------------------*/
template<typename TType>
CVector<TType> & CVector<TType>::operator = ( CVector src ) noexcept {
    swap(src);
    return * this;
}
/*----------------------------------------------------------------------*/
template<typename TType>
CVector<TType>::~CVector() noexcept {
    delete [] m_Data;
}
/*----------------------------------------------------------------------*/
template<typename TType>
void CVector<TType>::insert ( const TType & value, size_t position ) {
    if ( m_Size >= m_Capacity ){
        realloc((m_Capacity + 5) * 2);
    }
    for ( size_t i = m_Size; i > position; --i ){
        m_Data [i] = std::move ( m_Data [i - 1] );
    }
    m_Data [position] = value;
    ++m_Size;
}
/*----------------------------------------------------------------------*/
template<typename TType>
size_t CVector<TType>::lower_bound( const TType & value ) const {
    size_t left = 0;
    size_t right = m_Size;
    while ( left < right ){
        size_t mid = (left + right) / 2;
        if ( m_Data [mid] < value )
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}
/*----------------------------------------------------------------------*/
template<typename TType>
void CVector<TType>::push_back( const TType & value ){
    if ( m_Size >= m_Capacity ){
        realloc((m_Capacity + 5) * 2);
    }
    m_Data [m_Size++] = value;
}
/*----------------------------------------------------------------------*/
template<typename TType>
TType & CVector<TType>::operator [] ( size_t index ){
    return m_Data [index];
}
/*----------------------------------------------------------------------*/
template<typename TType>
const TType & CVector<TType>::operator [] ( size_t index ) const{
    return m_Data [index];
}
/*----------------------------------------------------------------------*/
template<typename TType>
size_t CVector<TType>::size() const{
    return m_Size;
}
/*----------------------------------------------------------------------*/
template<typename TType>
void CVector<TType>::swap ( CVector & src ){
    std::swap(m_Size, src.m_Size);
    std::swap(m_Capacity, src.m_Capacity);
    std::swap(m_Data, src.m_Data);
}
/*----------------------------------------------------------------------*/
template<typename TType>
void CVector<TType>::realloc( size_t newCapacity ) {
    m_Capacity = newCapacity;
    auto * tmp = new TType[newCapacity];
    for (size_t i = 0; i < m_Size; ++i) {
        tmp[i] = std::move(m_Data[i]);
    }
    delete[] m_Data;
    m_Data = tmp;
}

/**
 * @brief Class representing one mail that contains sender, receiver and body.
 */
class CMail
{
private:
    CSharedStr m_From;
    CSharedStr m_To;
    CStr m_Body;
    friend class CMailServer;
public:
    CMail();
    CMail( const char * from, const char * to, const char * body );
    bool operator == ( const CMail & x ) const;
    friend ostream & operator << ( ostream & os, const CMail & m );
};
/*----------------------------------------------------------------------*/
CMail::CMail()
    : m_From ( "" ), m_To ( "" ), m_Body ( "" )
{}
/*----------------------------------------------------------------------*/
CMail::CMail ( const char * from, const char * to, const char * body )
    : m_From ( from ), m_To ( to ), m_Body ( body )
{}
/*----------------------------------------------------------------------*/
bool CMail::operator == ( const CMail & x ) const {
    return m_From == x . m_From && m_To == x . m_To && m_Body == x . m_Body;
}
/*----------------------------------------------------------------------*/
ostream & operator << ( ostream & os, const CMail & m ) {
    return os << "From: " << m.m_From << ", To: " << m.m_To << ", Body: " << m.m_Body;
}

/**
 * @brief Struct which represents a mailbox of the particular user.
 * It contains the name of the mailbox and two vectors of mails - inbox and outbox.
 */
struct TMailBox {
    TMailBox () : m_Name ( "" ), m_Inbox ( 0 ), m_Outbox ( 0 ) {};
    CSharedStr m_Name;
    CVector<CMail> m_Inbox;
    CVector<CMail> m_Outbox;
    bool operator < ( const TMailBox & x ) const {
        return m_Name < x . m_Name;
    }
};

/**
 * @brief Iterator class for iterating through a vector of CMail objects.
 */
class CMailIterator
{
private:
    CVector <CMail> m_Mail;
    size_t m_Index = 0;
public:
    /**
     * @brief Constructs a new iterator for the given vector of CMail objects.
     *
     * @param mail The vector of CMail objects to iterate through.
     */
    explicit CMailIterator ( const CVector <CMail> & mail );
    /**
     * @brief Conversion to bool operator.
     *
     * @return true if the iterator is valid (has not reached the end of the vector), false otherwise.
     */
    explicit operator bool ( ) const;
    /**
     * @brief Negation operator.
     *
     * @return true if the iterator is invalid (has reached the end of the vector), false otherwise.
     */
    bool operator ! ( ) const;
    /**
     * @brief Dereference operator.
     *
     * @return The CMail object at the current position of the iterator.
     */
    const CMail & operator * ( ) const;
    /**
     * @brief Prefix increment operator.
     *
     * @return The iterator pointing to the next element in the vector.
     */
    CMailIterator & operator ++ ( );
};
/*----------------------------------------------------------------------*/
CMailIterator::CMailIterator ( const CVector <CMail> & mail )
    : m_Mail ( mail )
{}
/*----------------------------------------------------------------------*/
CMailIterator::operator bool ( ) const{
    return m_Index < m_Mail.size();
}
/*----------------------------------------------------------------------*/
bool CMailIterator::operator ! ( ) const{
    return m_Index >= m_Mail.size();
}
/*----------------------------------------------------------------------*/
const CMail & CMailIterator::operator * ( ) const{
    return m_Mail[m_Index];
}
/*----------------------------------------------------------------------*/
CMailIterator & CMailIterator::operator ++ ( ){
    ++m_Index;
    return * this;
}

/**
 * @brief Class which represents a mail server that contains multiple mailboxes for different users.
 */
class CMailServer
{
private:
    CVector<TMailBox> m_MailBoxes;
    /**
     * @brief Searches for a mailbox in the mail server that matches the given email address.
     *
     * @param email The email address to search for.
     * @param typeOfMailBox The type of mailbox to search for (inbox or outbox).
     * true - inbox, false - outbox.
     * @return A CMailIterator pointing to the mailbox that matches the email address and type, or an empty iterator if not found.
     */
    CMailIterator searchMailBox ( const char * email, bool typeOfMailBox ) const;
    /**
     * @brief Sends the given mail to the appropriate mailbox.
     *
     * @param m The mail to send.
     * @param typeOfMailBox The type of mailbox to send the mail to (inbox or outbox).
     * true - inbox, false - outbox.
     *
     */
    void sendMailToMailBox ( const CMail & m, bool typeOfMailBox );
public:
    CMailServer ( );
    /**
     * @brief Sends the given mail to both the inbox(true) and outbox(false) mailboxes of the respective recipients and senders.
     *
     * @param m The mail to send.
     */
    void sendMail ( const CMail & m );
    /**
     * @brief Searches for the outbox mailbox of the given email address.
     *
     * @param email The email address to search for.
     * @return A CMailIterator pointing to the outbox mailbox of the given email address, or an empty iterator if not found.
     */
    CMailIterator outbox ( const char * email ) const;
    /**
     * @brief Searches for the inbox mailbox of the given email address.
     *
     * @param email The email address to search for.
     * @return A CMailIterator pointing to the inbox mailbox of the given email address, or an empty iterator if not found.
     */
    CMailIterator inbox ( const char * email ) const;
};
/*---------------------------------------------------------------------------------------------------------------*/
CMailServer::CMailServer ( )
    : m_MailBoxes ( 0 )
{}
/*---------------------------------------------------------------------------------------------------------------*/
void CMailServer::sendMail ( const CMail & m ){
    sendMailToMailBox(m, true);
    sendMailToMailBox(m, false);
}
/*---------------------------------------------------------------------------------------------------------------*/
CMailIterator CMailServer::outbox ( const char * email ) const{
    return searchMailBox(email, false);
}
/*---------------------------------------------------------------------------------------------------------------*/
CMailIterator CMailServer::inbox ( const char * email ) const{
    return searchMailBox(email, true);
}
/*---------------------------------------------------------------------------------------------------------------*/
CMailIterator CMailServer::searchMailBox ( const char * email, bool typeOfMailBox ) const{
    TMailBox tmp;
    tmp.m_Name = CSharedStr ( email );
    size_t idx = m_MailBoxes.lower_bound( tmp );
    if ( idx != m_MailBoxes.size() && m_MailBoxes[idx].m_Name == email) {
        return CMailIterator(typeOfMailBox ? m_MailBoxes[idx].m_Inbox : m_MailBoxes[idx].m_Outbox);
    } else {
        return CMailIterator(CVector<CMail>(0));
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
void CMailServer::sendMailToMailBox ( const CMail & m, bool typeOfMailBox ){
    TMailBox tmp;
    typeOfMailBox ? tmp.m_Name = m.m_To : tmp.m_Name = m.m_From;
    typeOfMailBox ? tmp.m_Inbox.push_back(m) : tmp.m_Outbox.push_back(m);
    size_t idx = m_MailBoxes.lower_bound( tmp );
    if ( idx != m_MailBoxes.size() && m_MailBoxes[idx].m_Name == tmp.m_Name) {
        typeOfMailBox ? m_MailBoxes[idx].m_Inbox.push_back(m) : m_MailBoxes[idx].m_Outbox.push_back(m);
    } else {
        m_MailBoxes.insert( tmp, idx );
    }
}

#ifndef __PROGTEST__
bool matchOutput ( const CMail & m, const char * str )
{
    ostringstream oss;
    oss << m;
    return oss . str () == str;
}

int main ( void )
{
    /*
    char from[100], to[100], body[1024];

    assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );

    CMailServer s0;
    s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );

    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "thomas", sizeof ( to ) );
    strncpy ( body, "another important mail", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );

    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "alice", sizeof ( to ) );
    strncpy ( body, "deadline notice", sizeof ( body ) );
    s0 . sendMail ( CMail ( from, to, body ) );
    s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
    s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
    CMailIterator i0 = s0 . inbox ( "alice" );
    assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i0 );

    CMailIterator i1 = s0 . inbox ( "john" );
    assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
    assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
    assert ( ! ++i1 );

    CMailIterator i2 = s0 . outbox ( "john" );
    assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ! ++i2 );

    CMailIterator i3 = s0 . outbox ( "thomas" );
    assert ( ! i3 );

    CMailIterator i4 = s0 . outbox ( "steve" );
    assert ( ! i4 );

    CMailIterator i5 = s0 . outbox ( "thomas" );
    s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
    assert ( ! i5 );

    CMailIterator i6 = s0 . outbox ( "thomas" );
    assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
    assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
    assert ( ! ++i6 );

    CMailIterator i7 = s0 . inbox ( "alice" );
    s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
    assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ! ++i7 );

    CMailIterator i8 = s0 . inbox ( "alice" );
    assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ! ++i8 );

    CMailServer s1 ( s0 );
    s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
    s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
    CMailIterator i9 = s0 . inbox ( "alice" );
    assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ! ++i9 );

    CMailIterator i10 = s1 . inbox ( "alice" );
    assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
    assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
    assert ( ! ++i10 );

    CMailServer s2;
    s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
    CMailIterator i11 = s2 . inbox ( "alice" );
    assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
    assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
    assert ( ! ++i11 );

    s2 = s0;
    s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
    s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
    CMailIterator i12 = s0 . inbox ( "alice" );
    assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
    assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
    assert ( ! ++i12 );

    CMailIterator i13 = s2 . inbox ( "alice" );
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
    assert ( ! ++i13 );
     */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

