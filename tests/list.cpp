#include "../srcs/list/list.hpp"
#include "catch.hpp"
#include <list>
#include <vector>

#define TYPE_LIST ( int, char, unsigned int, std::string, std::vector<int>, std::vector<std::string>, ft::list<std::string>, std::list<int>, std::vector<std::list<int> >)
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("list construction works correctly", "[list][basics]")
{
    SECTION("declaration creates empty list") {
        ft::list<int> c;

        REQUIRE( c.empty() );
        REQUIRE( c.size() == 0 );
    }

    SECTION("fill constructor creates list of specified size & filled with elements") {
        size_t i = GENERATE(0, 1, 100);
        int j = GENERATE(-100, 914);
        ft::list<int> c (i, j);

        REQUIRE( c.size() == i );
        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == j );
    }

    SECTION("range constructor creates list with correct elements from range") {
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        int i = 0;

        if (last >= first) {
            ft::list<int> l (arr + first, arr + last);

            REQUIRE( l.size() == last - first );
            for (auto it = l.begin(); it != l.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }

        ft::list<int> l2 (arr, arr);
        REQUIRE( l2.empty() );
    }

    SECTION("copy constructor constructs an exact copy of a given list") {
        int i = 0;
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        ft::list<int> l1;
        ft::list<int> l2 (l1);

        REQUIRE( l1.empty() );
        REQUIRE( l2.empty() );

        if (last >= first) {
            ft::list<int> l3 (arr + first, arr + last);

            REQUIRE( l3.size() == last - first );
            for (auto it = l3.begin(); it != l3.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
}

/* ASSIGNATION OPERATOR */
TEST_CASE("assignation operator works correctly", "[list][basics]")
{
    int arr[] = {2, 4, 12, 5, 60, 99, -12};

    SECTION("different size of ranges are handled correctly") {
        size_t i = 0;
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            ft::list<int> mylist1 (arr + first, arr + last);
            ft::list<int> mylist2 = mylist1;

            REQUIRE( mylist2.size() == last - first );
            for (auto it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }

    SECTION("assignation from empty list works") {
        ft::list<int> mylist3;
        ft::list<int> mylist4 = mylist3;
        ft::list<int> mylist5 (arr, arr + 3);

        REQUIRE( mylist4.size() == mylist3.size() );
        REQUIRE( mylist4.empty() );

        mylist5 = mylist3;
        REQUIRE( mylist5.size() == mylist3.size() );
        REQUIRE( mylist5.empty() );
    }
}

/* RELATIONAL OPERATORS */
TEST_CASE("relational operators work correctly", "[list][basics]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, 12, 0, 0};
    ft::list<int> mylist1 (a, a + 5);
    ft::list<int> mylist2 (b, b + 2);
    ft::list<int> mylist3;
    ft::list<int> mylist4 (c, c + 6);
    ft::list<int> mylist5 (d, d + 6);

    // mylist1 vs mylist2
    REQUIRE_FALSE( mylist1 == mylist2 );
    REQUIRE( mylist1 != mylist2 );
    REQUIRE_FALSE( mylist1 < mylist2 );
    REQUIRE_FALSE( mylist1 <= mylist2 );
    REQUIRE( mylist1 > mylist2 );
    REQUIRE( mylist1 >= mylist2 );

    // mylist1 vs mylist1
    REQUIRE( mylist1 == mylist1 );
    REQUIRE_FALSE( mylist1 != mylist1 );
    REQUIRE_FALSE( mylist1 < mylist1 );
    REQUIRE( mylist1 <= mylist1 );
    REQUIRE_FALSE( mylist1 > mylist1 );
    REQUIRE( mylist1 >= mylist1 );

    // mylist1 vs mylist3
    REQUIRE_FALSE( mylist1 == mylist3 );
    REQUIRE( mylist1 != mylist3 );
    REQUIRE_FALSE( mylist1 < mylist3 );
    REQUIRE_FALSE( mylist1 <= mylist3 );
    REQUIRE( mylist1 > mylist3 );
    REQUIRE( mylist1 >= mylist3 );

    // mylist3 vs mylist3
    REQUIRE( mylist3 == mylist3 );
    REQUIRE_FALSE( mylist3 != mylist3 );
    REQUIRE_FALSE( mylist3 < mylist3 );
    REQUIRE( mylist3 <= mylist3 );
    REQUIRE_FALSE( mylist3 > mylist3 );
    REQUIRE( mylist3 >= mylist3 );

    // mylist1 vs mylist4
    REQUIRE_FALSE( mylist1 == mylist4 );
    REQUIRE( mylist1 != mylist4 );
    REQUIRE( mylist1 < mylist4 );
    REQUIRE( mylist1 <= mylist4 );
    REQUIRE_FALSE( mylist1 > mylist4 );
    REQUIRE_FALSE( mylist1 >= mylist4 );

    // mylist4 vs mylist5
    REQUIRE_FALSE( mylist4 == mylist5 );
    REQUIRE( mylist4 != mylist5 );
    REQUIRE( mylist4 < mylist5 );
    REQUIRE( mylist4 <= mylist5 );
    REQUIRE_FALSE( mylist4 > mylist5 );
    REQUIRE_FALSE( mylist4 >= mylist5 );
}

/* ITERATORS */
TEST_CASE( "list begin/rbegin work correctly", "[list][basics]")
{
    SECTION("non-const iterator behaviour") {
        ft::list<int> mylist (1, 10);       // { 10 }
        REQUIRE( *mylist.begin() == 10 );
        REQUIRE( *mylist.rbegin() == 10 );

        mylist.push_front(5);               // { 5, 10 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 10 );
        REQUIRE( *mylist.rbegin() == 10 );
        REQUIRE( *++mylist.rbegin() == 5 );

        mylist.push_back(20);               // { 5, 10, 20 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 10 );
        REQUIRE( *++++mylist.begin() == 20 );
        REQUIRE( *mylist.rbegin() == 20 );
        REQUIRE( *++mylist.rbegin() == 10 );
        REQUIRE( *++++mylist.rbegin() == 5 );

        mylist.pop_front();                 // { 10, 20 }
        REQUIRE( *mylist.begin() == 10 );
        REQUIRE( *++mylist.begin() == 20 );
        REQUIRE( *mylist.rbegin() == 20 );
        REQUIRE( *++mylist.rbegin() == 10 );

        *mylist.begin() = 21;                // { 21, 42 }
        *mylist.rbegin() = 42;               // { 21, 42 }
        REQUIRE( *mylist.begin() == 21 );
        REQUIRE( *++mylist.begin() == 42 );
        REQUIRE( *mylist.rbegin() == 42 );
        REQUIRE( *++mylist.rbegin() == 21 );
    }

    SECTION("const iterator behaviour") {
        int arr[] = {1, 2, 3, 4, 5, 6};
        const ft::list<int> mylist (1, 10);
        const ft::list<int> mylist1;
        ft::list<int> mylist2 (arr, arr + 6);
        ft::list<int>::const_iterator it = mylist2.begin();
        ft::list<int>::const_reverse_iterator rit = mylist2.rbegin();

        REQUIRE( *mylist.begin() == 10 );
        REQUIRE( *mylist.rbegin() == 10 );

        REQUIRE(( mylist1.begin() == mylist1.end() ));
        REQUIRE(( mylist1.rbegin() == mylist1.rend() ));

        for (int i = 0; i < 6; ++i, ++it)
            REQUIRE( *it == arr[i] );
        REQUIRE( it == mylist2.end() );

        for (int i = 5; i >= 0; --i, ++rit)
            REQUIRE( *rit == arr[i] );
        REQUIRE( rit == mylist2.rend() );
    }
}

TEST_CASE("list end works correctly", "[list][basics]")
{
    SECTION("non-const iterator behaviour") {
        ft::list<int> mylist (1, 10);               // { 10 }

        REQUIRE( *--mylist.end() == 10 );
        REQUIRE( *--mylist.rend() == 10 );

        mylist.push_front(5);                  // { 5, 10 }
        REQUIRE( *----mylist.end() == 5 );
        REQUIRE( *--mylist.end() == 10 );
        REQUIRE( *--mylist.rend() == 5 );
        REQUIRE( *----mylist.rend() == 10 );

        mylist.push_back(20);                  // { 5, 10, 20 }
        REQUIRE( *------mylist.end() == 5 );
        REQUIRE( *----mylist.end() == 10 );
        REQUIRE( *--mylist.end() == 20 );
        REQUIRE( *--mylist.rend() == 5 );
        REQUIRE( *----mylist.rend() == 10 );
        REQUIRE( *------mylist.rend() == 20 );

        mylist.pop_back();                     // { 5, 10 }
        REQUIRE( *----mylist.end() == 5 );
        REQUIRE( *--mylist.end() == 10 );
        REQUIRE( *--mylist.rend() == 5 );
        REQUIRE( *----mylist.rend() == 10 );

        *--mylist.end() = 21;                   // { 5, 21 }
        *--mylist.rend() = 42;                  // { 42, 21 }
        REQUIRE( *----mylist.end() == 42 );
        REQUIRE( *--mylist.end() == 21 );
        REQUIRE( *--mylist.rend() == 42 );
        REQUIRE( *----mylist.rend() == 21 );
    }

    SECTION("const iterator behaviour") {
        int arr[] = {1, 2, 3, 4, 5, 6};
        const ft::list<int> mylist (1, 10);
        const ft::list<int> mylist1;
        ft::list<int> mylist2 (arr, arr + 6);
        ft::list<int>::const_iterator it = --mylist2.end();
        ft::list<int>::const_reverse_iterator rit = --mylist2.rend();

        REQUIRE( *--mylist.end() == 10 );
        REQUIRE( *--mylist.rend() == 10 );

        REQUIRE(( mylist1.begin() == mylist1.end() ));
        REQUIRE(( mylist1.rbegin() == mylist1.rend() ));

        for (int i = 5; i >= 0; --i, --it)
            REQUIRE( *it == arr[i] );

        for (int i = 0; i < 6; ++i, --rit)
            REQUIRE( *rit == arr[i] );
    }
}

/* MODIFIERS */
TEMPLATE_PRODUCT_TEST_CASE( "assign work correctly", "[list][modifiers]", ft::list, TYPE_LIST )
{
    SECTION("range assign works correctly") {
        TestType c;
        std::vector<VALUE_TYPE> v (10);

        c.assign(v.begin() + 2, v.begin() + 7);
        REQUIRE( c.size() == 5 );

        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == VALUE_TYPE() );
    }

    SECTION("fill assign works correctly") {
        TestType c;
        size_t n = GENERATE(0, 100);

        c.assign(n, VALUE_TYPE() );
        REQUIRE( c.size() == n );

        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == VALUE_TYPE() );
    }
}

TEST_CASE("push_front works correctly", "[list][modifiers]")
{
    SECTION( "the element is added at the front & size is incremented" ) {
        int value;
        ft::list<int> l1;

        for (int i = 0; i < 50; ++i) {
            value = rand();
            l1.push_front(value);
            REQUIRE( *l1.begin() == value );
        }
        REQUIRE( l1.size() == (unsigned int)50 );
    }
}

TEST_CASE("push_back works correctly", "[list][modifiers]")
{
    SECTION( "containers size is incremented & the element is added at the back" ) {
        int value;
        ft::list<int> l1;

        for (int i = 0; i < 50; ++i) {
            value = rand();
            l1.push_back(value);
            REQUIRE( *(--l1.end()) == value );
        }
        REQUIRE( l1.size() == (unsigned int)50 );
    }
}

TEST_CASE("pop_front works correctly", "[list][modifiers]")
{
    unsigned int size = GENERATE(0, 1, 2, 15);
    ft::list<int> l1 (size, 100);

    while (!l1.empty()) {
        REQUIRE( l1.size() == size);
        REQUIRE( *l1.begin() == 100);
        REQUIRE( *--l1.end() == 100);
        l1.pop_front();
        --size;
    }
    REQUIRE( size == 0 );
}

TEST_CASE("pop_back works correctly", "[list][modifiers]")
{
    unsigned int size = GENERATE(0, 1, 2, 15);
    ft::list<int> l1 (size, 100);

    while (!l1.empty()) {
        REQUIRE( l1.size() == size);
        REQUIRE( *l1.begin() == 100);
        REQUIRE( *--l1.end() == 100);
        l1.pop_back();
        --size;
    }
    REQUIRE( size == 0 );
}

TEST_CASE("insert work correctly", "[list][modifiers]")
{
    ft::list<int> c;
    ft::list<int>::iterator it = c.begin();

    SECTION( "single element insert works correctly" ) {
        c.insert(it, 5);            // { 5 }
        REQUIRE( c.size() == 1 );
        REQUIRE( *c.begin() == 5 );

        c.insert(it, 10);           // { 5, 10}
        REQUIRE( c.size() == 2 );
        REQUIRE( *--c.end() == 10 );
    }

    SECTION( "fill insert works correctly" ) {
        c.insert(c.begin(), 5, 10);
        REQUIRE( c.size() == 5 );

        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == 10 );

        c.insert(it, 1, 100);
        REQUIRE( c.size() == 6 );
        REQUIRE( *--c.end() == 100 );
    }

    SECTION( "range insert works correctly" ) {
        int arr[] = {12, 1, 4, 5, 6, 7};

        c.push_front(0);
        c.push_back(-32);
        c.insert(--c.end(), arr + 1, arr + 5);
        REQUIRE( c.size() == 6 );

        auto it = c.begin();
        REQUIRE( *it == 0 );
        REQUIRE( *++it == 1 );
        REQUIRE( *++it == 4 );
        REQUIRE( *++it == 5 );
        REQUIRE( *++it == 6 );
        REQUIRE( *++it == -32 );
    }
}

TEST_CASE("erase return value & remaining values are consistent", "[list][modifiers]")
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    ft::list<int> mylist (arr, arr + 6);    // { 1, 2, 3, 4, 5, 6 }
    ft::list<int>::iterator ret;

    ret = mylist.erase(mylist.begin());     // { 2, 3, 4, 5, 6 }
    REQUIRE( mylist.size() == 5 );
    REQUIRE( *ret == 2 );

    ret = mylist.erase(--mylist.end());     // { 2, 3, 4, 5 }
    REQUIRE( mylist.size() == 4 );
    REQUIRE(( ret == mylist.end() ));
    REQUIRE( *(--mylist.end()) == 5 );

    ret = mylist.erase(++mylist.begin(), --mylist.end());   // { 2, 3 }
    REQUIRE( mylist.size() == 2 );
    REQUIRE( *ret == 5 );

    ret = mylist.erase(mylist.begin(), mylist.end());       // { }
    REQUIRE( mylist.empty() );
    REQUIRE(( ret == mylist.end() ));
}

TEST_CASE( "swap works correctly", "[list][modifiers]")
{
    int arr1[] = {1, 2, 3, 4, 5, 6};
    int arr2[] = {-3, 69, 1};
    ft::list<int>::iterator it;
    int i = 0;

    SECTION("non-empty lists can be swapped") {
        ft::list<int> mylist1 (arr1, arr1 + 6);
        ft::list<int> mylist2 (arr2, arr2 + 3);

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 3 );
        REQUIRE( mylist2.size() == 6 );

        for (it = mylist1.begin(); it != mylist1.end(); ++it, ++i)
            REQUIRE( *it == arr2[i] );

        i = 0;
        for (it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }

    SECTION("empty lists can be swapped w/ filled lists") {
        ft::list<int> mylist1 (arr1, arr1 + 6);
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 6 );

        for (it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }

    SECTION("empty lists can be swapped") {
        ft::list<int> mylist1;
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.empty() );
        REQUIRE( mylist2.empty() );
    }
}

TEMPLATE_PRODUCT_TEST_CASE( "resize works correctly", "[list][modifiers]", ft::list, TYPE_LIST )
{
    TestType c;

    SECTION( "size is reduced when n is smaller than current size" ) {
        c.resize(0);
        REQUIRE( c.size() == 0 );

        c.assign(4, VALUE_TYPE());
        c.resize(2);
        REQUIRE( c.size() == 2 );

        c.resize(1);
        REQUIRE( c.size() == 1 );

        c.resize(0);
        REQUIRE( c.size() == 0 );
    }

    SECTION( "size is expanded at it's end when n is greater than current size" ) {
        SECTION( "empty list" ) {
            c.resize(5);
            REQUIRE( c.size() == 5 );
        }

        SECTION( "non-empty list" ) {
            c.push_front(VALUE_TYPE());
            c.resize(5);
            REQUIRE( c.size() == 5 );
            c.push_front(VALUE_TYPE());
            c.resize(20);
            REQUIRE( c.size() == 20 );
        }
    }
}

TEST_CASE("resize added elements are consistent")
{
    int arr[] = {1, 2, 3, 4, 5};
    ft::list<int> mylist (arr, arr + 5);

    mylist.resize(4);
    REQUIRE( mylist.size() == 4 );
    REQUIRE( *(--mylist.end()) == 4 );

    mylist.resize(5);
    REQUIRE( mylist.size() == 5 );
    REQUIRE( *(--mylist.end()) == int() );

    mylist.resize(0);
    REQUIRE( mylist.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("clear works correctly", "[list][modifiers]", ft::list, TYPE_LIST)
{
    TestType c;

    c.clear();
    REQUIRE( c.empty() );

    c.assign(10, VALUE_TYPE());
    REQUIRE( c.size() == 10 );

    c.clear();
    REQUIRE( c.empty() );

    c.assign(10, VALUE_TYPE());
    REQUIRE( c.size() == 10 );
}

/* ELEMENT ACCESS */
TEST_CASE("element access works correctly", "[list][element access]")
{
    int arr[] = {1, 2, 3, 4, 5};
    ft::list<int> mylist (arr, arr + 5);

    mylist.front() -= 21;
    REQUIRE( mylist.front() == -20 );

    mylist.back() -= 42;
    REQUIRE( mylist.back() == -37 );
}

/* CAPACITY */

TEMPLATE_PRODUCT_TEST_CASE("empty reflects list state", "[list][basics]", ft::list, TYPE_LIST)
{
    TestType c;
    REQUIRE( c.empty() );
    c.push_back(VALUE_TYPE());
    REQUIRE_FALSE( c.empty() );
    c.pop_front();
    REQUIRE( c.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size works correctly", "[list][capacity]", ft::list, TYPE_LIST)
{
    TestType c;
    REQUIRE( c.size() == 0 );
    c.assign(5, VALUE_TYPE());
    REQUIRE( c.size() == 5 );
    c.clear();
    REQUIRE( c.size() == 0 );
}

/* LIST OPERATIONS */

TEST_CASE("splice work correctly", "[list][operations]")
{
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    ft::list<int> c1 (v, v + 8);
    ft::list<int> c2;
    ft::list<int>::iterator it;

    SECTION( "entire list splice works correctly" ) {
        c2.splice(c2.begin(), c1);
        REQUIRE( c1.size() == 0 );
        REQUIRE( c2.size() == 8 );

        int i = 0;
        for (it = c2.begin(); it != c2.end(); ++it, ++i)
            REQUIRE( *it == v[i] );
    }

    SECTION( "single element splice works correctly" ) {
        c2.splice(c2.begin(), c1, ++c1.begin());
        REQUIRE( c1.size() == 7 );
        REQUIRE( c2.size() == 1 );
        REQUIRE( *c1.begin() == 0 );
        REQUIRE( *c2.begin() == 1 );
    }

    SECTION( "element range splice works correctly" ) {
        c2.splice(c2.begin(), c1, c1.begin(), c1.begin());
        REQUIRE( c1.size() == 8 );
        REQUIRE( c2.size() == 0 );

        c2.splice(c2.begin(), c1, c1.begin(), c1.end());
        REQUIRE( c1.size() == 0 );
        REQUIRE( c2.size() == 8 );

        it = c2.begin();
        std::advance(it, 4);
        c1.splice(c1.begin(), c2, ++c2.begin(), it);
        REQUIRE( c1.size() == 3 );
        REQUIRE( c2.size() == 5 );
    }
}

TEST_CASE("remove(_if) works correctly", "[list][operations]")
{
    int v[] = { 0, 1, 2, 3, 3, 5, 1, 7 };
    ft::list<int> c (v, v + 8);

    SECTION("remove works correctly") {
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
    SECTION("remove_if works correctly") {
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

TEST_CASE("unique works correctly", "[list][operations]")
{
    int i = 0;
    int arr[] = { 1, 1, -66, -22, -22, -1, -22, 2, 2, 2, 9, 9, 109, 109, 109 };
    int arr_unique1[] = { 1, -66, -22, -1, -22, 2, 9, 109 };
    int arr_unique2[] = { 1, -66, -22, -22, -1, -22, 2, 9, 9, 109, 109, 109 };
    std::string arr2[] = {"fdjks", "fdjks", "haha", "hahaha", "ba", "ab", "echo", "echo", "echo", "echo1"};
    std::string arr2_unique[] = {"fdjks", "haha", "hahaha", "ba", "ab", "echo", "echo1"};

    ft::list<int> l (arr, arr + sizeof(arr) / sizeof(int));
    ft::list<std::string> l1 (arr2, arr2 + sizeof(arr2) / sizeof(*arr2));

    SECTION("not specifying how to compare") {
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

    SECTION("specifying how to compare") {
        l.unique( [](int i, int j) { return i + j == 2 || i + j == 4; } );

        REQUIRE( l.size() == (sizeof(arr_unique2) / sizeof(int)) );
        for (auto it = l.begin(); it != l.end(); ++it, ++i)
            REQUIRE( *it == arr_unique2[i] );
    }

    SECTION("special cases") {
        ft::list<int> l2;
        ft::list<int> l3 (arr, arr + sizeof(arr) / sizeof(*arr));

        l2.unique();
        REQUIRE( l2.empty() );

        l3.unique([] (int a, int b) { return a == 30000 && b == 21; });
        REQUIRE( l3.size() == sizeof(arr) / sizeof(*arr) );
        for (auto it = l3.begin(); it != l3.end(); ++it, ++i)
            REQUIRE( *it == arr[i] );
    }
}

TEST_CASE( "merge work correctly", "[list][operations]")
{
    SECTION( "merging without specifying how to compare" ) {
        int i = 0;
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        ft::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2);
        REQUIRE( c2.empty() );

        for (auto it = c1.begin(); it != c1.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }

    SECTION( "merging with specified comparing function" ) {
        int i = 0;
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        ft::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2, std::greater<int>());
        REQUIRE( c2.empty() );

        for (auto it = c1.begin(); it != c1.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }

    SECTION( "mergin with empty list" ) {
        ft::list<int> l1 (5, 10);
        ft::list<int> l2;
        ft::list<int> l3;

        l1.merge(l2);
        REQUIRE( l1.size() == 5 );

        l2.merge(l3);
        REQUIRE( l2.empty() );
        REQUIRE( l3.empty() );
    }
}

TEST_CASE("sort work correctly", "[list][operations]")
{
    int i = 0;
    // ARRAY 1
    int arr1[] = {2, 4, 12, 5, 2, 99, -12};
    int arr1_sorted[] = {-12, 2, 2, 4, 5, 12, 99};
    size_t arr1_size = sizeof(arr1) / sizeof(int);
    ft::list<int> l1 (arr1, arr1 + arr1_size);
    // ARRAY 2
    int arr2[] = {2, 32, 43, 4, 12, 5, 2, 99, 99, 1, 100, 15, 12, 1111111, 21, 42, 0, -1111, 43, -12};
    int arr2_sorted[] = {2, 2, 32, 4, 12, 5, 1, 15, 12, 21, 42, 0, -1111, -12, 43, 1111111, 100, 99, 99, 43};
    size_t arr2_size = sizeof(arr2) / sizeof(int);
    ft::list<int> l2 (arr2, arr2 + arr2_size);
    // ARRAY 3
    std::string arr3[] = {"abcdef", "ahblk", "$", "42", "?", "ßüäopajgh"};
    std::string arr3_sorted[] = {"$", "42", "?", "abcdef", "ahblk", "ßüäopajgh"};
    size_t arr3_size = sizeof(arr3) / sizeof(std::string);
    ft::list<std::string> l5 (arr3, arr3 + arr3_size);

    SECTION( "sorting without specifying how to compare" ) {
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

    SECTION( "sorting with specified comparing function" ) {
        // ARRAY 1
        i = arr1_size - 1;
        l1.sort(std::greater<int>());

        REQUIRE( l1.size() == arr1_size );
        for (auto it = l1.begin(); it != l1.end(); ++it, --i)
            REQUIRE( *it == arr1_sorted[i] );

        // ARRAY 2
        i = 0;
        l2.sort([]<class T>(const T& a, const T& b) { return a == 2 || b > 42; } );

        REQUIRE( l2.size() == arr2_size );
        for (auto it = l2.begin(); it != l2.end(); ++it, ++i)
            REQUIRE( *it == arr2_sorted[i] );
    }

    SECTION ( "sorting unsortable lists" ) {
        ft::list<int> l3;
        ft::list<int> l4 (1, 0);

        l3.sort();
        REQUIRE( l3.empty() );
        l4.sort();
        REQUIRE( l4.size() == 1 );
    }
}

TEST_CASE("reverse works correctly", "[list][operations]")
{
    int i = 8;
    int v[] = {2, 4, 2, 5, 2, 99, -12, 1, 312};
    ft::list<int> l1 (v, v + 9);
    ft::list<int> l2 (v, v + 1);

    l1.reverse();
    REQUIRE( l1.size() == 9 );
    for (auto it = l1.begin(); it != l1.end(); ++it, --i)
        REQUIRE( *it == v[i] );

    l2.reverse();
    REQUIRE( l2.size() == 1 );
    REQUIRE( *l2.begin() == 2 );

    l2.assign(v + 1, v + 3);
    l2.reverse();
    REQUIRE( l2.size() == 2 );
    REQUIRE( *l2.begin() == 2 );
    REQUIRE( *++l2.begin() == 4 );
}
