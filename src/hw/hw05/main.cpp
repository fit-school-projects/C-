#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
public:
    //---------------------------------------------------------------------------------------------
    CDate                         ( int               y,
                                    int               m,
                                    int               d )
            : m_Year ( y ),
              m_Month ( m ),
              m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
        if ( m_Year != x . m_Year )
            return m_Year - x . m_Year;
        if ( m_Month != x . m_Month )
            return m_Month - x . m_Month;
        return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const
    {
        return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const
    {
        return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const
    {
        return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
        char oldFill = os . fill ();
        return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-"
                  << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-"
                  << setw ( 2 ) << static_cast<int> ( x . m_Day )
                  << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

class CInvoice
{
public:
    CInvoice ( const CDate & date, const string & seller, const string & buyer, unsigned int amount, double vat )
        : m_date(date), m_seller(seller), m_buyer(buyer), m_amount(amount), m_vat(vat) {
    }
    CDate date ( void ) const{
        return this->m_date;
    }
    string buyer ( void ) const{
        return this->m_buyer;
    }
    string seller ( void ) const{
        return this->m_seller;
    }
    unsigned int amount ( void ) const{
        return this->m_amount;
    }
    double vat ( void ) const{
        return this->m_vat;
    }
    size_t id ( void ) const{
        return this->m_id;
    }
    void setId ( size_t id ){
        this->m_id = id;
    }

    void setBuyer ( const string & buyer ){
        this->m_buyer = buyer;

    }
    void setSeller ( const string & seller ){
        this->m_seller = seller;
    }
    bool operator < (const CInvoice & x) const{
        if (this->m_date.compare(x.m_date) != 0)
            return this->m_date.compare(x.m_date) < 0;
        if (this->m_seller.compare(x.m_seller) != 0)
            return this->m_seller.compare(x.m_seller) < 0;
        if (this->m_buyer.compare(x.m_buyer) != 0)
            return this->m_buyer.compare(x.m_buyer) < 0;
        if (this->m_amount != x.m_amount)
            return this->m_amount < x.m_amount;
        return this->m_vat < x.m_vat;
    }
private:
    CDate m_date = CDate(0,0,0);
    string m_seller;
    string m_buyer;
    unsigned int m_amount;
    double m_vat;
    size_t m_id;
};

class CSortOpt
{
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;
    CSortOpt ( void ) = default;
    CSortOpt & addKey ( int key, bool ascending = true ){
        m_keys.push_back(make_pair(key, ascending));
        return *this;
    }
    bool operator () ( const CInvoice & x, const CInvoice & y ) const{
        for (auto & key : m_keys){
            if (key.first == BY_DATE){
                if (x.date().compare(y.date()) != 0)
                    return key.second ? x.date().compare(y.date()) < 0 : x.date().compare(y.date()) > 0;
            }
            else if (key.first == BY_BUYER){
                if (x.buyer().compare(y.buyer()) != 0){
                    string xBuyer = x.buyer();
                    string yBuyer = y.buyer();
                    transform(xBuyer.begin(), xBuyer.end(), xBuyer.begin(), ::tolower);
                    transform(yBuyer.begin(), yBuyer.end(), yBuyer.begin(), ::tolower);
                    return key.second ? xBuyer.compare(yBuyer) < 0 : xBuyer.compare(yBuyer) > 0;
                }
            }
            else if (key.first == BY_SELLER){
                if (x.seller().compare(y.seller()) != 0){
                    string xSeller = x.seller();
                    string ySeller = y.seller();
                    transform(xSeller.begin(), xSeller.end(), xSeller.begin(), ::tolower);
                    transform(ySeller.begin(), ySeller.end(), ySeller.begin(), ::tolower);
                    return key.second ? xSeller.compare(ySeller) < 0 : xSeller.compare(ySeller) > 0;
                }
            }
            else if (key.first == BY_AMOUNT){
                if (x.amount() != y.amount())
                    return key.second ? x.amount() < y.amount() : x.amount() > y.amount();
            }
            else if (key.first == BY_VAT){
                if (x.vat() != y.vat())
                    return key.second ? x.vat() < y.vat() : x.vat() > y.vat();
            }
        }
        return x.id() < y.id();
    }
private:
    vector<pair<int, bool>> m_keys;
    friend class CVATRegister;
};

class CVATRegister
{
public:
    CVATRegister ( void ) = default;
    bool registerCompany ( const string & name ){
        string normalized = normalizeName(name);
        if (m_Companies.find(normalized) != m_Companies.end())
            return false;
        m_Companies[normalized] = name;
        return true;
    }
    bool addIssued ( const CInvoice  & x ){
        string normalizedSeller = normalizeName(x.seller());
        string normalizedBuyer = normalizeName(x.buyer());
        if (m_Companies.find(normalizedSeller) == m_Companies.end() || m_Companies.find(normalizedBuyer) == m_Companies.end() || normalizedSeller == normalizedBuyer)
            return false;
        CInvoice normalizedInvoice = CInvoice(x.date(), normalizedSeller, normalizedBuyer, x.amount(), x.vat());
        normalizedInvoice.setId(m_addedAt++);
        if ( m_registeredInvoices[normalizedSeller].find (normalizedInvoice) != m_registeredInvoices[normalizedSeller].end() ){
            if (!m_registeredInvoices[normalizedSeller][normalizedInvoice].second )
                return false;
            m_registeredInvoices[normalizedSeller][normalizedInvoice].first = true;
            m_registeredInvoices[normalizedBuyer][normalizedInvoice].first = true;
        }
        else{
            m_registeredInvoices[normalizedSeller][normalizedInvoice] = make_pair(false, 0);
            m_registeredInvoices[normalizedBuyer][normalizedInvoice] = make_pair(false, 0);
        }
        return true;
    }
    bool addAccepted ( const CInvoice  & x ){
        string normalizedSeller = normalizeName(x.seller());
        string normalizedBuyer = normalizeName(x.buyer());
        if (m_Companies.find(normalizedSeller) == m_Companies.end() || m_Companies.find(normalizedBuyer) == m_Companies.end() || normalizedSeller == normalizedBuyer)
            return false;
        CInvoice normalizedInvoice = CInvoice(x.date(), normalizedSeller, normalizedBuyer, x.amount(), x.vat());
        normalizedInvoice.setId(m_addedAt++);
        if ( m_registeredInvoices[normalizedSeller].find (normalizedInvoice) != m_registeredInvoices[normalizedSeller].end() ){
            if (m_registeredInvoices[normalizedSeller][normalizedInvoice].second )
                return false;
            m_registeredInvoices[normalizedSeller][normalizedInvoice].first = true;
            m_registeredInvoices[normalizedBuyer][normalizedInvoice].first = true;
        }
        else{
            m_registeredInvoices[normalizedSeller][normalizedInvoice] = make_pair(false, 1);
            m_registeredInvoices[normalizedBuyer][normalizedInvoice] = make_pair(false, 1);
        }
        return true;
    }
    bool delIssued ( const CInvoice  & x ){
        string normalizedSeller = normalizeName(x.seller());
        string normalizedBuyer = normalizeName(x.buyer());
        if (m_Companies.find(normalizedSeller) == m_Companies.end() || m_Companies.find(normalizedBuyer) == m_Companies.end() || normalizedSeller == normalizedBuyer)
            return false;
        CInvoice normalizedInvoice = CInvoice(x.date(), normalizedSeller, normalizedBuyer, x.amount(), x.vat());
        if ( m_registeredInvoices[normalizedSeller].find (normalizedInvoice) != m_registeredInvoices[normalizedSeller].end() ){
            if ( !m_registeredInvoices[normalizedSeller][normalizedInvoice].second && !m_registeredInvoices[normalizedSeller][normalizedInvoice].first){
                m_registeredInvoices[normalizedSeller].erase(normalizedInvoice);
                m_registeredInvoices[normalizedBuyer].erase(normalizedInvoice);
            }
            else if ( m_registeredInvoices[normalizedSeller][normalizedInvoice].first && !m_registeredInvoices[normalizedSeller][normalizedInvoice].second){
                m_registeredInvoices[normalizedSeller][normalizedInvoice].first = false;
                m_registeredInvoices[normalizedSeller][normalizedInvoice].second = 1;
                m_registeredInvoices[normalizedBuyer][normalizedInvoice].first = false;
                m_registeredInvoices[normalizedBuyer][normalizedInvoice].second = 1;
            } else
                return false;
        } else
            return false;
        return true;
    }
    bool delAccepted ( const CInvoice  & x ){
        string normalizedSeller = normalizeName(x.seller());
        string normalizedBuyer = normalizeName(x.buyer());
        if (m_Companies.find(normalizedSeller) == m_Companies.end() || m_Companies.find(normalizedBuyer) == m_Companies.end() || normalizedSeller == normalizedBuyer)
            return false;
        CInvoice normalizedInvoice = CInvoice(x.date(), normalizedSeller, normalizedBuyer, x.amount(), x.vat());
        if ( m_registeredInvoices[normalizedSeller].find (normalizedInvoice) != m_registeredInvoices[normalizedSeller].end() ){
            if ( m_registeredInvoices[normalizedSeller][normalizedInvoice].second && !m_registeredInvoices[normalizedSeller][normalizedInvoice].first){
                m_registeredInvoices[normalizedSeller].erase(normalizedInvoice);
                m_registeredInvoices[normalizedBuyer].erase(normalizedInvoice);
            }
            else if ( m_registeredInvoices[normalizedSeller][normalizedInvoice].first && !m_registeredInvoices[normalizedSeller][normalizedInvoice].second){
                m_registeredInvoices[normalizedSeller][normalizedInvoice].first = false;
                m_registeredInvoices[normalizedSeller][normalizedInvoice].second = 0;
                m_registeredInvoices[normalizedBuyer][normalizedInvoice].first = false;
                m_registeredInvoices[normalizedBuyer][normalizedInvoice].second = 0;
            } else
                return false;
        } else
            return false;
        return true;
    }
    list<CInvoice> unmatched ( const string & company, const CSortOpt & sortBy ) const{
        list<CInvoice> unmatchedInvoices = list<CInvoice>();
        string normalizedCompany = normalizeName(company);
        if (m_Companies.find(normalizedCompany) == m_Companies.end())
            return unmatchedInvoices;
        for (auto & invoice: m_registeredInvoices.at(normalizedCompany)){
            if (!invoice.second.first)
                unmatchedInvoices.push_back(invoice.first);
        }

        for (auto & invoice: unmatchedInvoices) {
            invoice.setBuyer(m_Companies.at(invoice.buyer()));
            invoice.setSeller(m_Companies.at(invoice.seller()));
        }
        unmatchedInvoices.sort(sortBy);

        return unmatchedInvoices;
    }
private:

    map<string, string> m_Companies;
    map<string, map <CInvoice, pair<bool, int>>> m_registeredInvoices;
    size_t m_addedAt = 0;

    string normalizeName (const string & name) const{
        string normalized;
        unique_copy(name.begin(), name.end(), back_insert_iterator<string>(normalized), [](char a, char b) { return isspace(a) && isspace(b); });
        if (isspace(normalized.front ()))
            normalized.erase(normalized.begin());
        if (isspace(normalized.back ()))
            normalized.pop_back();
        for (auto & c: normalized)
            c = tolower(c);
        return normalized;
    }
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
    if ( a . size () != b . size () )
        return false;
    for ( auto itA = a . begin (), itB = b . begin (); itA != a . end (); ++ itA, ++ itB )
        if ( itA -> date () .compare( itB -> date () ) != 0
             || itA -> seller () != itB -> seller ()
             || itA -> buyer () != itB -> buyer ()
             || itA -> amount () != itB -> amount ()
             || itA -> vat () != itB -> vat () )
            return false;
    return true;
}

int main ( void )
{
    /*
    CVATRegister r;
    assert ( r . registerCompany ( "first Company" ) );
    assert ( r . registerCompany ( "Second     Company" ) );
    assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
    assert ( r . registerCompany ( "Third Company, Ltd." ) );
    assert ( !r . registerCompany ( "Third Company, Ltd." ) );
    assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
    //r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ));

    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );

    assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
                                  } ) );

    assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
                          list<CInvoice>
                                  {
                                  } ) );

    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

    // My test data
    CVATRegister r2;
    r2 .registerCompany( "First Company" );
    r2 .registerCompany( " Second Company " );
    assert ( !r2 . registerCompany ( "second comPany" ));
    assert ( r2 . registerCompany ( "secon       d comPany" ));
    assert ( r2 . registerCompany ( " secondcomPany " ));
    assert ( !r2 . registerCompany ( "          second          cOMPany         " ));
    assert ( !r2 . registerCompany ( "second comPANy    " ));

    assert ( r2 . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( r2 . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Secon       d Company", 200, 30 ) ) );
    assert ( r2 . addAccepted( CInvoice ( CDate ( 2000, 1, 2 ), "First Company      ", "        sEcOnd     COmpany", 200, 30 ) ) );

    assert ( equalLists ( r2 . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                            list<CInvoice>
                                    {
                                        CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Second Company ", 200, 30.000000 ),
                                        CInvoice ( CDate ( 2000, 1, 2 ), "First Company", " Second Company ", 200, 30.000000 ),
                                        CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "secon       d comPany", 200, 30.000000 )
                                    } ) );

    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

