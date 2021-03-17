#include "../srcs/list/list.hpp"
#include "catch.hpp"
#include <list>
#include <vector>

#ifdef VERIFY_UNIT_TESTS
# define LIB std
#else
# define LIB ft
#endif

#define TYPE_LIST ( int, char, unsigned int, std::string, std::vector<int>, std::vector<std::string>, LIB::list<std::string>, std::list<int>, std::vector<std::list<int> >)
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("construction list", "[list][basics]")
{
    SECTION("declaration creates an empty list") {
        LIB::list<int> l;
        REQUIRE( l.empty() );
        REQUIRE( l.size() == 0 );
    }
    SECTION("fill construction creates a list filled with the supplied value") {
        size_t i = GENERATE(0, 1, 100);
        int j = GENERATE(-100, 914);
        LIB::list<int> l (i, j);

        REQUIRE( l.size() == i );
        for (auto it = l.begin(); it != l.end(); ++it)
            REQUIRE( *it == j );
    }
    SECTION("range construction returns a list filled with the ranges values") {
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        int arr[] = {2, 4, 12, 5, 60, 99, -12};

        if (last >= first) {
            int i = 0;
            LIB::list<int> l (arr + first, arr + last);
            REQUIRE( l.size() == last - first );
            for (auto it = l.begin(); it != l.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
    SECTION ("copy construction returns an copy of the supplied list") {
        int i = 0;
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        LIB::list<int> l1;
        LIB::list<int> l2 (l1);

        REQUIRE( l2.empty() );
        if (last >= first) {
            LIB::list<int> l3 (arr + first, arr + last);
            REQUIRE( l3.size() == last - first );
            for (auto it = l3.begin(); it != l3.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
}

/* ASSIGNATION OPERATOR */
TEST_CASE("assignation operator list", "[list][operators]")
{
    int arr[] = {2, 4, 12, 5, 60, 99, -12};

    SECTION("different sizes of ranges can be assigned") {
        size_t i = 0;
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            LIB::list<int> l1 (arr + first, arr + last);
            LIB::list<int> l2 = l1;

            REQUIRE( l2.size() == last - first );
            for (auto it = l2.begin(); it != l2.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
    SECTION("assigning from & between empty lists works") {
        LIB::list<int> l3;
        LIB::list<int> l4 = l3;
        LIB::list<int> l5 (arr, arr + 3);
        l5 = l3;

        REQUIRE( l4.empty() );
        REQUIRE( l5.empty() );
    }
}

/* RELATIONAL OPERATORS */
TEST_CASE("relational operators list", "[list][operators]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, 12, 0, 0};
    LIB::list<int> l1 (a, a + 5);
    LIB::list<int> l2 (b, b + 2);
    LIB::list<int> l3;
    LIB::list<int> l4 (c, c + 6);
    LIB::list<int> l5 (d, d + 6);

    // l1 vs l2
    REQUIRE_FALSE( l1 == l2 );
    REQUIRE( l1 != l2 );
    REQUIRE_FALSE( l1 < l2 );
    REQUIRE_FALSE( l1 <= l2 );
    REQUIRE( l1 > l2 );
    REQUIRE( l1 >= l2 );

    // l1 vs l1
    REQUIRE( l1 == l1 );
    REQUIRE_FALSE( l1 != l1 );
    REQUIRE_FALSE( l1 < l1 );
    REQUIRE( l1 <= l1 );
    REQUIRE_FALSE( l1 > l1 );
    REQUIRE( l1 >= l1 );

    // l1 vs l3
    REQUIRE_FALSE( l1 == l3 );
    REQUIRE( l1 != l3 );
    REQUIRE_FALSE( l1 < l3 );
    REQUIRE_FALSE( l1 <= l3 );
    REQUIRE( l1 > l3 );
    REQUIRE( l1 >= l3 );

    // l3 vs l3
    REQUIRE( l3 == l3 );
    REQUIRE_FALSE( l3 != l3 );
    REQUIRE_FALSE( l3 < l3 );
    REQUIRE( l3 <= l3 );
    REQUIRE_FALSE( l3 > l3 );
    REQUIRE( l3 >= l3 );

    // l1 vs l4
    REQUIRE_FALSE( l1 == l4 );
    REQUIRE( l1 != l4 );
    REQUIRE( l1 < l4 );
    REQUIRE( l1 <= l4 );
    REQUIRE_FALSE( l1 > l4 );
    REQUIRE_FALSE( l1 >= l4 );

    // l4 vs l5
    REQUIRE_FALSE( l4 == l5 );
    REQUIRE( l4 != l5 );
    REQUIRE( l4 < l5 );
    REQUIRE( l4 <= l5 );
    REQUIRE_FALSE( l4 > l5 );
    REQUIRE_FALSE( l4 >= l5 );
}

/* ITERATORS */
TEST_CASE("iterators list", "[list][basics]")
{
    SECTION("begin returns an iterator to the first element") {
        // non const iterators
        LIB::list<int> l (1, 10);       // { 10 }
        REQUIRE( *l.begin() == 10 );
        REQUIRE( *l.rbegin() == 10 );
        REQUIRE( ++l.begin() == l.end() );
        REQUIRE( ++l.rbegin() == l.rend() );
        REQUIRE( l.begin() != l.end() );
        REQUIRE( l.rbegin() != l.rend() );

        l.push_front(5);               // { 5, 10 }
        LIB::list<int>::iterator it = l.begin();
        LIB::list<int>::reverse_iterator rit = l.rbegin();
        REQUIRE( *(it++) == 5 );
        REQUIRE( *it == 10 );
        REQUIRE( *(rit++) == 10 );
        REQUIRE( *rit == 5 );

        l.push_back(20);               // { 5, 10, 20 }
        REQUIRE( *l.begin() == 5 );
        REQUIRE( *++l.begin() == 10 );
        REQUIRE( *++++l.begin() == 20 );
        REQUIRE( *l.rbegin() == 20 );
        REQUIRE( *++l.rbegin() == 10 );
        REQUIRE( *++++l.rbegin() == 5 );

        l.pop_front();                 // { 10, 20 }
        it = l.begin();
        rit = l.rbegin();
        REQUIRE( *it == 10 );
        REQUIRE( *++it == 20 );
        REQUIRE( *rit == 20 );
        REQUIRE( *++rit == 10 );

        *l.begin() = 21;                // { 21, 42 }
        *l.rbegin() = 42;               // { 21, 42 }
        REQUIRE( *l.begin() == 21 );
        REQUIRE( *++l.begin() == 42 );
        REQUIRE( *l.rbegin() == 42 );
        REQUIRE( *++l.rbegin() == 21 );

        // const iterators
        {
            const LIB::list<int> l0 (1, 10);
            REQUIRE( *l0.begin() == 10 );
            REQUIRE( *l0.rbegin() == 10 );

            int arr[] = {1, 2, 3, 4, 5, 6};
            LIB::list<int> l2 (arr, arr + 6);
            LIB::list<int>::const_iterator it = l2.begin();
            LIB::list<int>::const_reverse_iterator rit = l2.rbegin();
            for (int i = 0; i < 6; ++i, ++it)
                REQUIRE( *it == arr[i] );
            REQUIRE( it == l2.end() );
            for (int i = 5; i >= 0; --i, ++rit)
                REQUIRE( *rit == arr[i] );
            REQUIRE( rit == l2.rend() );

            const LIB::list<int> l1;
            REQUIRE(( l1.begin() == l1.end() ));
            REQUIRE(( l1.rbegin() == l1.rend() ));
        }
    }
    SECTION("end returns an iterator to one past the last element") {
        //non const iterators
        LIB::list<int> l (1, 10);               // { 10 }
        REQUIRE( *--l.end() == 10 );
        REQUIRE( *--l.rend() == 10 );

        l.push_front(5);                  // { 5, 10 }
        REQUIRE( *----l.end() == 5 );
        REQUIRE( *--l.end() == 10 );
        REQUIRE( *--l.rend() == 5 );
        REQUIRE( *----l.rend() == 10 );

        l.push_back(20);                  // { 5, 10, 20 }
        REQUIRE( *------l.end() == 5 );
        REQUIRE( *----l.end() == 10 );
        REQUIRE( *--l.end() == 20 );
        REQUIRE( *--l.rend() == 5 );
        REQUIRE( *----l.rend() == 10 );
        REQUIRE( *------l.rend() == 20 );

        l.pop_back();                     // { 5, 10 }
        REQUIRE( *----l.end() == 5 );
        REQUIRE( *--l.end() == 10 );
        REQUIRE( *--l.rend() == 5 );
        REQUIRE( *----l.rend() == 10 );

        *--l.end() = 21;                   // { 5, 21 }
        *--l.rend() = 42;                  // { 42, 21 }
        REQUIRE( *----l.end() == 42 );
        REQUIRE( *--l.end() == 21 );
        REQUIRE( *--l.rend() == 42 );
        REQUIRE( *----l.rend() == 21 );

        // const iterators
        const LIB::list<int> l0 (1, 10);
        REQUIRE( *--l0.end() == 10 );
        REQUIRE( *--l0.rend() == 10 );

        int arr[] = {1, 2, 3, 4, 5, 6};
        LIB::list<int> l2 (arr, arr + 6);
        LIB::list<int>::const_iterator it = --l2.end();
        LIB::list<int>::const_reverse_iterator rit = --l2.rend();
        for (int i = 5; i >= 0; --i, --it)
            REQUIRE( *it == arr[i] );
        for (int i = 0; i < 6; ++i, --rit)
            REQUIRE( *rit == arr[i] );

        const LIB::list<int> l1;
        REQUIRE(( l1.begin() == l1.end() ));
        REQUIRE(( l1.rbegin() == l1.rend() ));
    }
}

/* MODIFIERS */
TEST_CASE("assign list", "[list][modifiers]")
{
    int arr[] = {1, 4, -1, 2, 33};
    LIB::list<int> l;
    LIB::list<int> l1 (10, 1);

    SECTION("ranges can be assigned to lists") {
        unsigned int i = GENERATE(0, 1, 3);
        unsigned int j = GENERATE(0, 4, 5);
        unsigned int y = 0;

        if (j >= i) {
            l.assign(arr + i, arr + j);
            REQUIRE( l.size() == j - i );
            for (auto it = l.begin(); it != l.end(); ++it, ++y)
                REQUIRE( *it == arr[y + i] );

            y = 0;
            l1.assign(arr + i, arr + j);
            REQUIRE( l1.size() == j - i );
            for (auto it = l1.begin(); it != l1.end(); ++it, ++y)
                REQUIRE( *it == arr[y + i] );
        }
    }
    SECTION("fills can be assigned to lists") {
        unsigned int i = GENERATE(0, 1, 25);
        int j = GENERATE(0, 42, -12);

        l.assign(i, j);
        REQUIRE( l.size() == i );
        for (auto it = l.begin(); it != l.end(); ++it)
            REQUIRE( *it == j );
        l1.assign(i, j);
        REQUIRE( l1.size() == i );
        for (auto it = l1.begin(); it != l1.end(); ++it)
            REQUIRE( *it == j );
    }
}

TEST_CASE("push/pop list", "[list][modifiers]")
{
    LIB::list<int> list;
    std::vector<int> stl;
    stl.reserve(100);

    SECTION("values can be pushed/popped to the front") {
        for (unsigned int i = 0; i < 100; ++i) {
            int rand = std::rand() % 200000 - 100000;
            list.push_back(rand);
            stl.push_back(rand);
            REQUIRE( list.back() == stl.back() );
        }
        REQUIRE( list.size() == stl.size() );
        while (list.size() != 0 ) {
            REQUIRE( list.back() == stl.back() );
            list.pop_back();
            stl.pop_back();
        }
        REQUIRE( list.size() == stl.size() );
    }
    SECTION("values can be pushed/popped to the back") {
        for (unsigned int i = 0; i < 100; ++i) {
            int rand = std::rand() % 200000 - 100000;
            list.push_front(rand);
            stl.insert(stl.begin(), rand);
            REQUIRE( list.front() == stl.front() );
        }
        REQUIRE( list.size() == stl.size() );
        while (list.size() != 0 ) {
            REQUIRE( list.front() == stl.front() );
            list.pop_front();
            stl.erase(stl.begin());
        }
        REQUIRE( list.size() == stl.size() );
    }
}

TEST_CASE("insert list", "[list][modifiers]")
{
    LIB::list<int> l;
    LIB::list<int>::iterator it = l.begin();
    LIB::list<int>::iterator ret;

    SECTION("single elements can be inserted") {
        ret = l.insert(it, 5);            // { 5 }
        REQUIRE( l.size() == 1 );
        REQUIRE( l.front() == 5 );
        REQUIRE( l.back() == 5 );
        REQUIRE( *ret == 5 );

        ret = l.insert(it, 10);           // { 5, 10}
        REQUIRE( l.size() == 2 );
        REQUIRE( l.front() == 5 );
        REQUIRE( l.back() == 10 );
        REQUIRE( *ret == 10 );
    }
    SECTION("fills can be inserted") {
        l.insert(l.begin(), 5, 10);
        REQUIRE( l.size() == 5 );

        for (it = l.begin(); it != l.end(); ++it)
            REQUIRE( *it == 10 );

        l.insert(it, 1, 100);
        REQUIRE( l.size() == 6 );
        REQUIRE( l.back() == 100 );
    }
    SECTION("ranges can be inserted") {
        int arr[] = {12, 1, 4, 5, 6, 7};

        l.push_front(0);
        l.insert(l.end(), arr + 1, arr + 5);
        l.push_back(-32);
        REQUIRE( l.size() == 6 );

        auto it = l.begin();
        REQUIRE( *it++ == 0 );
        REQUIRE( *it++ == 1 );
        REQUIRE( *it++ == 4 );
        REQUIRE( *it == 5 );
        REQUIRE( *++it == 6 );
        REQUIRE( *++it == -32 );
    }
}

TEST_CASE("erase list", "[list][modifiers]")
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    LIB::list<int> l (arr, arr + 6);                   // { 1, 2, 3, 4, 5, 6 }
    LIB::list<int>::iterator ret;

    // single element erase
    ret = l.erase(l.begin());                     // { 2, 3, 4, 5, 6 }
    REQUIRE( l.size() == 5 );
    REQUIRE( *ret == 2 );

    ret = l.erase(--l.end());                     // { 2, 3, 4, 5 }
    REQUIRE( l.size() == 4 );
    REQUIRE(( ret == l.end() ));
    REQUIRE( l.back() == 5 );

    // range erase
    ret = l.erase(++l.begin(), --l.end());   // { 2, 3 }
    REQUIRE( l.size() == 2 );
    REQUIRE( *ret == 5 );

    ret = l.erase(l.begin(), l.end());       // { }
    REQUIRE( l.empty() );
    REQUIRE(( ret == l.end() ));
}

TEST_CASE("swap list", "[list][modifiers]")
{
    int arr1[] = {1, 2, 3, 4, 5, 6};
    int arr2[] = {-3, 69, 1};
    LIB::list<int>::iterator it;
    int i = 0;

    SECTION("non empty lists can be swapped") {
        LIB::list<int> l1 (arr1, arr1 + 6);
        LIB::list<int> l2 (arr2, arr2 + 3);

        l1.swap(l2);
        REQUIRE( l1.size() == 3 );
        REQUIRE( l2.size() == 6 );

        for (it = l1.begin(); it != l1.end(); ++it, ++i)
            REQUIRE( *it == arr2[i] );
        i = 0;
        for (it = l2.begin(); it != l2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }
    SECTION("non empty and empty lists can be swapped") {
        LIB::list<int> l1 (arr1, arr1 + 6);
        LIB::list<int> l2;

        l1.swap(l2);
        REQUIRE( l1.empty() );
        REQUIRE( l2.size() == 6 );
        for (it = l2.begin(); it != l2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }
    SECTION("empty lists can be swapped") {
        LIB::list<int> l1;
        LIB::list<int> l2;

        l1.swap(l2);
        REQUIRE( l1.empty() );
        REQUIRE( l2.empty() );
    }
}


TEST_CASE("resize list", "[list][modifiers]")
{
    int arr[] = {1, 2, 3, 4, 5};
    LIB::list<int> l0 (arr, arr + 5);

    SECTION("list elements are removed when arg is smaller than current size") {
        l0.resize(4);
        REQUIRE( l0.size() == 4 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == 4 );
        l0.resize(2);
        REQUIRE( l0.size() == 2 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == 2 );
        l0.resize(0);
        REQUIRE( l0.empty() );
    }
    SECTION("list is expanded when arg is greater than current size") {
        l0.resize(7);
        REQUIRE( l0.size() == 7 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == int() );
        l0.resize(10);
        REQUIRE( l0.size() == 10 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == int() );
    }
    SECTION("list does not change if arg is equal to current size") {
        l0.resize(5);
        REQUIRE( l0.size() == 5 );
    }

}

TEMPLATE_PRODUCT_TEST_CASE("clear list", "[list][modifiers]", LIB::list, TYPE_LIST)
{
    TestType l;
    l.clear();
    REQUIRE( l.empty() );
    l.assign(10, VALUE_TYPE());
    REQUIRE( l.size() == 10 );
    l.clear();
    REQUIRE( l.empty() );
    l.assign(3, VALUE_TYPE());
    REQUIRE( l.size() == 3 );
    l.clear();
    REQUIRE( l.empty() );
}

/* ELEMENT ACCESS */
TEST_CASE("front/back list", "[list][element access]")
{
    int arr[] = {1, 2, 3, 4, 5};
    LIB::list<int> l (arr, arr + 5);
    const LIB::list<int> cl (arr + 1, arr + 5);

    // front
    l.front() -= 21;
    REQUIRE( l.front() == -20 );
    l.front() += 20;
    REQUIRE( l.front() == 0 );

    // back
    l.back() -= 42;
    REQUIRE( l.back() == -37 );
    l.back() += 37;
    REQUIRE( l.back() == 0 );

    // const
    REQUIRE( cl.front() == 2 );
    REQUIRE( cl.back() == 5 );
}

/* CAPACITY */

TEMPLATE_PRODUCT_TEST_CASE("empty reflects if the list is empty", "[list][basics]", LIB::list, TYPE_LIST)
{
    TestType l;
    REQUIRE( l.empty() );
    l.push_back(VALUE_TYPE());
    REQUIRE_FALSE( l.empty() );
    l.pop_front();
    REQUIRE( l.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size returns the lists size", "[list][capacity]", LIB::list, TYPE_LIST)
{
    TestType l;
    REQUIRE( l.size() == 0 );
    l.assign(5, VALUE_TYPE());
    REQUIRE( l.size() == 5 );
    l.clear();
    REQUIRE( l.size() == 0 );
}

/* LIST OPERATIONS */
TEST_CASE("splice list", "[list][operations]")
{
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    LIB::list<int> l0 (v, v + 8);
    LIB::list<int> l1;
    LIB::list<int>::iterator it;

    SECTION("entire lists can be spliced") {
        l1.splice(l1.begin(), l0);
        REQUIRE( l0.size() == 0 );
        REQUIRE( l1.size() == 8 );

        int i = 0;
        for (it = l1.begin(); it != l1.end(); ++it, ++i)
            REQUIRE( *it == v[i] );
    }
    SECTION("single elements can be spliced") {
        l1.splice(l1.begin(), l0, ++l0.begin());
        REQUIRE( l0.size() == 7 );
        REQUIRE( l1.size() == 1 );
        REQUIRE( *l0.begin() == 0 );
        REQUIRE( *l1.begin() == 1 );
    }
    SECTION("ranges can be spliced") {
        l1.splice(l1.begin(), l0, l0.begin(), l0.begin());
        REQUIRE( l0.size() == 8 );
        REQUIRE( l1.size() == 0 );

        l1.splice(l1.begin(), l0, l0.begin(), l0.end());
        REQUIRE( l0.size() == 0 );
        REQUIRE( l1.size() == 8 );

        it = l1.begin();
        std::advance(it, 4);
        l0.splice(l0.begin(), l1, ++l1.begin(), it);
        REQUIRE( l0.size() == 3 );
        REQUIRE( l1.size() == 5 );
    }
}

TEST_CASE("remove list", "[list][operations]")
{
    int v[] = { 0, 1, 2, 3, 3, 5, 1, 7 };
    LIB::list<int> c (v, v + 8);

    SECTION("remove erases the element if its found") {
        c.remove(42);
        REQUIRE( c.size() == 8 );
        c.remove(1);
        REQUIRE( c.size() == 6 );
        c.remove(3);
        REQUIRE( c.size() == 4 );
        c.remove(7);
        REQUIRE( c.size() == 3 );
        c.remove(0);
        REQUIRE( c.size() == 2 );
        c.remove(2);
        REQUIRE( c.size() == 1 );
        c.remove(5);
        REQUIRE( c.size() == 0 );
        c.remove(0);
        REQUIRE( c.size() == 0 );
    }
    SECTION("remove_if uses the specified function") {
        c.remove_if([](int i) { return i == 42; });
        REQUIRE( c.size() == 8 );
        c.remove_if([](int i) { return i == 1; });
        REQUIRE( c.size() == 6 );
        c.remove_if([](int i) { return i == 3; });
        REQUIRE( c.size() == 4 );
        c.remove_if([](int i) { return i == 7; });
        REQUIRE( c.size() == 3 );
        c.remove_if([](int i) { return i == 0; });
        REQUIRE( c.size() == 2 );
        c.remove_if([](int i) { return i == 2; });
        REQUIRE( c.size() == 1 );
        c.remove_if([](int i) { return i == 5; });
        REQUIRE( c.size() == 0 );
        c.remove_if([](int i) { return i == 0; });
        REQUIRE( c.size() == 0 );
    }
}

TEST_CASE("unique list", "[list][operations]")
{
    int i = 0;
    int arr[] = { 1, 1, -66, -22, -22, -1, -22, 2, 2, 2, 9, 9, 109, 109, 109 };
    int arr_unique1[] = { 1, -66, -22, -1, -22, 2, 9, 109 };
    int arr_unique2[] = { 1, -66, -22, -22, -1, -22, 2, 9, 9, 109, 109, 109 };
    std::string arr2[] = {"fdjks", "fdjks", "haha", "hahaha", "ba", "ab", "echo", "echo", "echo", "echo1"};
    std::string arr2_unique[] = {"fdjks", "haha", "hahaha", "ba", "ab", "echo", "echo1"};

    LIB::list<int> l (arr, arr + sizeof(arr) / sizeof(int));
    LIB::list<std::string> l1 (arr2, arr2 + sizeof(arr2) / sizeof(*arr2));

    SECTION("calling unique without a specified comparing function") {
        // INT ARRAY
        l.unique();
        REQUIRE( l.size() == (sizeof(arr_unique1) / sizeof(int)) );
        for (auto it = l.begin(); it != l.end(); ++it, ++i)
            REQUIRE( *it == arr_unique1[i] );

        // STD::STRING ARRAY
        l1.unique();
        i = 0;
        REQUIRE( l1.size() == (sizeof(arr2_unique) / sizeof(*arr2_unique)) );
        for (auto it = l1.begin(); it != l1.end(); ++it, ++i)
            REQUIRE( *it == arr2_unique[i] );
    }
    SECTION("calling unique with a specified comparing function") {
        l.unique( [](int i, int j) { return i + j == 2 || i + j == 4; } );

        REQUIRE( l.size() == (sizeof(arr_unique2) / sizeof(int)) );
        for (auto it = l.begin(); it != l.end(); ++it, ++i)
            REQUIRE( *it == arr_unique2[i] );
    }
    SECTION("calling unique with a weird comparing function") {
        LIB::list<int> l2;
        LIB::list<int> l3 (arr, arr + sizeof(arr) / sizeof(*arr));

        l2.unique();
        REQUIRE( l2.empty() );

        l3.unique([] (int a, int b) { return a == 30000 && b == 21; });
        REQUIRE( l3.size() == sizeof(arr) / sizeof(*arr) );
        for (auto it = l3.begin(); it != l3.end(); ++it, ++i)
            REQUIRE( *it == arr[i] );
    }
}

TEST_CASE("merge list", "[list][operations]")
{
    SECTION("merging without a specified comparing function") {
        int i = 0;
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        LIB::list<int> l0 (a, a + sizeof(a) / sizeof(int));
        LIB::list<int> l1 (b, b + sizeof(b) / sizeof(int));

        l0.merge(l1);
        REQUIRE( l1.empty() );

        for (auto it = l0.begin(); it != l0.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }
    SECTION("merging with a specified comparing function") {
        int i = 0;
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        LIB::list<int> l0 (a, a + sizeof(a) / sizeof(int));
        LIB::list<int> l1 (b, b + sizeof(b) / sizeof(int));

        l0.merge(l1, std::greater<int>());
        REQUIRE( l1.empty() );

        for (auto it = l0.begin(); it != l0.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }
    SECTION("merging with or between empty lists") {
        LIB::list<int> l1 (5, 10);
        LIB::list<int> l2;
        LIB::list<int> l3;

        l1.merge(l2);
        REQUIRE( l1.size() == 5 );

        l2.merge(l3);
        REQUIRE( l2.empty() );
        REQUIRE( l3.empty() );
    }
}

TEST_CASE("sort list", "[list][operations]")
{
    int i = 0;
    // ARRAY 1
    int arr1[] = {2, 4, 12, 5, 2, 99, -12};
    int arr1_sorted[] = {-12, 2, 2, 4, 5, 12, 99};
    size_t arr1_size = sizeof(arr1) / sizeof(int);
    LIB::list<int> l1 (arr1, arr1 + arr1_size);
    // ARRAY 2
    int arr2[] = {2, 32, 43, 4, 12, 5, 2, 99, 99, 1, 100, 15, 12, 1111111, 21, 42, 0, -1111, 43, -12};
    int arr2_sorted[] = {2, 2, 32, 4, 12, 5, 1, 15, 12, 21, 42, 0, -1111, -12, 43, 1111111, 100, 99, 99, 43};
    size_t arr2_size = sizeof(arr2) / sizeof(int);
    LIB::list<int> l2 (arr2, arr2 + arr2_size);
    // ARRAY 3
    std::string arr3[] = {"abcdef", "ahblk", "$", "42", "?", "ßüäopajgh"};
    std::string arr3_sorted[] = {"$", "42", "?", "abcdef", "ahblk", "ßüäopajgh"};
    size_t arr3_size = sizeof(arr3) / sizeof(std::string);
    LIB::list<std::string> l5 (arr3, arr3 + arr3_size);

    SECTION("sorting without a specified comparing function") {
        // ARRAY 1
        i = 0;
        l1.sort();

        REQUIRE( l1.size() == arr1_size );
        for (auto it = l1.begin(); it != l1.end(); ++it, ++i)
            REQUIRE( *it == arr1_sorted[i] );

        // ARRAY 3
        i = 0;
        l5.sort();

        REQUIRE( l5.size() == arr3_size );
        for (auto it = l5.begin(); it != l5.end(); ++it, ++i)
            REQUIRE( *it == arr3_sorted[i] );
    }
    SECTION("sorting with a specified comparing function") {
        // ARRAY 1
        i = arr1_size - 1;
        l1.sort(std::greater<int>());

        REQUIRE( l1.size() == arr1_size );
        for (auto it = l1.begin(); it != l1.end(); ++it, --i)
            REQUIRE( *it == arr1_sorted[i] );

        // ARRAY 2
        i = 0;
        l2.sort([](const int& a, const int& b) { return a == 2 || b > 42; } );

        REQUIRE( l2.size() == arr2_size );
        for (auto it = l2.begin(); it != l2.end(); ++it, ++i)
            REQUIRE( *it == arr2_sorted[i] );
    }
    SECTION ("sorting already sorted or empty lists does not modify them") {
        LIB::list<int> l3;
        LIB::list<int> l4 (1, 0);
        LIB::list<int> l5 (arr1_sorted, arr1_sorted + arr1_size);

        l3.sort();
        REQUIRE( l3.empty() );

        l4.sort();
        REQUIRE( l4.size() == 1 );

        l5.sort();
        for (auto it = l5.begin(); it != l5.end(); ++it, ++i)
            REQUIRE( *it == arr1_sorted[i] );
    }
}

TEST_CASE("reverse list", "[list][operations]")
{
    int i = 8;
    int arr[] = {2, 4, 2, 5, 2, 99, -12, 1, 312};
    LIB::list<int> l0;
    LIB::list<int> l1 (arr, arr + 9);
    LIB::list<int> l2 (arr, arr + 1);

    // empty list
    l0.reverse();
    REQUIRE( l0.empty() );

    // list of size 1
    l2.reverse();
    REQUIRE( l2.size() == 1 );
    REQUIRE( *l2.begin() == 2 );

    // "normal" lists
    l1.reverse();
    REQUIRE( l1.size() == 9 );
    for (auto it = l1.begin(); it != l1.end(); ++it, --i)
        REQUIRE( *it == arr[i] );

    l2.assign(arr + 1, arr + 3);
    l2.reverse();
    REQUIRE( l2.size() == 2 );
    REQUIRE( *l2.begin() == 2 );
    REQUIRE( *++l2.begin() == 4 );
}
