#include "../srcs/list/list.hpp"
#include "../srcs/vector/vector.hpp"

#include "catch.hpp"

#include <list>
#include <vector>

#define TYPE_LIST ( int, char, const int,std::string, ft::vector<int>, std::vector<std::string>, ft::list<std::string>, std::list<int> )
#define VALUE_TYPE typename TestType::value_type

/* CONSTRUCTION */
TEST_CASE("list construction works correctly", "[list][basics]")
{
    SECTION("default constructor creates empty list") {
        ft::list<int> c;
        REQUIRE( c.empty() );
        REQUIRE( c.size() == 0 );
    }
    SECTION("fill constructor creates list of specified size & filled with elements") {
        size_t i = GENERATE(0, 1, 100);
        size_t j = GENERATE(-100, 914);
        ft::list<int> c (i, j);
        REQUIRE( c.size() == i );
        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == (int)j );
    }
    SECTION("range constructor creates list with correct elements from range") {
        size_t s = GENERATE(0, 4, 7);
        int v[] = {2, 4, 12, 5, 60, 99, -12};
        ft::list<int> c1 (v, v + s);
        REQUIRE( c1.size() == s );

        size_t i = 0;
        for (auto it = c1.begin(); it != c1.end(); ++it)
            REQUIRE( *it == v[i++] );
        REQUIRE_FALSE( i != s);

        ft::list<int> c2 (v, v);
        REQUIRE( c2.size() == 0 );
        ft::list<int> c3 (v + 2, v + 4);
        REQUIRE( c3.size() == 2 );
    }
    SECTION("copy constructor constructs an exact copy of a given list") {
        SECTION ( "works on construction from non-empty list" ) {
            size_t s = GENERATE(0, 4, 7);
            int v[] = {2, 4, 12, 5, 60, 99, -12};
            ft::list<int> c1 (v, v + s);
            ft::list<int> c2 (c1);

            REQUIRE( c1.size() == c2.size() );

            auto it1 = c1.begin();
            auto it2 = c2.begin();
            while (it1 != c1.end() || it2 != c2.end()) {
                REQUIRE( *it1 == *it2 );
                ++it1;
                ++it2;
            }
        }
        SECTION ( "works on construction from empty list" ) {
            ft::list<int> c1;
            ft::list<int> c2 (c1);
            REQUIRE( c1.empty() );
        }
    }
}

/* ASSIGNATION OPERATOR */
TEST_CASE("assignation operator works correctly", "[list][basics]")
{
    size_t s = GENERATE(0, 4, 7);
    int v[] = {2, 4, 12, 5, 60, 99, -12};
    ft::list<int> mylist1 (v, v + s);
    ft::list<int> mylist2 = mylist1;

    REQUIRE( mylist2.size() == s );
    size_t i = 0;
    for (auto it = mylist2.begin(); it != mylist2.end(); ++it) {
        REQUIRE( *it == v[i] );
        ++i;
    }
}

/* RELATIONAL OPERATORS */
TEST_CASE("relational operators work correctly", "[list][basics]")
{
    int a[] = {1, 4, -1, 2, 33};
    int b[] = {1, 4};
    int c[] = {1, 4, -1, 2, 33, 0};
    ft::list<int> mylist1 (a, a + 5);
    ft::list<int> mylist2 (b, b + 2);
    ft::list<int> mylist3;
    ft::list<int> mylist4 (c, c + 6);

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
}

/* ITERATORS */
TEST_CASE( "list begin works correctly", "[list][basics]")
{
    SECTION("non-const iterator behaviour") {
        ft::list<int> mylist (1, 10);       // { 10 }
        REQUIRE( *mylist.begin() == 10 );
        mylist.push_front(5);               // { 5, 10 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 10 );
        mylist.push_back(20);               // { 5, 10, 20 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 10 );
        REQUIRE( *++++mylist.begin() == 20 );
        mylist.pop_front();                 // { 10, 20 }
        REQUIRE( *mylist.begin() == 10 );
        REQUIRE( *++mylist.begin() == 20 );
        *mylist.begin() = 5;                // { 5, 20 }
        REQUIRE( *mylist.begin() == 5 );
        REQUIRE( *++mylist.begin() == 20 );
    }
    SECTION("const iterator behaviour") {
        const ft::list<int> mylist (1, 10);
        const ft::list<int> mylist1;
        REQUIRE( *mylist.begin() == 10 );
        REQUIRE(( mylist1.begin() == mylist1.end() ));

        int arr[] = {1, 2, 3, 4, 5, 6};
        ft::list<int> mylist2 (arr, arr + 6);
        ft::list<int>::const_iterator it = mylist2.begin();
        for (int i = 0; i < 6; ++i) {
            REQUIRE( *it == arr[i] );
            ++it;
        }
    }
}

TEST_CASE("list end works correctly", "[list][basics]")
{
    SECTION("non-const iterator behaviour") {
        ft::list<int> mylist (1, 10);               // { 10 }
        REQUIRE( *--mylist.end() == 10 );
        mylist.push_front(5);                  // { 5, 10 }
        REQUIRE( *----mylist.end() == 5 );
        REQUIRE( *--mylist.end() == 10 );
        mylist.push_back(20);                  // { 5, 10, 20 }
        REQUIRE( *------mylist.end() == 5 );
        REQUIRE( *----mylist.end() == 10 );
        REQUIRE( *--mylist.end() == 20 );
        mylist.pop_back();                     // { 5, 10 }
        REQUIRE( *----mylist.end() == 5 );
        REQUIRE( *--mylist.end() == 10 );
        *--mylist.end() = 5;                   // { 5, 5 }
        REQUIRE( *----mylist.end() == 5 );
        REQUIRE( *--mylist.end() == 5 );
    }
    SECTION("const iterator behaviour") {
        int arr[] = {1, 2, 3, 4, 5, 6};
        const ft::list<int> mylist (arr, arr + 6);
        ft::list<int>::const_iterator it = mylist.end();
        for (int i = 6; i > 0;) {
            --it;
            --i;
            REQUIRE( *it == arr[i] );
        }
    }
}

/* MODIFIERS */

TEMPLATE_PRODUCT_TEST_CASE( "assign work correctly", "[list][modifiers]", ft::list, TYPE_LIST )
{
    SECTION("range assign works correctly") {
        std::vector<VALUE_TYPE> v (10);
        TestType c;
        c.assign(v.begin() + 2, v.begin() + 7);
        REQUIRE( c.size() == 5 );
        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == VALUE_TYPE() );
    }
    SECTION("fill assign works correctly") {
        size_t n = GENERATE(0, 100);
        TestType c;
        c.assign(n, VALUE_TYPE() );
        REQUIRE( c.size() == n );
        for (auto it = c.begin(); it != c.end(); ++it)
            REQUIRE( *it == VALUE_TYPE() );
    }
}

TEST_CASE("push_front works correctly", "[list][modifiers]")
{
    SECTION( "the element is added at the front & size is incremented" ) {
        ft::list<int> c;
        int value;
        for (int i = 0; i < 50; ++i) {
            value = rand();
            c.push_front(value);
            REQUIRE( *c.begin() == value );
        }
        REQUIRE( (int)c.size() == 50 );
    }
}

TEST_CASE("push_back works correctly", "[list][modifiers]")
{
    SECTION( "containers size is incremented & the element is added at the back" ) {
        int value;
        ft::list<int> c;
        for (int i = 0; i < 50; ++i) {
            value = rand();
            c.push_back(value);
            REQUIRE( *(--c.end()) == value );
        }
        REQUIRE( (int)c.size() == 50 );
    }
}

TEST_CASE("pop_front works correctly", "[list][modifiers]")
{
    ft::list<int> c (10, 100);
    size_t i = c.size();
    while (!c.empty()) {
        REQUIRE( c.size() == i);
        REQUIRE( *c.begin() == 100);
        REQUIRE( *(--c.end()) == 100);
        c.pop_front();
        --i;
    }
}

TEST_CASE("pop_back works correctly", "[list][modifiers]")
{
    ft::list<int> c (10, 100);
    size_t i = c.size();
    while (!c.empty()) {
        REQUIRE( c.size() == i);
        REQUIRE( *c.begin() == 100);
        REQUIRE( *--c.end() == 100);
        c.pop_back();
        --i;
    }
}

TEST_CASE("insert work correctly", "[list][modifiers]")
{
    ft::list<int> c;
    auto it = c.begin();

    SECTION( "single element insert works correctly" ) {
        c.insert(it, 5);           // { 5 }
    REQUIRE( c.size() == 1 );
    REQUIRE( *c.begin() == 5 );
    c.insert(it, 10);           // { 5, 10}
    REQUIRE( c.size() == 2 );
    REQUIRE( *--c.end() == 10 );
    }
    SECTION( "fill insert works correctly" ) {
        c.insert(it, 5, 10);
        REQUIRE( c.size() == 5 );
        for (auto ite = c.begin(); ite != c.end(); ++ite)
            REQUIRE( *ite == 10 );
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
    ft::list<int> mylist (arr, arr + 6);
    ft::list<int>::iterator ret;

    ret = mylist.erase(mylist.begin());
    REQUIRE( mylist.size() == 5 );
    REQUIRE( *ret == 2 );
    ret = mylist.erase(--mylist.end());
    REQUIRE( mylist.size() == 4 );
    REQUIRE(( ret == mylist.end() ));
    REQUIRE( *(--mylist.end()) == 5 );
    ret = mylist.erase(++mylist.begin(), --mylist.end());
    REQUIRE( mylist.size() == 2 );
    REQUIRE( *ret == 5 );
    ret = mylist.erase(mylist.begin(), mylist.end());
    REQUIRE( mylist.size() == 0 );
    REQUIRE(( ret == mylist.end() ));
}

TEST_CASE( "swap works correctly", "[list][modifiers]")
{
    int arr1[] = {1, 2, 3, 4, 5, 6};
    int arr2[] = {-3, 69, 1};
    ft::list<int>::iterator it;
    int i;

    SECTION("non-empty lists can be swapped") {
        ft::list<int> mylist1 (arr1, arr1 + 6);
        ft::list<int> mylist2 (arr2, arr2 + 3);

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 3 );
        REQUIRE( mylist2.size() == 6 );
        i = 0;
        for (it = mylist1.begin(); it != mylist1.end(); ++it) {
            REQUIRE( *it == arr2[i] );
            ++i;
        }
        i = 0;
        for (it = mylist2.begin(); it != mylist2.end(); ++it) {
            REQUIRE( *it == arr1[i] );
            ++i;
        }
    }
    SECTION("empty lists can be swapped w/ filled lists") {
        ft::list<int> mylist1 (arr1, arr1 + 6);
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 6 );
        i = 0;
        for (it = mylist2.begin(); it != mylist2.end(); ++it) {
            REQUIRE( *it == arr1[i] );
            ++i;
        }
    }
    SECTION("empty lists can be swapped") {
        ft::list<int> mylist1;
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 0 );
    }
    SECTION("lists with const content can be swapped") {
        ft::list<const int> mylist1 (arr1, arr1 + 6);
        ft::list<const int> mylist2;
        ft::list<const int>::iterator cit;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 6 );
        i = 0;
        for (cit = mylist2.begin(); cit != mylist2.end(); ++cit) {
            REQUIRE( *cit == arr1[i] );
            ++i;
        }
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
}

TEMPLATE_PRODUCT_TEST_CASE("clear works correctly", "[list][modifiers]", ft::list, TYPE_LIST)
{
    TestType c;
    c.clear();
    REQUIRE( c.size() == 0 );
    c.assign(10, VALUE_TYPE());
    REQUIRE( c.size() == 10 );
    c.clear();
    REQUIRE( c.size() == 0 );
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
    int i;

    SECTION( "entire list splice works correctly" ) {
        c2.splice(c2.begin(), c1);
        REQUIRE( c1.size() == 0 );
        REQUIRE( c2.size() == 8 );
        i = 0;
        for (it = c2.begin(); it != c2.end(); ++it) {
            REQUIRE( *it == v[i] );
            ++i;
        }
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
    int a[] = { 1, 1, -66, -22, -22, -1, -22, 2, 2, 2, 9, 9, 109, 109, 109 };
    ft::list<int> c (a, a + sizeof(a) / sizeof(int));

    SECTION("unique without specifying how to compare") {
        int unique[] = { 1, -66, -22, -1, -22, 2, 9, 109 };
        c.unique();
        REQUIRE( c.size() == (sizeof(unique) / sizeof(int)) );
        int i = 0;
        for (auto it = c.begin(); it != c.end(); ++it) {
            REQUIRE( *it == unique[i] );
            ++i;
        }
    }
    SECTION("unique w/ specifying how to compare") {
        int duplicates[] = { 1, -66, -22, -22, -1, -22, 2, 9, 9, 109, 109, 109 };
        c.unique( [] (int i, int j) { return i + j == 2 || i + j == 4; });

        REQUIRE( c.size() == (sizeof(duplicates) / sizeof(int)) );
        int i = 0;
        for (auto it = c.begin(); it != c.end(); ++it) {
            REQUIRE( *it == duplicates[i] );
            ++i;
        }
    }
}

TEST_CASE( "merge work correctly", "[list][operations]")
{
    SECTION( "merging without specifying how to compare" ) {
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        ft::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2);
        REQUIRE( c2.empty() );
        int i = 0;
        for (auto it = c1.begin(); it != c1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "merging with specified comparing function" ) {
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        ft::list<int> c1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> c2 (b, b + sizeof(b) / sizeof(int));

        c1.merge(c2, std::greater<int>());
        REQUIRE( c2.empty() );
        int i = 0;
        for (auto it = c1.begin(); it != c1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "mergin w/ empty list" ) {
        ft::list<int> c1 (5, 10);
        ft::list<int> c2;
        ft::list<int> c3;

        c1.merge(c2);
        REQUIRE( c1.size() == 5 );
        c2.merge(c3);
        REQUIRE( c2.size() == 0 );
        REQUIRE( c3.size() == 0 );
    }
}

TEST_CASE("sort work correctly", "[list][operations]")
{
    int v[] = {2, 4, 12, 5, 2, 99, -12};
    int v_sorted[] = {-12, 2, 2, 4, 5, 12, 99};
    size_t v_size = sizeof(v) / sizeof(int);
    ft::list<int> c (v, v + v_size);

    SECTION( "sorting without specifying how to compare" ) {
        c.sort();
        REQUIRE( c.size() == v_size );
        int i = 0;
        for (auto it = c.begin(); it != c.end(); ++it) {
            REQUIRE( *it == v_sorted[i] );
            ++i;
        }
    }
    SECTION( "sorting with specified comparing function" ) {
        c.sort(std::greater<int>());
        REQUIRE( c.size() == v_size );
        int j = v_size;
        for (auto it = c.begin(); it != c.end(); ++it) {
            --j;
            REQUIRE( *it == v_sorted[j] );
        }
    }
    SECTION ( "sorting unsortable lists" ) {
        ft::list<int> c1;
        ft::list<int> c2 (1, 0);

        c1.sort();
        REQUIRE( c1.size() == 0 );
        c2.sort();
        REQUIRE( c2.size() == 1 );
    }
}

TEST_CASE("reverse works correctly", "[list][operations]")
{
    int v[] = {2, 4, 2, 5, 2, 99, -12, 1, 312};
    ft::list<int> c (v, v + 9);
    c.reverse();
    REQUIRE( c.size() == 9 );
    int i = 8;
    for (auto it = c.begin(); it != c.end(); ++it) {
        REQUIRE( *it == v[i] );
        --i;
    }
}

TEMPLATE_TEST_CASE( "ft::list returns same value as std::list", "[list][capacity]",
        int, char, std::string, std::list<int>, std::vector<std::string> )
{
    ft::list<TestType> ft_c;
    std::list<TestType> stl_c;
    REQUIRE( ft_c.max_size() == stl_c.max_size() );
}
