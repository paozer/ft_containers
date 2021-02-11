#include "catch.hpp"

#include "../srcs/list/list.hpp"

#include <list>
#include <vector>
#include <iterator>

// template arguments used to test container
using templateTestList = std::tuple<int, char, std::string, std::list<int> >;

/* CONSTRUCTOR & DESTRUCTOR */

TEMPLATE_LIST_TEST_CASE( "default constructor works correctly", "[list][basics]", templateTestList )
{
    ft::list<TestType> mylist;
    REQUIRE( mylist.empty() );
    REQUIRE( mylist.size() == 0 );
}

TEST_CASE( "fill constructor works correctly", "[list][basics]" )
{
    SECTION( "list(0, 5) returns list w/ size 0" ) {
        ft::list<int> mylist (0, 5);
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "list(1, 5) returns list w/ size 1 and 5 at the front" ) {
        ft::list<int> mylist (1, 5);
        REQUIRE( mylist.size() == 1 );
        REQUIRE( mylist.front() == 5 );
    }
    SECTION( "list(1000, 5) returns list w/ size 1000 and filled w/ 5" ) {
        ft::list<int> mylist (1000, 5);
        REQUIRE( mylist.size() == 1000 );
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            REQUIRE( *it == 5 );
    }
}

TEST_CASE( "range constructor works correctly", "[list][basics]" )
{
    int v[] = {2, 4, 12, 5, 60, 99, -12};
    size_t v_size = sizeof(v) / sizeof(int);
    ft::list<int> mylist (v, v + v_size);

    REQUIRE( mylist.size() == v_size );
    auto it = mylist.begin();
    for (size_t i = 0; i < v_size; ++i) {
        REQUIRE( *it == v[i] );
        ++it;
    }
}

TEST_CASE( "copy constructor works correctly", "[list][basics]" )
{
    SECTION ( "works on construction from non-empty list" ) {
        ft::list<int> mylist1;
        for (int i = 0; i < 10; ++i) {
            mylist1.push_back(i);
        }
        ft::list<int> mylist2 (mylist1);
        REQUIRE( mylist1.size() == mylist2.size() );

        auto it1 = mylist1.begin();
        auto it2 = mylist2.begin();
        while (it1 != mylist1.end() || it2 != mylist2.end()) {
            REQUIRE( *it1 == *it2 );
            ++it1;
            ++it2;
        }
    }
    SECTION ( "works on construction from empty list" ) {
        ft::list<int> mylist1;
        ft::list<int> mylist2 (mylist1);

        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist1.size() == mylist2.size() );
    }
}

//TEST_CASE( "destructor works correctly", "[list][basics]" );

/* OPERATORS */

TEST_CASE( "operator= works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2 = mylist1;

    REQUIRE( mylist2.size() == 10 );
    for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
        REQUIRE( *it == 10 );
}

TEST_CASE( "operator== works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2;
    ft::list<int> mylist3;
    ft::list<int> mylist4;

    REQUIRE_FALSE( (mylist1 == mylist2) );
    mylist2 = mylist1;
    REQUIRE( (mylist1 == mylist2) );
    REQUIRE( (mylist3 == mylist4) );
}

TEST_CASE( "operator!= works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2;
    ft::list<int> mylist3;
    ft::list<int> mylist4;

    REQUIRE( (mylist1 != mylist2) );
    mylist2 = mylist1;
    REQUIRE_FALSE( (mylist1 != mylist2) );
    REQUIRE_FALSE( (mylist3 != mylist4) );
}

TEST_CASE( "operator< works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2 (11, 10);
    ft::list<int> mylist3 (10, 99);
    ft::list<int> mylist4;
    ft::list<int> mylist5 (10, 10);

    REQUIRE( (mylist1 < mylist2) );
    REQUIRE( (mylist2 < mylist3) );
    REQUIRE( (mylist4 < mylist3) );
    REQUIRE_FALSE( (mylist3 < mylist4) );
    REQUIRE_FALSE( (mylist1 < mylist5) );
    REQUIRE_FALSE( (mylist4 < mylist4) );
}

TEST_CASE( "operator<= works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2 (11, 10);
    ft::list<int> mylist3 (10, 99);
    ft::list<int> mylist4;
    ft::list<int> mylist5 (10, 10);

    REQUIRE( (mylist1 <= mylist2) );
    REQUIRE( (mylist2 <= mylist3) );
    REQUIRE( (mylist4 <= mylist3) );
    REQUIRE( (mylist1 <= mylist5) );
    REQUIRE( (mylist4 <= mylist4) );
    REQUIRE_FALSE( (mylist3 <= mylist4) );
    REQUIRE_FALSE( (mylist2 <= mylist1) );
}

TEST_CASE( "operator> works correctly", "[list][basics]" )
{
    ft::list<int> mylist1 (10, 10);
    ft::list<int> mylist2 (11, 10);
    ft::list<int> mylist3 (10, 99);
    ft::list<int> mylist4;
    ft::list<int> mylist5 (10, 10);

    REQUIRE_FALSE( (mylist1 > mylist2) );
    REQUIRE_FALSE( (mylist2 > mylist3) );
    REQUIRE_FALSE( (mylist4 > mylist3) );
    REQUIRE_FALSE( (mylist1 > mylist5) );
    REQUIRE_FALSE( (mylist4 > mylist4) );
    REQUIRE( (mylist3 > mylist4) );
    REQUIRE( (mylist3 > mylist1) );
    REQUIRE( (mylist2 > mylist1) );
}

TEST_CASE( "operator>= works correctly", "[list][basics]" )
{
    int a[] = { 1, 4, -1, 2, 33 };
    int b[] = { 1, 4, -6, 4, 2 };
    int c[] = { 0, 12, -6, 4, 2 };
    ft::list<int> mylist1 (a, a + sizeof(a) / sizeof(int));
    ft::list<int> mylist2 (mylist1);
    ft::list<int> mylist3 (b, b + sizeof(b) / sizeof(int));
    ft::list<int> mylist4 (c, c + sizeof(c) / sizeof(int));

    REQUIRE( (mylist1 >= mylist2) );
    REQUIRE( (mylist1 >= mylist3) );
    REQUIRE( (mylist1 >= mylist4) );

    mylist2.push_back(-13);
    REQUIRE_FALSE( (mylist1 >= mylist2) );
}

/* ITERATORS */

// add const_iterator tests & non const to const construction
// add tests using std::distance

TEST_CASE( "begin works correctly", "[list][iterators]" )
{
    ft::list<int> mylist (1, 10);       // { 10 }
    REQUIRE( *mylist.begin() == 10 );
    mylist.push_front(5);               // { 5, 10 }
    REQUIRE( *mylist.begin() == 5 );
    mylist.push_back(20);               // { 5, 10, 20 }
    REQUIRE( *mylist.begin() == 5 );
    mylist.pop_front();                 // { 10, 20 }
    REQUIRE( *mylist.begin() == 10 );
    *mylist.begin() = 5;                // { 5, 20 }
    REQUIRE( *mylist.begin() == 5 );
}

TEST_CASE( "end works correctly", "[list][iterators]" )
{
    ft::list<int> mylist (1, 10);       // { 10 }
    REQUIRE( *--mylist.end() == 10 );
    mylist.push_front(5);               // { 5, 10 }
    REQUIRE( *--mylist.end() == 10 );
    mylist.push_back(20);               // { 5, 10, 20 }
    REQUIRE( *--mylist.end() == 20 );
    mylist.pop_back();                 // { 5, 10 }
    REQUIRE( *--mylist.end() == 10 );
    *--mylist.end() = 5;                // { 5, 5 }
    REQUIRE( *--mylist.end() == 5 );
}

/* CAPACITY */

TEMPLATE_LIST_TEST_CASE( "empty reflects list state", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;
    REQUIRE( mylist.empty() );
    mylist.push_front(TestType());
    REQUIRE_FALSE( mylist.empty() );
    mylist.pop_front();
    REQUIRE( mylist.empty() );
}

TEMPLATE_LIST_TEST_CASE( "size works correctly", "[list][capacity]", templateTestList )
{
    ft::list<TestType> mylist;

    SECTION( "uninitialized list has size 0" ) {
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "size() returns updated size when elements are added" ) {
        for (int i = 0; i < 5; ++i)
            mylist.push_front(TestType());
        REQUIRE( mylist.size() == 5 );

        SECTION( "size() returns updated size after clearing the list" ) {
            mylist.clear();
            REQUIRE( mylist.size() == 0 );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "max_size works correctly", "[list][capacity]", templateTestList )
{
    SECTION ( "max_size() returns same value as std::list" ) {
        ft::list<TestType> mylist;
        std::list<TestType> stl_list;
        REQUIRE( mylist.max_size() == stl_list.max_size() );
    }
}

/* ELEMENT ACCESS */

TEST_CASE( "front works correctly", "[list][element access]" )
{
    SECTION( "first element is changed when assigning to reference" ) {
        ft::list<int> mylist (1, 77);
        mylist.front() -= 22;
        REQUIRE( mylist.front() == 55 );
    }
}

TEST_CASE( "back works correctly", "[list][element access]" )
{
    SECTION( "last element is changed when assigning to reference" ) {
        ft::list<int> mylist (1, 77);
        mylist.back() -= 22;
        REQUIRE( mylist.back() == 55 );
    }
}

/* MODIFIERS */

TEMPLATE_LIST_TEST_CASE( "assign work correctly", "[list][modifiers]", templateTestList )
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

        std::vector<TestType> v (size, TestType());
        auto first = v.begin();
        auto last = v.begin();
        std::advance(first, i);
        std::advance(last, j);
        ft::list<TestType> l;
        l.assign(first, last);
        REQUIRE( (l.size() == j - i) );
    }

    SECTION( "fill assign works correctly" ) {
        size_t n = GENERATE(0, 5, 1000);
        ft::list<TestType> mylist;
        TestType val = TestType();
        mylist.assign(n, val);

        REQUIRE( mylist.size() == n);
        for (auto it = mylist.begin(); it != mylist.end(); ++it)
            REQUIRE( *it == val);
    }
}

TEMPLATE_LIST_TEST_CASE( "push_front works correctly", "[list][modifiers]", templateTestList )
{
    SECTION( "the containers size is incremented" ) {
        ft::list<TestType> mylist;
        int size = GENERATE(range(1, 1000));
        for (int i = 0; i < size; ++i)
            mylist.push_front(TestType());
        REQUIRE( (int)mylist.size() == size );
    }

    SECTION( "the element is added at the front of the list" ) {
        ft::list<int> mylist;
        for (int i = 0; i < 100; ++i) {
            mylist.push_front(i);
            REQUIRE( *mylist.begin() == i );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "push_back works correctly", "[list][modifiers]", templateTestList )
{
    SECTION( "the containers size is incremented" ) {
        ft::list<TestType> mylist;
        int size = GENERATE(range(1, 1000));
        for (int i = 0; i < size; ++i)
            mylist.push_back(TestType());
        REQUIRE( (int)mylist.size() == size );
    }
    SECTION( "the element is added at the back of the list" ) {
        ft::list<int> mylist;
        for (int i = 0; i < 100; ++i) {
            mylist.push_back(i);
            REQUIRE( *--mylist.end() == i );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "pop_front works correctly", "[list][modifiers]", templateTestList )
{
    ft::list<int> mylist (10, 100);
    size_t i = mylist.size();
    while (!mylist.empty()) {
        REQUIRE( mylist.size() == i);
        REQUIRE( *mylist.begin() == 100);
        REQUIRE( *--mylist.end() == 100);
        mylist.pop_front();
        --i;
    }
}

TEMPLATE_LIST_TEST_CASE( "pop_back works correctly", "[list][modifiers]", templateTestList )
{
    ft::list<int> mylist (10, 100);
    size_t i = mylist.size();
    while (!mylist.empty()) {
        REQUIRE( mylist.size() == i);
        REQUIRE( *mylist.begin() == 100);
        REQUIRE( *--mylist.end() == 100);
        mylist.pop_back();
        --i;
    }
}

TEST_CASE( "insert work correctly", "[list][modifiers]" )
{
    ft::list<int> mylist;
    auto it = mylist.begin();

    SECTION( "single element insert works correctly" ) {
        mylist.insert(it, 5);           // { 5 }
        REQUIRE( mylist.size() == 1 );
        REQUIRE( *mylist.begin() == 5 );
        mylist.insert(it, 10);           // { 5, 10}
        REQUIRE( mylist.size() == 2 );
        REQUIRE( *--mylist.end() == 10 );
    }
    SECTION( "fill insert works correctly" ) {
        mylist.insert(it, 5, 10);
        REQUIRE( mylist.size() == 5 );
        for (auto ite = mylist.begin(); ite != mylist.end(); ++ite)
            REQUIRE( *ite == 10 );
        mylist.insert(it, 1, 100);
        REQUIRE( mylist.size() == 6 );
        REQUIRE( *--mylist.end() == 100 );
    }
    SECTION( "range insert works correctly" ) {
        int arr[] = {12, 1, 4, 5, 6, 7};
        mylist.push_front(-32);
        mylist.push_front(0);
        mylist.insert(--mylist.end(), arr + 1, arr + 5);
        REQUIRE( mylist.size() == 6 );
        auto it = mylist.begin();
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
TEMPLATE_LIST_TEST_CASE( "erase work correctly", "[list][modifiers]", templateTestList )
{
    size_t n = 5;
    ft::list<TestType> mylist (n, TestType());

    SECTION( "single element erase works correctly" ) {
        mylist.erase(mylist.begin());
        REQUIRE( mylist.size() == --n );
        mylist.erase(--mylist.end());
        REQUIRE( mylist.size() == --n );
        auto it = mylist.begin();
        std::advance(it, 2);
        mylist.erase(it);
        REQUIRE( mylist.size() == --n );
    }
    SECTION( "multiple elements erase works correctly" ) {
        auto first = mylist.begin();
        std::advance(first, 2);
        auto last = first;
        std::advance(last, 2);
        mylist.erase(first, last);
        REQUIRE( mylist.size() == (n - 2) );
    }
}

// add check of allocator swap ? don't have to implement get_allocator
TEST_CASE( "swap works correctly", "[list][modifiers]" )
{
    SECTION( "non-empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1 (10, 100);
        ft::list<int> mylist2 (2, -12);

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 2 );
        REQUIRE( mylist2.size() == 10 );
        for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
            REQUIRE( *it == -12 );
        for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION( "empty w/ non-empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1 (10, 100);
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 10 );
        REQUIRE( (mylist1.begin() == mylist1.end()) );
        for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
            REQUIRE( *it == 100 );
    }
    SECTION( "empty w/ empty lists swap swaps sizes and elements" ) {
        ft::list<int> mylist1;
        ft::list<int> mylist2;

        mylist1.swap(mylist2);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 0 );
        REQUIRE( (mylist1.begin() == mylist1.end()) );
        REQUIRE( (mylist2.begin() == mylist2.end()) );
    }
}

// add test to check if resize adds elements @ the end if needed
TEMPLATE_LIST_TEST_CASE( "resize works correctly", "[list][modifiers]", templateTestList )
{
    ft::list<TestType> mylist;

    SECTION( "container size is reduced when n is smaller than current size" ) {
        mylist.resize(0);
        REQUIRE( mylist.size() == 0 );
        mylist.push_front(TestType());
        mylist.push_front(TestType());
        mylist.push_front(TestType());
        mylist.push_front(TestType());
        mylist.resize(2);
        REQUIRE( mylist.size() == 2 );
        mylist.resize(1);
        REQUIRE( mylist.size() == 1 );
        mylist.resize(0);
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "container is expanded at it's end when n is greater than current size" ) {
        SECTION( "empty list" ) {
            mylist.resize(5);
            REQUIRE( mylist.size() == 5 );
        }
        SECTION( "non-empty list" ) {
            mylist.push_front(TestType());
            mylist.resize(5);
            REQUIRE( mylist.size() == 5 );
            mylist.push_front(TestType());
            mylist.resize(20);
            REQUIRE( mylist.size() == 20 );
        }
    }
}

TEMPLATE_LIST_TEST_CASE( "clear works correctly", "[list][modifiers]", templateTestList )
{
    ft::list<TestType> mylist;
    mylist.clear();
    REQUIRE( mylist.size() == 0 );
    mylist.assign(10, TestType());
    REQUIRE( mylist.size() == 10 );
    mylist.clear();
    REQUIRE( mylist.size() == 0 );
}

/* OPERATIONS */

// add test to check consistency of removed/added values
TEST_CASE( "splice work correctly", "[list][operations]" )
{
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    ft::list<int> mylist1 (v, v + 7);
    ft::list<int> mylist2;

    SECTION( "entire list splice works correctly" ) {
        mylist2.splice(mylist2.begin(), mylist1);
        REQUIRE( mylist1.size() == 0 );
        REQUIRE( mylist2.size() == 7 );
    }
    SECTION( "single element splice works correctly" ) {
        mylist2.splice(mylist2.begin(), mylist1, ++mylist1.begin());
        REQUIRE( mylist1.size() == 6 );
        REQUIRE( mylist2.size() == 1 );
    }
    SECTION( "element range splice works correctly" ) {
        auto first = mylist1.begin();
        std::advance(first, 3);
        auto last = first;
        std::advance(last, 4);

        mylist2.splice(mylist2.begin(), mylist1, first, last);
        REQUIRE( mylist1.size() == 3 );
        REQUIRE( mylist2.size() == 4 );
    }
}

// check consistency of removed elements
TEST_CASE( "remove(_if) work correctly", "[list][operations]" )
{
    int v[] = { 0, 1, 2, 3, 3, 5, 1, 7 };
    ft::list<int> mylist (v, v + 8);

    SECTION( "remove works correctly" ) {
        mylist.remove(42);
        REQUIRE( mylist.size() == 8 );
        mylist.remove(1);
        REQUIRE( mylist.size() == 6 );
        mylist.remove(3);
        REQUIRE( mylist.size() == 4 );
        mylist.remove(7);
        REQUIRE( mylist.size() == 3 );
        mylist.remove(0);
        REQUIRE( mylist.size() == 2 );
        mylist.remove(2);
        REQUIRE( mylist.size() == 1 );
        mylist.remove(5);
        REQUIRE( mylist.size() == 0 );
        mylist.remove(0);
        REQUIRE( mylist.size() == 0 );
    }
    SECTION( "remove_if works correctly" ) {
        mylist.remove_if([](int i) { return i == 42; });
        REQUIRE( mylist.size() == 8 );
        mylist.remove_if([](int i) { return i == 1; });
        REQUIRE( mylist.size() == 6 );
        mylist.remove_if([](int i) { return i == 3; });
        REQUIRE( mylist.size() == 4 );
        mylist.remove_if([](int i) { return i == 7; });
        REQUIRE( mylist.size() == 3 );
        mylist.remove_if([](int i) { return i == 0; });
        REQUIRE( mylist.size() == 2 );
        mylist.remove_if([](int i) { return i == 2; });
        REQUIRE( mylist.size() == 1 );
        mylist.remove_if([](int i) { return i == 5; });
        REQUIRE( mylist.size() == 0 );
        mylist.remove_if([](int i) { return i == 0; });
        REQUIRE( mylist.size() == 0 );
    }
}

TEST_CASE( "unique work correctly", "[list][operations]" )
{
    int a[] = { 1, 1, -66, -22, -22, -1, -22, 2, 2, 2, 9, 9, 109, 109, 109 };
    ft::list<int> mylist (a, a + sizeof(a) / sizeof(int));

    SECTION( "unique without specifying how to compare" ) {
        int unique[] = { 1, -66, -22, -1, -22, 2, 9, 109 };
        mylist.unique();
        REQUIRE( mylist.size() == (sizeof(unique) / sizeof(int)) );
        int i = 0;
        for (auto it = mylist.begin(); it != mylist.end(); ++it) {
            REQUIRE( *it == unique[i] );
            ++i;
        }
    }
    SECTION( "unique w/ specifying how to compare" ) {
        int duplicates[] = { 1, -66, -22, -22, -1, -22, 2, 9, 9, 109, 109, 109 };
        mylist.unique( [] (int i, int j) { return i + j == 2 || i + j == 4; });

        REQUIRE( mylist.size() == (sizeof(duplicates) / sizeof(int)) );
        int i = 0;
        for (auto it = mylist.begin(); it != mylist.end(); ++it) {
            REQUIRE( *it == duplicates[i] );
            ++i;
        }
    }
}

TEST_CASE( "merge work correctly", "[list][operations]" )
{
    SECTION( "merging without specifying how to compare" ) {
        int a[] = { -66, -22, -1, 9, 109 };
        int b[] = { -12, 2, 2, 4, 5, 12, 99 };
        int c[] = { -66, -22, -12, -1, 2, 2, 4, 5, 9, 12, 99, 109 };
        ft::list<int> mylist1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> mylist2 (b, b + sizeof(b) / sizeof(int));

        mylist1.merge(mylist2);
        REQUIRE( mylist2.empty() );
        int i = 0;
        for (auto it = mylist1.begin(); it != mylist1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "merging with specified comparing function" ) {
        int a[] = { 109, 9, -1, -22, -66 };
        int b[] = { 99, 12, 5, 4, 2, 2, -12 };
        int c[] = { 109, 99, 12, 9, 5, 4, 2, 2, -1, -12, -22, -66 };
        ft::list<int> mylist1 (a, a + sizeof(a) / sizeof(int));
        ft::list<int> mylist2 (b, b + sizeof(b) / sizeof(int));

        mylist1.merge(mylist2, std::greater<int>());
        REQUIRE( mylist2.empty() );
        int i = 0;
        for (auto it = mylist1.begin(); it != mylist1.end(); ++it) {
            REQUIRE( *it == c[i] );
            ++i;
        }
    }
    SECTION( "mergin w/ empty list" ) {
        ft::list<int> mylist1 (5, 10);
        ft::list<int> mylist2;
        ft::list<int> mylist3;

        mylist1.merge(mylist2);
        REQUIRE( mylist1.size() == 5 );
        mylist2.merge(mylist3);
        REQUIRE( mylist2.size() == 0 );
        REQUIRE( mylist3.size() == 0 );
    }
}

TEST_CASE( "sort work correctly", "[list][operations]" )
{
    int v[] = {2, 4, 12, 5, 2, 99, -12};
    int v_sorted[] = {-12, 2, 2, 4, 5, 12, 99};
    size_t v_size = sizeof(v) / sizeof(int);
    ft::list<int> mylist (v, v + v_size);

    SECTION( "sorting without specifying how to compare" ) {
        mylist.sort();
        REQUIRE( mylist.size() == v_size );
        int i = 0;
        for (auto it = mylist.begin(); it != mylist.end(); ++it) {
            REQUIRE( *it == v_sorted[i] );
            ++i;
        }
    }
    SECTION( "sorting with specified comparing function" ) {
        mylist.sort(std::greater<int>());
        REQUIRE( mylist.size() == v_size );
        int j = v_size;
        for (auto it = mylist.begin(); it != mylist.end(); ++it) {
            --j;
            REQUIRE( *it == v_sorted[j] );
        }
    }
    SECTION ( "sorting unfit lists" ) {
        ft::list<int> mylist1;
        ft::list<int> mylist2 (1, 0);

        mylist1.sort();
        REQUIRE( mylist1.size() == 0 );
        mylist2.sort();
        REQUIRE( mylist2.size() == 1 );
    }
}

TEST_CASE( "reverse works correctly", "[list][operations]" )
{
    int v[] = { 2, 4, 2, 5, 2, 99, -12, 1, 312 };
    ft::list<int> mylist (v, v + sizeof(v) / sizeof(int));
    mylist.reverse();
    REQUIRE( mylist.size() == (sizeof(v) / sizeof(int)) );
    int i = sizeof(v) / sizeof(int);
    for (auto it = mylist.begin(); it != mylist.end(); ++it) {
        --i;
        REQUIRE( *it == v[i] );
    }
}
