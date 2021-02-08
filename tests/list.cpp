#include "catch.hpp"
#include "../srcs/list/list.hpp"

TEST_CASE( "this is my first test", "[list]" )
{
    ft::list<int> mylist;
    REQUIRE( mylist.size() == 0 );
}
