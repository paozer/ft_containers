#include "catch.hpp"
#include <list>
#include <vector>
#include "../srcs/list/list.hpp"

using templateTestList = std::tuple<int, char, std::string, std::list<int> >;

// CONSTRUCTOR & DESTRUCTOR & = OPERATOR
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

TEMPLATE_LIST_TEST_CASE( "range constructor works correctly", "[list][basics]", templateTestList )
{
}

// ITERATORS
// CAPACITY
TEMPLATE_LIST_TEST_CASE( "empty member function reflects list state", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;
    REQUIRE( mylist.empty() );
    mylist.push_front(TestType());
    REQUIRE_FALSE( mylist.empty() );
    mylist.pop_front();
    REQUIRE( mylist.empty() );
}

TEMPLATE_LIST_TEST_CASE( "size member function reflects list state", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;

    SECTION( "declared list has size 0" ) {
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "size updates after adding to the list" ) {
        for (int i = 0; i < 5; ++i)
            mylist.push_front(TestType());
        REQUIRE( mylist.size() == 5 );

        SECTION( "size updates after clearing the list" ) {
            mylist.clear();
            REQUIRE( mylist.size() == 0 );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "max size member function returns same as STL lists member function", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;
    std::list<TestType> stl_list;

    REQUIRE( mylist.max_size() == stl_list.max_size() );
}

// ELEMENT ACCESS
TEST_CASE("front element access returns reference to first element", "[list][element access]" )
{
    ft::list<int> mylist (1, 77);
    const ft::list<int> cmylist (mylist);

    mylist.front() -= 22;
    REQUIRE( mylist.front() == 55 );
    REQUIRE( cmylist.front() == 77 );
}

TEST_CASE("back element access returns reference to first element", "[list][element access]" )
{
    ft::list<int> mylist (1, 77);
    const ft::list<int> cmylist (mylist);

    mylist.back() -= 22;
    REQUIRE( mylist.back() == 55 );
    REQUIRE( cmylist.back() == 77 );
}

// MODIFIERS

TEMPLATE_LIST_TEST_CASE( "assign methods work correctly", "[list][modifiers]", templateTestList)
{
    SECTION( "assign by range method works correctly" ) {
        size_t vector_size = GENERATE(0, 5, 1000);
        size_t list_size = GENERATE(0, 5, 1000);
        std::vector<TestType> v (vector_size, TestType());
        // generate starting index between 0 and vector_size
        // generate advance step between 0 and vector_size - index
        // call assign with iterators from starting to starting + step
    }
    SECTION( "assign by fill method works correctly" ) {
        ft::list<TestType> mylist;
        size_t n = GENERATE(0, 5, 1000);
        auto val = TestType();
        mylist.assign(n, val);
        REQUIRE( mylist.size() == n);
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            REQUIRE( *it == val);
    }
}

// OPERATIONS
// NON-MEMBER FUNCTION OVERLOADS
