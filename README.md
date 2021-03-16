The ft_containers project aims at introducing students to C++'s Standard Template Library. 
Along the way we will encounter new data structures (bst, deque, stack..) and new concepts (iterators, allocators...).

### Rules
This project should be C++98 compliant, we cannot use external libraries or C++11/17/20.
Since we are to reimplement the STL containers we are not allowed to use them.
[cplusplus](https://www.cplusplus.com) is used as reference as to what methods/functionalities have to be implemented.
The containers iterator interfaces have to be implemented.

#### Mandatory
* list: doubly linked list
* vector: dynamic array
* stack: lifo adaptative container
* queue: fifo adaptative container
* map: associative ordered container without duplicates

#### Bonus
* multimap: associative ordered container with duplicates
* multiset: ordered container with duplicates
* set: ordered container without duplicates
* deque: double ended queue

### Unit tests
Proving your containers work as expected is part of the project.

The unit tests provided in this repository should only test defined behaviour.
To use my tests on your own project compile `tests/main.cpp` and the container test file or take a look at the Makefile.
Using the `coverage` recipe you can also generate an html coverage report to detect bugs, unused code aswell as check the unit tests coverage.
Last but not least the `verify-tests` recipe compiles the unit tests with the STL containers to prove their correctness.

Unfortunately branch coverage is broken if you don't compile with `-fno-exceptions` and comment out lines which use exception functionality.
If you did not finish the tested container you will need to comment out sections which test methods you did not yet implement.

### Resources
* [general guide](https://medium.com/@vgasparyan1995/how-to-write-an-stl-compatible-container-fc5b994462c6)
* [const](https://quuxplusone.github.io/blog/2019/01/03/const-is-a-contract/)
* [enable_if](https://stackoverflow.com/questions/13401716/selecting-a-member-function-using-different-enable-if-conditions)
* [const_iterators](https://web.archive.org/web/20130820212103/https://www.drdobbs.com/the-standard-librarian-defining-iterato/184401331)
* [data structures](https://www.youtube.com/playlist?list=PLpPXw4zFa0uKKhaSz87IowJnOTzh9tiBk)
* [deque](https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl)

as well as countless tutorials, so posts and git repositories!
