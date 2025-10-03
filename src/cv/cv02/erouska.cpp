#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
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
        CEFaceMask & addContact ( const CContact & contact ){
            contacts.push_back(contact);
            return *this;
        }
        vector<int> listContacts(int id) const {
            vector<int> result;
            for (auto contact : contacts){
                if (contact.id1 == contact.id2)
                    continue;
                if (contact.id1 == id && find(result.begin(), result.end(), contact.id2) == result.end())
                    result.push_back(contact.id2);
                else if (contact.id2 == id && find(result.begin(), result.end(), contact.id1) == result.end())
                    result.push_back(contact.id1);
            }
            return result;
        }

        vector<int> listContacts ( int id, const CTimeStamp & from, const CTimeStamp & to ) const {
            vector<int> result;
            for (auto contact : contacts){
                if (contact.id1 == contact.id2 || tie(contact.time.year, contact.time.month, contact.time.day, contact.time.hour, contact.time.minute, contact.time.second) < tie(from.year, from.month, from.day, from.hour, from.minute, from.second) ||
                    tie(contact.time.year, contact.time.month, contact.time.day, contact.time.hour, contact.time.minute, contact.time.second) > tie(to.year, to.month, to.day, to.hour, to.minute, to.second))
                    continue;
                
                if (contact.id1 == id && find(result.begin(), result.end(), contact.id2) == result.end())
                    result.push_back(contact.id2);
                else if (contact.id2 == id && find(result.begin(), result.end(), contact.id1) == result.end())
                    result.push_back(contact.id1);
            }
            return result;
        }

    private:
        vector<CContact> contacts; 
};

#ifndef __PROGTEST__

int main ()
{

    CEFaceMask test;

    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
        . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
    
    assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
    assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
    assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
    
    assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
    assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
    assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
    
    return 0;
}
#endif /* __PROGTEST__ */
