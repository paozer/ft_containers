#include "../srcs/map/map.hpp"
#include "catch.hpp"
#include <map>
#include <array>
#include <list>
#include <string>

#ifdef VERIFY_UNIT_TESTS
# define LIB std
#else
# define LIB ft
#endif

/* CONSTRUCTION */
TEST_CASE("map contructors work as expected", "[map][basics]")
{
    SECTION("default constructor constructs an empty map") {
        LIB::map<int, int> map;
        REQUIRE( map.empty() );
        REQUIRE( map.size() == 0 );
    }
    SECTION("range constructor fills with corresponding element in range") {
        std::map<char, int> stl_map;
        for (int i = 0; i < 25; ++i)
            stl_map[rand() % 93 + 33] = rand();
        LIB::map<char, int> my_map (stl_map.begin(), stl_map.end());

        REQUIRE( stl_map.size() == my_map.size() );
        REQUIRE_FALSE( my_map.empty() );

        auto stl_it = stl_map.begin();
        auto my_it = my_map.begin();
        while (stl_it != stl_map.end()) {
            REQUIRE( *my_it == *stl_it );
            ++my_it;
            ++stl_it;
        }
    }
    SECTION("copy constructor constructs a copy with a copy of each element") {
        LIB::map<char, int> my_map1;
        for (int i = 0; i < 25; ++i)
            my_map1[rand() % 93 + 33] = rand();
        LIB::map<char, int> my_map2 (my_map1);

        REQUIRE_FALSE( my_map2.empty() );
        REQUIRE( my_map2.size() == my_map1.size() );

        auto it1 = my_map1.begin();
        auto it2 = my_map2.begin();
        while (it1 != my_map1.end()) {
            REQUIRE( *it1 == *it2 );
            ++it1;
            ++it2;
        }
        my_map1.begin()->second = my_map2.begin()->second - 10;
        (--my_map1.end())->second = (--my_map2.end())->second - 1029;
        REQUIRE(( my_map1.begin()->second != my_map2.begin()->second ));
        REQUIRE(( (--my_map1.end())->second != (--my_map2.end())->second ));
    }
}

TEST_CASE("Assignment operator copies elements", "[map][operators]")
{
    LIB::map<char, int> my_map1;
    for (int i = 0; i < 10; ++i)
        my_map1[rand() % 93 + 33] = rand() % 10000;
    LIB::map<char, int> my_map2 = my_map1;

    REQUIRE( my_map2.size() == my_map1.size() );

    auto it1 = my_map1.begin();
    auto it2 = my_map2.begin();
    while (it1 != my_map1.end()) {
        REQUIRE( *it1 == *it2 );
        ++it1;
        ++it2;
    }

    my_map1.begin()->second -= 1;
    (--my_map1.end())->second += 1;
    REQUIRE(( my_map1.begin()->second != my_map2.begin()->second ));
    REQUIRE(( (--my_map1.end())->second != (--my_map2.end())->second ));
}

TEST_CASE("there are no duplicates in a map", "[map][basics]")
{
    LIB::map<int, int> map;
    std::list<int> list;

    for (int i = 0; i < 10000; ++i) {
        int rand = std::rand() % 5000;
        list.push_back(rand);
        map.insert(std::make_pair(rand, i));
    }
    list.sort();
    list.unique();
    REQUIRE( map.size() == list.size() );
    REQUIRE( (unsigned long)std::distance(map.begin(), map.end()) == map.size() );
}

/* RELATIONAL OPERATORS */
TEST_CASE("map relational operators work correctly", "[map][operators]")
{
    std::array<std::pair<char, int>, 4> a = {{ {'b', 2}, {'z', 40}, {'p', 1}, {'a', 680} }};
    std::array<std::pair<char, int>, 4> b = {{ {'b', 1}, {'z', 10}, {'c', 1}, {'z', 680} }};
    LIB::map<char, int> mymap1 (a.begin(), a.end());
    LIB::map<char, int> mymap2 (a.begin(), a.begin() + 2);
    LIB::map<char, int> mymap3;
    LIB::map<char, int> mymap4 (b.begin(), b.end());

    // mymap1 vs mymap2
    REQUIRE_FALSE( mymap1 == mymap2 );
    REQUIRE( mymap1 != mymap2 );
    REQUIRE( mymap1 < mymap2 );
    REQUIRE( mymap1 <= mymap2 );
    REQUIRE_FALSE( mymap1 > mymap2 );
    REQUIRE_FALSE( mymap1 >= mymap2 );

    // mymap1 vs mymap1
    REQUIRE( mymap1 == mymap1 );
    REQUIRE_FALSE( mymap1 != mymap1 );
    REQUIRE_FALSE( mymap1 < mymap1 );
    REQUIRE( mymap1 <= mymap1 );
    REQUIRE_FALSE( mymap1 > mymap1 );
    REQUIRE( mymap1 >= mymap1 );

    // mymap1 vs mymap3
    REQUIRE_FALSE( mymap1 == mymap3 );
    REQUIRE( mymap1 != mymap3 );
    REQUIRE_FALSE( mymap1 < mymap3 );
    REQUIRE_FALSE( mymap1 <= mymap3 );
    REQUIRE( mymap1 > mymap3 );
    REQUIRE( mymap1 >= mymap3 );

    // mymap3 vs mymap3
    REQUIRE( mymap3 == mymap3 );
    REQUIRE_FALSE( mymap3 != mymap3 );
    REQUIRE_FALSE( mymap3 < mymap3 );
    REQUIRE( mymap3 <= mymap3 );
    REQUIRE_FALSE( mymap3 > mymap3 );
    REQUIRE( mymap3 >= mymap3 );

    // mymap1 vs mymap4
    REQUIRE_FALSE( mymap1 == mymap4 );
    REQUIRE( mymap1 != mymap4 );
    REQUIRE( mymap1 < mymap4 );
    REQUIRE( mymap1 <= mymap4 );
    REQUIRE_FALSE( mymap1 > mymap4 );
    REQUIRE_FALSE( mymap1 >= mymap4 );
}

/* ITERATORS */
TEST_CASE("Iterators works correctly", "[map][iterators]")
{
    SECTION("iterators can be created as expected") {
        LIB::map<char, int> my_map;

        LIB::map<char, int>::iterator it = my_map.begin();
        LIB::map<char, int>::const_iterator cit = it;
        LIB::map<char, int>::const_iterator cit2 = my_map.begin();
        REQUIRE(( cit == my_map.begin() ));
        REQUIRE(( cit2 == my_map.begin() ));

        LIB::map<char, int>::reverse_iterator rit = my_map.rbegin();
        LIB::map<char, int>::const_reverse_iterator rcit = rit;
        LIB::map<char, int>::const_reverse_iterator rcit2 = my_map.rbegin();
        REQUIRE(( rcit == my_map.rbegin() ));
        REQUIRE(( rcit2 == my_map.rbegin() ));
    }

    SECTION("iterators allow in-order access to the map elements") {
        int rand = 0;
        LIB::map<int, char> map;
        std::list<int> list;

        for (int i = 0; i < 100; ++i) {
            rand = std::rand() % 1000;
            list.push_back(rand);
            map.insert(std::make_pair(rand, std::rand()));
        }

        list.sort();
        list.unique();
        REQUIRE( std::distance(map.begin(), map.end()) == std::distance(list.begin(), list.end()) );
        auto it = map.begin();
        auto lit = list.begin();
        for (; it != map.end(); ++it, ++lit)
            REQUIRE( it->first == *lit );
    }
}

/* MODIFIERS */
TEST_CASE("insert works as expected", "[map][modifiers]")
{
    LIB::map<char, int> my_map;
    std::pair<LIB::map<char, int>::iterator, bool> ret;
    LIB::map<char, int>::iterator it_ret;

    SECTION("single element insert add the pair to the map if its not present") {
        REQUIRE( my_map.empty() );

        ret = my_map.insert(std::make_pair('g', 32));
        REQUIRE( my_map.size() == 1);
        REQUIRE( ret.first->first == 'g' );
        REQUIRE( ret.first->second == 32 );
        REQUIRE( ret.second );

        ret = my_map.insert(std::make_pair('g', 32));
        REQUIRE( my_map.size() == 1);
        REQUIRE( ret.first->first == 'g' );
        REQUIRE( ret.first->second == 32 );
        REQUIRE_FALSE( ret.second );

        ret = my_map.insert(std::make_pair('f', 21));
        REQUIRE( my_map.size() == 2);
        REQUIRE( ret.first->first == 'f' );
        REQUIRE( ret.first->second == 21 );
        REQUIRE( ret.second );
    }
    SECTION("position hint insert adds the pair regardless of the validity of hint") {
        my_map['a'] = 'a';
        my_map['e'] = 'e';
        my_map['f'] = 'f';
        my_map['h'] = 'h';
        my_map['i'] = 'i';
        my_map['s'] = 's';
        my_map['u'] = 'u';
        REQUIRE( my_map.size() == 7 );

        auto ita = my_map.find('a');
        auto ite = my_map.find('e');
        auto itf = my_map.find('f');

        //valid hint and key not in map
        it_ret = my_map.insert(ita, std::make_pair('b', 'b'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'b' );
        REQUIRE( it_ret->second == 'b' );
        //valid hint but key already in map
        it_ret = my_map.insert(ita, std::make_pair('b', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'b' );
        REQUIRE( it_ret->second == 'b' );
        it_ret = my_map.insert(my_map.find('b'), std::make_pair('e', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'e' );
        REQUIRE( it_ret->second == 'e' );
        //invalid hint but key in map
        it_ret = my_map.insert(ite, std::make_pair('u', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'u' );
        REQUIRE( it_ret->second == 'u' );
        //invalid hint but key not in map
        it_ret = my_map.insert(itf, std::make_pair('c', 'c'));
        REQUIRE( my_map.size() == 9 );
        REQUIRE( it_ret->first == 'c' );
        REQUIRE( it_ret->second == 'c' );
        it_ret = my_map.insert(ita, std::make_pair('z', 'z'));
        REQUIRE( my_map.size() == 10 );
        REQUIRE( it_ret->first == 'z' );
        REQUIRE( it_ret->second == 'z' );

        //assert content is the sum of inserts
        std::array< std::pair<const char, int>, 10 > arr = {{{'a','a'},{'b','b'},{'c','c'},{'e','e'},{'f','f'},{'h','h'},{'i','i'},{'s','s'},{'u','u'},{'z','z'}}};
        auto mapit = my_map.begin();
        auto arrit = arr.begin();
        while (mapit != my_map.end()) {
            REQUIRE( *mapit == *arrit );
            ++mapit;
            ++arrit;
        }
    }
    SECTION("range insert inserts a copy of the ranges elements") {
        std::vector<std::pair<char, int> > v;
        for (char i = 'a'; i < 'z'; ++i)
            v.push_back(std::make_pair(i, rand()));
        auto first = v.begin() + 5;
        auto last = v.begin() + 15;
        my_map.insert(first, last);
        REQUIRE( (my_map.size() == (unsigned long)std::distance(first, last)) );
        for (auto it = my_map.begin(); it != my_map.end(); ++it) {
            REQUIRE( it->first == first->first );
            REQUIRE( it->second == first->second );
            ++first;
        }
    }
}

TEST_CASE("erase works as expected", "[map][modifiers]")
{
    std::list<std::pair<const char, int> > data = {{{'a',1}, {'z', 25}, {'b',2}, {'c', 3}, {'d', 4}, {'r', 1}, {'e', 5}}};
    LIB::map<char, int> my_map (data.begin(), data.end());
    data.sort();

    SECTION("erase at iterator removes correct element") {
        my_map.erase(my_map.begin());
        my_map.erase(++++my_map.begin());
        my_map.erase(--my_map.end());
        my_map.erase(++++++my_map.begin());
        data.erase(data.begin());
        data.erase(++++data.begin());
        data.erase(--data.end());
        data.erase(++++++data.begin());
        REQUIRE( my_map.size() == data.size() );
        auto it = my_map.begin();
        auto lit = data.begin();
        for (; it != my_map.end(); ++it, ++lit)
            REQUIRE( *it == *lit );
    }
    SECTION("erase removes correct element when given a key") {
        int sum = 0;
        int ret = 0;
        ret = my_map.erase('b');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_map.erase('b');
        REQUIRE( ret == 0 );
        ret = my_map.erase('a');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_map.erase('z');
        REQUIRE( ret == 1 );
        sum += ret;
        ret = my_map.erase('z');
        REQUIRE( ret == 0 );
        ret = my_map.erase('/');
        REQUIRE( ret == 0 );
        ret = my_map.erase('$');
        REQUIRE( ret == 0 );
        REQUIRE( my_map.size() == data.size() - sum );
    }
    SECTION("erase iterator range") {
        my_map.erase(my_map.end(), my_map.end());
        my_map.erase(++++my_map.begin(), --my_map.end());
        data.erase(++++data.begin(), --data.end());
        REQUIRE( my_map.size() == 3 );
        REQUIRE( my_map.size() == data.size() );
        auto it = my_map.begin();
        auto lit = data.begin();
        for (; it != my_map.end(); ++it, ++lit)
            REQUIRE( *it == *lit );
        my_map.erase(my_map.begin(), my_map.end());
        REQUIRE( my_map.size() == 0 );
    }
}

TEST_CASE("swap works as expected", "[map][modifiers]")
{
    LIB::map<int, std::string> my_map1;
    LIB::map<int, std::string> my_map2;

    my_map1[14] = "fifteen";
    my_map1[19] = "dog";
    my_map1[132] = "78";
    my_map1[-22] = "34";
    my_map1[10] = "autobahn";
    my_map2.swap(my_map1);
    REQUIRE( my_map1.size() == 0 );
    REQUIRE( my_map2.size() == 5 );
    REQUIRE( my_map2[14] == "fifteen" );
    REQUIRE( my_map2[19] == "dog" );
    REQUIRE( my_map2[132] == "78" );
    REQUIRE( my_map2[-22] == "34" );
    REQUIRE( my_map2[10] == "autobahn" );
    my_map2.erase(++my_map2.begin(), my_map2.end());
    my_map2.swap(my_map1);
    REQUIRE( my_map1.size() == 1 );
    REQUIRE( my_map2.size() == 0 );
}

TEST_CASE("clear works as expected", "[map][modifiers]")
{
    LIB::map<int, int> my_map;
    my_map.clear();
    REQUIRE( my_map.empty() );
    my_map['a'] = 1;
    my_map['b'] = 2;
    my_map['z'] = 3;
    my_map['i'] = 4;
    REQUIRE( my_map.size() == 4 );
    my_map.clear();
    REQUIRE( my_map.empty() );
    my_map['b'] = 2;
    REQUIRE( my_map.size() == 1 );
    my_map.clear();
    REQUIRE( my_map.empty() );
    REQUIRE(( my_map.begin() == my_map.end() ));
}

/* OBSERVERS */
TEST_CASE("value_comp & key_comp work as expected", "[map][observers]")
{
    LIB::map<char, int> mymap;
    LIB::map<char, int>::key_compare kc = mymap.key_comp();
    LIB::map<char, int>::value_compare vc = mymap.value_comp();

    mymap['a'] = 20;
    mymap['d'] = 20;

    REQUIRE( kc(mymap.begin()->first, (++mymap.begin())->first) );
    REQUIRE_FALSE( kc((++mymap.begin())->first, mymap.begin()->first) );
    REQUIRE_FALSE( kc(mymap.begin()->first, mymap.begin()->first) );
    REQUIRE_FALSE( kc(mymap.begin()->first, (----mymap.end())->first) );
    REQUIRE_FALSE( kc((----mymap.end())->first, mymap.begin()->first) );

    REQUIRE( vc(*mymap.begin(), *++mymap.begin()) );
    REQUIRE_FALSE( vc(*++mymap.begin(), *mymap.begin()) );
    REQUIRE_FALSE( vc(*mymap.begin(), *mymap.begin()) );
    REQUIRE_FALSE( vc(*mymap.begin(), *(----mymap.end())) );
    REQUIRE_FALSE( vc(*(----mymap.end()), *mymap.begin()) );
}

/* OPERATIONS */
TEST_CASE("find works as expected", "[map][operations]")
{
    LIB::map<char, int> mymap;
    mymap['a'] = 20;
    mymap['d'] = 80;

    REQUIRE( mymap.find('a')->first == 'a' );
    REQUIRE( mymap.find('a')->second == 20 );
    REQUIRE( mymap.find('d')->first == 'd' );
    REQUIRE( mymap.find('d')->second == 80 );
    REQUIRE(( mymap.find('f') == mymap.end() ));
    mymap.erase('a');
    REQUIRE(( mymap.find('a') == mymap.end() ));
}

TEST_CASE("count works as expected", "[map][operations]")
{
    LIB::map<char, int> mymap;
    mymap['a'] = 20;
    mymap['d'] = 80;
    REQUIRE( mymap.count('a') == 1 );
    REQUIRE( mymap.count('d') == 1 );
    REQUIRE( mymap.count('c') == 0 );
    REQUIRE( mymap.count('p') == 0 );
    mymap.erase('a');
    REQUIRE( mymap.count('a') == 0 );
}

TEST_CASE("lower_bound & upper_bound work as expected", "[map][operations]")
{
    LIB::map<char, int> mymap;

    SECTION("reference example") {
        mymap['a'] = 20;
        mymap['b'] = 40;
        mymap['c'] = 60;
        mymap['d'] = 80;
        mymap['e'] = 100;
        LIB::map<char, int>::iterator itlow = mymap.lower_bound ('b');
        LIB::map<char, int>::iterator itup = mymap.upper_bound ('d');
        REQUIRE( itlow->first == 'b' );
        REQUIRE( itlow->second == 40 );
        REQUIRE( itup->first == 'e' );
        REQUIRE( itup->second == 100 );
        mymap.erase(itlow, itup);
        REQUIRE( mymap.size() == 2 );
        REQUIRE( mymap.begin()->first == 'a' );
        REQUIRE( mymap.begin()->second == 20 );
        REQUIRE( (--mymap.end())->first == 'e' );
        REQUIRE( (--mymap.end())->second == 100 );
        REQUIRE( (mymap.upper_bound('z') == mymap.end()) );
    }
    SECTION("") {
        mymap[1] = 10;
        mymap[2] = 20;
        mymap[4] = 40;
        mymap[5] = 50;
        REQUIRE(( mymap.lower_bound(2)->first == 2 ));
        REQUIRE(( mymap.lower_bound(6) == mymap.end() ));
        REQUIRE(( mymap.upper_bound(3) == mymap.lower_bound(3) ));
        REQUIRE(( mymap.upper_bound(2)->first == 4 ));
    }
    SECTION("const versions") {
        mymap['a'] = 20;
        mymap['b'] = 40;
        mymap['d'] = 80;
        mymap['e'] = 100;
        mymap['f'] = 60;
        const LIB::map<char, int> cmymap (mymap);
        REQUIRE(( cmymap.upper_bound('c') == cmymap.lower_bound('c') ));
        REQUIRE(( cmymap.upper_bound('a')->first == 'b' ));
        REQUIRE(( cmymap.upper_bound('b')->first == 'd' ));
        REQUIRE(( cmymap.upper_bound('z') == cmymap.end() ));
        REQUIRE(( cmymap.lower_bound('d')->first == 'd' ));
        REQUIRE(( cmymap.lower_bound('z') == cmymap.end() ));
    }
}

TEST_CASE("equal_range works as expected", "[map][operations]")
{
    LIB::map<char, int> mymap;
    mymap['a'] = 10;
    mymap['b'] = 20;
    mymap['c'] = 30;
    const LIB::map<char, int> cmymap (mymap);

    std::pair<LIB::map<char, int>::iterator, LIB::map<char, int>::iterator> ret = mymap.equal_range('b');
    REQUIRE( ret.first->first == 'b' );
    REQUIRE( ret.second->first == 'c' );
    REQUIRE( ret.first->second == 20 );
    REQUIRE( ret.second->second == 30 );
    REQUIRE(( mymap.equal_range('c').first->first == 'c' ));
    REQUIRE(( mymap.equal_range('c').first->second == 30 ));
    REQUIRE(( mymap.equal_range('c').second == mymap.end() ));
    REQUIRE(( mymap.equal_range('u').first == mymap.end() ));
    REQUIRE(( mymap.equal_range('u').second == mymap.end() ));

    std::pair<LIB::map<char, int>::const_iterator, LIB::map<char, int>::const_iterator> cret = cmymap.equal_range('c');

    REQUIRE( cret.first->first == 'c' );
    REQUIRE( cret.first->second == 30 );
    REQUIRE( cret.second == cmymap.end() );
    REQUIRE(( cmymap.equal_range('a').first->first == 'a' ));
    REQUIRE(( cmymap.equal_range('a').first->second == 10 ));
    REQUIRE(( cmymap.equal_range('u').first == cmymap.end() ));
    REQUIRE(( cmymap.equal_range('u').second == cmymap.end() ));
}
