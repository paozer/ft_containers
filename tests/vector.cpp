#include "../srcs/vector/vector.hpp"
#include "catch.hpp"
#include <vector>
#include <list>

#ifdef VERIFY_UNIT_TESTS
# define LIB std
#else
# define LIB ft
#endif

#define TYPE_LIST  ( int, unsigned char, double, std::string, LIB::vector<int>, std::vector<std::string>, std::list<std::string>, std::list<std::vector<int> > )
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("vector construction works correctly", "[vector][basics]")
{
    SECTION("vector declaration creates empty vector") {
        LIB::vector<int> cnt;
        REQUIRE( cnt.empty() );
        REQUIRE( cnt.size() == 0 );
    }
    SECTION("fill constructor returns correct vector") {
        unsigned int size = GENERATE(0, 1, 5);
        int fill = GENERATE(0, -41);
        LIB::vector<int> cnt (size, fill);

        REQUIRE( cnt.size() == size );
        for (unsigned int i = 0; i < size; ++i)
            REQUIRE( cnt[i] == fill );
    }
    SECTION("range constructor returns correct vector") {
        int v[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            LIB::vector<int> cnt (v + first, v + last);

            REQUIRE( cnt.size() == last - first );
            for (size_t i = 0; i < last - first; ++i)
                REQUIRE( cnt[i] == v[i + first] );
        }
    }
    SECTION("copy constructor returns correct vector") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        LIB::vector<int> v1;
        LIB::vector<int> v2 (v1);
        LIB::vector<int> v3 (arr + 2, arr + 6); // { 12, 5, 60, 99 }

        REQUIRE( v1.size() == 0 );
        REQUIRE( v2.size() == 0 );
        REQUIRE( v3.size() == 4 );
        REQUIRE( v3[0] == 12 );
        REQUIRE( v3[1] == 5 );
        REQUIRE( v3[2] == 60 );
        REQUIRE( v3[3] == 99 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("vector correctly copies value upon construction", "[vector][basics]", LIB::vector, TYPE_LIST)
{
    // this test may produces double free there is a problem with
    // your allocation/copying with non builtin types
    TestType v1 (10, VALUE_TYPE());
    TestType v2 (v1);
    v1.clear();
    v2.clear();
}

/* ASSIGNATION OPERATOR */
TEST_CASE("vector assignation works correctly", "[vector][assignation operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    LIB::vector<int> v1 (a, a + 5);
    LIB::vector<int> v2 (b, b + 2);
    LIB::vector<int> v3;
    LIB::vector<int>::iterator lit;
    LIB::vector<int>::iterator rit;

    v1 = v2;
    REQUIRE( v1.size() == v2.size() );
    lit = v1.begin();
    rit = v2.begin();
    for (; lit != v1.end(); ++lit, ++rit)
        REQUIRE( *lit == *rit );
    v2 = v3;
    REQUIRE( v2.size() == v3.size() );
    REQUIRE( v2.begin() == v2.end() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("vector relational operators work correctly", "[vector][relational operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, -32, 33, 0};
    LIB::vector<int> v1 (a, a + 5);
    LIB::vector<int> v2 (b, b + 2);
    LIB::vector<int> v3;
    LIB::vector<int> v4 (c, c + 6);
    LIB::vector<int> v5 (d, d + 6);

    // v1 vs v2
    REQUIRE_FALSE( v1 == v2 );
    REQUIRE( v1 != v2 );
    REQUIRE_FALSE( v1 < v2 );
    REQUIRE_FALSE( v1 <= v2 );
    REQUIRE( v1 > v2 );
    REQUIRE( v1 >= v2 );

    // v1 vs v1
    REQUIRE( v1 == v1 );
    REQUIRE_FALSE( v1 != v1 );
    REQUIRE_FALSE( v1 < v1 );
    REQUIRE( v1 <= v1 );
    REQUIRE_FALSE( v1 > v1 );
    REQUIRE( v1 >= v1 );

    // v1 vs v3
    REQUIRE_FALSE( v1 == v3 );
    REQUIRE( v1 != v3 );
    REQUIRE_FALSE( v1 < v3 );
    REQUIRE_FALSE( v1 <= v3 );
    REQUIRE( v1 > v3 );
    REQUIRE( v1 >= v3 );

    // v3 vs v3
    REQUIRE( v3 == v3 );
    REQUIRE_FALSE( v3 != v3 );
    REQUIRE_FALSE( v3 < v3 );
    REQUIRE( v3 <= v3 );
    REQUIRE_FALSE( v3 > v3 );
    REQUIRE( v3 >= v3 );

    // v1 vs v4
    REQUIRE_FALSE( v1 == v4 );
    REQUIRE( v1 != v4 );
    REQUIRE( v1 < v4 );
    REQUIRE( v1 <= v4 );
    REQUIRE_FALSE( v1 > v4 );
    REQUIRE_FALSE( v1 >= v4 );

    // v4 vs v5
    REQUIRE_FALSE( v4 == v5 );
    REQUIRE( v4 != v5 );
    REQUIRE_FALSE( v4 < v5 );
    REQUIRE_FALSE( v4 <= v5 );
    REQUIRE( v4 > v5 );
    REQUIRE( v4 >= v5 );
}

/* ITERATORS */
TEST_CASE("begin returns first element and can be incremented", "[vector][iterators]")
{
    SECTION("non-const iterator behaviour") {
        LIB::vector<int> v (5, 10);                             // { 10, 10, 10, 10, 10 }
        LIB::vector<int>::iterator it = v.begin();
        LIB::vector<int>::reverse_iterator rit = v.rbegin();

        REQUIRE( *rit == 10 );
        REQUIRE( *it == 10 );
        *(it++) = 5;                                            // { 5, 10, 10, 10, 10 }
        *it = 6;                                                // { 5, 6, 10, 10, 10 }
        REQUIRE( *v.begin() == 5 );
        REQUIRE( v.begin()[1] == 6 );
        REQUIRE( v.begin()[2] == 10 );
        REQUIRE( v.rend()[-1] == 5 );
        REQUIRE( v.rend()[-2] == 6 );
        REQUIRE( v.rend()[-3] == 10 );
        *v.begin() = 2;                                         // { 2, 6, 10, 10, 10 }
        REQUIRE( *v.begin() == 2 );
        REQUIRE( *(v.begin() + 1) == 6 );
        REQUIRE( *(v.begin() + 2) == 10 );
        REQUIRE( *(v.rend() - 1) == 2 );
        REQUIRE( *(v.rend() - 2) == 6 );
        REQUIRE( *(v.rend() - 3) == 10 );

        REQUIRE( v.end() - v.begin() == 5 );
        REQUIRE( v.begin() - v.end() == -5 );
        REQUIRE( v.rend() - v.rbegin() == 5 );
        REQUIRE( v.begin() - v.end() == -5 );
        REQUIRE( v.rbegin() + 5  == v.rend() );
        REQUIRE( v.rbegin() + 4  != v.rend() );
        REQUIRE( v.rend() - 5  == v.rbegin() );
        REQUIRE( v.rend() - 3  != v.rbegin() );
    }

    SECTION("const iterator behaviour") {
        int arr[] = {1, 4, -1, 2, 33};
        LIB::vector<int> v2 (arr, arr + 5);
        const LIB::vector<int> v (v2);
        const LIB::vector<int> v1;

        REQUIRE( *v.begin() == 1 );
        REQUIRE( v.begin()[1] == 4 );
        REQUIRE( v.begin()[2] == -1 );
        REQUIRE( v.begin()[3] == 2 );
        REQUIRE( v.begin()[4] == 33 );
        REQUIRE( v.begin() + 5 == v.end() );
        REQUIRE( v1.begin() == v1.end() );
        REQUIRE( v1.rbegin() == v1.rend() );
        REQUIRE( v.rbegin() + 5 == v.rend() );

        LIB::vector<int>::const_iterator it = v2.begin();
        REQUIRE( *it == 1 );
        REQUIRE( *(it + 1) == 4 );
        REQUIRE( *(it + 2) == -1 );
        REQUIRE( *(it + 3) == 2 );
        REQUIRE( *(it + 4) == 33 );
        REQUIRE( it + 5 == v2.end() );
    }

    SECTION("misc") {
        int arr[] = {1, 4, -1, 2, 33};
        LIB::vector<int> v (arr, arr + 5);
        LIB::vector<int>::iterator it1 = v.begin();

        REQUIRE( it1 == v.begin() );
        it1 += 2;
        REQUIRE( *it1 == -1 );
        REQUIRE( it1[0] == -1 );
        REQUIRE( it1[1] == 2 );
        REQUIRE( it1[2] == 33 );
        REQUIRE( it1 + 3 == v.end() );
        REQUIRE( 3 + it1 == v.end() );
        REQUIRE( -3 + v.end() == it1 );
        REQUIRE( it1 > v.begin() );
        REQUIRE( it1 >= v.begin() );
        REQUIRE_FALSE( it1 < v.begin() );
        REQUIRE_FALSE( it1 <= v.begin() );
    }

}

/* MODIFIERS */
TEST_CASE("vector assign methods work correctly", "[vector][modifiers]")
{
    int arr[] = {1, 4, -1, 2, 33};
    LIB::vector<int> v;
    LIB::vector<int> v1 (10, 1);

    SECTION("range assign creates vector with correct values") {
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
    SECTION("fill assign fills vector with correct values") {
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


TEST_CASE("push/pop_back add/remove elements at back of the vector", "[vector][modifiers]")
{
    LIB::vector<int> v;
    std::vector<int> stl;

    for (unsigned int i = 0; i < 100; ++i) {
        int rand = std::rand() % 200000 - 100000;
        v.push_back(rand);
        stl.push_back(rand);
        REQUIRE( *(v.end() - 1) == *(stl.end() - 1) );
    }
    REQUIRE( v.size() == stl.size() );

    LIB::vector<int>::iterator it = v.begin();
    std::vector<int>::iterator stlit = stl.begin();
    for (; it != v.end() && stlit != stl.end(); ++it, ++stlit)
        REQUIRE( *it == *stlit );
    while (v.size() != 0 ) {
        REQUIRE( *(v.end() - 1) == *(stl.end() - 1) );
        v.pop_back();
        stl.pop_back();
    }
    REQUIRE( v.size() == stl.size() );
}

TEST_CASE("vector insert work correctly", "[vector][modifiers]")
{
    LIB::vector<int> v;

    SECTION("single element insert works correctly") {
        LIB::vector<int>::iterator ret;
        ret = v.insert(v.begin(), 5);               // { 5 }
        REQUIRE( *ret == 5 );
        ret = v.insert(v.end(), 10);                // { 5, 10 }
        REQUIRE( *ret == 10 );
        ret = v.insert(v.begin() + 1, 7);           // { 5, 7, 10 }
        REQUIRE( *ret == 7 );
        ret = v.insert(v.end() - 2, 6);             // { 5, 6, 7, 10 }
        REQUIRE( *ret == 6 );
        ret = v.insert(v.begin() + 2, 0);           // { 5, 6, 0, 7, 10 }
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
        v.insert(v.begin() + 1, 2, 0);              // { 10, 0, 0, 10, 10, 10, 10, 100 }
        v.insert(v.begin() + 2, 1, 42);             // { 10, 0, 42, 0, 10, 10, 10, 10, 100 }
        v.insert(v.end() - 1, 3, 21);               // { 10, 0, 42, 0, 10, 10, 10, 10, 21, 21, 21, 100 }
        v.insert(v.end(), 0, -1);

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
        v.insert(v.end(), arr, arr);

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
    LIB::vector<int> v (arr, arr + 6);             // { 23, 1, 233, 4, 55, 3 }
    LIB::vector<int>::iterator ret;

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
        ret = v.erase(v.end() - 1, v.end());              // { 23, 1, 233, 4, 55 }
        REQUIRE( v.size() == 5 );
        REQUIRE( ret == v.end() );
        REQUIRE( *(ret - 1) == 55 );

        ret = v.erase(v.begin() + 1, v.begin() + 2);     // { 23, 233, 4, 55 }
        REQUIRE( v.size() == 4 );
        REQUIRE( *(ret - 1) == 23 );
        REQUIRE( *ret == 233 );
        REQUIRE( *(ret + 1) == 4 );

        ret = v.erase(v.begin() + 1, v.end() - 1);      // { 23, 55 }
        REQUIRE( *(ret - 1) == 23 );
        REQUIRE( *ret == 55 );

        ret = v.erase(v.begin(), v.end());              // { }
        REQUIRE( ret == v.end() );
        REQUIRE( ret == v.begin() );
    }
}

TEST_CASE("swap swaps vector sizes, capacities and content", "[vector][modifiers]")
{
    SECTION("works for non empty vectors") {
        LIB::vector<int> v1 (10, 100);
        LIB::vector<int> v2 (2, -12);
        auto v1_cap = v1.capacity();
        auto v2_cap = v2.capacity();

        v1.swap(v2);
        REQUIRE( v1.size() == 2 );
        REQUIRE( v2.size() == 10 );
        REQUIRE( v1.capacity() == v2_cap );
        REQUIRE( v2.capacity() == v1_cap );
        for (auto it = v1.begin(); it != v1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = v2.begin(); it != v2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("works for swapping non-empty and empty vectors") {
        LIB::vector<int> v1 (10, 100);
        LIB::vector<int> v2;
        auto v1_cap = v1.capacity();
        auto v2_cap = v2.capacity();

        v1.swap(v2);
        REQUIRE( v1.size() == 0 );
        REQUIRE( v2.size() == 10 );
        REQUIRE( v1.capacity() == v2_cap );
        REQUIRE( v2.capacity() == v1_cap );
        REQUIRE(( v1.begin() == v1.end() ));
        for (auto it = v2.begin(); it != v2.end(); ++it)
            REQUIRE( *it == 100 );

        LIB::swap(v1, v2);
        REQUIRE( v1.size() == 10 );
        REQUIRE( v2.size() == 0 );
        REQUIRE( v1.capacity() == v1_cap );
        REQUIRE( v2.capacity() == v2_cap );
        for (auto it = v1.begin(); it != v1.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("works for empty vectors") {
        LIB::vector<int> v1;
        LIB::vector<int> v2;
        auto v1_cap = v1.capacity();
        auto v2_cap = v2.capacity();

        v1.swap(v2);
        REQUIRE( v1.size() == 0 );
        REQUIRE( v2.size() == 0 );
        REQUIRE( v1.capacity() == v2_cap );
        REQUIRE( v2.capacity() == v1_cap );
        REQUIRE(( v1.begin() == v1.end() ));
        REQUIRE(( v2.begin() == v2.end() ));
    }
}

TEST_CASE("resize works correctly", "[vector][modifiers]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::vector<int> v (arr, arr + 4);

    SECTION("vector elements are removed when arg is smaller than current size") {
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
    SECTION("vector is expanded when arg is greater than current size") {
        v.resize(6);
        REQUIRE( v.size() == 6 );
        for (int i = 0; i < 4; ++i)
            REQUIRE( v[i] == arr[i] );
        REQUIRE( v[4] == 0 );
        REQUIRE( v[5] == 0 );
    }
    SECTION("vector does not change if arg is equal to current size") {
        v.resize(4);
        REQUIRE( v.size() == 4 );
        for (unsigned int i = 0; i < v.size(); ++i)
            REQUIRE( v[i] == arr[i] );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("clear removes all elements and sets size to zero", "[vector][modifiers]", LIB::vector, TYPE_LIST)
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
TEST_CASE("front and back return vectors first element", "[vector][element access]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::vector<int> v (arr, arr + 4);
    const LIB::vector<int> cv (arr, arr + 3);
    LIB::vector<int>::reference front_ref = v.front();
    LIB::vector<int>::reference back_ref = v.back();

    // const
    REQUIRE( cv.front() == 1 );
    REQUIRE( cv.back() == 0 );

    // front
    REQUIRE( v.front() == 1 );
    REQUIRE( front_ref == 1 );
    v.front() -= 22;
    REQUIRE( v.front() == -21 );
    REQUIRE( front_ref == -21 );
    v.front() += 21;
    REQUIRE( v.front() == 0 );
    REQUIRE( front_ref == 0 );

    // back
    REQUIRE( v.back() == -23 );
    REQUIRE( back_ref == -23 );
    v.back() -= 22;
    REQUIRE( v.back() == -45 );
    REQUIRE( back_ref == -45 );
    v.back() += 45;
    REQUIRE( v.back() == 0 );
    REQUIRE( back_ref == 0 );
}

TEMPLATE_PRODUCT_TEST_CASE("at throws only when index is out of range", "[vector][element access]", LIB::vector, TYPE_LIST)
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
TEST_CASE("reserve allocates enough memory", "[vector][capacity]")
{
    unsigned int i = GENERATE(0, 1, 25, 100);
    LIB::vector<std::string> v;
    std::string arr[] = {"hello", "this", "is", "a", "test"};

    v.reserve(i);
    unsigned int cap = v.capacity();
    // vector allocates at the least the requested memory
    REQUIRE( cap >= i );
    for (unsigned int j = 0; j < v.capacity(); ++j)
        v.push_back(arr[std::rand() % 4]);
    // vector only reallocates when full
    REQUIRE( v.capacity() == cap );
    v.push_back(arr[std::rand() % 4]);
    REQUIRE( v.capacity() > cap );
}

TEMPLATE_PRODUCT_TEST_CASE("empty reflects vector state", "[vector][capacity]", LIB::vector, TYPE_LIST)
{
    TestType cnt;
    REQUIRE( cnt.empty() );
    cnt.push_back(VALUE_TYPE());
    REQUIRE_FALSE( cnt.empty() );
    cnt.pop_back();
    REQUIRE( cnt.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size returns updated vector size", "[vector][capacity]", LIB::vector, TYPE_LIST)
{
    TestType v;

    REQUIRE( v.size() == 0 );
    v.insert(v.begin(), 5, VALUE_TYPE());
    REQUIRE( v.size() == 5 );
    v.clear();
    REQUIRE( v.size() == 0 );
}
