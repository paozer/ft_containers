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
TEST_CASE("construction deque", "[deque][basics]")
{
    SECTION("declaration returns an empty deque") {
        LIB::deque<int> dq;
        CHECK( dq.empty() );
        CHECK( dq.size() == 0 );
    }
    SECTION("fill construction returns a deque filled with the supplied value") {
        unsigned int size = GENERATE(0, 1, 5);
        int fill = GENERATE(0, -41);
        LIB::deque<int> dq (size, fill);

        CHECK( dq.size() == size );
        for (unsigned int i = 0; i < size; ++i)
            CHECK( dq[i] == fill );
    }
    SECTION("range construction returns a deque filled with the ranges values") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            LIB::deque<int> dq (arr + first, arr + last);
            REQUIRE( dq.size() == last - first );
            for (size_t i = 0; i < last - first; ++i)
                CHECK( dq[i] == arr[i + first] );
        }
    }
    SECTION ("copy construction returns an copy of the supplied deque") {
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        LIB::deque<int> dq1;
        LIB::deque<int> dq2 (dq1);
        LIB::deque<int> dq3 (arr + 2, arr + 6); // { 12, 5, 60, 99 }

        CHECK( dq1.empty() );
        CHECK( dq2.empty() );
        REQUIRE( dq3.size() == 4 );
        CHECK( dq3[0] == 12 );
        CHECK( dq3[1] == 5 );
        CHECK( dq3[2] == 60 );
        CHECK( dq3[3] == 99 );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("allocation/destruction/copying of values deque", "[deque][basics]", LIB::deque, TYPE_LIST)
{
    // this test may produces double free there is a problem with
    // your allocation/copying with non builtin types
    TestType dq1 (10, VALUE_TYPE());
    TestType dq2 (dq1);
    dq1.clear();
    dq2.clear();
}

/* ASSIGNATION OPERATOR */
TEST_CASE("assignation operator deque", "[deque][operators]")
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
        CHECK( *lit == *rit );
    dq2 = dq3;
    CHECK( dq2.empty() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("relational operators deque", "[deque][operators]")
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
    CHECK_FALSE( dq1 == dq2 );
    CHECK( dq1 != dq2 );
    CHECK_FALSE( dq1 < dq2 );
    CHECK_FALSE( dq1 <= dq2 );
    CHECK( dq1 > dq2 );
    CHECK( dq1 >= dq2 );

    // dq1 vs dq1
    CHECK( dq1 == dq1 );
    CHECK_FALSE( dq1 != dq1 );
    CHECK_FALSE( dq1 < dq1 );
    CHECK( dq1 <= dq1 );
    CHECK_FALSE( dq1 > dq1 );
    CHECK( dq1 >= dq1 );

    // dq1 vs dq3
    CHECK_FALSE( dq1 == dq3 );
    CHECK( dq1 != dq3 );
    CHECK_FALSE( dq1 < dq3 );
    CHECK_FALSE( dq1 <= dq3 );
    CHECK( dq1 > dq3 );
    CHECK( dq1 >= dq3 );

    // dq3 vs dq3
    CHECK( dq3 == dq3 );
    CHECK_FALSE( dq3 != dq3 );
    CHECK_FALSE( dq3 < dq3 );
    CHECK( dq3 <= dq3 );
    CHECK_FALSE( dq3 > dq3 );
    CHECK( dq3 >= dq3 );

    // dq1 vs dq4
    CHECK_FALSE( dq1 == dq4 );
    CHECK( dq1 != dq4 );
    CHECK( dq1 < dq4 );
    CHECK( dq1 <= dq4 );
    CHECK_FALSE( dq1 > dq4 );
    CHECK_FALSE( dq1 >= dq4 );

    // dq4 vs dq5
    CHECK_FALSE( dq4 == dq5 );
    CHECK( dq4 != dq5 );
    CHECK( dq4 < dq5 );
    CHECK( dq4 <= dq5 );
    CHECK_FALSE( dq4 > dq5 );
    CHECK_FALSE( dq4 >= dq5 );
}

/* ITERATORS */
TEST_CASE("iterators deque", "[deque][iterators]")
{
    LIB::deque<int> dq (5, 10);                     // { 10, 10, 10, 10, 10 }

    SECTION("begin returns an iterator to the first element") {
        CHECK( *dq.begin() == 10 );
        *dq.begin() = 5;                            // { 5, 10, 10, 10, 10 }
        *++dq.begin() = 6;                          // { 5, 6, 10, 10, 10 }
        CHECK( dq.begin()[0] == 5 );
        CHECK( dq.begin()[1] == 6 );
        CHECK( dq.begin()[2] == 10 );
        dq.begin()[0] = 2;                          // { 2, 6, 10, 10, 10 }
        CHECK( *dq.begin() == 2 );
        CHECK( *(dq.begin() + 1) == 6 );
        CHECK( *(dq.begin() + 2) == 10 );
        CHECK(( dq.begin() - dq.end() == -5 ));

        int arr[] = {1, 4, -1, 2, 33};
        LIB::deque<int> dq2 (arr, arr + 5);
        const LIB::deque<int> dq3 (dq2);
        const LIB::deque<int> dq1;
        CHECK( *dq3.begin() == 1 );
        CHECK( dq3.begin()[1] == 4 );
        CHECK( dq3.begin()[2] == -1 );
        CHECK( dq3.begin()[3] == 2 );
        CHECK( dq3.begin()[4] == 33 );
        CHECK( (dq3.begin() + 5) == dq3.end() );
        CHECK( dq1.begin() == dq1.end() );
        LIB::deque<int>::const_iterator it = dq2.begin();
        CHECK( *it == 1 );
        CHECK( *(it + 1) == 4 );
        CHECK( *(it + 2) == -1 );
        CHECK( *(it + 3) == 2 );
        CHECK( *(it + 4) == 33 );
        CHECK( (it + 5) == dq2.end() );
    }
    SECTION("end returns an iterator to one past the last element") {
        CHECK( dq.end()[-1] == 10 );
        *(dq.end() - 1) = 5;                        // { 10, 10, 10, 10, 5 }
        dq.end()[-2] = 6;                           // { 10, 10, 10, 6, 5 }
        CHECK( dq.end()[-1] == 5 );
        CHECK( *(dq.end() - 2) == 6 );
        *(dq.end() - 1) = 2;                        // { 10, 10, 10, 6, 2 }
        dq.end()[-4] = 1;                           // { 10, 1, 10, 6, 2 }
        CHECK( *(dq.end() - 1) == 2 );
        CHECK( *(dq.end() - 2) == 6 );
        CHECK( dq.begin()[1] == 1 );

        int arr[] = {1, 4, -1, 2, 33};
        LIB::deque<int> dq2 (arr, arr + 5);
        const LIB::deque<int> dq3 (dq2);
        const LIB::deque<int> dq1;

        CHECK( *(dq3.end() - 1) == 33 );
        CHECK( *(dq3.end() - 2) == 2 );
        CHECK( *(dq3.end() - 3) == -1 );
        CHECK( dq3.end()[-4] == 4 );
        CHECK( dq3.end()[-5] == 1 );
        CHECK( dq3.begin() == (dq3.end() - 5) );
        CHECK( dq1.begin() == dq1.end() );

        LIB::deque<int>::const_iterator it = dq2.end();
        CHECK( it[-1] == 33 );
        CHECK( it[-2] == 2 );
        CHECK( it[-3] == -1 );
        CHECK( *(it - 4) == 4 );
        CHECK( *(it - 5) == 1 );
        CHECK( (it - 5) == dq2.begin() );
    }
    SECTION("reverse iterators are functional", "[deque][iterator]")
    {
        int arr[] = {1, 4, -1, 2, 33};
        LIB::deque<int> dq0 (arr, arr + 5);
        const LIB::deque<int> dq1 (dq0);

        CHECK( (dq0.rbegin() + 2)[0] == dq0.rbegin()[2] );
        CHECK( dq0.rbegin()[3] == dq0.rend()[-2] );
        CHECK( dq0.rbegin() + 5 == dq0.rend() );
        CHECK( dq0.rbegin() == dq0.rend() - 5 );
        CHECK( dq0.rbegin() + 1 == dq0.rend() - 4 );
    }
}

/* MODIFIERS */
TEST_CASE("assign deque", "[deque][modifiers]")
{
    int arr[] = {1, 4, -1, 2, 33};
    LIB::deque<int> dq;
    LIB::deque<int> dq1 (10, 1);

    SECTION("ranges can be assigned to deques") {
        unsigned int i = GENERATE(0, 1, 3);
        unsigned int j = GENERATE(0, 4, 5);
        if (j >= i) {
            dq.assign(arr + i, arr + j);
            REQUIRE( dq.size() == j - i );
            for (size_t y = 0; y < dq.size(); ++y)
                CHECK( dq[y] == arr[y + i] );
            dq1.assign(arr + i, arr + j);
            REQUIRE( dq1.size() == j - i );
            for (size_t y = 0; y < dq1.size(); ++y)
                CHECK( dq1[y] == arr[y + i] );
        }
    }
    SECTION("fills can be assigned to deques") {
        unsigned int i = GENERATE(0, 1, 25);
        int j = GENERATE(0, 42, -12);
        dq.assign(i, j);
        REQUIRE( dq.size() == i );
        for (size_t y = 0; y < dq.size(); ++y)
            CHECK( dq[y] == j );
        dq1.assign(i, j);
        REQUIRE( dq1.size() == i );
        for (size_t y = 0; y < dq1.size(); ++y)
            CHECK( dq1[y] == j );
    }
}


TEST_CASE("push/pop deque", "[deque][modifiers]")
{
    LIB::deque<int> dq;
    std::vector<int> stl;
    stl.reserve(100);

    SECTION("values can be pushed/popped to the front") {
        for (unsigned int i = 0; i < 100; ++i) {
            int rand = std::rand() % 200000 - 100000;
            dq.push_back(rand);
            stl.push_back(rand);
            CHECK( dq.back() == stl.back() );
        }
        REQUIRE( dq.size() == stl.size() );
        while (dq.size() != 0 ) {
            CHECK( dq.back() == stl.back() );
            dq.pop_back();
            stl.pop_back();
        }
        REQUIRE( dq.size() == stl.size() );
    }
    SECTION("values can be pushed/popped to the back") {
        for (unsigned int i = 0; i < 100; ++i) {
            int rand = std::rand() % 200000 - 100000;
            dq.push_front(rand);
            stl.insert(stl.begin(), rand);
            CHECK( dq.front() == stl.front() );
        }
        REQUIRE( dq.size() == stl.size() );
        while (dq.size() != 0 ) {
            CHECK( dq[0] == stl[0] );
            dq.pop_front();
            stl.erase(stl.begin());
        }
        CHECK( dq.size() == stl.size() );
    }
}

TEST_CASE("insert deque", "[deque][modifiers]")
{
    LIB::deque<int> dq;

    SECTION("single elements can be inserted") {
        LIB::deque<int>::iterator ret;
        ret = dq.insert(dq.begin(), 5);               // { 5 }
        CHECK( *ret == 5 );
        ret = dq.insert(dq.end(), 10);                // { 5, 10 }
        CHECK( *ret == 10 );
        ret = dq.insert(dq.begin() + 1, 7);             // { 5, 7, 10 }
        CHECK( *ret == 7 );
        ret = dq.insert(dq.end() - 2, 6);             // { 5, 6, 7, 10 }
        CHECK( *ret == 6 );
        ret = dq.insert(dq.begin() + 2, 0);           // { 5, 6, 0, 7, 10 }
        CHECK( *ret == 0 );

        REQUIRE( dq.size() == 5 );
        CHECK( dq[0] == 5 );
        CHECK( dq[1] == 6 );
        CHECK( dq[2] == 0 );
        CHECK( dq[3] == 7 );
        CHECK( dq[4] == 10 );
    }
    SECTION("fills can be inserted") {
        dq.insert(dq.begin(), 5, 10);                 // { 10, 10, 10, 10, 10 }
        dq.insert(dq.end(), 1, 100);                  // { 10, 10, 10, 10, 10, 100 }
        dq.insert(++dq.begin(), 2, 0);                // { 10, 0, 0, 10, 10, 10, 10, 100 }
        dq.insert(++++dq.begin(), 1, 42);             // { 10, 0, 42, 0, 10, 10, 10, 10, 100 }
        dq.insert(--dq.end(), 3, 21);                 // { 10, 0, 42, 0, 10, 10, 10, 10, 21, 21, 21, 100 }

        REQUIRE( dq.size() == 12 );
        CHECK( dq[0] == 10 );
        CHECK( dq[1] == 0 );
        CHECK( dq[2] == 42 );
        CHECK( dq[3] == 0 );
        for (size_t i = 4; i < 8; ++i)
            CHECK( dq[i] == 10 );
        CHECK( dq[8] == 21 );
        CHECK( dq[9] == 21 );
        CHECK( dq[10] == 21 );
        CHECK( dq[11] == 100 );
    }
    SECTION("ranges can be inserted") {
        int arr[] = {12, 1, 4, 5, 6, 7};
        int arr1[] = { 0 , -32 };
        dq.insert(dq.begin(), arr1, arr1 + 1);        // { 0 }
        dq.insert(dq.end(), arr + 1, arr + 5);        // { 0, 1, 4, 5, 6 }
        dq.insert(++dq.begin(), arr + 3, arr + 5);    // { 0, 5, 6, 1, 4, 5, 6 }
        dq.insert(----dq.end(), arr1 + 1, arr1 + 2);  // { 0, 5, 6, 1, 4, -32, 5, 6 }

        REQUIRE( dq.size() == 8 );
        CHECK( dq[0] == 0 );
        CHECK( dq[1] == 5 );
        CHECK( dq[2] == 6 );
        CHECK( dq[3] == 1 );
        CHECK( dq[4] == 4 );
        CHECK( dq[5] == -32 );
        CHECK( dq[6] == 5 );
        CHECK( dq[7] == 6 );
    }
}

TEST_CASE("erase deque", "[deque][modifiers]")
{
    int arr[] = {23, 1, 233, 4, 55, 3};
    LIB::deque<int> dq (arr, arr + 6);             // { 23, 1, 233, 4, 55, 3 }
    LIB::deque<int>::iterator ret;

    SECTION("single elements can be erased") {
        ret = dq.erase(dq.begin() + 1);                // { 23, 233, 4, 55, 3 }
        REQUIRE( dq.size() == 5 );
        CHECK( ret[-1] == 23 );
        CHECK( ret[0] == 233 );
        CHECK( ret[1] == 4 );

        ret = dq.erase(dq.end() - 1);              // { 23, 233, 4, 55 }
        REQUIRE( dq.size() == 4 );
        CHECK( ret == dq.end() );
        CHECK( ret[-1] == 55 );

        ret = dq.erase(dq.begin() + 2);            // { 23, 233, 55 }
        REQUIRE( dq.size() == 3 );
        CHECK( ret[-2] == 23 );
        CHECK( ret[-1] == 233 );
        CHECK( ret[0] == 55 );
    }
    SECTION("ranges can be erased") {
        ret = dq.erase(dq.end() - 1, dq.end());              // { 23, 1, 233, 4, 55 }
        REQUIRE( dq.size() == 5 );
        CHECK( ret == dq.end() );
        CHECK( ret[-1] == 55 );

        ret = dq.erase(dq.begin(), dq.begin() + 1);     // { 1, 233, 4, 55 }
        REQUIRE( dq.size() == 4 );
        CHECK( ret[0] == 1 );
        CHECK( ret[1] == 233 );
        CHECK( ret[2] == 4 );

        ret = dq.erase(dq.begin() + 1, dq.end() - 1);      // { 1, 55 }
        CHECK( ret[-1] == 1 );
        CHECK( ret[0] == 55 );

        ret = dq.erase(dq.begin(), dq.end());              // { }
        CHECK( ret == dq.end() );
        CHECK( ret == dq.begin() );
    }
}

TEST_CASE("swap deque", "[deque][modifiers]")
{
    SECTION("non empty deques can be swapped") {
        LIB::deque<int> dq1 (10, 100);
        LIB::deque<int> dq2 (2, -12);

        dq1.swap(dq2);
        REQUIRE( dq1.size() == 2 );
        REQUIRE( dq2.size() == 10 );
        for (auto it = dq1.begin(); it != dq1.end(); ++it)
            CHECK( *it == -12 );
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            CHECK( *it == 100 );
        LIB::swap(dq1, dq2);
        REQUIRE( dq1.size() == 10 );
        REQUIRE( dq2.size() == 2 );
        for (auto it = dq1.begin(); it != dq1.end(); ++it)
            CHECK( *it == 100 );
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            CHECK( *it == -12 );
    }
    SECTION("non empty and empty deques can be swapped") {
        LIB::deque<int> dq1 (10, 100);
        LIB::deque<int> dq2;

        dq1.swap(dq2);
        REQUIRE( dq1.empty() );
        REQUIRE( dq2.size() == 10 );
        REQUIRE(( dq1.begin() == dq1.end() ));
        for (auto it = dq2.begin(); it != dq2.end(); ++it)
            CHECK( *it == 100 );
    }
    SECTION("empty deques can be swapped") {
        LIB::deque<int> dq1;
        LIB::deque<int> dq2;

        dq1.swap(dq2);
        CHECK( dq1.empty() );
        CHECK( dq2.empty() );
        CHECK(( dq1.begin() == dq1.end() ));
        CHECK(( dq2.begin() == dq2.end() ));

        LIB::swap(dq1, dq2);
        CHECK( dq1.empty() );
        CHECK( dq2.empty() );
        CHECK(( dq1.begin() == dq1.end() ));
        CHECK(( dq2.begin() == dq2.end() ));
    }
}

TEST_CASE("resize deque", "[deque][modifiers]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::deque<int> dq (arr, arr + 4);

    SECTION("deque elements are removed when arg is smaller than current size") {
        REQUIRE( dq.size() == 4 );
        dq.resize(2);
        REQUIRE( dq.size() == 2 );
        CHECK( dq[0] == 1 );
        CHECK( dq[1] == 32 );
        CHECK(( dq.begin() + 2 == dq.end() ));
        dq.resize(1);
        REQUIRE( dq.size() == 1 );
        CHECK( dq[0] == 1 );
        CHECK(( dq.begin() + 1 == dq.end() ));
        dq.resize(0);
        REQUIRE( dq.size() == 0 );
        CHECK(( dq.begin() == dq.end() ));
    }
    SECTION("deque is expanded when arg is greater than current size") {
        dq.resize(6);
        REQUIRE( dq.size() == 6 );
        for (int i = 0; i < 4; ++i)
            CHECK( dq[i] == arr[i] );
        CHECK( dq[4] == 0 );
        CHECK( dq[5] == 0 );
    }
    SECTION("deque does not change if arg is equal to current size") {
        dq.resize(4);
        REQUIRE( dq.size() == 4 );
        for (unsigned int i = 0; i < dq.size(); ++i)
            CHECK( dq[i] == arr[i] );
    }
}

TEMPLATE_PRODUCT_TEST_CASE("clear deque", "[deque][modifiers]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    dq.clear();
    CHECK( dq.size() == 0 );
    dq.assign(10, VALUE_TYPE());
    CHECK( dq.size() == 10 );
    dq.clear();
    CHECK( dq.size() == 0 );
}

/* ELEMENT ACCESS */
TEST_CASE("front/back deque", "[deque][element access]")
{
    int arr[] = { 1, 32, 0, -23 };
    LIB::deque<int> dq (arr, arr + 4);
    const LIB::deque<int> cdq (dq);

    // const
    CHECK( cdq.front() == 1 );
    CHECK( cdq.back() == -23 );

    // front
    CHECK( dq.front() == 1 );
    dq.front() -= 22;
    CHECK( dq.front() == -21 );
    dq.front() += 21;
    CHECK( dq.front() == 0 );

    // back
    CHECK( dq.back() == -23 );
    dq.back() -= 22;
    CHECK( dq.back() == -45 );
    dq.back() += 45;
    CHECK( dq.back() == 0 );
}

TEMPLATE_PRODUCT_TEST_CASE("at deque", "[deque][element access]", LIB::deque, TYPE_LIST)
{
        unsigned int i = GENERATE(1, 25, 100);
        TestType dq1;
        TestType dq2 (i);
        const TestType const_dq1;
        const TestType const_dq2 (i);

        CHECK_NOTHROW( dq2.at(rand() % i) );
        CHECK_NOTHROW( const_dq2.at(rand() % i) );
        CHECK_THROWS_AS( dq1.at(rand() % i + i), std::out_of_range);
        CHECK_THROWS_AS( dq2.at(rand() % i + i), std::out_of_range);
        CHECK_THROWS_AS( const_dq1.at(rand() % i + i), std::out_of_range);
        CHECK_THROWS_AS( const_dq2.at(rand() % i + i), std::out_of_range);
}

/* CAPACITY */
TEMPLATE_PRODUCT_TEST_CASE("empty deque", "[deque][capacity]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    CHECK( dq.empty() );
    dq.push_back(VALUE_TYPE());
    CHECK_FALSE( dq.empty() );
    dq.pop_back();
    CHECK( dq.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size deque", "[deque][capacity]", LIB::deque, TYPE_LIST)
{
    TestType dq;
    CHECK( dq.size() == 0 );
    dq.insert(dq.begin(), 5, VALUE_TYPE());
    CHECK( dq.size() == 5 );
    dq.clear();
    CHECK( dq.size() == 0 );
}
