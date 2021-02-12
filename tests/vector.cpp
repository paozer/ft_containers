#include "catch.hpp"

#include <vector>
#include <list>

#include "../srcs/vector/vector.hpp"

#define TYPE_LIST ( int, char, std::string, std::vector<int>, std::vector<std::string> )
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
    size_t v_size = sizeof(v) / sizeof(int);
    TestType cnt (v, v + v_size);

    REQUIRE( cnt.size() == v_size );
    auto it = cnt.begin();
    for (size_t i = 0; i < v_size; ++i) {
        REQUIRE( *it == v[i] );
        ++it;
    }
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

//TEST_CASE( "destructor works correctly", "[vector][basics]" );

/* RELATIONAL OPERATORS */

TEMPLATE_TEST_CASE( "operator= works correctly", "[vector][basics]", ft::vector<int> )
{
    TestType cnt1 (10, 10);
    TestType cnt2 = cnt1;

    REQUIRE( cnt2.size() == 10 );
    for (auto it = cnt2.begin(); it != cnt2.end(); ++it)
        REQUIRE( *it == 10 );
}

TEMPLATE_TEST_CASE( "operator== works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "operator!= works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "operator< works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "operator<= works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "operator> works correctly", "[vector][basics]", ft::vector<int> )
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

TEMPLATE_TEST_CASE( "operator>= works correctly", "[vector][basics]", ft::vector<int> )
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

/* ITERATORS */

// add const_iterator tests & non const to const construction
// add tests using std::distance

TEMPLATE_TEST_CASE( "begin works correctly", "[vector][basics]", ft::vector<int> )
{
}

TEMPLATE_TEST_CASE( "end works correctly", "[vector][basics]", ft::vector<int> )
{
}

/* MODIFIERS */

TEMPLATE_PRODUCT_TEST_CASE( "assign work correctly", "[vector][modifiers]", ft::vector, TYPE_LIST )
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

        std::vector<VALUE_TYPE> v (size, VALUE_TYPE());
        auto first = v.begin();
        auto last = v.begin();
        std::advance(first, i);
        std::advance(last, j);
        TestType l;
        l.assign(first, last);
        REQUIRE( (l.size() == j - i) );
    }

    SECTION( "fill assign works correctly" ) {
        size_t n = GENERATE(0, 5, 1000);
        TestType cnt;
        cnt.assign(n, VALUE_TYPE());

        REQUIRE( cnt.size() == n);
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

TEMPLATE_TEST_CASE( "insert work correctly", "[vector][modifiers]", ft::vector<int> )
{
    TestType cnt;
    auto it = cnt.begin();

    SECTION( "single element insert works correctly" ) {
        cnt.insert(it, 5);           // { 5 }
        REQUIRE( cnt.size() == 1 );
        REQUIRE( *cnt.begin() == 5 );
        cnt.insert(it, 10);           // { 5, 10}
        REQUIRE( cnt.size() == 2 );
        REQUIRE( *--cnt.end() == 10 );
    }
    SECTION( "fill insert works correctly" ) {
        cnt.insert(it, 5, 10);
        REQUIRE( cnt.size() == 5 );
        for (auto ite = cnt.begin(); ite != cnt.end(); ++ite)
            REQUIRE( *ite == 10 );
        cnt.insert(it, 1, 100);
        REQUIRE( cnt.size() == 6 );
        REQUIRE( *--cnt.end() == 100 );
    }
    SECTION( "range insert works correctly" ) {
        int arr[] = {12, 1, 4, 5, 6, 7};
        //cnt.push_front(0);
        //cnt.push_front(-32);
        cnt.insert(--cnt.end(), arr + 1, arr + 5);
        REQUIRE( cnt.size() == 6 );
        auto it = cnt.begin();
        REQUIRE( *it == 0 );
        REQUIRE( *++it == 1 );
        REQUIRE( *++it == 4 );
        REQUIRE( *++it == 5 );
        REQUIRE( *++it == 6 );
        REQUIRE( *++it == -32 );
    }
}

// add test of return value
// add test of consistency of values after erase
TEMPLATE_PRODUCT_TEST_CASE( "erase work correctly", "[vector][modifiers]", ft::vector, TYPE_LIST )
{
    size_t n = 5;
    TestType cnt (n, VALUE_TYPE());

    SECTION( "single element erase works correctly" ) {
        cnt.erase(cnt.begin());
        REQUIRE( cnt.size() == --n );
        cnt.erase(--cnt.end());
        REQUIRE( cnt.size() == --n );
        auto it = cnt.begin();
        std::advance(it, 2);
        cnt.erase(it);
        REQUIRE( cnt.size() == --n );
    }
    SECTION( "multiple elements erase works correctly" ) {
        auto first = cnt.begin();
        std::advance(first, 2);
        auto last = first;
        std::advance(last, 2);
        cnt.erase(first, last);
        REQUIRE( cnt.size() == (n - 2) );
    }
}

// add check of allocator swap ? don't have to implement get_allocator
TEMPLATE_TEST_CASE( "swap works correctly", "[vector][modifiers]", ft::vector<int> )
{
    SECTION( "non-empty lists swap swaps sizes and elements" ) {
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
    SECTION( "empty w/ non-empty lists swap swaps sizes and elements" ) {
        TestType cnt1 (10, 100);
        TestType cnt2;

        cnt1.swap(cnt2);
        REQUIRE( cnt1.size() == 0 );
        REQUIRE( cnt2.size() == 10 );
        REQUIRE( (cnt1.begin() == cnt1.end()) );
        for (auto it = cnt2.begin(); it != cnt2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION( "empty w/ empty lists swap swaps sizes and elements" ) {
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
        cnt.push_front(VALUE_TYPE());
        cnt.push_front(VALUE_TYPE());
        cnt.push_front(VALUE_TYPE());
        cnt.push_front(VALUE_TYPE());
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
            cnt.push_front(VALUE_TYPE());
            cnt.resize(5);
            REQUIRE( cnt.size() == 5 );
            cnt.push_front(VALUE_TYPE());
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

// vector test at()
// vector test operator[]

/* CAPACITY */

TEMPLATE_PRODUCT_TEST_CASE( "empty reflects list state", "[vector][basics]", ft::vector, TYPE_LIST )
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    cnt.push_back(VALUE_TYPE());
    REQUIRE_FALSE( cnt.empty() );
    cnt.pop_front();
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
            cnt.push_front(VALUE_TYPE());
        REQUIRE( cnt.size() == 5 );
        SECTION( "size() returns updated size after clearing the list" ) {
            cnt.clear();
            REQUIRE( cnt.size() == 0 );
        }
    }
}

TEMPLATE_TEST_CASE( "max_size works correctly", "[vector][capacity]",
        int, char, std::string, std::vector<int>, std::vector<std::string> )
{
    SECTION ( "ft::vector returns same value as std::vector" ) {
        ft::vector<TestType> ft_cnt;
        std::vector<TestType> stl_cnt;
        REQUIRE( ft_cnt.max_size() == stl_cnt.max_size() );
    }
}
