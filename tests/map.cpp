#include <map>
#include "catch.hpp"

#include "../srcs/map/map.hpp"
#include "../srcs/vector/vector.hpp"

/* CONSTRUCTION */
TEST_CASE("map contructors work as expected", "[map][basics]")
{
    SECTION("default constructor constructs an empty map") {
        ft::map<int, int> map;
        REQUIRE( map.empty() );
        REQUIRE( map.size() == 0 );
    }
    SECTION("range constructor fills with corresponding element in range") {
        std::map<char, int> stl_map;
        for (int i = 0; i < 25; ++i)
            stl_map[rand() % 93 + 33] = rand();
        ft::map<char, int> my_map (stl_map.begin(), stl_map.end());

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
        ft::map<char, int> my_map1;
        for (int i = 0; i < 25; ++i)
            my_map1[rand() % 93 + 33] = rand();
        ft::map<char, int> my_map2 (my_map1);

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
        --my_map1.end()->second = --my_map2.end()->second - 1029;
        REQUIRE( (my_map1.begin()->second != my_map2.begin()->second) );
        REQUIRE( (--my_map1.end()->second != --my_map2.end()->second) );
    }
}

TEST_CASE("Assignment operator copies elements", "[map][operators]")
{
    ft::map<char, int> my_map1;
    for (int i = 0; i < 25; ++i)
        my_map1[rand() % 93 + 33] = rand() % 10000;
    ft::map<char, int> my_map2 = my_map1;

    REQUIRE_FALSE( my_map2.empty() );
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
    REQUIRE( (my_map1.begin()->second != my_map2.begin()->second) );
    REQUIRE( ((--my_map1.end())->second != (--my_map2.end())->second) );
}

/* ITERATORS */
TEST_CASE("Iterators works correctly", "[map][iterators]")
{
    ft::map<char, int> my_map;

    ft::map<char, int>::iterator it = my_map.begin();
    ft::map<char, int>::const_iterator cit = it;
    REQUIRE( (cit == my_map.begin()) );

    ft::map<char, int>::reverse_iterator rit = my_map.rbegin();
    ft::map<char, int>::const_reverse_iterator rcit = rit;
    REQUIRE( (rcit == my_map.rbegin()) );
}

/* CAPACITY */
TEST_CASE( "max_size returns same value as std::map", "[map][capacity]")
{
    ft::map<char,int> my_map;
    std::map<char,int> stl_map;
    REQUIRE( my_map.max_size() == stl_map.max_size() );
}

/* MODIFIERS */
TEST_CASE("insert works as expected", "[map][modifiers]")
{
    ft::map<char, int> my_map;
    std::pair<ft::map<char, int>::iterator, bool> ret;
    ft::map<char, int>::iterator it_ret;

    SECTION("single element insert add the pair to the map if its not present") {
        REQUIRE( my_map.size() == 0);

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
        it_ret = my_map.insert(ite, std::make_pair('b', 'b'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'b' );
        REQUIRE( it_ret->second == 'b' );
        //valid hint but key already in map
        it_ret = my_map.insert(ite, std::make_pair('b', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'b' );
        REQUIRE( it_ret->first == 'b' );
        it_ret = my_map.insert(my_map.find('b'), std::make_pair('a', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'a' );
        REQUIRE( it_ret->first == 'a' );
        //invalid hint but key in map
        it_ret = my_map.insert(ite, std::make_pair('u', 'c'));
        REQUIRE( my_map.size() == 8 );
        REQUIRE( it_ret->first == 'u' );
        REQUIRE( it_ret->first == 'u' );
        //invalid hint but key not in map
        it_ret = my_map.insert(itf, std::make_pair('c', 'c'));
        REQUIRE( my_map.size() == 9 );
        REQUIRE( it_ret->first == 'c' );
        REQUIRE( it_ret->first == 'c' );
        it_ret = my_map.insert(ita, std::make_pair('z', 'z'));
        REQUIRE( my_map.size() == 10 );
        REQUIRE( it_ret->first == 'z' );
        REQUIRE( it_ret->first == 'z' );
    }
    SECTION("range insert inserts a copy of the ranges elements") {
        ft::vector<std::pair<char, int> > v;
        for (char i = 'a'; i < 'z'; ++i)
            v.push_back(std::make_pair(i, rand()));
        auto first = v.begin();
        first += 5;
        auto last = first + 10;
        my_map.insert(first, last);
        REQUIRE( (my_map.size() == (unsigned long)std::distance(first, last)) );
        for (auto it = my_map.begin(); it != my_map.end(); ++it) {
            REQUIRE( it->first == first->first );
            REQUIRE( it->second == first->second );
            ++first;
        }
    }
}

TEST_CASE("erase works as expected")
{
    ft::map<char, int> my_map;

    SECTION("erase at iterator") {
        my_map.erase(my_map.begin());
        my_map.erase(my_map.end());
        REQUIRE( my_map.size() == 0 );
        my_map['a'] = 1;
        my_map.erase(my_map.begin());
        REQUIRE( my_map.size() == 0 );
    }
    SECTION("erase key") {
        int ret = my_map.erase('b');
        REQUIRE( ret == 0 );
        my_map['a'] = 1;
        ret = my_map.erase('b');
        REQUIRE( ret == 0 );
        ret = my_map.erase('a');
        REQUIRE( ret == 1 );
        REQUIRE( my_map.size() == 0 );
    }
    SECTION("erase iterator range") {
        my_map.erase(my_map.end(), my_map.end());
        my_map.erase(my_map.begin(), my_map.end());
        my_map['a'] = 1;
        my_map['b'] = 2;
        my_map['z'] = 3;
        my_map['i'] = 4;
        my_map['p'] = 5;
        my_map['q'] = 6;
        auto first = my_map.begin();
        std::advance(first, 2);
        my_map.erase(first, --my_map.end());
        REQUIRE( my_map.size() == 3 );
        my_map.erase(my_map.begin(), my_map.end());
        REQUIRE( my_map.size() == 0 );
    }
}

TEST_CASE("swap works as expected")
{
}

TEST_CASE("clear works as expected")
{
    ft::map<int, int> my_map;
    my_map.clear();
    REQUIRE( my_map.size() == 0 );
    my_map['a'] = 1;
    my_map['b'] = 2;
    my_map['z'] = 3;
    my_map['i'] = 4;
    my_map.clear();
    REQUIRE( my_map.size() == 0 );
    my_map['b'] = 2;
    REQUIRE( my_map.size() == 1 );
    my_map.clear();
    REQUIRE( my_map.size() == 0 );
    REQUIRE( (my_map.begin() == my_map.end()) );
}

/* OBSERVERS */
TEST_CASE("key_comp works as expected")
{
}

TEST_CASE("value_comp works as expected")
{
}

/* OPERATIONS */
TEST_CASE("find works as expected")
{
}

TEST_CASE("count works as expected")
{
}

TEST_CASE("lower_bound works as expected")
{
}

TEST_CASE("upper_bound works as expected")
{
}

TEST_CASE("equal_range works as expected")
{
}
