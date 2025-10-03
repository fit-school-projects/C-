#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CPersonalAgenda
{
public:
    CPersonalAgenda  ( ) = default;
    ~CPersonalAgenda ( ) = default;
    bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
    {
        pair < string, string > fullName = make_pair(name, surname);
        CEmployee newEmployee = {name, surname, email, salary};

        auto itNames = lower_bound(m_EmployeesSortedByName.begin(), m_EmployeesSortedByName.end(), fullName , compareName);
        if (itNames != m_EmployeesSortedByName.end() && itNames->name == name && itNames->surname == surname)
            return false;
        auto itEmail = lower_bound(m_EmployeesSortedByMail.begin(), m_EmployeesSortedByMail.end(), email, compareMail);
        if (itEmail != m_EmployeesSortedByMail.end() && itEmail->email == email)
            return false;

        m_EmployeesSortedByName.insert(itNames, newEmployee);
        m_EmployeesSortedByMail.insert(itEmail, newEmployee);

        auto itSalary = lower_bound(m_Salaries.begin(), m_Salaries.end(), salary, compareSalary);


        if (itSalary != m_Salaries.end() && itSalary->salary == salary)
            itSalary->count++;
        else
            m_Salaries.insert(itSalary, {salary, 1});

        return true;
    }
    bool del ( const string & name, const string & surname )
    {
        int i = findIndexByName(m_EmployeesSortedByName, name, surname);
        if (i == -1)
            return false;
        string mail = m_EmployeesSortedByName[i].email;
        deleteSalary(m_EmployeesSortedByName[i].salary);
        m_EmployeesSortedByName.erase(m_EmployeesSortedByName.begin() + i);
        m_EmployeesSortedByMail.erase(m_EmployeesSortedByMail.begin() + findIndexByMail(m_EmployeesSortedByMail, mail));

        return true;
    }
    bool del ( const string & email )
    {
        int i = findIndexByMail(m_EmployeesSortedByMail, email);
        if (i == -1)
            return false;
        string name = m_EmployeesSortedByMail[i].name;
        string surname = m_EmployeesSortedByMail[i].surname;
        deleteSalary(m_EmployeesSortedByMail[i].salary);
        m_EmployeesSortedByMail.erase(m_EmployeesSortedByMail.begin() + i);

        m_EmployeesSortedByName.erase(m_EmployeesSortedByName.begin() + findIndexByName(m_EmployeesSortedByName, name, surname));
        return true;
    }
    bool changeName ( const string & email, const string & newName, const string & newSurname )
    {
        int i = findIndexByMail(m_EmployeesSortedByMail, email);
        if (i == -1)
            return false;
        unsigned int salary = m_EmployeesSortedByMail[i].salary;
        int j = findIndexByName(m_EmployeesSortedByName, newName, newSurname);
        if (j != -1)
            return false;
        this->del(email);
        if (!this->add(newName, newSurname, email, salary))
            return false;
        return true;
    }
    bool changeEmail ( const string & name, const string & surname, const string & newEmail )
    {
        int i = findIndexByName(m_EmployeesSortedByName, name, surname);
        if (i == -1)
            return false;
        unsigned int salary = m_EmployeesSortedByName[i].salary;
        int j = findIndexByMail(m_EmployeesSortedByMail, newEmail);
        if (j != -1)
            return false;
        this->del(name, surname);
        if (!this->add(name, surname, newEmail, salary))
            return false;
        return true;
    }
    bool setSalary ( const string & name, const string & surname, unsigned int salary )
    {
        int i = findIndexByName(m_EmployeesSortedByName, name, surname);
        if (i == -1)
            return false;
        changeSalary(m_EmployeesSortedByName[i].salary, salary);
        m_EmployeesSortedByName[i].salary = salary;
        string mail = m_EmployeesSortedByName[i].email;
        m_EmployeesSortedByMail[findIndexByMail(m_EmployeesSortedByMail, mail)].salary = salary;
        return true;
    }
    bool setSalary ( const string & email, unsigned int salary )
    {
        int i = findIndexByMail(m_EmployeesSortedByMail, email);
        if (i == -1)
            return false;
        changeSalary(m_EmployeesSortedByMail[i].salary, salary);
        m_EmployeesSortedByMail[i].salary = salary;
        string name = m_EmployeesSortedByMail[i].name;
        string surname = m_EmployeesSortedByMail[i].surname;
        m_EmployeesSortedByName[findIndexByName(m_EmployeesSortedByName, name, surname)].salary = salary;
        return true;
    }
    unsigned int  getSalary ( const string & name, const string & surname ) const
    {
        int i = findIndexByName(m_EmployeesSortedByName, name, surname);
        return i == -1 ? 0 : m_EmployeesSortedByName[i].salary;
    }
    unsigned int  getSalary ( const string & email ) const
    {
        int i = findIndexByMail(m_EmployeesSortedByMail, email);
        return i == -1 ? 0 : m_EmployeesSortedByMail[i].salary;
    }
    bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
    {
        int i = findIndexByName(m_EmployeesSortedByName, name, surname);
        if (i == -1)
            return false;
        unsigned int salary = m_EmployeesSortedByName[i].salary;
        setRank(salary, rankMin, rankMax);
        return true;
    }
    bool getRank ( const string & email, int & rankMin, int & rankMax ) const
    {
        int i = findIndexByMail(m_EmployeesSortedByMail, email);
        if (i == -1)
            return false;
        unsigned int salary = m_EmployeesSortedByMail[i].salary;
        setRank(salary, rankMin, rankMax);
        return true;
    }
    bool getFirst ( string & outName, string & outSurname ) const
    {
        if (!m_EmployeesSortedByName.empty()){
            outName = m_EmployeesSortedByName[0].name;
            outSurname = m_EmployeesSortedByName[0].surname;
            return true;
        }
        return false;
    }
    bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
    {
        pair <string, string> fullName(name, surname);
        auto it = lower_bound(m_EmployeesSortedByName.begin(), m_EmployeesSortedByName.end(), fullName, compareName);
        if (it != m_EmployeesSortedByName.end() && it->name == name && it->surname == surname){
            it++;
            if (it != m_EmployeesSortedByName.end()){
                outName = it->name;
                outSurname = it->surname;
                return true;
            }
        }
        return false;
    }
private:
    struct CEmployee{
        string name;
        string surname;
        string email;
        unsigned int salary;
    };
    struct CSalaries{
        unsigned int salary;
        int count;
    };
    vector <CEmployee> m_EmployeesSortedByName;
    vector <CEmployee> m_EmployeesSortedByMail;
    vector <CSalaries> m_Salaries;

    static bool compareName(const CEmployee &a, const pair <string, string> & b)
    {
        if (a.surname == b.second)
            return a.name < b.first;
        return a.surname < b.second;
    }

    static bool compareMail(const CEmployee &a, const string & b){
        return a.email < b;
    }
    static bool compareSalary(const CSalaries &a, const unsigned int & b){
        return a.salary < b;
    }
    static int findIndexByName(const vector <CEmployee> &employees, const string &name, const string &surname){
        pair<string, string> fullName = make_pair(name, surname);
        auto it = lower_bound(employees.begin(), employees.end(), fullName, compareName);
        if (it != employees.end() && it->name == name && it->surname == surname)
            return it - employees.begin();
        return -1;
    }
    static int findIndexByMail(const vector <CEmployee> &employees, const string & mail){
        auto it = lower_bound(employees.begin(), employees.end(), mail, compareMail);
        if (it != employees.end() && it->email == mail)
            return it - employees.begin();
        return -1;
    }
    void changeSalary(unsigned int salary, unsigned int newSalary){
        auto it = lower_bound(m_Salaries.begin(), m_Salaries.end(), salary, compareSalary);
        if (it != m_Salaries.end() && it->salary == salary){
            it->count--;
            if (it->count == 0)
                m_Salaries.erase(it);
        }
        it = lower_bound(m_Salaries.begin(), m_Salaries.end(), newSalary, compareSalary);
        if (it != m_Salaries.end() && it->salary == newSalary){
            it->count++;
        }
        else{
            CSalaries s{};
            s.salary = newSalary;
            s.count = 1;
            m_Salaries.insert(it, s);
        }
    }
    void deleteSalary(unsigned int salary) {
        auto it = lower_bound(m_Salaries.begin(), m_Salaries.end(), salary, compareSalary);
        if (it != m_Salaries.end() && it->salary == salary) {
            it->count--;
            if (it->count == 0)
                m_Salaries.erase(it);
        }
    }
    void setRank(unsigned int salary, int &rankMin, int &rankMax) const {
        auto it = lower_bound(m_Salaries.begin(), m_Salaries.end(), salary, compareSalary);

        rankMin = 0;
        for (auto it2 = m_Salaries.begin(); it2 != it; it2++)
            rankMin += it2->count;

        rankMax = rankMin + it->count - 1;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    /*
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );

    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );

    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );

    assert ( b1 . setSalary ( "john", 32000 ) );

    assert ( b1 . getSalary ( "john" ) ==  32000 );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );

    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );

    assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . getSalary ( "john" ) ==  35000 );

    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );

    assert ( b1 . changeName ( "peter", "James", "Bond" ) );

    assert ( b1 . getSalary ( "peter" ) ==  23000 );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );

    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );

    assert ( b1 . changeEmail ( "James", "Bond", "james" ) );

    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "james" ) ==  23000 );
    assert ( b1 . getSalary ( "peter" ) ==  0 );
    assert ( b1 . del ( "james" ) );

    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );

    assert ( b1 . del ( "John", "Miller" ) );

    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );

    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . del ( "john" ) );
    assert ( ! b1 . getFirst ( outName, outSurname ) );
    assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . getFirst ( outName, outSurname ) );
    assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . setSalary ( "joe", 90000 ) );
    assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . getSalary ( "joe" ) ==  0 );

    assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . getRank ( "joe", lo, hi ) );

    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . del ( "Joe", "Black" ) );
    assert ( ! b2 . del ( "joe" ) );
    assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );

    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . setSalary ( "peter", 37000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  0 );
    assert ( ! b2 . getRank ( "peter", lo, hi ) );
    assert ( ! b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . del ( "peter" ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  40000 );

    assert (b2.changeName("peter", "Peter", "Falcon"));
    assert (b2.changeEmail("Peter", "Falcon", "peter2"));
    assert (b2.setSalary("Peter", "Falcon", 37000));
    assert (b2.getSalary("Peter", "Falcon") == 37000);
    assert (b2.getRank("Peter", "Falcon", lo, hi));

    assert (b2.del("Peter", "Falcon"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);
    assert (b2.getRank("peter", lo, hi));

    assert (b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);
    assert(b2.changeName("peter", "Peter", "Falcon"));
    assert (b2.changeName("peter", "Peter", "Falconooo"));
    assert (b2.changeEmail("Peter", "Falconooo", "peter2"));
    assert (b2.setSalary("Peter", "Falconooo", 37000));
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
