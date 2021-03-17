#include "../srcs/set/set.hpp"
#include "catch.hpp"
#include <set>
#include <array>
#include <list>
#include <string>

#ifdef VERIFY_UNIT_TESTS
# define LIB std
#else
# define LIB ft
#endif

/* CONSTRUCTION */
TEST_CASE("set contructors work as expected", "[set][basics]")
{
    SECTION("default constructor constructs an empty set") {
        LIB::set<int> set;
        REQUIRE( set.empty() );
        REQUIRE( set.size() == 0 );
    }

    SECTION("range constructor fills with corresponding element in range") {
        std::list<int> stl;

        for (int i = 0; i < 25; ++i)
            stl.push_back(rand());

        LIB::set<int> my_set (stl.begin(), stl.end());
        REQUIRE( stl.size() == my_set.size() );
        REQUIRE_FALSE( my_set.empty() );

        stl.sort();
        stl.unique();
        auto stl_it = stl.begin();
        auto my_it = my_set.begin();
        for (; stl_it != stl.end(); ++my_it, ++stl_it)
            REQUIRE( *my_it == *stl_it );
    }
    SECTION("copy constructor constructs a copy with a copy of each element") {
        LIB::set<int> my_set1;
        for (int i = 0; i < 25; ++i)
            my_set1.insert(rand());
        LIB::set<int> my_set2 (my_set1);

        REQUIRE_FALSE( my_set2.empty() );
        REQUIRE( my_set2.size() == my_set1.size() );

        auto it1 = my_set1.begin();
        auto it2 = my_set2.begin();
        for (; it1 != my_set1.end(); ++it1, ++it2)
            REQUIRE( *it1 == *it2 );
    }
}

TEST_CASE("set assignment operator copies elements", "[set][operators]")
{
    LIB::set<int> my_set1;

    for (int i = 0; i < 10; ++i)
        my_set1.insert(rand());

    LIB::set<int> my_set2 = my_set1;

    REQUIRE( my_set2.size() == my_set1.size() );

    auto it1 = my_set1.begin();
    auto it2 = my_set2.begin();
    for (; it1 != my_set1.end(); ++it1, ++it2)
        REQUIRE( *it1 == *it2 );
}

TEST_CASE("there are no duplicates in a set", "[set][basics]")
{
    LIB::set<int> set;
    std::list<int> list;

    for (int i = 0; i < 10000; ++i) {
        int rand = std::rand() % 5000;
        list.push_back(rand);
        set.insert(rand);
    }
    list.sort();
    list.unique();
    REQUIRE( set.size() == list.size() );
    REQUIRE( (unsigned long)std::distance(set.begin(), set.end()) == set.size() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("set relational operators work correctly", "[set][operators]")
{
    std::array<char, 4> a = { 'b', 'z', 'p', 'a' };
    std::array<char, 4> b = { 'b', 'z', 'c', 'z' };

    LIB::set<char> myset1 (a.begin(), a.end());
    LIB::set<char> myset2 (a.begin(), a.begin() + 2);
    LIB::set<char> myset3;
    LIB::set<char> myset4 (b.begin(), b.end());

    // myset1 vs myset2
    REQUIRE_FALSE( myset1 == myset2 );
    REQUIRE( myset1 != myset2 );
    REQUIRE( myset1 < myset2 );
    REQUIRE( myset1 <= myset2 );
    REQUIRE_FALSE( myset1 > myset2 );
    REQUIRE_FALSE( myset1 >= myset2 );

    // myset1 vs myset1
    REQUIRE( myset1 == myset1 );
    REQUIRE_FALSE( myset1 != myset1 );
    REQUIRE_FALSE( myset1 < myset1 );
    REQUIRE( myset1 <= myset1 );
    REQUIRE_FALSE( myset1 > myset1 );
    REQUIRE( myset1 >= myset1 );

    // myset1 vs myset3
    REQUIRE_FALSE( myset1 == myset3 );
    REQUIRE( myset1 != myset3 );
    REQUIRE_FALSE( myset1 < myset3 );
    REQUIRE_FALSE( myset1 <= myset3 );
    REQUIRE( myset1 > myset3 );
    REQUIRE( myset1 >= myset3 );

    // myset3 vs myset3
    REQUIRE( myset3 == myset3 );
    REQUIRE_FALSE( myset3 != myset3 );
    REQUIRE_FALSE( myset3 < myset3 );
    REQUIRE( myset3 <= myset3 );
    REQUIRE_FALSE( myset3 > myset3 );
    REQUIRE( myset3 >= myset3 );

    // myset1 vs myset4
    REQUIRE_FALSE( myset1 == myset4 );
    REQUIRE( myset1 != myset4 );
    REQUIRE( myset1 < myset4 );
    REQUIRE( myset1 <= myset4 );
    REQUIRE_FALSE( myset1 > myset4 );
    REQUIRE_FALSE( myset1 >= myset4 );
}

/* ITERATORS */
TEST_CASE("set iterators works correctly", "[set][iterators]")
{
    SECTION("iterators can be created as expected") {
        LIB::set<int> my_set;

        LIB::set<int>::iterator it = my_set.begin();
        LIB::set<int>::const_iterator cit = it;
        LIB::set<int>::const_iterator cit2 = my_set.begin();
        REQUIRE(( cit == my_set.begin() ));
        REQUIRE(( cit2 == my_set.begin() ));

        LIB::set<int>::reverse_iterator rit = my_set.rbegin();
        LIB::set<int>::const_reverse_iterator rcit = rit;
        LIB::set<int>::const_reverse_iterator rcit2 = my_set.rbegin();
        REQUIRE(( rcit == my_set.rbegin() ));
        REQUIRE(( rcit2 == my_set.rbegin() ));
    }

    SECTION("iterators allow in-order access to the set elements") {
        int rand = 0;
        LIB::set<int> set;
        std::list<int> list;

        for (int i = 0; i < 100; ++i) {
            rand = std::rand() % 1000;
            list.push_back(rand);
            set.insert(rand);
        }

        list.sort();
        list.unique();
        REQUIRE( std::distance(set.begin(), set.end()) == std::distance(list.begin(), list.end()) );

        auto it = set.begin();
        auto lit = list.begin();
        for (; it != set.end(); ++it, ++lit)
            REQUIRE( *it == *lit );
    }
}

/* MODIFIERS */
TEST_CASE("set insert works as expected", "[set][modifiers]")
{
    LIB::set<int> my_set;
    std::pair<LIB::set<int>::iterator, bool> ret;
    LIB::set<int>::iterator it_ret;

    SECTION("single element insert add the pair to the set if its not present") {
        REQUIRE( my_set.empty() );

        ret = my_set.insert(32);
        REQUIRE( my_set.size() == 1);
        REQUIRE( *ret.first == 32 );
        REQUIRE( ret.second );

        ret = my_set.insert(32);
        REQUIRE( my_set.size() == 1);
        REQUIRE( *ret.first == 32 );
        REQUIRE_FALSE( ret.second );

        ret = my_set.insert(21);
        REQUIRE( my_set.size() == 2);
        REQUIRE( *ret.first == 21 );
        REQUIRE( ret.second );
    }
    SECTION("position hint insert adds the pair regardless of the validity of hint") {

        for (int i = 0; i < 10; ++i) {
            if (i % 2 == 0)
                my_set.insert(i);           // { 0, 2, 4, 6, 8 }
        }
        my_set.insert(-12);
        my_set.insert(42);
        REQUIRE( my_set.size() == 7 );      // { -12, 0, 2, 4, 6, 8, 42 }

        //valid hint and key not in set
        auto it = my_set.insert(my_set.find(4), 5);
        REQUIRE( my_set.size() == 8 );
        REQUIRE( *it == 5 );
        //valid hint but key already in set
        it = my_set.insert(my_set.find(0), 2);
        REQUIRE( my_set.size() == 8 );
        REQUIRE( *it == 2 );
        it = my_set.insert(my_set.find(6), 6);
        REQUIRE( my_set.size() == 8 );
        REQUIRE( *it == 6 );
        //invalid hint but key in set
        it = my_set.insert(my_set.find(-12), 42);
        REQUIRE( my_set.size() == 8 );
        REQUIRE( *it == 42 );
        //invalid hint but key not in set
        it = my_set.insert(my_set.find(-12), 3);
        REQUIRE( my_set.size() == 9 );
        REQUIRE( *it == 3 );
        it = my_set.insert(my_set.find(42), -5);
        REQUIRE( my_set.size() == 10 );
        REQUIRE( *it == -5 );
        it = my_set.insert(my_set.find(-123), 123);
        REQUIRE( my_set.size() == 11 );
        REQUIRE( *it == 123 );

        // { -12, -5, 0, 2, 3, 4, 5, 6, 8, 42, 123 }
        //assert content is the sum of inserts
        std::array<int, 11> arr = { -12, -5, 0, 2, 3, 4, 5, 6, 8, 42, 123 };
        auto setit = my_set.begin();
        auto arrit = arr.begin();
        for (; setit != my_set.end(); ++setit, ++arrit)
            REQUIRE( *setit == *arrit );
    }
    SECTION("range insert inserts a copy of the ranges elements") {
        std::vector<char> v;
        for (char i = 'a'; i < 'z'; ++i)
            v.push_back(i);

        auto first = v.begin() + 5;
        auto last = v.begin() + 15;
        my_set.insert(first, last);
        REQUIRE( (my_set.size() == (unsigned long)std::distance(first, last)) );
        for (auto it = my_set.begin(); it != my_set.end(); ++it, ++first) {
            REQUIRE( *it == *first );
            REQUIRE( *it == *first );
        }
    }
}

TEST_CASE("set erase works as expected", "[set][modifiers]")
{
    std::list<char> data = { 'a', '1', '%', '#', 'z', 'b', 'c', 'd', '@', 'r', 'e'};
    LIB::set<char> my_set (data.begin(), data.end());
    data.sort();

    SECTION("erase at iterator removes correct element") {
        my_set.erase(my_set.begin());
        my_set.erase(++++my_set.begin());
        my_set.erase(--my_set.end());
        my_set.erase(++++++my_set.begin());
        data.erase(data.begin());
        data.erase(++++data.begin());
        data.erase(--data.end());
        data.erase(++++++data.begin());

        REQUIRE( my_set.size() == data.size() );
        auto it = my_set.begin();
        auto lit = data.begin();
        for (; it != my_set.end(); ++it, ++lit)
            REQUIRE( *it == *lit );
    }
    SECTION("erase removes correct element when given a key") {
        int sum = 0;
        int ret = 0;
        ret = my_set.erase('b');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_set.erase('b');
        REQUIRE( ret == 0 );
        ret = my_set.erase('a');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_set.erase('z');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_set.erase('z');
        REQUIRE( ret == 0 );
        ret = my_set.erase('/');
        REQUIRE( ret == 0 );
        ret = my_set.erase('$');
        REQUIRE( ret == 0 );
        REQUIRE( my_set.size() == data.size() - sum );
    }
    SECTION("erase iterator range") {
        my_set.erase(my_set.end(), my_set.end());
        my_set.erase(++++my_set.begin(), --my_set.end());
        data.erase(++++data.begin(), --data.end());
        REQUIRE( my_set.size() == 3 );
        REQUIRE( my_set.size() == data.size() );
        auto it = my_set.begin();
        auto lit = data.begin();
        for (; it != my_set.end(); ++it, ++lit)
            REQUIRE( *it == *lit );
        my_set.erase(my_set.begin(), my_set.end());
        REQUIRE( my_set.empty() );
    }
}

TEST_CASE("set swap works as expected", "[set][modifiers]")
{
    LIB::set<std::string> my_set1;
    LIB::set<std::string> my_set2;
    my_set1.insert("fifteen");
    my_set1.insert("dog");
    my_set1.insert("78");
    my_set1.insert("34");
    my_set1.insert("autobahn");
    my_set2.swap(my_set1);

    REQUIRE( my_set1.size() == 0 );
    REQUIRE( my_set2.size() == 5 );

    REQUIRE( *my_set2.begin() == "34" );
    REQUIRE( *++my_set2.begin() == "78" );
    REQUIRE( *++++my_set2.begin() == "autobahn" );
    REQUIRE( *----my_set2.end() == "dog" );
    REQUIRE( *--my_set2.end() == "fifteen" );

    my_set2.erase(++my_set2.begin(), my_set2.end());
    my_set2.swap(my_set1);
    REQUIRE( my_set1.size() == 1 );
    REQUIRE( my_set2.size() == 0 );
}

TEST_CASE("set clear works as expected", "[set][modifiers]")
{
    LIB::set<int> my_set;
    my_set.clear();
    REQUIRE( my_set.empty() );
    my_set.insert(1);
    my_set.insert(2);
    my_set.insert(3);
    my_set.insert(4);

    REQUIRE( my_set.size() == 4 );
    my_set.clear();
    REQUIRE( my_set.empty() );
    my_set.insert(2);
    REQUIRE( my_set.size() == 1 );
    my_set.clear();
    REQUIRE( my_set.empty() );
    REQUIRE(( my_set.begin() == my_set.end() ));
}

/* OBSERVERS */
TEST_CASE("set value_comp & key_comp work as expected", "[set][observers]")
{
    LIB::set<int> myset;
    LIB::set<int>::key_compare kc = myset.key_comp();
    LIB::set<int>::value_compare vc = myset.value_comp();

    myset.insert(10);
    myset.insert(20);

    REQUIRE( kc(*myset.begin(), (*++myset.begin())) );
    REQUIRE_FALSE( kc((*++myset.begin()), *myset.begin()) );
    REQUIRE_FALSE( kc(*myset.begin(), *myset.begin()) );
    REQUIRE_FALSE( kc(*myset.begin(), (*----myset.end())) );
    REQUIRE_FALSE( kc((*----myset.end()), *myset.begin()) );

    REQUIRE( vc(*myset.begin(), *++myset.begin()) );
    REQUIRE_FALSE( vc(*++myset.begin(), *myset.begin()) );
    REQUIRE_FALSE( vc(*myset.begin(), *myset.begin()) );
    REQUIRE_FALSE( vc(*myset.begin(), *(----myset.end())) );
    REQUIRE_FALSE( vc(*(----myset.end()), *myset.begin()) );
}

/* OPERATIONS */
TEST_CASE("set find works as expected", "[set][operations]")
{
    LIB::set<int> myset;
    myset.insert(20);
    myset.insert(80);

    REQUIRE(( myset.find(20) == myset.begin() ));
    REQUIRE( *myset.find(20) == 20 );
    REQUIRE( *++myset.find(20) == 80 );
    REQUIRE( ++++myset.find(20) == myset.end() );

    REQUIRE( --myset.find(80) == myset.begin() );
    REQUIRE( *myset.find(80) == 80 );
    REQUIRE( *--myset.find(80) == 20 );
    REQUIRE(( ++myset.find(80) == myset.end() ));
    REQUIRE(( myset.find(100) == myset.end() ));

    myset.erase(20);
    REQUIRE(( myset.find(20) == myset.end() ));
}

TEST_CASE("set count works as expected", "[set][operations]")
{
    LIB::set<int> myset;
    myset.insert(20);
    myset.insert(80);

    REQUIRE( myset.count(20) == 1 );
    REQUIRE( myset.count(80) == 1 );
    REQUIRE( myset.count(8) == 0 );
    REQUIRE( myset.count(-12) == 0 );
    myset.erase(20);
    REQUIRE( myset.count(20) == 0 );
}

TEST_CASE("set lower_bound & upper_bound work as expected", "[set][operations]")
{
    LIB::set<int> myset;

    SECTION("reference example") {
        myset.insert(20);
        myset.insert(40);
        myset.insert(60);
        myset.insert(80);
        myset.insert(100);
        LIB::set<int>::iterator itlow = myset.lower_bound (40);
        LIB::set<int>::iterator itup = myset.upper_bound (80);

        REQUIRE( *itlow == 40 );
        REQUIRE( *itup == 100 );
        myset.erase(itlow, itup);
        REQUIRE( myset.size() == 2 );
        REQUIRE( *myset.begin() == 20 );
        REQUIRE( *--myset.end() == 100 );
        REQUIRE(( myset.upper_bound(120) == myset.end() ));
    }
    SECTION("") {
        myset.insert(10);
        myset.insert(20);
        myset.insert(40);
        myset.insert(50);

        REQUIRE(( *myset.lower_bound(20) == 20 ));
        REQUIRE(( myset.lower_bound(60) == myset.end() ));
        REQUIRE(( myset.upper_bound(30) == myset.lower_bound(30) ));
        REQUIRE(( *myset.upper_bound(20) == 40 ));
    }
    SECTION("const versions") {
        myset.insert(20);
        myset.insert(40);
        myset.insert(80);
        myset.insert(100);
        myset.insert(60);
        const LIB::set<int> cmyset (myset);

        REQUIRE(( cmyset.upper_bound(50) == cmyset.lower_bound(50) ));
        REQUIRE(( *cmyset.upper_bound(20) == 40 ));
        REQUIRE(( *cmyset.upper_bound(40) == 60 ));
        REQUIRE(( cmyset.upper_bound(420) == cmyset.end() ));
        REQUIRE(( *cmyset.lower_bound(80) == 80 ));
        REQUIRE(( cmyset.lower_bound(420) == cmyset.end() ));
    }
}

TEST_CASE("set equal_range works as expected", "[set][operations]")
{
    LIB::set<int> myset;
    myset.insert(10);
    myset.insert(20);
    myset.insert(30);
    const LIB::set<int> cmyset (myset);

    std::pair<LIB::set<int>::iterator, LIB::set<int>::iterator> ret = myset.equal_range(20);
    REQUIRE( *ret.first == 20 );
    REQUIRE( *ret.second == 30 );
    REQUIRE(( *myset.equal_range(30).first == 30 ));
    REQUIRE(( myset.equal_range(30).second == myset.end() ));
    REQUIRE(( myset.equal_range(50).first == myset.end() ));
    REQUIRE(( myset.equal_range(50).second == myset.end() ));

    std::pair<LIB::set<int>::const_iterator, LIB::set<int>::const_iterator> cret = cmyset.equal_range(30);

    REQUIRE( *cret.first == 30 );
    REQUIRE( cret.second == cmyset.end() );
    REQUIRE(( *cmyset.equal_range(10).first == 10 ));
    REQUIRE(( *cmyset.equal_range(10).second == 20 ));
    REQUIRE(( cmyset.equal_range(50).first == cmyset.end() ));
    REQUIRE(( cmyset.equal_range(50).second == cmyset.end() ));
}
