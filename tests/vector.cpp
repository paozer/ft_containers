#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"

#include "catch.hpp"

#include <vector>
#include <list>

#ifndef __linux__
#define TYPE_LIST ( const int, int, char, std::string, ft::vector<int>, std::vector<std::string>, ft::list<std::string>, std::list<int> )
#else
#define TYPE_LIST ( char, std::string, ft::vector<int>, std::vector<std::string>, ft::list<std::string>, std::list<int> )
#endif
#define VALUE_TYPE typename TestType::value_type

#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEMPLATE_PRODUCT_TEST_CASE( "default constructor works correctly", "[vector][basics]", ft::vector, TYPE_LIST )
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    REQUIRE( cnt.size() == 0 );
}

TEMPLATE_TEST_CASE( "fill constructor works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "range constructor works correctly", "[vector][basics]", ft::vector<int> )
{
    int v[] = {2, 4, 12, 5, 60, 99, -12};
    TestType cnt (v, v + 7);

    REQUIRE( cnt.size() == 7 );
    for (size_t i = 0; i < 7; ++i)
        REQUIRE( cnt[i] == v[i] );
}

TEMPLATE_TEST_CASE( "copy constructor works correctly", "[vector][basics]", ft::vector<int> )
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

/* ASSIGNATION OPERATOR */
TEST_CASE("vector assignation operator works correctly", "[vector][assignation operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    ft::vector<int> myvector1 (a, a + 5);
    ft::vector<int> myvector2 (b, b + 2);
    ft::vector<int> myvector3;
    ft::vector<int>::iterator lit;
    ft::vector<int>::iterator rit;

    myvector1 = myvector2;
    REQUIRE( myvector1.size() == myvector2.size() );
    lit = myvector1.begin();
    rit = myvector2.begin();
    while (lit != myvector1.end()) {
        REQUIRE( *lit == *rit );
        ++lit;
        ++rit;
    }

    myvector2 = myvector3;
    REQUIRE( myvector2.size() == myvector3.size() );
    REQUIRE( myvector2.begin() == myvector2.end() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("vector relational operators work correctly", "[vector][relational operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    ft::vector<int> myvector1 (a, a + 5);
    ft::vector<int> myvector2 (b, b + 2);
    ft::vector<int> myvector3;
    ft::vector<int> myvector4 (c, c + 6);

    // myvector1 vs myvector2
    REQUIRE_FALSE( myvector1 == myvector2 );
    REQUIRE( myvector1 != myvector2 );
    REQUIRE_FALSE( myvector1 < myvector2 );
    REQUIRE_FALSE( myvector1 <= myvector2 );
    REQUIRE( myvector1 > myvector2 );
    REQUIRE( myvector1 >= myvector2 );

    // myvector1 vs myvector1
    REQUIRE( myvector1 == myvector1 );
    REQUIRE_FALSE( myvector1 != myvector1 );
    REQUIRE_FALSE( myvector1 < myvector1 );
    REQUIRE( myvector1 <= myvector1 );
    REQUIRE_FALSE( myvector1 > myvector1 );
    REQUIRE( myvector1 >= myvector1 );

    // myvector1 vs myvector3
    REQUIRE_FALSE( myvector1 == myvector3 );
    REQUIRE( myvector1 != myvector3 );
    REQUIRE_FALSE( myvector1 < myvector3 );
    REQUIRE_FALSE( myvector1 <= myvector3 );
    REQUIRE( myvector1 > myvector3 );
    REQUIRE( myvector1 >= myvector3 );

    // myvector3 vs myvector3
    REQUIRE( myvector3 == myvector3 );
    REQUIRE_FALSE( myvector3 != myvector3 );
    REQUIRE_FALSE( myvector3 < myvector3 );
    REQUIRE( myvector3 <= myvector3 );
    REQUIRE_FALSE( myvector3 > myvector3 );
    REQUIRE( myvector3 >= myvector3 );

    // myvector1 vs myvector4
    REQUIRE_FALSE( myvector1 == myvector4 );
    REQUIRE( myvector1 != myvector4 );
    REQUIRE( myvector1 < myvector4 );
    REQUIRE( myvector1 <= myvector4 );
    REQUIRE_FALSE( myvector1 > myvector4 );
    REQUIRE_FALSE( myvector1 >= myvector4 );
}

/* ITERATORS */
TEMPLATE_TEST_CASE( "begin works correctly", "[vector][iterators]", ft::vector<int> )
{
    SECTION("non-const iterator behaviour") {
        TestType cnt (1, 10);               // { 10 }
        REQUIRE( *cnt.begin() == 10 );
        cnt.push_back(5);                   // { 10, 5 }
        REQUIRE( *cnt.begin() == 10 );
        cnt.push_back(20);
        cnt[0] = 2;                         // { 2, 14, 20 }
        REQUIRE( *cnt.begin() == 2 );
        cnt.pop_back();                     // { 2, 14 }
        REQUIRE( *cnt.begin() == 2 );
        *cnt.begin() = 5;                   // { 5, 14 }
        REQUIRE( *cnt.begin() == 5 );
    }
    SECTION("const iterator behaviour") {
        const TestType cnt (1, 10);
        REQUIRE( *cnt.begin() == 10 );

        const TestType cnt1;
        REQUIRE( cnt1.begin() == cnt1.end() );

        TestType cnt2 (1, 9);
        ft::vector<int>::const_iterator it = cnt2.begin();
        REQUIRE( *it == 9 );
    }
}

TEMPLATE_TEST_CASE( "end works correctly", "[vector][iterators]", ft::vector<int> )
{
    SECTION("non-const iterator behaviour") {
        TestType cnt (1, 10);               // { 10 }
        REQUIRE( *--cnt.end() == 10 );
        cnt.push_back(5);                   // { 10, 5 }
        REQUIRE( *--cnt.end() == 5 );
        cnt.push_back(20);
        cnt[0] = 2;                         // { 2, 5, 20 }
        REQUIRE( *--cnt.end() == 20 );
        cnt.pop_back();                     // { 2, 5 }
        REQUIRE( *--cnt.end() == 5 );
        *--cnt.end() = 1;                     // { 2, 1 }
        REQUIRE( *--cnt.end() == 1 );
    }
    SECTION("const iterator behaviour") {
        const TestType cnt (1, 10);
        REQUIRE( *(--cnt.end()) == 10 );

        const TestType cnt1;
        REQUIRE( cnt1.begin() == cnt1.end() );

        TestType cnt2 (1, 9);
        REQUIRE( *--cnt2.end() == 9 );
    }
}

/* MODIFIERS */

TEMPLATE_PRODUCT_TEST_CASE( "assign work correctly", "[vector][modifiers]", ft::vector, TYPE_LIST )
{
    SECTION( "range assign works correctly" ) {
        size_t size = 10;
        size_t i = 2;
        size_t j = 7;

        std::vector<VALUE_TYPE> v (size);
        auto first = v.begin() + i;
        auto last = v.begin() + j;

        TestType my_vector;
        my_vector.assign(first, last);
        REQUIRE( (my_vector.size() == j - i) );
    }

    SECTION( "fill assign works correctly" ) {
        size_t n = GENERATE(0, 100);
        TestType cnt;
        cnt.assign(n, VALUE_TYPE());

        REQUIRE( cnt.size() == n );
        for (auto it = cnt.begin(); it != cnt.end(); ++it)
            REQUIRE( *it == VALUE_TYPE());
    }
}


TEMPLATE_TEST_CASE( "push_back works correctly", "[vector][modifiers]", ft::vector<int> )
{
    SECTION( "containers size is incremented & the element is added at the back" ) {
        TestType cnt;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            cnt.push_back(i);
            REQUIRE( *--cnt.end() == i );
        }
        REQUIRE( (int)cnt.size() == size );
    }
}


TEMPLATE_TEST_CASE( "pop_back works correctly", "[vector][modifiers]", ft::vector<int> )
{
    TestType cnt (10, 100);
    size_t i = cnt.size();
    while (!cnt.empty()) {
        REQUIRE( cnt.size() == i);
        REQUIRE( *cnt.begin() == 100);
        REQUIRE( *--cnt.end() == 100);
        cnt.pop_back();
        --i;
    }
}

TEST_CASE("vector insert work correctly", "[vector][modifiers]")
{
    ft::vector<int> v;

    SECTION( "single element insert works correctly" ) {
        ft::vector<int>::iterator ret;
        ret = v.insert(v.begin(), 5);               // { 5 }
        REQUIRE( *ret == 5 );
        ret = v.insert(v.end(), 10);                // { 5, 10 }
        REQUIRE( *ret == 10 );
        ret = v.insert(++v.begin(), 7);             // { 5, 7, 10 }
        REQUIRE( *ret == 7 );
        ret = v.insert(----v.end(), 6);             // { 5, 6, 7, 10 }
        REQUIRE( *ret == 6 );
        ret = v.insert(++++v.begin(), 0);           // { 5, 6, 0, 7, 10 }
        REQUIRE( *ret == 0 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v[0] == 5 );
        REQUIRE( v[1] == 6 );
        REQUIRE( v[2] == 0 );
        REQUIRE( v[3] == 7 );
        REQUIRE( v[4] == 10 );
    }
    SECTION( "fill insert works correctly" ) {
        v.insert(v.begin(), 5, 10);                 // { 10, 10, 10, 10, 10 }
        v.insert(v.end(), 1, 100);                  // { 10, 10, 10, 10, 10, 100 }
        v.insert(++v.begin(), 2, 0);                // { 10, 0, 0, 10, 10, 10, 10, 100 }
        v.insert(++++v.begin(), 1, 42);             // { 10, 0, 42, 0, 10, 10, 10, 10, 100 }
        v.insert(--v.end(), 3, 21);                 // { 10, 0, 42, 0, 10, 10, 10, 10, 21, 21, 21, 100 }

        REQUIRE( v.size() == 12 );
        REQUIRE( v[0] == 10 );
        REQUIRE( v[1] == 0 );
        REQUIRE( v[2] == 42 );
        REQUIRE( v[3] == 0 );
        for (size_t i = 4; i < 8; ++i)
            REQUIRE( v[i] == 10 );
        REQUIRE( v[8] == 21 );
        REQUIRE( v[9] == 21 );
        REQUIRE( v[10] == 21 );
        REQUIRE( v[11] == 100 );
    }
    SECTION( "range insert works correctly" ) {
        int arr[] = {12, 1, 4, 5, 6, 7};
        int arr1[] = { 0 , -32 };
        v.insert(v.begin(), arr1, arr1 + 1);        // { 0 }
        v.insert(v.end(), arr + 1, arr + 5);        // { 0, 1, 4, 5, 6 }
        v.insert(++v.begin(), arr + 3, arr + 5);    // { 0, 5, 6, 1, 4, 5, 6 }
        v.insert(----v.end(), arr1 + 1, arr1 + 2);  // { 0, 5, 6, 1, 4, -32, 5, 6 }

        REQUIRE( v.size() == 8 );
        REQUIRE( v[0] == 0 );
        REQUIRE( v[1] == 5 );
        REQUIRE( v[2] == 6 );
        REQUIRE( v[3] == 1 );
        REQUIRE( v[4] == 4 );
        REQUIRE( v[5] == -32 );
        REQUIRE( v[6] == 5 );
        REQUIRE( v[7] == 6 );
    }
}

TEST_CASE("erase work correctly", "[vector][modifiers]")
{
    int arr[] = {23, 1, 233, 4, 55, 3};
    ft::vector<int> v (arr, arr + 6);             // { 23, 1, 233, 4, 55, 3 }
    ft::vector<int>::iterator ret;

    SECTION("single element erase works correctly") {
        ret = v.erase(v.begin());                // { 1, 233, 4, 55, 3 }
        REQUIRE( v.size() == 5 );
        REQUIRE( *ret == 1 );
        REQUIRE( *(ret + 1) == 233 );

        ret = v.erase(v.end() - 1);              // { 1, 233, 4, 55 }
        REQUIRE( v.size() == 4 );
        REQUIRE( ret == v.end() );
        REQUIRE( *(ret - 1) == 55 );

        ret = v.erase(v.begin() + 2);            // { 1, 233, 55 }
        REQUIRE( v.size() == 3 );
        REQUIRE( *(ret - 2) == 1 );
        REQUIRE( *(ret - 1) == 233 );
        REQUIRE( *ret == 55 );
    }
    SECTION("multilple element erase works correctly") {
        ret = v.erase(v.end() - 1, v.end());           // { 23, 1, 233, 4, 55 }
        REQUIRE( v.size() == 5 );
        REQUIRE( ret == v.end() );
        REQUIRE( *(ret - 1) == 55 );

        ret = v.erase(v.begin() + 1, v.begin() + 2);  // { 23, 233, 4, 55 }
        REQUIRE( v.size() == 4 );
        REQUIRE( *(ret - 1) == 23 );
        REQUIRE( *ret == 233 );
        REQUIRE( *(ret + 1) == 4 );

        ret = v.erase(v.begin() + 1, v.end() - 1); // { 23, 55 }
        REQUIRE( *(ret - 1) == 23 );
        REQUIRE( *ret == 55 );

        ret = v.erase(v.begin(), v.end());         // { }
        REQUIRE( ret == v.end() );
        REQUIRE( ret == v.begin() );
    }
}

TEMPLATE_TEST_CASE( "swap works correctly", "[vector][modifiers]", ft::vector<int> )
{
    SECTION("non-empty lists swap swaps sizes and elements") {
        TestType cnt1 (10, 100);
        TestType cnt2 (2, -12);

        cnt1.swap(cnt2);
        REQUIRE( cnt1.size() == 2 );
        REQUIRE( cnt2.size() == 10 );
        for (auto it = cnt1.begin(); it != cnt1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = cnt2.begin(); it != cnt2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("empty w/ non-empty lists swap swaps sizes and elements") {
        TestType cnt1 (10, 100);
        TestType cnt2;

        cnt1.swap(cnt2);
        REQUIRE( cnt1.size() == 0 );
        REQUIRE( cnt2.size() == 10 );
        REQUIRE( (cnt1.begin() == cnt1.end()) );
        for (auto it = cnt2.begin(); it != cnt2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("empty w/ empty lists swap swaps sizes and elements") {
        TestType cnt1;
        TestType cnt2;

        cnt1.swap(cnt2);
        REQUIRE( cnt1.size() == 0 );
        REQUIRE( cnt2.size() == 0 );
        REQUIRE( (cnt1.begin() == cnt1.end()) );
        REQUIRE( (cnt2.begin() == cnt2.end()) );
    }
}

// add test to check if resize adds elements @ the end if needed
TEMPLATE_PRODUCT_TEST_CASE( "resize works correctly", "[vector][modifiers]", ft::vector, TYPE_LIST )
{
    TestType cnt;

    SECTION( "container size is reduced when n is smaller than current size" ) {
        cnt.resize(0);
        REQUIRE( cnt.size() == 0 );
        cnt.push_back(VALUE_TYPE());
        cnt.push_back(VALUE_TYPE());
        cnt.push_back(VALUE_TYPE());
        cnt.push_back(VALUE_TYPE());
        cnt.resize(2);
        REQUIRE( cnt.size() == 2 );
        cnt.resize(1);
        REQUIRE( cnt.size() == 1 );
        cnt.resize(0);
        REQUIRE( cnt.size() == 0 );
    }
    SECTION( "container is expanded at it's end when n is greater than current size" ) {
        SECTION( "empty list" ) {
            cnt.resize(5);
            REQUIRE( cnt.size() == 5 );
        }
        SECTION( "non-empty list" ) {
            cnt.push_back(VALUE_TYPE());
            cnt.resize(5);
            REQUIRE( cnt.size() == 5 );
            cnt.push_back(VALUE_TYPE());
            cnt.resize(20);
            REQUIRE( cnt.size() == 20 );
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "clear works correctly", "[vector][modifiers]", ft::vector, TYPE_LIST )
{
    TestType cnt;
    cnt.clear();
    REQUIRE( cnt.size() == 0 );
    cnt.assign(10, VALUE_TYPE());
    REQUIRE( cnt.size() == 10 );
    cnt.clear();
    REQUIRE( cnt.size() == 0 );
}

/* ELEMENT ACCESS */

TEMPLATE_TEST_CASE( "front works correctly", "[vector][element access]", ft::vector<int> )
{
    SECTION( "first element is changed when assigning to reference" ) {
        TestType cnt (1, 77);
        cnt.front() -= 22;
        REQUIRE( cnt.front() == 55 );
    }
}

TEMPLATE_TEST_CASE( "back works correctly", "[vector][element access]", ft::vector<int> )
{
    SECTION( "last element is changed when assigning to reference" ) {
        TestType cnt (1, 77);
        cnt.back() -= 22;
        REQUIRE( cnt.back() == 55 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "at works correctly", "[vector][element access]", ft::vector, TYPE_LIST )
{
    SECTION("throws out_of_range exception if n is greater than the vector size") {
        TestType cnt1;
        TestType cnt2 (50);
        const TestType const_cnt1;
        const TestType const_cnt2 (50);

        REQUIRE_THROWS_AS( cnt1.at(100), std::out_of_range);
        REQUIRE_THROWS_AS( cnt2.at(100), std::out_of_range);
        REQUIRE_THROWS_AS( const_cnt1.at(100), std::out_of_range);
        REQUIRE_THROWS_AS( const_cnt2.at(100), std::out_of_range);
    }
}
// vector test operator[]

/* CAPACITY */

TEMPLATE_PRODUCT_TEST_CASE( "empty reflects list state", "[vector][capacity]", ft::vector, TYPE_LIST )
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    cnt.push_back(VALUE_TYPE());
    REQUIRE_FALSE( cnt.empty() );
    cnt.pop_back();
    REQUIRE( cnt.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE( "size works correctly", "[vector][capacity]", ft::vector, TYPE_LIST )
{
    TestType cnt;

    SECTION( "uninitialized list has size 0" ) {
        REQUIRE( cnt.size() == 0 );
    }
    SECTION( "size() returns updated size when elements are added" ) {
        for (int i = 0; i < 5; ++i)
            cnt.push_back(VALUE_TYPE());
        REQUIRE( cnt.size() == 5 );
        SECTION( "size() returns updated size after clearing the list" ) {
            cnt.clear();
            REQUIRE( cnt.size() == 0 );
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "ft::vector max_size returns same value as std::vector", "[vector][capacity]", ft::vector,
        (int, std::string, ft::vector<int>, std::list<int>) )
{
    TestType ft_cnt;
    std::vector<VALUE_TYPE> stl_cnt;
    REQUIRE( ft_cnt.max_size() == stl_cnt.max_size() );
}
