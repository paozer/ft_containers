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
TEST_CASE("list constructors return the requested list", "[list][basics]")
{
    SECTION("declaration creates an empty list") {
        LIB::list<int> c;
        REQUIRE( c.empty() );
        REQUIRE( c.size() == 0 );
    }
    SECTION("fill constructor creates a list of specified size & filled correct with elements") {
        size_t i = GENERATE(0, 1, 100);
        int j = GENERATE(-100, 914);
        LIB::list<int> c (i, j);

        REQUIRE( c.size() == i );
        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == j );
    }
    SECTION("range constructor creates list filled with the elements of the passed range") {
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        int i = 0;

        if (last >= first) {
            LIB::list<int> l (arr + first, arr + last);
            REQUIRE( l.size() == last - first );
            for (auto it = l.begin(); it != l.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
    SECTION("copy constructor constructs an exact copy of a given list") {
        int i = 0;
        int arr[] = {2, 4, 12, 5, 60, 99, -12};
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);
        LIB::list<int> l1;
        LIB::list<int> l2 (l1);

        REQUIRE( l1.empty() );
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
TEST_CASE("lists can be assigned using the = operator", "[list][operators]")
{
    int arr[] = {2, 4, 12, 5, 60, 99, -12};

    SECTION("different sizes of ranges can be assigned to lists") {
        size_t i = 0;
        unsigned int first = GENERATE(0, 1, 3);
        unsigned int last = GENERATE(0, 3, 6);

        if (last >= first) {
            LIB::list<int> mylist1 (arr + first, arr + last);
            LIB::list<int> mylist2 = mylist1;

            REQUIRE( mylist2.size() == last - first );
            for (auto it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
                REQUIRE( *it == arr[first + i] );
        }
    }
    SECTION("assigning from & between empty lists works as expected") {
        LIB::list<int> mylist3;
        LIB::list<int> mylist4 = mylist3;
        LIB::list<int> mylist5 (arr, arr + 3);

        REQUIRE( mylist4.size() == mylist3.size() );
        REQUIRE( mylist4.empty() );

        mylist5 = mylist3;
        REQUIRE( mylist5.size() == mylist3.size() );
        REQUIRE( mylist5.empty() );
    }
}

/* RELATIONAL OPERATORS */
TEST_CASE("lists can be compared using relational operators", "[list][basics]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    int d[] = {1, 4, -1, 12, 0, 0};
    LIB::list<int> mylist1 (a, a + 5);
    LIB::list<int> mylist2 (b, b + 2);
    LIB::list<int> mylist3;
    LIB::list<int> mylist4 (c, c + 6);
    LIB::list<int> mylist5 (d, d + 6);

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
TEST_CASE("list begin/rbegin work correctly", "[list][basics]")
{
    SECTION("non-const iterator behaviour") {
        LIB::list<int> mylist (1, 10);       // { 10 }
        REQUIRE( *mylist.begin() == 10 );
        REQUIRE( *mylist.rbegin() == 10 );
        REQUIRE( ++mylist.begin() == mylist.end() );
        REQUIRE( ++mylist.rbegin() == mylist.rend() );
        REQUIRE( mylist.begin() != mylist.end() );
        REQUIRE( mylist.rbegin() != mylist.rend() );

        mylist.push_front(5);               // { 5, 10 }
        LIB::list<int>::iterator it = mylist.begin();
        LIB::list<int>::reverse_iterator rit = mylist.rbegin();
        REQUIRE( *(it++) == 5 );
        REQUIRE( *it == 10 );
        REQUIRE( *(rit++) == 10 );
        REQUIRE( *rit == 5 );

        mylist.push_back(20);               // { 5, 10, 20 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 10 );
        REQUIRE( *++++mylist.begin() == 20 );
        REQUIRE( *mylist.rbegin() == 20 );
        REQUIRE( *++mylist.rbegin() == 10 );
        REQUIRE( *++++mylist.rbegin() == 5 );

        mylist.pop_front();                 // { 10, 20 }
        it = mylist.begin();
        rit = mylist.rbegin();
        REQUIRE( *it == 10 );
        REQUIRE( *++it == 20 );
        REQUIRE( *rit == 20 );
        REQUIRE( *++rit == 10 );

        *mylist.begin() = 21;                // { 21, 42 }
        *mylist.rbegin() = 42;               // { 21, 42 }
        REQUIRE( *mylist.begin() == 21 );
        REQUIRE( *++mylist.begin() == 42 );
        REQUIRE( *mylist.rbegin() == 42 );
        REQUIRE( *++mylist.rbegin() == 21 );
    }
    SECTION("const iterator behaviour") {
        int arr[] = {1, 2, 3, 4, 5, 6};
        const LIB::list<int> mylist (1, 10);
        const LIB::list<int> mylist1;
        LIB::list<int> mylist2 (arr, arr + 6);
        LIB::list<int>::const_iterator it = mylist2.begin();
        LIB::list<int>::const_reverse_iterator rit = mylist2.rbegin();

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
        LIB::list<int> mylist (1, 10);               // { 10 }

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
        const LIB::list<int> mylist (1, 10);
        const LIB::list<int> mylist1;
        LIB::list<int> mylist2 (arr, arr + 6);
        LIB::list<int>::const_iterator it = --mylist2.end();
        LIB::list<int>::const_reverse_iterator rit = --mylist2.rend();

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
TEMPLATE_PRODUCT_TEST_CASE("lists can be assigned values", "[list][modifiers]", LIB::list, TYPE_LIST)
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

TEST_CASE("lists can push elements", "[list][modifiers]")
{
    int value;
    LIB::list<int> l1;

    SECTION("elements can be pushed to the front of the list") {
        for (int i = 0; i < 50; ++i) {
            value = rand();
            l1.push_front(value);
            REQUIRE( *l1.begin() == value );
        }
        REQUIRE( l1.size() == (unsigned int)50 );
    }
    SECTION("elements can be pushed to the back of the list") {
        for (int i = 0; i < 50; ++i) {
            value = rand();
            l1.push_back(value);
            REQUIRE( l1.back() == value );
        }
        REQUIRE( l1.size() == (unsigned int)50 );
    }
}

TEST_CASE("lists can pop elements", "[list][modifiers]")
{
    unsigned int size = GENERATE(0, 1, 2, 15);
    LIB::list<int> l1 (size, 100);

    SECTION("elements can be popped from the front of the list") {
        while (!l1.empty()) {
            REQUIRE( l1.size() == size);
            REQUIRE( l1.front() == 100);
            REQUIRE( l1.back() == 100);
            l1.pop_front();
            --size;
        }
        REQUIRE( size == 0 );
    }
    SECTION("elements can be popped from the front of the list") {
        while (!l1.empty()) {
            REQUIRE( l1.size() == size);
            REQUIRE( l1.front() == 100);
            REQUIRE( l1.back() == 100);
            l1.pop_back();
            --size;
        }
        REQUIRE( size == 0 );
    }
}

TEST_CASE("inserting elements works correctly", "[list][modifiers]")
{
    LIB::list<int> l;
    LIB::list<int>::iterator it = l.begin();
    LIB::list<int>::iterator ret;

    SECTION("single element insert works correctly") {
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
    SECTION("fill insert works correctly") {
        l.insert(l.begin(), 5, 10);
        REQUIRE( l.size() == 5 );

        for (it = l.begin(); it != l.end(); ++it)
            REQUIRE( *it == 10 );

        l.insert(it, 1, 100);
        REQUIRE( l.size() == 6 );
        REQUIRE( l.back() == 100 );
    }
    SECTION("range insert works correctly") {
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

TEST_CASE("elements can be erase from the list", "[list][modifiers]")
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    LIB::list<int> mylist (arr, arr + 6);                   // { 1, 2, 3, 4, 5, 6 }
    LIB::list<int>::iterator ret;

    ret = mylist.erase(mylist.begin());                     // { 2, 3, 4, 5, 6 }
    REQUIRE( mylist.size() == 5 );
    REQUIRE( *ret == 2 );

    ret = mylist.erase(--mylist.end());                     // { 2, 3, 4, 5 }
    REQUIRE( mylist.size() == 4 );
    REQUIRE(( ret == mylist.end() ));
    REQUIRE( mylist.back() == 5 );

    ret = mylist.erase(++mylist.begin(), --mylist.end());   // { 2, 3 }
    REQUIRE( mylist.size() == 2 );
    REQUIRE( *ret == 5 );

    ret = mylist.erase(mylist.begin(), mylist.end());       // { }
    REQUIRE( mylist.empty() );
    REQUIRE(( ret == mylist.end() ));
}

TEST_CASE("lists can be swapped", "[list][modifiers]")
{
    int arr1[] = {1, 2, 3, 4, 5, 6};
    int arr2[] = {-3, 69, 1};
    LIB::list<int>::iterator it;
    int i = 0;

    SECTION("non-empty lists can be swapped") {
        LIB::list<int> mylist1 (arr1, arr1 + 6);
        LIB::list<int> mylist2 (arr2, arr2 + 3);

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 3 );
        REQUIRE( mylist2.size() == 6 );

        for (it = mylist1.begin(); it != mylist1.end(); ++it, ++i)
            REQUIRE( *it == arr2[i] );
        i = 0;
        for (it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }
    SECTION("empty lists can be swapped with filled lists") {
        LIB::list<int> mylist1 (arr1, arr1 + 6);
        LIB::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.empty() );
        REQUIRE( mylist2.size() == 6 );
        for (it = mylist2.begin(); it != mylist2.end(); ++it, ++i)
            REQUIRE( *it == arr1[i] );
    }
    SECTION("empty lists can be swapped") {
        LIB::list<int> mylist1;
        LIB::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.empty() );
        REQUIRE( mylist2.empty() );
    }
}


TEST_CASE("lists can be resized", "[list][modifiers]")
{
    int arr[] = {1, 2, 3, 4, 5};
    LIB::list<int> l0 (arr, arr + 5);

    SECTION("the lists size can be reduced with resize") {
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
    SECTION("resize adds elements to the lists end") {
        l0.resize(5);
        REQUIRE( l0.size() == 5 );
        l0.resize(7);
        REQUIRE( l0.size() == 7 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == int() );
        l0.resize(10);
        REQUIRE( l0.size() == 10 );
        REQUIRE( l0.front() == 1 );
        REQUIRE( l0.back() == int() );
    }

}

TEMPLATE_PRODUCT_TEST_CASE("lists can be cleared from their content", "[list][modifiers]", LIB::list, TYPE_LIST)
{
    TestType c;

    c.clear();
    REQUIRE( c.empty() );
    c.assign(10, VALUE_TYPE());
    REQUIRE( c.size() == 10 );
    c.clear();
    REQUIRE( c.empty() );
    c.assign(3, VALUE_TYPE());
    REQUIRE( c.size() == 3 );
    c.clear();
    REQUIRE( c.empty() );
}

/* ELEMENT ACCESS */
TEST_CASE("front/back return references to the list first/last element", "[list][element access]")
{
    int arr[] = {1, 2, 3, 4, 5};
    LIB::list<int> mylist (arr, arr + 5);
    const LIB::list<int> cmylist (arr + 1, arr + 5);

    mylist.front() -= 21;
    REQUIRE( mylist.front() == -20 );
    mylist.back() -= 42;
    REQUIRE( mylist.back() == -37 );

    REQUIRE( cmylist.front() == 2 );
    REQUIRE( cmylist.back() == 5 );
}

/* CAPACITY */

TEMPLATE_PRODUCT_TEST_CASE("empty reflects if the list is empty", "[list][basics]", LIB::list, TYPE_LIST)
{
    TestType c;

    REQUIRE( c.empty() );
    c.push_back(VALUE_TYPE());
    REQUIRE_FALSE( c.empty() );
    c.pop_front();
    REQUIRE( c.empty() );
}

TEMPLATE_PRODUCT_TEST_CASE("size returns the lists size", "[list][capacity]", LIB::list, TYPE_LIST)
{
    TestType c;

    REQUIRE( c.size() == 0 );
    c.assign(5, VALUE_TYPE());
    REQUIRE( c.size() == 5 );
    c.clear();
    REQUIRE( c.size() == 0 );
}

/* LIST OPERATIONS */
TEST_CASE("lists can be spliced", "[list][operations]")
{
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    LIB::list<int> c1 (v, v + 8);
    LIB::list<int> c2;
    LIB::list<int>::iterator it;

    SECTION("splicing entire lists works") {
        c2.splice(c2.begin(), c1);
        REQUIRE( c1.size() == 0 );
        REQUIRE( c2.size() == 8 );

        int i = 0;
        for (it = c2.begin(); it != c2.end(); ++it, ++i)
            REQUIRE( *it == v[i] );
    }
    SECTION("splicing single elements works") {
        c2.splice(c2.begin(), c1, ++c1.begin());
        REQUIRE( c1.size() == 7 );
        REQUIRE( c2.size() == 1 );
        REQUIRE( *c1.begin() == 0 );
        REQUIRE( *c2.begin() == 1 );
    }
    SECTION("splicing a range works") {
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
    LIB::list<int> c (v, v + 8);

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

TEST_CASE("unique removes duplicates from a list", "[list][operations]")
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

TEST_CASE("lists can be merged", "[list][operations]")
{
    SECTION("merging without a specified comparing function") {
        int i = 0;
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        LIB::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        LIB::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2);
        REQUIRE( c2.empty() );

        for (auto it = c1.begin(); it != c1.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }
    SECTION("merging with a specified comparing function") {
        int i = 0;
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        LIB::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        LIB::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2, std::greater<int>());
        REQUIRE( c2.empty() );

        for (auto it = c1.begin(); it != c1.end(); ++it, ++i)
            REQUIRE( *it == c[i] );
    }
    SECTION("merging with or between empty lists works") {
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

TEST_CASE("list can be sorted", "[list][operations]")
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
        l2.sort([]<class T>(const T& a, const T& b) { return a == 2 || b > 42; } );

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

TEST_CASE("lists can be reversed", "[list][operations]")
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
