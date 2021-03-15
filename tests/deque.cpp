#include "../srcs/deque/deque.hpp"
#include "catch.hpp"
#include <deque>
#include <list>

#define TYPE_LIST  ( int, unsigned char, double, std::string, ft::deque<int>, std::deque<std::string>, std::list<std::string>, std::list<std::deque<int> > )
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("deque construction works correctly", "[deque][basics]")
{
    SECTION("deque declaration creates empty deque") {
        ft::deque<int> cnt;
        REQUIRE( cnt.empty() );
        REQUIRE( cnt.size() == 0 );
    }

    SECTION("fill constructor returns correct deque") {
        unsigned int size = GENERATE(0, 1, 5);
        int fill = GENERATE(0, -41);
        ft::deque<int> cnt (size, fill);

        REQUIRE( cnt.size() == size );
        for (unsigned int i = 0; i < size; ++i)
            REQUIRE( cnt[i] == fill );
    }

    SECTION("range constructor returns correct deque") {
        int v[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            ft::deque<int> cnt (v + first, v + last);

            REQUIRE( cnt.size() == last - first );
            for (size_t i = 0; i < last - first; ++i)
                REQUIRE( cnt[i] == v[i + first] );
        }
    }

    SECTION ("copy constructor returns correct deque") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        ft::deque<int> v1;
        ft::deque<int> v2 (v1);
        ft::deque<int> v3 (arr + 2, arr + 6); // { 12, 5, 60, 99 }

        REQUIRE( v1.size() == 0 );
        REQUIRE( v2.size() == 0 );
        REQUIRE( v3.size() == 4 );
        REQUIRE( v3[0] == 12 );
        REQUIRE( v3[1] == 5 );
        REQUIRE( v3[2] == 60 );
        REQUIRE( v3[3] == 99 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("deque correctly copies value upon construction", "[deque][basics]", ft::deque, TYPE_LIST)
{
    // this test may produces double free there is a problem with
    // your allocation/copying with non builtin types
    TestType v1 (10, VALUE_TYPE());
    TestType v2 (v1);
    v1.clear();
    v2.clear();
}

/* ASSIGNATION OPERATOR */
TEST_CASE("deque assignation works correctly", "[deque][assignation operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    ft::deque<int> mydeque1 (a, a + 5);
    ft::deque<int> mydeque2 (b, b + 2);
    ft::deque<int> mydeque3;
    ft::deque<int>::iterator lit;
    ft::deque<int>::iterator rit;

    mydeque1 = mydeque2;
    REQUIRE( mydeque1.size() == mydeque2.size() );
    lit = mydeque1.begin();
    rit = mydeque2.begin();
    while (lit != mydeque1.end()) {
        REQUIRE( *lit == *rit );
        ++lit;
        ++rit;
    }
    mydeque2 = mydeque3;
    REQUIRE( mydeque2.size() == mydeque3.size() );
    REQUIRE( mydeque2.begin() == mydeque2.end() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("deque relational operators work correctly", "[deque][relational operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, 67, 1, 0};
    ft::deque<int> mydeque1 (a, a + 5);
    ft::deque<int> mydeque2 (b, b + 2);
    ft::deque<int> mydeque3;
    ft::deque<int> mydeque4 (c, c + 6);
    ft::deque<int> mydeque5 (d, d + 6);

    // mydeque1 vs mydeque2
    REQUIRE_FALSE( mydeque1 == mydeque2 );
    REQUIRE( mydeque1 != mydeque2 );
    REQUIRE_FALSE( mydeque1 < mydeque2 );
    REQUIRE_FALSE( mydeque1 <= mydeque2 );
    REQUIRE( mydeque1 > mydeque2 );
    REQUIRE( mydeque1 >= mydeque2 );

    // mydeque1 vs mydeque1
    REQUIRE( mydeque1 == mydeque1 );
    REQUIRE_FALSE( mydeque1 != mydeque1 );
    REQUIRE_FALSE( mydeque1 < mydeque1 );
    REQUIRE( mydeque1 <= mydeque1 );
    REQUIRE_FALSE( mydeque1 > mydeque1 );
    REQUIRE( mydeque1 >= mydeque1 );

    // mydeque1 vs mydeque3
    REQUIRE_FALSE( mydeque1 == mydeque3 );
    REQUIRE( mydeque1 != mydeque3 );
    REQUIRE_FALSE( mydeque1 < mydeque3 );
    REQUIRE_FALSE( mydeque1 <= mydeque3 );
    REQUIRE( mydeque1 > mydeque3 );
    REQUIRE( mydeque1 >= mydeque3 );

    // mydeque3 vs mydeque3
    REQUIRE( mydeque3 == mydeque3 );
    REQUIRE_FALSE( mydeque3 != mydeque3 );
    REQUIRE_FALSE( mydeque3 < mydeque3 );
    REQUIRE( mydeque3 <= mydeque3 );
    REQUIRE_FALSE( mydeque3 > mydeque3 );
    REQUIRE( mydeque3 >= mydeque3 );

    // mydeque1 vs mydeque4
    REQUIRE_FALSE( mydeque1 == mydeque4 );
    REQUIRE( mydeque1 != mydeque4 );
    REQUIRE( mydeque1 < mydeque4 );
    REQUIRE( mydeque1 <= mydeque4 );
    REQUIRE_FALSE( mydeque1 > mydeque4 );
    REQUIRE_FALSE( mydeque1 >= mydeque4 );

    // mydeque4 vs mydeque5
    REQUIRE_FALSE( mydeque4 == mydeque5 );
    REQUIRE( mydeque4 != mydeque5 );
    REQUIRE( mydeque4 < mydeque5 );
    REQUIRE( mydeque4 <= mydeque5 );
    REQUIRE_FALSE( mydeque4 > mydeque5 );
    REQUIRE_FALSE( mydeque4 >= mydeque5 );
}

/* ITERATORS */
TEST_CASE("deque begin returns first element and can be incremented", "[deque][iterators]")
{
    SECTION("non-const iterator behaviour") {
        ft::deque<int> v (5, 10);              // { 10, 10, 10, 10, 10 }
        REQUIRE( *v.begin() == 10 );
        *v.begin() = 5;                         // { 5, 10, 10, 10, 10 }
        *++v.begin() = 6;                       // { 5, 6, 10, 10, 10 }
        REQUIRE( *v.begin() == 5 );
        REQUIRE( *(v.begin() + 1) == 6 );
        REQUIRE( *(v.begin() + 2) == 10 );
        *v.begin() = 2;                         // { 2, 6, 10, 10, 10 }
        REQUIRE( *v.begin() == 2 );
        REQUIRE( *(v.begin() + 1) == 6 );
        REQUIRE( *(v.begin() + 2) == 10 );

        REQUIRE(( v.begin() - v.end() == -5 ));
    }

    SECTION("const iterator behaviour") {
        int arr[] = {1, 4, -1, 2, 33};
        ft::deque<int> v2 (arr, arr + 5);
        const ft::deque<int> v (v2);
        const ft::deque<int> v1;

        REQUIRE( *v.begin() == 1 );
        REQUIRE( *(v.begin() + 1) == 4 );
        REQUIRE( *(v.begin() + 2) == -1 );
        REQUIRE( *(v.begin() + 3) == 2 );
        REQUIRE( *(v.begin() + 4) == 33 );
        REQUIRE( (v.begin() + 5) == v.end() );
        REQUIRE( v1.begin() == v1.end() );

        ft::deque<int>::const_iterator it = v2.begin();
        REQUIRE( *it == 1 );
        REQUIRE( *(it + 1) == 4 );
        REQUIRE( *(it + 2) == -1 );
        REQUIRE( *(it + 3) == 2 );
        REQUIRE( *(it + 4) == 33 );
        REQUIRE( (it + 5) == v2.end() );
    }
}

TEMPLATE_TEST_CASE("deque end works correctly", "[deque][iterators]", ft::deque<int>)
{
    SECTION("non-const iterator behaviour") {
        ft::deque<int> v (5, 10);                  // { 10, 10, 10, 10, 10 }
        REQUIRE( *(v.end() - 1) == 10 );
        *(v.end() - 1) = 5;                         // { 10, 10, 10, 10, 5 }
        *(v.end() - 2) = 6;                         // { 10, 10, 10, 6, 5 }
        REQUIRE( *(v.end() - 1) == 5 );
        REQUIRE( *(v.end() - 2) == 6 );
        *(v.end() - 1) = 2;                         // { 10, 10, 10, 6, 2 }
        *(v.end() - 4) = 1;                         // { 10, 1, 10, 6, 2 }
        REQUIRE( *(v.end() - 1) == 2 );
        REQUIRE( *(v.end() - 2) == 6 );
        REQUIRE( *(v.begin() + 1) == 1 );
    }

    SECTION("const iterator behaviour") {
        int arr[] = {1, 4, -1, 2, 33};
        ft::deque<int> v2 (arr, arr + 5);
        const ft::deque<int> v (v2);
        const ft::deque<int> v1;

        REQUIRE( *(v.end() - 1) == 33 );
        REQUIRE( *(v.end() - 2) == 2 );
        REQUIRE( *(v.end() - 3) == -1 );
        REQUIRE( *(v.end() - 4) == 4 );
        REQUIRE( *(v.end() - 5) == 1 );
        REQUIRE( v.begin() == (v.end() - 5) );
        REQUIRE( v1.begin() == v1.end() );

        ft::deque<int>::const_iterator it = v2.end();
        REQUIRE( *(it - 1) == 33 );
        REQUIRE( *(it - 2) == 2 );
        REQUIRE( *(it - 3) == -1 );
        REQUIRE( *(it - 4) == 4 );
        REQUIRE( *(it - 5) == 1 );
        REQUIRE( (it - 5) == v2.begin() );
    }
}

/* MODIFIERS */
TEST_CASE("deque assign methods work correctly", "[deque][modifiers]")
{
    int arr[] = {1, 4, -1, 2, 33};
    ft::deque<int> v;
    ft::deque<int> v1 (10, 1);

    SECTION("range assign creates deque with correct values") {
        unsigned int i = GENERATE(0, 1, 3);
        unsigned int j = GENERATE(0, 4, 5);

        if (j >= i) {
            v.assign(arr + i, arr + j);

            REQUIRE( v.size() == j - i );
            for (size_t y = 0; y < v.size(); ++y)
                REQUIRE( v[y] == arr[y + i] );

            SECTION("range assign clears values before assigning") {
                v1.assign(arr + i, arr + j);

                REQUIRE( v1.size() == j - i );
                for (size_t y = 0; y < v1.size(); ++y)
                    REQUIRE( v1[y] == arr[y + i] );
            }
        }
    }
    SECTION("fill assign fills deque with correct values") {
        unsigned int i = GENERATE(0, 1, 25);
        int j = GENERATE(0, 42, -12);
        v.assign(i, j);

        REQUIRE( v.size() == i );
        for (size_t y = 0; y < v.size(); ++y)
            REQUIRE( v[y] == j );

        SECTION("fill assign clears values before assigning") {
            v1.assign(i, j);

            REQUIRE( v1.size() == i );
            for (size_t y = 0; y < v1.size(); ++y)
                REQUIRE( v1[y] == j );
        }
    }
}


TEST_CASE("push/pop_back add/remove elements at back of the deque", "[deque][modifiers]")
{
    ft::deque<int> v;
    std::vector<int> stl;
    stl.reserve(100);

    v.pop_back();
    REQUIRE( v.empty() );

    for (unsigned int i = 0; i < 100; ++i) {
        int rand = std::rand() % 200000 - 100000;
        v.push_back(rand);
        stl.push_back(rand);
        REQUIRE( v.back() == stl.back() );
    }
    REQUIRE( v.size() == stl.size() );

    while (v.size() != 0 ) {
        REQUIRE( v.back() == stl.back() );
        v.pop_back();
        stl.pop_back();
    }
    REQUIRE( v.size() == stl.size() );
}

TEST_CASE("push/pop_front add/remove elements at the front of the deque", "[deque][modifiers]")
{
    ft::deque<int> v;
    std::vector<int> stl;
    stl.reserve(100);

    v.pop_front();
    REQUIRE( v.empty() );

    for (unsigned int i = 0; i < 100; ++i) {
        int rand = std::rand() % 200000 - 100000;
        v.push_front(rand);
        stl.insert(stl.begin(), rand);
        REQUIRE( v.front() == stl.front() );
    }
    REQUIRE( v.size() == stl.size() );

    while (v.size() != 0 ) {
        REQUIRE( v[0] == stl[0] );
        v.pop_front();
        stl.erase(stl.begin());
    }
    REQUIRE( v.size() == stl.size() );
}

TEST_CASE("deque insert work correctly", "[deque][modifiers]")
{
    ft::deque<int> v;

    SECTION("single element insert works correctly") {
        ft::deque<int>::iterator ret;
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
    SECTION("fill insert works correctly") {
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
    SECTION("range insert works correctly") {
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

TEST_CASE("deque erase work correctly", "[deque][modifiers]")
{
    int arr[] = {23, 1, 233, 4, 55, 3};
    ft::deque<int> v (arr, arr + 6);             // { 23, 1, 233, 4, 55, 3 }
    ft::deque<int>::iterator ret;

    SECTION("single element erase works correctly") {
        ret = v.erase(v.begin() + 1);                // { 23, 233, 4, 55, 3 }
        REQUIRE( v.size() == 5 );
        REQUIRE( ret[-1] == 23 );
        REQUIRE( ret[0] == 233 );
        REQUIRE( ret[1] == 4 );

        ret = v.erase(v.end() - 1);              // { 23, 233, 4, 55 }
        REQUIRE( v.size() == 4 );
        REQUIRE( ret == v.end() );
        REQUIRE( ret[-1] == 55 );

        ret = v.erase(v.begin() + 2);            // { 23, 233, 55 }
        REQUIRE( v.size() == 3 );
        REQUIRE( ret[-2] == 23 );
        REQUIRE( ret[-1] == 233 );
        REQUIRE( ret[0] == 55 );
    }

    SECTION("multiple element erase works correctly") {
        ret = v.erase(v.end() - 1, v.end());              // { 23, 1, 233, 4, 55 }
        REQUIRE( v.size() == 5 );
        REQUIRE( ret == v.end() );
        REQUIRE( ret[-1] == 55 );

        ret = v.erase(v.begin(), v.begin() + 1);     // { 1, 233, 4, 55 }
        REQUIRE( v.size() == 4 );
        REQUIRE( ret[0] == 1 );
        REQUIRE( ret[1] == 233 );
        REQUIRE( ret[2] == 4 );

        ret = v.erase(v.begin() + 1, v.end() - 1);      // { 1, 55 }
        REQUIRE( ret[-1] == 1 );
        REQUIRE( ret[0] == 55 );

        ret = v.erase(v.begin(), v.end());              // { }
        REQUIRE( ret == v.end() );
        REQUIRE( ret == v.begin() );
    }
}

TEST_CASE("swap swaps deque sizes and content", "[deque][modifiers]")
{
    SECTION("works for non empty deques") {
        ft::deque<int> v1 (10, 100);
        ft::deque<int> v2 (2, -12);

        v1.swap(v2);
        REQUIRE( v1.size() == 2 );
        REQUIRE( v2.size() == 10 );
        for (auto it = v1.begin(); it != v1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = v2.begin(); it != v2.end(); ++it)
            REQUIRE( *it == 100 );
        ft::swap(v1, v2);
        REQUIRE( v1.size() == 10 );
        REQUIRE( v2.size() == 2 );
        for (auto it = v1.begin(); it != v1.end(); ++it)
            REQUIRE( *it == 100 );
        for (auto it = v2.begin(); it != v2.end(); ++it)
            REQUIRE( *it == -12 );
    }
    SECTION("works for swapping non-empty and empty deques") {
        ft::deque<int> v1 (10, 100);
        ft::deque<int> v2;

        v1.swap(v2);
        REQUIRE( v1.empty() );
        REQUIRE( v2.size() == 10 );
        REQUIRE(( v1.begin() == v1.end() ));
        for (auto it = v2.begin(); it != v2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("works for empty deques") {
        ft::deque<int> v1;
        ft::deque<int> v2;

        v1.swap(v2);
        REQUIRE( v1.empty() );
        REQUIRE( v2.empty() );
        REQUIRE(( v1.begin() == v1.end() ));
        REQUIRE(( v2.begin() == v2.end() ));

        ft::swap(v1, v2);
        REQUIRE( v1.empty() );
        REQUIRE( v2.empty() );
        REQUIRE(( v1.begin() == v1.end() ));
        REQUIRE(( v2.begin() == v2.end() ));
    }
}

TEST_CASE("deque resizing works correctly", "[deque][modifiers]")
{
    int arr[] = { 1, 32, 0, -23 };
    ft::deque<int> v (arr, arr + 4);

    SECTION("deque elements are removed when arg is smaller than current size") {
        REQUIRE( v.size() == 4 );
        v.resize(2);
        REQUIRE( v.size() == 2 );
        REQUIRE( v[0] == 1 );
        REQUIRE( v[1] == 32 );
        REQUIRE(( v.begin() + 2 == v.end() ));
        v.resize(1);
        REQUIRE( v.size() == 1 );
        REQUIRE( v[0] == 1 );
        REQUIRE(( v.begin() + 1 == v.end() ));
        v.resize(0);
        REQUIRE( v.size() == 0 );
        REQUIRE(( v.begin() == v.end() ));
    }
    SECTION("deque is expanded when arg is greater than current size") {
        v.resize(6);
        REQUIRE( v.size() == 6 );
        for (int i = 0; i < 4; ++i)
            REQUIRE( v[i] == arr[i] );
        REQUIRE( v[4] == 0 );
        REQUIRE( v[5] == 0 );
    }
    SECTION("deque does not change if arg is equal to current size") {
        v.resize(4);
        REQUIRE( v.size() == 4 );
        for (unsigned int i = 0; i < v.size(); ++i)
            REQUIRE( v[i] == arr[i] );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("clear removes all elements and sets size to zero", "[deque][modifiers]", ft::deque, TYPE_LIST)
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
TEST_CASE("front and back return deques first element", "[deque][element access]")
{
    int arr[] = { 1, 32, 0, -23 };
    ft::deque<int> v (arr, arr + 4);
    const ft::deque<int> cdq (v);

    // const
    REQUIRE( cdq.front() == 1 );
    REQUIRE( cdq.back() == -23 );

    // front
    REQUIRE( v.front() == 1 );
    v.front() -= 22;
    REQUIRE( v.front() == -21 );
    v.front() += 21;
    REQUIRE( v.front() == 0 );

    // back
    REQUIRE( v.back() == -23 );
    v.back() -= 22;
    REQUIRE( v.back() == -45 );
    v.back() += 45;
    REQUIRE( v.back() == 0 );
}

TEMPLATE_PRODUCT_TEST_CASE("at throws only when index is out of range", "[deque][element access]", ft::deque, TYPE_LIST)
{
        unsigned int i = GENERATE(1, 25, 100);
        TestType cnt1;
        TestType cnt2 (i);
        const TestType const_cnt1;
        const TestType const_cnt2 (i);

        REQUIRE_NOTHROW( cnt2.at(rand() % i) );
        REQUIRE_NOTHROW( const_cnt2.at(rand() % i) );
        REQUIRE_THROWS_AS( cnt1.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( cnt2.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( const_cnt1.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( const_cnt2.at(rand() % i + i), std::out_of_range);
}

/* CAPACITY */
TEMPLATE_PRODUCT_TEST_CASE("empty reflects deque state", "[deque][capacity]", ft::deque, TYPE_LIST)
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    cnt.push_back(VALUE_TYPE());
    REQUIRE_FALSE( cnt.empty() );
    cnt.pop_back();
    REQUIRE( cnt.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size returns updated deque size", "[deque][capacity]", ft::deque, TYPE_LIST)
{
    TestType v;

    REQUIRE( v.size() == 0 );
    v.insert(v.begin(), 5, VALUE_TYPE());
    REQUIRE( v.size() == 5 );
    v.clear();
    REQUIRE( v.size() == 0 );
}
