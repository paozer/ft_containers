#include "../srcs/deque/deque.hpp"
#include "catch.hpp"
#include <deque>
#include <list>

#ifdef VERIFY_UNIT_TESTS
# define LIB std
#else
# define LIB ft
#endif

#define TYPE_LIST  ( int, unsigned char, double, std::string, LIB::deque<int>, std::deque<std::string>, std::list<std::string>, std::list<std::deque<int> > )
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("deque construction works correctly", "[deque][basics]")
{
    SECTION("deque declaration creates empty deque") {
        LIB::deque<int> dq;
        REQUIRE( dq.empty() );
        REQUIRE( dq.size() == 0 );
    }
    SECTION("fill constructor returns correct deque") {
        unsigned int size = GENERATE(0, 1, 5);
        int fill = GENERATE(0, -41);
        LIB::deque<int> dq (size, fill);

        REQUIRE( dq.size() == size );
        for (unsigned int i = 0; i < size; ++i)
            REQUIRE( dq[i] == fill );
    }
    SECTION("range constructor returns correct deque") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            LIB::deque<int> dq (arr + first, arr + last);

            REQUIRE( dq.size() == last - first );
            for (size_t i = 0; i < last - first; ++i)
                REQUIRE( dq[i] == arr[i + first] );
        }
    }
    SECTION ("copy constructor returns correct deque") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        LIB::deque<int> dq1;
        LIB::deque<int> dq2 (dq1);
        LIB::deque<int> dq3 (arr + 2, arr + 6); // { 12, 5, 60, 99 }

        REQUIRE( dq1.empty() );
        REQUIRE( dq2.empty() );
        REQUIRE( dq3.size() == 4 );
        REQUIRE( dq3[0] == 12 );
        REQUIRE( dq3[1] == 5 );
        REQUIRE( dq3[2] == 60 );
        REQUIRE( dq3[3] == 99 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("deque correctly copies value upon construction", "[deque][basics]", LIB::deque, TYPE_LIST)
{
    // this test may produces double free there is a problem with
    // your allocation/copying with non builtin types
    TestType dq1 (10, VALUE_TYPE());
    TestType dq2 (dq1);
    dq1.clear();
    dq2.clear();
}

/* ASSIGNATION OPERATOR */
TEST_CASE("deque assignation works correctly", "[deque][assignation operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    LIB::deque<int> dq1 (a, a + 5);
    LIB::deque<int> dq2 (b, b + 2);
    LIB::deque<int> dq3;
    LIB::deque<int>::iterator lit;
    LIB::deque<int>::iterator rit;

    dq1 = dq2;
    REQUIRE( dq1.size() == dq2.size() );
    lit = dq1.begin();
    rit = dq2.begin();
    for (; lit != dq1.end(); ++lit, ++rit)
        REQUIRE( *lit == *rit );
    dq2 = dq3;
    REQUIRE( dq2.size() == dq3.size() );
    REQUIRE( dq2.begin() == dq2.end() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("deque relational operators work correctly", "[deque][relational operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, 67, 1, 0};
    LIB::deque<int> dq1 (a, a + 5);
    LIB::deque<int> dq2 (b, b + 2);
    LIB::deque<int> dq3;
    LIB::deque<int> dq4 (c, c + 6);
    LIB::deque<int> dq5 (d, d + 6);

    // dq1 vs dq2
    REQUIRE_FALSE( dq1 == dq2 );
    REQUIRE( dq1 != dq2 );
    REQUIRE_FALSE( dq1 < dq2 );
    REQUIRE_FALSE( dq1 <= dq2 );
    REQUIRE( dq1 > dq2 );
    REQUIRE( dq1 >= dq2 );

    // dq1 vs dq1
    REQUIRE( dq1 == dq1 );
    REQUIRE_FALSE( dq1 != dq1 );
    REQUIRE_FALSE( dq1 < dq1 );
    REQUIRE( dq1 <= dq1 );
    REQUIRE_FALSE( dq1 > dq1 );
    REQUIRE( dq1 >= dq1 );

    // dq1 vs dq3
    REQUIRE_FALSE( dq1 == dq3 );
    REQUIRE( dq1 != dq3 );
    REQUIRE_FALSE( dq1 < dq3 );
    REQUIRE_FALSE( dq1 <= dq3 );
    REQUIRE( dq1 > dq3 );
    REQUIRE( dq1 >= dq3 );

    // dq3 vs dq3
    REQUIRE( dq3 == dq3 );
    REQUIRE_FALSE( dq3 != dq3 );
    REQUIRE_FALSE( dq3 < dq3 );
    REQUIRE( dq3 <= dq3 );
    REQUIRE_FALSE( dq3 > dq3 );
    REQUIRE( dq3 >= dq3 );

    // dq1 vs dq4
    REQUIRE_FALSE( dq1 == dq4 );
    REQUIRE( dq1 != dq4 );
    REQUIRE( dq1 < dq4 );
    REQUIRE( dq1 <= dq4 );
    REQUIRE_FALSE( dq1 > dq4 );
    REQUIRE_FALSE( dq1 >= dq4 );

    // dq4 vs dq5
    REQUIRE_FALSE( dq4 == dq5 );
    REQUIRE( dq4 != dq5 );
    REQUIRE( dq4 < dq5 );
    REQUIRE( dq4 <= dq5 );
    REQUIRE_FALSE( dq4 > dq5 );
    REQUIRE_FALSE( dq4 >= dq5 );
}

/* ITERATORS */
TEST_CASE("deque begin returns first element and can be incremented", "[deque][iterators]")
{
    SECTION("non-const iterator behaviour") {
        LIB::deque<int> dq (5, 10);              // { 10, 10, 10, 10, 10 }
        REQUIRE( *dq.begin() == 10 );
        *dq.begin() = 5;                         // { 5, 10, 10, 10, 10 }
        *++dq.begin() = 6;                       // { 5, 6, 10, 10, 10 }
        REQUIRE( dq.begin()[0] == 5 );
        REQUIRE( dq.begin()[1] == 6 );
        REQUIRE( dq.begin()[2] == 10 );
        dq.begin()[0] = 2;                         // { 2, 6, 10, 10, 10 }
        REQUIRE( *dq.begin() == 2 );
        REQUIRE( *(dq.begin() + 1) == 6 );
        REQUIRE( *(dq.begin() + 2) == 10 );
        REQUIRE(( dq.begin() - dq.end() == -5 ));
    }
    SECTION("const iterator behaviour") {
        int arr[] = {1, 4, -1, 2, 33};
        LIB::deque<int> dq2 (arr, arr + 5);
        const LIB::deque<int> dq (dq2);
        const LIB::deque<int> dq1;

        REQUIRE( *dq.begin() == 1 );
        REQUIRE( dq.begin()[1] == 4 );
        REQUIRE( dq.begin()[2] == -1 );
        REQUIRE( dq.begin()[3] == 2 );
        REQUIRE( dq.begin()[4] == 33 );
        REQUIRE( (dq.begin() + 5) == dq.end() );
        REQUIRE( dq1.begin() == dq1.end() );

        LIB::deque<int>::const_iterator it = dq2.begin();
        REQUIRE( *it == 1 );
        REQUIRE( *(it + 1) == 4 );
        REQUIRE( *(it + 2) == -1 );
        REQUIRE( *(it + 3) == 2 );
        REQUIRE( *(it + 4) == 33 );
        REQUIRE( (it + 5) == dq2.end() );
    }
}

TEMPLATE_TEST_CASE("deque end works correctly", "[deque][iterators]", LIB::deque<int>)
{
    SECTION("non-const iterator behaviour") {
        LIB::deque<int> dq (5, 10);                 // { 10, 10, 10, 10, 10 }
        REQUIRE( dq.end()[-1] == 10 );
        *(dq.end() - 1) = 5;                        // { 10, 10, 10, 10, 5 }
        dq.end()[-2] = 6;                           // { 10, 10, 10, 6, 5 }
        REQUIRE( dq.end()[-1] == 5 );
        REQUIRE( *(dq.end() - 2) == 6 );
        *(dq.end() - 1) = 2;                        // { 10, 10, 10, 6, 2 }
        dq.end()[-4] = 1;                           // { 10, 1, 10, 6, 2 }
        REQUIRE( *(dq.end() - 1) == 2 );
        REQUIRE( *(dq.end() - 2) == 6 );
        REQUIRE( dq.begin()[1] == 1 );
    }
    SECTION("const iterator behaviour") {
        int arr[] = {1, 4, -1, 2, 33};
        LIB::deque<int> dq2 (arr, arr + 5);
        const LIB::deque<int> dq (dq2);
        const LIB::deque<int> dq1;

        REQUIRE( *(dq.end() - 1) == 33 );
        REQUIRE( *(dq.end() - 2) == 2 );
        REQUIRE( *(dq.end() - 3) == -1 );
        REQUIRE( dq.end()[-4] == 4 );
        REQUIRE( dq.end()[-5] == 1 );
        REQUIRE( dq.begin() == (dq.end() - 5) );
        REQUIRE( dq1.begin() == dq1.end() );

        LIB::deque<int>::const_iterator it = dq2.end();
        REQUIRE( it[-1] == 33 );
        REQUIRE( it[-2] == 2 );
        REQUIRE( it[-3] == -1 );
        REQUIRE( *(it - 4) == 4 );
        REQUIRE( *(it - 5) == 1 );
        REQUIRE( (it - 5) == dq2.begin() );
    }
}

TEST_CASE("reverse iterators can be used to iterate over deques", "[deque][iterator]")
{
    int arr[] = {1, 4, -1, 2, 33};
    LIB::deque<int> dq0 (arr, arr + 5);
    const LIB::deque<int> dq1 (dq0);

    REQUIRE( (dq0.rbegin() + 2)[0] == dq0.rbegin()[2] );
    REQUIRE( dq0.rbegin()[3] == dq0.rend()[-2] );
    REQUIRE( dq0.rbegin() + 5 == dq0.rend() );
    REQUIRE( dq0.rbegin() == dq0.rend() - 5 );
    REQUIRE( dq0.rbegin() + 1 == dq0.rend() - 4 );
}

/* MODIFIERS */
TEST_CASE("deque assign methods work correctly", "[deque][modifiers]")
{
    int arr[] = {1, 4, -1, 2, 33};
    LIB::deque<int> dq;
    LIB::deque<int> dq1 (10, 1);

    SECTION("range assign creates deque with correct values") {
        unsigned int i = GENERATE(0, 1, 3);
        unsigned int j = GENERATE(0, 4, 5);

        if (j >= i) {
            dq.assign(arr + i, arr + j);

            REQUIRE( dq.size() == j - i );
            for (size_t y = 0; y < dq.size(); ++y)
                REQUIRE( dq[y] == arr[y + i] );

            SECTION("range assign clears values before assigning") {
                dq1.assign(arr + i, arr + j);

                REQUIRE( dq1.size() == j - i );
                for (size_t y = 0; y < dq1.size(); ++y)
                    REQUIRE( dq1[y] == arr[y + i] );
            }
        }
    }
    SECTION("fill assign fills deque with correct values") {
        unsigned int i = GENERATE(0, 1, 25);
        int j = GENERATE(0, 42, -12);
        dq.assign(i, j);

        REQUIRE( dq.size() == i );
        for (size_t y = 0; y < dq.size(); ++y)
            REQUIRE( dq[y] == j );

        SECTION("fill assign clears values before assigning") {
            dq1.assign(i, j);

            REQUIRE( dq1.size() == i );
            for (size_t y = 0; y < dq1.size(); ++y)
                REQUIRE( dq1[y] == j );
        }
    }
}


TEST_CASE("push/pop_back add/remove elements at back of the deque", "[deque][modifiers]")
{
    LIB::deque<int> dq;
    std::vector<int> stl;
    stl.reserve(100);

    for (unsigned int i = 0; i < 100; ++i) {
        int rand = std::rand() % 200000 - 100000;
        dq.push_back(rand);
        stl.push_back(rand);
        REQUIRE( dq.back() == stl.back() );
    }
    REQUIRE( dq.size() == stl.size() );

    while (dq.size() != 0 ) {
        REQUIRE( dq.back() == stl.back() );
        dq.pop_back();
        stl.pop_back();
    }
    REQUIRE( dq.size() == stl.size() );
}

TEST_CASE("push/pop_front add/remove elements at the front of the deque", "[deque][modifiers]")
{
    LIB::deque<int> dq;
    std::vector<int> stl;
    stl.reserve(100);

    for (unsigned int i = 0; i < 100; ++i) {
        int rand = std::rand() % 200000 - 100000;
        dq.push_front(rand);
        stl.insert(stl.begin(), rand);
        REQUIRE( dq.front() == stl.front() );
    }
    REQUIRE( dq.size() == stl.size() );

    while (dq.size() != 0 ) {
        REQUIRE( dq[0] == stl[0] );
        dq.pop_front();
        stl.erase(stl.begin());
    }
    REQUIRE( dq.size() == stl.size() );
}

TEST_CASE("deque insert work correctly", "[deque][modifiers]")
{
    LIB::deque<int> dq;

    SECTION("single element insert works correctly") {
        LIB::deque<int>::iterator ret;
        ret = dq.insert(dq.begin(), 5);               // { 5 }
        REQUIRE( *ret == 5 );
        ret = dq.insert(dq.end(), 10);                // { 5, 10 }
        REQUIRE( *ret == 10 );
        ret = dq.insert(dq.begin() + 1, 7);             // { 5, 7, 10 }
        REQUIRE( *ret == 7 );
        ret = dq.insert(dq.end() - 2, 6);             // { 5, 6, 7, 10 }
        REQUIRE( *ret == 6 );
        ret = dq.insert(dq.begin() + 2, 0);           // { 5, 6, 0, 7, 10 }
        REQUIRE( *ret == 0 );

        REQUIRE( dq.size() == 5 );
        REQUIRE( dq[0] == 5 );
        REQUIRE( dq[1] == 6 );
        REQUIRE( dq[2] == 0 );
        REQUIRE( dq[3] == 7 );
        REQUIRE( dq[4] == 10 );
    }
    SECTION("fill insert works correctly") {
        dq.insert(dq.begin(), 5, 10);                 // { 10, 10, 10, 10, 10 }
        dq.insert(dq.end(), 1, 100);                  // { 10, 10, 10, 10, 10, 100 }
        dq.insert(++dq.begin(), 2, 0);                // { 10, 0, 0, 10, 10, 10, 10, 100 }
        dq.insert(++++dq.begin(), 1, 42);             // { 10, 0, 42, 0, 10, 10, 10, 10, 100 }
        dq.insert(--dq.end(), 3, 21);                 // { 10, 0, 42, 0, 10, 10, 10, 10, 21, 21, 21, 100 }

        REQUIRE( dq.size() == 12 );
        REQUIRE( dq[0] == 10 );
        REQUIRE( dq[1] == 0 );
        REQUIRE( dq[2] == 42 );
        REQUIRE( dq[3] == 0 );
        for (size_t i = 4; i < 8; ++i)
            REQUIRE( dq[i] == 10 );
        REQUIRE( dq[8] == 21 );
        REQUIRE( dq[9] == 21 );
        REQUIRE( dq[10] == 21 );
        REQUIRE( dq[11] == 100 );
    }
    SECTION("range insert works correctly") {
        int arr[] = {12, 1, 4, 5, 6, 7};
        int arr1[] = { 0 , -32 };
        dq.insert(dq.begin(), arr1, arr1 + 1);        // { 0 }
        dq.insert(dq.end(), arr + 1, arr + 5);        // { 0, 1, 4, 5, 6 }
        dq.insert(++dq.begin(), arr + 3, arr + 5);    // { 0, 5, 6, 1, 4, 5, 6 }
        dq.insert(----dq.end(), arr1 + 1, arr1 + 2);  // { 0, 5, 6, 1, 4, -32, 5, 6 }

        REQUIRE( dq.size() == 8 );
        REQUIRE( dq[0] == 0 );
        REQUIRE( dq[1] == 5 );
        REQUIRE( dq[2] == 6 );
        REQUIRE( dq[3] == 1 );
        REQUIRE( dq[4] == 4 );
        REQUIRE( dq[5] == -32 );
        REQUIRE( dq[6] == 5 );
        REQUIRE( dq[7] == 6 );
    }
}

TEST_CASE("deque erase work correctly", "[deque][modifiers]")
{
    int arr[] = {23, 1, 233, 4, 55, 3};
    LIB::deque<int> dq (arr, arr + 6);             // { 23, 1, 233, 4, 55, 3 }
    LIB::deque<int>::iterator ret;

    SECTION("single element erase works correctly") {
        ret = dq.erase(dq.begin() + 1);                // { 23, 233, 4, 55, 3 }
        REQUIRE( dq.size() == 5 );
        REQUIRE( ret[-1] == 23 );
        REQUIRE( ret[0] == 233 );
        REQUIRE( ret[1] == 4 );

        ret = dq.erase(dq.end() - 1);              // { 23, 233, 4, 55 }
        REQUIRE( dq.size() == 4 );
        REQUIRE( ret == dq.end() );
        REQUIRE( ret[-1] == 55 );

        ret = dq.erase(dq.begin() + 2);            // { 23, 233, 55 }
        REQUIRE( dq.size() == 3 );
        REQUIRE( ret[-2] == 23 );
        REQUIRE( ret[-1] == 233 );
        REQUIRE( ret[0] == 55 );
    }
    SECTION("multiple element erase works correctly") {
        ret = dq.erase(dq.end() - 1, dq.end());              // { 23, 1, 233, 4, 55 }
        REQUIRE( dq.size() == 5 );
        REQUIRE( ret == dq.end() );
        REQUIRE( ret[-1] == 55 );

        ret = dq.erase(dq.begin(), dq.begin() + 1);     // { 1, 233, 4, 55 }
        REQUIRE( dq.size() == 4 );
        REQUIRE( ret[0] == 1 );
        REQUIRE( ret[1] == 233 );
        REQUIRE( ret[2] == 4 );

        ret = dq.erase(dq.begin() + 1, dq.end() - 1);      // { 1, 55 }
        REQUIRE( ret[-1] == 1 );
        REQUIRE( ret[0] == 55 );

        ret = dq.erase(dq.begin(), dq.end());              // { }
        REQUIRE( ret == dq.end() );
        REQUIRE( ret == dq.begin() );
    }
}

TEST_CASE("swap swaps deque sizes and content", "[deque][modifiers]")
{
    SECTION("works for non empty deques") {
        LIB::deque<int> dq1 (10, 100);
        LIB::deque<int> dq2 (2, -12);

        dq1.swap(dq2);
        REQUIRE( dq1.size() == 2 );
        REQUIRE( dq2.size() == 10 );
        for (auto it = dq1.begin(); it != dq1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            REQUIRE( *it == 100 );
        LIB::swap(dq1, dq2);
        REQUIRE( dq1.size() == 10 );
        REQUIRE( dq2.size() == 2 );
        for (auto it = dq1.begin(); it != dq1.end(); ++it)
            REQUIRE( *it == 100 );
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            REQUIRE( *it == -12 );
    }
    SECTION("works for swapping non-empty and empty deques") {
        LIB::deque<int> dq1 (10, 100);
        LIB::deque<int> dq2;

        dq1.swap(dq2);
        REQUIRE( dq1.empty() );
        REQUIRE( dq2.size() == 10 );
        REQUIRE(( dq1.begin() == dq1.end() ));
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION("works for empty deques") {
        LIB::deque<int> dq1;
        LIB::deque<int> dq2;

        dq1.swap(dq2);
        REQUIRE( dq1.empty() );
        REQUIRE( dq2.empty() );
        REQUIRE(( dq1.begin() == dq1.end() ));
        REQUIRE(( dq2.begin() == dq2.end() ));

        LIB::swap(dq1, dq2);
        REQUIRE( dq1.empty() );
        REQUIRE( dq2.empty() );
        REQUIRE(( dq1.begin() == dq1.end() ));
        REQUIRE(( dq2.begin() == dq2.end() ));
    }
}

TEST_CASE("deque resizing works correctly", "[deque][modifiers]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::deque<int> dq (arr, arr + 4);

    SECTION("deque elements are removed when arg is smaller than current size") {
        REQUIRE( dq.size() == 4 );
        dq.resize(2);
        REQUIRE( dq.size() == 2 );
        REQUIRE( dq[0] == 1 );
        REQUIRE( dq[1] == 32 );
        REQUIRE(( dq.begin() + 2 == dq.end() ));
        dq.resize(1);
        REQUIRE( dq.size() == 1 );
        REQUIRE( dq[0] == 1 );
        REQUIRE(( dq.begin() + 1 == dq.end() ));
        dq.resize(0);
        REQUIRE( dq.size() == 0 );
        REQUIRE(( dq.begin() == dq.end() ));
    }
    SECTION("deque is expanded when arg is greater than current size") {
        dq.resize(6);
        REQUIRE( dq.size() == 6 );
        for (int i = 0; i < 4; ++i)
            REQUIRE( dq[i] == arr[i] );
        REQUIRE( dq[4] == 0 );
        REQUIRE( dq[5] == 0 );
    }
    SECTION("deque does not change if arg is equal to current size") {
        dq.resize(4);
        REQUIRE( dq.size() == 4 );
        for (unsigned int i = 0; i < dq.size(); ++i)
            REQUIRE( dq[i] == arr[i] );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("clear removes all elements and sets size to zero", "[deque][modifiers]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    dq.clear();
    REQUIRE( dq.size() == 0 );
    dq.assign(10, VALUE_TYPE());
    REQUIRE( dq.size() == 10 );
    dq.clear();
    REQUIRE( dq.size() == 0 );
}

/* ELEMENT ACCESS */
TEST_CASE("front and back return deques first element", "[deque][element access]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::deque<int> dq (arr, arr + 4);
    const LIB::deque<int> cdq (dq);

    // const
    REQUIRE( cdq.front() == 1 );
    REQUIRE( cdq.back() == -23 );

    // front
    REQUIRE( dq.front() == 1 );
    dq.front() -= 22;
    REQUIRE( dq.front() == -21 );
    dq.front() += 21;
    REQUIRE( dq.front() == 0 );

    // back
    REQUIRE( dq.back() == -23 );
    dq.back() -= 22;
    REQUIRE( dq.back() == -45 );
    dq.back() += 45;
    REQUIRE( dq.back() == 0 );
}

TEMPLATE_PRODUCT_TEST_CASE("at throws only when index is out of range", "[deque][element access]", LIB::deque, TYPE_LIST)
{
        unsigned int i = GENERATE(1, 25, 100);
        TestType dq1;
        TestType dq2 (i);
        const TestType const_dq1;
        const TestType const_dq2 (i);

        REQUIRE_NOTHROW( dq2.at(rand() % i) );
        REQUIRE_NOTHROW( const_dq2.at(rand() % i) );
        REQUIRE_THROWS_AS( dq1.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( dq2.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( const_dq1.at(rand() % i + i), std::out_of_range);
        REQUIRE_THROWS_AS( const_dq2.at(rand() % i + i), std::out_of_range);
}

/* CAPACITY */
TEMPLATE_PRODUCT_TEST_CASE("empty reflects deque state", "[deque][capacity]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    REQUIRE( dq.empty() );
    dq.push_back(VALUE_TYPE());
    REQUIRE_FALSE( dq.empty() );
    dq.pop_back();
    REQUIRE( dq.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size returns updated deque size", "[deque][capacity]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    REQUIRE( dq.size() == 0 );
    dq.insert(dq.begin(), 5, VALUE_TYPE());
    REQUIRE( dq.size() == 5 );
    dq.clear();
    REQUIRE( dq.size() == 0 );
}
