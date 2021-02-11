#include "catch.hpp"

#include <list>
#include <vector>

#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"

#define CONTAINER_LIST ( ft::list )
#define TYPE_LIST ( int, char, std::string, std::list<int>, std::vector<std::string> )
#define VALUE_TYPE typename TestType::value_type

// OPERATIONS

// add test to check consistency of removed/added values
TEMPLATE_PRODUCT_TEST_CASE( "splice work correctly", "[list][vector][operations]", CONTAINER_LIST, int )
{
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    TestType cnt1 (v, v + 7);
    TestType cnt2;

    SECTION( "entire list splice works correctly" ) {
        cnt2.splice(cnt2.begin(), cnt1);
        REQUIRE( cnt1.size() == 0 );
        REQUIRE( cnt2.size() == 7 );
    }
    SECTION( "single element splice works correctly" ) {
        cnt2.splice(cnt2.begin(), cnt1, ++cnt1.begin());
        REQUIRE( cnt1.size() == 6 );
        REQUIRE( cnt2.size() == 1 );
    }
    SECTION( "element range splice works correctly" ) {
        auto first = cnt1.begin();
        std::advance(first, 3);
        auto last = first;
        std::advance(last, 4);

        cnt2.splice(cnt2.begin(), cnt1, first, last);
        REQUIRE( cnt1.size() == 3 );
        REQUIRE( cnt2.size() == 4 );
    }
}

// check consistency of removed elements
TEMPLATE_PRODUCT_TEST_CASE( "remove(_if) work correctly", "[list][vector][operations]", CONTAINER_LIST, int )
{
    int v[] = { 0, 1, 2, 3, 3, 5, 1, 7 };
    TestType cnt (v, v + 8);

    SECTION( "remove works correctly" ) {
        cnt.remove(42);
        REQUIRE( cnt.size() == 8 );
        cnt.remove(1);
        REQUIRE( cnt.size() == 6 );
        cnt.remove(3);
        REQUIRE( cnt.size() == 4 );
        cnt.remove(7);
        REQUIRE( cnt.size() == 3 );
        cnt.remove(0);
        REQUIRE( cnt.size() == 2 );
        cnt.remove(2);
        REQUIRE( cnt.size() == 1 );
        cnt.remove(5);
        REQUIRE( cnt.size() == 0 );
        cnt.remove(0);
        REQUIRE( cnt.size() == 0 );
    }
    SECTION( "remove_if works correctly" ) {
        cnt.remove_if([](int i) { return i == 42; });
        REQUIRE( cnt.size() == 8 );
        cnt.remove_if([](int i) { return i == 1; });
        REQUIRE( cnt.size() == 6 );
        cnt.remove_if([](int i) { return i == 3; });
        REQUIRE( cnt.size() == 4 );
        cnt.remove_if([](int i) { return i == 7; });
        REQUIRE( cnt.size() == 3 );
        cnt.remove_if([](int i) { return i == 0; });
        REQUIRE( cnt.size() == 2 );
        cnt.remove_if([](int i) { return i == 2; });
        REQUIRE( cnt.size() == 1 );
        cnt.remove_if([](int i) { return i == 5; });
        REQUIRE( cnt.size() == 0 );
        cnt.remove_if([](int i) { return i == 0; });
        REQUIRE( cnt.size() == 0 );
    }
}

TEMPLATE_TEST_CASE( "unique work correctly", "[list][operations]", ft::list<int> )
{
    int a[] = { 1, 1, -66, -22, -22, -1, -22, 2, 2, 2, 9, 9, 109, 109, 109 };
    TestType cnt (a, a + sizeof(a) / sizeof(int));

    SECTION( "unique without specifying how to compare" ) {
        int unique[] = { 1, -66, -22, -1, -22, 2, 9, 109 };
        cnt.unique();
        REQUIRE( cnt.size() == (sizeof(unique) / sizeof(int)) );
        int i = 0;
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            REQUIRE( *it == unique[i] );
            ++i;
        }
    }
    SECTION( "unique w/ specifying how to compare" ) {
        int duplicates[] = { 1, -66, -22, -22, -1, -22, 2, 9, 9, 109, 109, 109 };
        cnt.unique( [] (int i, int j) { return i + j == 2 || i + j == 4; });

        REQUIRE( cnt.size() == (sizeof(duplicates) / sizeof(int)) );
        int i = 0;
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            REQUIRE( *it == duplicates[i] );
            ++i;
        }
    }
}

TEMPLATE_TEST_CASE( "merge work correctly", "[list][operations]", ft::list<int> )
{
    SECTION( "merging without specifying how to compare" ) {
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        TestType cnt1 (a, a + sizeof(a) / sizeof(int));
        TestType cnt2 (b, b + sizeof(b) / sizeof(int));

        cnt1.merge(cnt2);
        REQUIRE( cnt2.empty() );
        int i = 0;
        for (auto it = cnt1.begin(); it != cnt1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "merging with specified comparing function" ) {
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        TestType cnt1 (a, a + sizeof(a) / sizeof(int));
        TestType cnt2 (b, b + sizeof(b) / sizeof(int));

        cnt1.merge(cnt2, std::greater<int>());
        REQUIRE( cnt2.empty() );
        int i = 0;
        for (auto it = cnt1.begin(); it != cnt1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "mergin w/ empty list" ) {
        TestType cnt1 (5, 10);
        TestType cnt2;
        TestType cnt3;

        cnt1.merge(cnt2);
        REQUIRE( cnt1.size() == 5 );
        cnt2.merge(cnt3);
        REQUIRE( cnt2.size() == 0 );
        REQUIRE( cnt3.size() == 0 );
    }
}

TEMPLATE_TEST_CASE( "sort work correctly", "[list][operations]", ft::list<int> )
{
    int v[] = {2, 4, 12, 5, 2, 99, -12};
    int v_sorted[] = {-12, 2, 2, 4, 5, 12, 99};
    size_t v_size = sizeof(v) / sizeof(int);
    TestType cnt (v, v + v_size);

    SECTION( "sorting without specifying how to compare" ) {
        cnt.sort();
        REQUIRE( cnt.size() == v_size );
        int i = 0;
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            REQUIRE( *it == v_sorted[i] );
            ++i;
        }
    }
    SECTION( "sorting with specified comparing function" ) {
        cnt.sort(std::greater<int>());
        REQUIRE( cnt.size() == v_size );
        int j = v_size;
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            --j;
            REQUIRE( *it == v_sorted[j] );
        }
    }
    SECTION ( "sorting unfit lists" ) {
        TestType cnt1;
        TestType cnt2 (1, 0);

        cnt1.sort();
        REQUIRE( cnt1.size() == 0 );
        cnt2.sort();
        REQUIRE( cnt2.size() == 1 );
    }
}

TEMPLATE_TEST_CASE( "reverse works correctly", "[list][operations]", ft::list<int> )
{
    int v[] = { 2, 4, 2, 5, 2, 99, -12, 1, 312 };
    TestType cnt (v, v + sizeof(v) / sizeof(int));
    cnt.reverse();
    REQUIRE( cnt.size() == (sizeof(v) / sizeof(int)) );
    int i = sizeof(v) / sizeof(int);
    for (auto it = cnt.begin(); it != cnt.end(); ++it) {
        --i;
        REQUIRE( *it == v[i] );
    }
}

// MODIFIERS

TEMPLATE_PRODUCT_TEST_CASE( "assign work correctly", "[list][vector][modifiers]", CONTAINER_LIST, TYPE_LIST )
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

TEMPLATE_TEST_CASE( "push_front works correctly", "[list][modifiers]", ft::list<int> )
{
    SECTION( "containers size is incremented & the element is added at the front" ) {
        TestType cnt;
        int size = 100;
        for (int i = 0; i < size; ++i) {
            cnt.push_front(i);
            REQUIRE( *cnt.begin() == i );
        }
        REQUIRE( (int)cnt.size() == size );
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "push_back works correctly", "[list][vector][modifiers]", CONTAINER_LIST, int )
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

TEMPLATE_TEST_CASE( "pop_front works correctly", "[list][modifiers]", ft::list<int> )
{
    TestType cnt (10, 100);
    size_t i = cnt.size();
    while (!cnt.empty()) {
        REQUIRE( cnt.size() == i);
        REQUIRE( *cnt.begin() == 100);
        REQUIRE( *--cnt.end() == 100);
        cnt.pop_front();
        --i;
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "pop_back works correctly", "[list][vector][modifiers]", CONTAINER_LIST, int)
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

TEMPLATE_PRODUCT_TEST_CASE( "insert work correctly", "[list][vector][modifiers]", CONTAINER_LIST, int )
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
        cnt.push_front(-32);
        cnt.push_front(0);
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
TEMPLATE_PRODUCT_TEST_CASE( "erase work correctly", "[list][vector][modifiers]", CONTAINER_LIST, TYPE_LIST )
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
TEMPLATE_PRODUCT_TEST_CASE( "swap works correctly", "[list][vector][modifiers]", CONTAINER_LIST, int)
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
TEMPLATE_PRODUCT_TEST_CASE( "resize works correctly", "[list][vector][modifiers]", CONTAINER_LIST, TYPE_LIST )
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

TEMPLATE_PRODUCT_TEST_CASE( "clear works correctly", "[list][vector][modifiers]", CONTAINER_LIST, TYPE_LIST )
{
    TestType cnt;
    cnt.clear();
    REQUIRE( cnt.size() == 0 );
    cnt.assign(10, VALUE_TYPE());
    REQUIRE( cnt.size() == 10 );
    cnt.clear();
    REQUIRE( cnt.size() == 0 );
}

// ELEMENT ACCESS

TEMPLATE_PRODUCT_TEST_CASE( "front works correctly", "[list][vector][element access]", CONTAINER_LIST, int )
{
    SECTION( "first element is changed when assigning to reference" ) {
        TestType cnt (1, 77);
        cnt.front() -= 22;
        REQUIRE( cnt.front() == 55 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "back works correctly", "[list][vector][element access]", CONTAINER_LIST, int )
{
    SECTION( "last element is changed when assigning to reference" ) {
        TestType cnt (1, 77);
        cnt.back() -= 22;
        REQUIRE( cnt.back() == 55 );
    }
}

// vector test at()
// vector test operator[]

// CAPACITY

TEMPLATE_PRODUCT_TEST_CASE( "empty reflects list state", "[list][vector][basics]", CONTAINER_LIST, TYPE_LIST )
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    cnt.push_back(VALUE_TYPE());
    REQUIRE_FALSE( cnt.empty() );
    cnt.pop_front();
    REQUIRE( cnt.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE( "size works correctly", "[list][vector][capacity]", CONTAINER_LIST, TYPE_LIST )
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

TEMPLATE_TEST_CASE( "max_size works correctly", "[list][vector][capacity]",
        int, char, std::string, std::list<int>, std::vector<std::string> )
{
    SECTION ( "ft::list returns same value as std::list" ) {
        ft::list<TestType> ft_cnt;
        std::list<TestType> stl_cnt;
        REQUIRE( ft_cnt.max_size() == stl_cnt.max_size() );
    }
    //SECTION ( "ft::vector returns same value as std::vector" ) {
    //    ft::vector<TestType> ft_cnt;
    //    std::vector<TestType> stl_cnt;
    //    REQUIRE( ft_cnt.max_size() == stl_cnt.max_size() );
    //}
}
