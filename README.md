# Assignment 1 - Lists

## Task

In this assignment you will program a doubly-linked list (with sentinel node) data structures. This class must be programmed as class templates and follow the C++ STL convention of accessing and manipulating elements. Most of the boilerplate code is already written and resides in [src/include/list.h](src/include/list.h).


## Instructions

To start this assignment do the following:

1. Clone the repository to your own GitHub account (or download it).

1. Create a new CLion project by checking out the repository from GitHub.

1. After creating the project, go to the project directory in terminal and execute the following two git commands:

    ~~~bash
    git submodule init
    git submodule update
    ~~~
    
    This will pull googletest repository as a submodule and add it to your project.
    
1. Reload the CMake project in CLion (Tools->CMake->Reload...)

1. Try to run the `tests_singly` target - most likely all the tests will pass. This target tests the singly-linked list implementation in [src/include/forward_list.h](src/include/forward_list.h).

1. Now try to run 'tests_doubly' target - you will observe that all but 3 tests fail. What's worse the test itself will crash. Your goal is to fix the implementation in [src/include/list.h](src/include/list.h), so that as many tests as possible pass.

1. You get one point for each fixed test to a maximum of 20 points. There are 26 tests in total, but the three already pass, so you have 23 chances to get a point.

1. There is already an implementation of the singly-linked list. Use it. Many functions of the doubly-linked list look very similar to their singly-linked siblings. 

1. The places where you should write or change some code are conveniently marked with `todo` tags, there are also short descriptions in those places.

1. You will notice that in many functions there are lines like: `(void)other;    //remove this one!`. You can safely remove those after you fixed a function. Those weird casts to void silence a compiler warning about an unused parameter.

That's all!
