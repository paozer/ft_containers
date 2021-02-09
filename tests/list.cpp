#include "catch.hpp"

#include "../srcs/list/list.hpp"

#include <list>
#include <vector>
#include <iterator>

using templateTestList = std::tuple<int, char, std::string, std::list<int> >;

/* CONSTRUCTOR & DESTRUCTOR & = OPERATOR */

//TEST_CASE( "default constructor works correctly", "[list][basics]" );

TEST_CASE( "fill constructor works correctly", "[list][basics]" )
{
    SECTION( "list(0, 5) returns list w/ size 0" ) {
        ft::list<int> mylist (0, 5);
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "list(1, 5) returns list w/ size 1 and 5 at the front" ) {
        ft::list<int> mylist (1, 5);
        REQUIRE( mylist.size() == 1 );
        REQUIRE( mylist.front() == 5 );
    }
    SECTION( "list(1000, 5) returns list w/ size 1000 and filled w/ 5" ) {
        ft::list<int> mylist (1000, 5);
        REQUIRE( mylist.size() == 1000 );
        for (ft::list<int>::iterator it = mylist.begin(); it != mylist.end(); ++it)
            REQUIRE( *it == 5 );
    }
}

//TEST_CASE( "range constructor works correctly", "[list][basics]" );
//TEST_CASE( "copy constructor works correctly", "[list][basics]" );
//TEST_CASE( "destructor works correctly", "[list][basics]" );

/* ITERATORS */

//TEST_CASE( "begin work correctly", "[list][basics]" );
//TEST_CASE( "end work correctly", "[list][basics]" );

/* CAPACITY (DONE) */
TEMPLATE_LIST_TEST_CASE( "empty() reflects list state", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;
    REQUIRE( mylist.empty() );
    mylist.push_front(TestType());
    REQUIRE_FALSE( mylist.empty() );
    mylist.pop_front();
    REQUIRE( mylist.empty() );
}

TEMPLATE_LIST_TEST_CASE( "size() works correctly", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;

    SECTION( "uninitialized list has size 0" ) {
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "size() returns updated size when elements are added" ) {
        for (int i = 0; i < 5; ++i)
            mylist.push_front(TestType());
        REQUIRE( mylist.size() == 5 );

        SECTION( "size() returns updated size after clearing the list" ) {
            mylist.clear();
            REQUIRE( mylist.size() == 0 );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "max_size() works correctly", "[list][capacity]", templateTestList )
{
    SECTION ( "max_size() returns same value as std::list" ) {
        ft::list<TestType> mylist;
        std::list<TestType> stl_list;
        REQUIRE( mylist.max_size() == stl_list.max_size() );
    }
}

// ELEMENT ACCESS
TEST_CASE( "front() works correctly", "[list][element access]" )
{
    SECTION( "front() element access returns reference to first element" ) {
        ft::list<int> mylist (1, 77);
        const ft::list<int> cmylist (mylist);

        mylist.front() -= 22;
        REQUIRE( mylist.front() == 55 );
        REQUIRE( cmylist.front() == 77 );
    }
}

TEST_CASE( "back() works correctly", "[list][element access]" )
{
    SECTION( "back() element access returns reference to first element" ) {
        ft::list<int> mylist (1, 77);
        const ft::list<int> cmylist (mylist);

        mylist.back() -= 22;
        REQUIRE( mylist.back() == 55 );
        REQUIRE( cmylist.back() == 77 );
    }
}

// MODIFIERS

TEMPLATE_LIST_TEST_CASE( "assign() work correctly", "[list][modifiers]", templateTestList)
{
    SECTION( "range assign works correctly" ) {
        // generate starting index between 0 and vector_size
        // generate advance step between 0 and vector_size - index
        // call assign with iterators from starting to starting + step

        //auto size = GENERATE(range(0, 100));
        //auto i = GENERATE_COPY(range(0, size));
        //auto j = GENERATE_COPY(range(i, size - i));

        // automate
        size_t size = 10;
        size_t i = 2;
        size_t j = 7;

        std::vector<TestType> v (size, TestType());
        auto first = v.begin();
        auto last = v.begin();
        std::advance(first, i);
        std::advance(last, j);
        ft::list<TestType> l;
        l.assign(first, last);
        REQUIRE( (l.size() == j - i) );
    }

    SECTION( "fill assign works correctly" ) {
        size_t n = GENERATE(0, 5, 1000);
        ft::list<TestType> mylist;
        TestType val = TestType();
        mylist.assign(n, val);

        REQUIRE( mylist.size() == n);
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            REQUIRE( *it == val);
    }
}

TEMPLATE_LIST_TEST_CASE( "push_front() works correctly", "[list][modifiers]", templateTestList )
{
    SECTION( "the containers size is incremented" ) {
        ft::list<TestType> mylist;
        int size = GENERATE(range(1, 1000));
        for (int i = 0; i < size; ++i)
            mylist.push_front(TestType());
        REQUIRE( (int)mylist.size() == size );
    }

    SECTION( "the element is added at the front of the list" ) {
        ft::list<int> mylist;
        for (int i = 0; i < 100; ++i) {
            mylist.push_front(i);
            REQUIRE( *mylist.begin() == i );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "push_back() works correctly", "[list][modifiers]", templateTestList )
{
    SECTION( "the containers size is incremented" ) {
        ft::list<TestType> mylist;
        int size = GENERATE(range(1, 1000));
        for (int i = 0; i < size; ++i)
            mylist.push_back(TestType());
        REQUIRE( (int)mylist.size() == size );
    }
    SECTION( "the element is added at the back of the list" ) {
        ft::list<int> mylist;
        for (int i = 0; i < 100; ++i) {
            mylist.push_back(i);
            REQUIRE( *--mylist.end() == i );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "pop_front works correctly", "[list][modifiers]", templateTestList)
{
    ft::list<int> mylist (10, 100);
    size_t i = mylist.size();
    while (!mylist.empty()) {
        REQUIRE( mylist.size() == i);
        REQUIRE( *mylist.begin() == 100);
        REQUIRE( *--mylist.end() == 100);
        mylist.pop_front();
        --i;
    }
}

TEMPLATE_LIST_TEST_CASE( "pop_back works correctly", "[list][modifiers]", templateTestList)
{
    ft::list<int> mylist (10, 100);
    size_t i = mylist.size();
    while (!mylist.empty()) {
        REQUIRE( mylist.size() == i);
        REQUIRE( *mylist.begin() == 100);
        REQUIRE( *--mylist.end() == 100);
        mylist.pop_back();
        --i;
    }
}

//TEST_CASE( "insert work correctly", "[list][modifiers]");
//TEST_CASE( "erase work correctly", "[list][modifiers]");

// add check of allocator swap
TEST_CASE( "swap works correctly", "[list][modifiers]")
{
    SECTION( "non-empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1 (10, 100);
        ft::list<int> mylist2 (2, -12);

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 2 );
        REQUIRE( mylist2.size() == 10 );
        for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION( "empty w/ non-empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1 (10, 100);
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 10 );
        REQUIRE( (mylist1.begin() == mylist1.end()) );
        for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION( "empty w/ non-empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1;
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 0 );
        REQUIRE( (mylist1.begin() == mylist1.end()) );
        REQUIRE( (mylist2.begin() == mylist2.end()) );
    }
}

//TEST_CASE( "resize works correctly", "[list][modifiers]");
//TEST_CASE( "clear works correctly", "[list][modifiers]");

// OPERATIONS

//TEST_CASE( "splice work correctly", "[list][modifiers]");
//TEST_CASE( "remove works correctly", "[list][modifiers]");
//TEST_CASE( "unique work correctly", "[list][modifiers]");
//TEST_CASE( "merge work correctly", "[list][modifiers]");
//TEST_CASE( "sort work correctly", "[list][modifiers]");
//TEST_CASE( "reverse works correctly", "[list][modifiers]");

// NON-MEMBER FUNCTION OVERLOADS
