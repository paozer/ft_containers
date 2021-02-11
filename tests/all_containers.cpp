#include "catch.hpp"
#include <list>
#include <vector>

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"

#define CONTAINER_LIST ( ft::list )
#define TYPE_LIST ( int, char, std::string, std::list<int>, std::vector<std::string> )

// CONSTRUCTION

TEMPLATE_PRODUCT_TEST_CASE( "default constructor works correctly", "[list][vector][basics]", CONTAINER_LIST , TYPE_LIST )
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    REQUIRE( cnt.size() == 0 );
}

TEMPLATE_PRODUCT_TEST_CASE( "fill constructor works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    SECTION( "cnt(0, 5) returns cnt w/ size 0" ) {
        TestType cnt (0, 5);
        REQUIRE( cnt.size() == 0 );
    }
    SECTION( "cnt(1, 5) returns cnt w/ size 1 and 5 at the front" ) {
        TestType cnt (1, 5);
        REQUIRE( cnt.size() == 1 );
        REQUIRE( cnt.front() == 5 );
    }
    SECTION( "cnt(1000, 5) returns cnt w/ size 1000 and filled w/ 5" ) {
        TestType cnt (1000, 5);
        REQUIRE( cnt.size() == 1000 );
        for (auto it = cnt.begin(); it != cnt.end(); ++it)
            REQUIRE( *it == 5 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "range constructor works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    int v[] = {2, 4, 12, 5, 60, 99, -12};
    size_t v_size = sizeof(v) / sizeof(int);
    TestType cnt (v, v + v_size);

    REQUIRE( cnt.size() == v_size );
    auto it = cnt.begin();
    for (size_t i = 0; i < v_size; ++i) {
        REQUIRE( *it == v[i] );
        ++it;
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "copy constructor works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    SECTION ( "works on construction from non-empty list" ) {
        TestType cnt1;
        for (int i = 0; i < 10; ++i)
            cnt1.push_back(i);
        TestType cnt2 (cnt1);
        REQUIRE( cnt1.size() == cnt2.size() );

        auto it1 = cnt1.begin();
        auto it2 = cnt2.begin();
        while (it1 != cnt1.end() || it2 != cnt2.end()) {
            REQUIRE( *it1 == *it2 );
            ++it1;
            ++it2;
        }
    }
    SECTION ( "works on construction from empty list" ) {
        TestType cnt1;
        TestType cnt2 (cnt1);

        REQUIRE( cnt1.size() == 0 );
        REQUIRE( cnt1.size() == cnt2.size() );
    }
}

//TEST_CASE( "destructor works correctly", "[list][basics]" );

// RELATIONAL OPERATORS

TEMPLATE_PRODUCT_TEST_CASE( "operator= works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2 = cnt1;

    REQUIRE( cnt2.size() == 10 );
    for (auto it = cnt2.begin(); it != cnt2.end(); ++it)
        REQUIRE( *it == 10 );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator== works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2;
    TestType cnt3;
    TestType cnt4;

    REQUIRE_FALSE( (cnt1 == cnt2) );
    cnt2 = cnt1;
    REQUIRE( (cnt1 == cnt2) );
    REQUIRE( (cnt3 == cnt4) );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator!= works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2;
    TestType cnt3;
    TestType cnt4;

    REQUIRE( (cnt1 != cnt2) );
    cnt2 = cnt1;
    REQUIRE_FALSE( (cnt1 != cnt2) );
    REQUIRE_FALSE( (cnt3 != cnt4) );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator< works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2 (11, 10);
    TestType cnt3 (10, 99);
    TestType cnt4;
    TestType cnt5 (10, 10);

    REQUIRE( (cnt1 < cnt2) );
    REQUIRE( (cnt2 < cnt3) );
    REQUIRE( (cnt4 < cnt3) );
    REQUIRE_FALSE( (cnt3 < cnt4) );
    REQUIRE_FALSE( (cnt1 < cnt5) );
    REQUIRE_FALSE( (cnt4 < cnt4) );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator<= works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2 (11, 10);
    TestType cnt3 (10, 99);
    TestType cnt4;
    TestType cnt5 (10, 10);

    REQUIRE( (cnt1 <= cnt2) );
    REQUIRE( (cnt2 <= cnt3) );
    REQUIRE( (cnt4 <= cnt3) );
    REQUIRE( (cnt1 <= cnt5) );
    REQUIRE( (cnt4 <= cnt4) );
    REQUIRE_FALSE( (cnt3 <= cnt4) );
    REQUIRE_FALSE( (cnt2 <= cnt1) );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator> works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt1 (10, 10);
    TestType cnt2 (11, 10);
    TestType cnt3 (10, 99);
    TestType cnt4;
    TestType cnt5 (10, 10);

    REQUIRE_FALSE( (cnt1 > cnt2) );
    REQUIRE_FALSE( (cnt2 > cnt3) );
    REQUIRE_FALSE( (cnt4 > cnt3) );
    REQUIRE_FALSE( (cnt1 > cnt5) );
    REQUIRE_FALSE( (cnt4 > cnt4) );
    REQUIRE( (cnt3 > cnt4) );
    REQUIRE( (cnt3 > cnt1) );
    REQUIRE( (cnt2 > cnt1) );
}

TEMPLATE_PRODUCT_TEST_CASE( "operator>= works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    int a[] = { 1, 4, -1, 2, 33 };
    int b[] = { 1, 4, -6, 4, 2 };
    int c[] = { 0, 12, -6, 4, 2 };
    TestType cnt1 (a, a + sizeof(a) / sizeof(int));
    TestType cnt2 (cnt1);
    TestType cnt3 (b, b + sizeof(b) / sizeof(int));
    TestType cnt4 (c, c + sizeof(c) / sizeof(int));

    REQUIRE( (cnt1 >= cnt2) );
    REQUIRE( (cnt1 >= cnt3) );
    REQUIRE( (cnt1 >= cnt4) );

    cnt2.push_back(-13);
    REQUIRE_FALSE( (cnt1 >= cnt2) );
}

// ITERATORS

// add const_iterator tests & non const to const construction
// add tests using std::distance

TEMPLATE_PRODUCT_TEST_CASE( "begin works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt (1, 10);       // { 10 }
    REQUIRE( *cnt.begin() == 10 );
    cnt.push_front(5);               // { 5, 10 }
    REQUIRE( *cnt.begin() == 5 );
    cnt.push_back(20);               // { 5, 10, 20 }
    REQUIRE( *cnt.begin() == 5 );
    cnt.pop_front();                 // { 10, 20 }
    REQUIRE( *cnt.begin() == 10 );
    *cnt.begin() = 5;                // { 5, 20 }
    REQUIRE( *cnt.begin() == 5 );
}

TEMPLATE_PRODUCT_TEST_CASE( "end works correctly", "[list][vector][basics]", CONTAINER_LIST, int )
{
    TestType cnt (1, 10);       // { 10 }
    REQUIRE( *--cnt.end() == 10 );
    cnt.push_front(5);               // { 5, 10 }
    REQUIRE( *--cnt.end() == 10 );
    cnt.push_back(20);               // { 5, 10, 20 }
    REQUIRE( *--cnt.end() == 20 );
    cnt.pop_back();                 // { 5, 10 }
    REQUIRE( *--cnt.end() == 10 );
    *--cnt.end() = 5;                // { 5, 5 }
    REQUIRE( *--cnt.end() == 5 );
}
