//
// Created by Saxion ACS.
//

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <random>

#include "list.h"

namespace {
    static auto names = {"alice", "bob", "cindy", "eve", "felix", "gina", "harold", "ilse", "jack"};

    using names_type = typename decltype(names)::value_type;

    static std::vector<std::string> more_names{"kate", "lars", "mary", "nick", "ophelia"};

    // 0 points
    TEST(list_basic, should_always_pass) {
        ASSERT_TRUE(1 + 1 == 2) << "This test must always pass";
    }

    // 0 points
    TEST(list_constructors, default_ctor) {
        saxion::list<int> lst;
        ASSERT_EQ(lst.size(), 0) << "Default constructor should initialize a list with 0 elements.";
        ASSERT_TRUE(lst.empty()) << "A default constructed list should be empty";
    }

    // 0 points
    TEST(list_constructors, initializer_list_type) {
        saxion::list lsti{1.0, 2.0, 3.0};
        ASSERT_TRUE((std::is_same_v<double, decltype(lsti)::value_type>))
                                    << "Type inference from a std::initializer list failed.";

        saxion::list lsts{"alice", "bob"};
        ASSERT_TRUE((std::is_same_v<std::string, decltype(lsts)::value_type>))
                                    << "Type inference from a std::initializer list failed.";
    }

    TEST(list_constructors, initializer_list) {
        saxion::list lsti{1.0, 2.0, 3.0};
        ASSERT_EQ(lsti.size(), 3) << "Number of elements of a list initialized from std::initializer_list is wrong.";
        ASSERT_FALSE(lsti.empty()) << "This list shouldn't be empty.";
        ASSERT_FLOAT_EQ(lsti.front(), 1.0) << "The front() element should be: " << 1.0 << ".";
        ASSERT_FLOAT_EQ(lsti.back(), 3.0) << "The back() element should be: " << 3.0 << ".";
    }

    TEST(list_constructors, copy) {
        saxion::list lst(names);
        ASSERT_EQ(lst.size(), names.size());
        auto copy(lst);
        ASSERT_EQ(copy.size(), names.size());
        ASSERT_EQ(lst.size(), names.size());

        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(lst[i], copy[i]) << "unexpected item at index: " << i;
        }
    }

    TEST(list_constructors, copy_assignment) {
        saxion::list lst(names);
        ASSERT_EQ(lst.size(), names.size());
        saxion::list<decltype(lst)::value_type> copy;
        ASSERT_EQ(copy.size(), 0);
        copy = lst;
        ASSERT_EQ(copy.size(), names.size());
        ASSERT_EQ(lst.size(), names.size());

        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(lst[i], copy[i]) << "unexpected item at index: " << i;
        }
    }

    TEST(list_constructors, move) {
        saxion::list lst(names);
        ASSERT_EQ(lst.size(), names.size());

        auto moved(std::move(lst));
        ASSERT_EQ(moved.size(), names.size());
        ASSERT_EQ(lst.size(), 0);

        auto name = names.begin();
        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(moved[i], *name) << "unexpected item at index: " << i;
            ++name;
        }
    }

    TEST(list_constructors, move_assignment) {
        saxion::list lst(names);
        ASSERT_EQ(lst.size(), names.size());

        saxion::list moved{"just", "word"};
        ASSERT_EQ(moved.size(), 2);

        moved = std::move(lst);

        ASSERT_EQ(lst.size(), 0);
        ASSERT_EQ(moved.size(), names.size());

        auto name = names.begin();
        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(moved[i], *name) << "unexpected item at index: " << i;
            ++name;
        }
    }

    TEST(list_constructors, iterator_pair) {

        std::vector<std::string> v;
        for (auto n: names) v.emplace_back(n);

        saxion::list from_iter(v.begin(), v.end());

        ASSERT_EQ(from_iter.size(), names.size());

        auto name = names.begin();
        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(from_iter[i], *name) << "unexpected item at index: " << i;
            ++name;
        }
    }

    TEST(list_destructors, small_list) {

        auto list = new saxion::list<float>();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis;

        long count = 1'000'000;
        while(count--){
            list->emplace_back(dis(gen));
        }

        ASSERT_NO_THROW([&list](){ delete list; }) << "List destructor should not throw";
    }

    TEST(list_specializations, swap) {
        saxion::list lst(names);
        saxion::list<decltype(lst)::value_type> oth;

        ASSERT_EQ(oth.size(), 0);

        lst.swap(oth);

        ASSERT_EQ(lst.size(), 0);
        ASSERT_TRUE(lst.empty());

        ASSERT_EQ(oth.size(), names.size());

        auto name = names.begin();
        for (std::size_t i = 0; i < names.size(); ++i) {
            ASSERT_EQ(oth[i], *name) << "unexpected item at index: " << i;
            ++name;
        }
    }

    TEST(list_accessors, front_back) {
        saxion::list lst(names);

        ASSERT_EQ(lst.front(), *(names.begin())) << "front() should return the first element";
        ASSERT_EQ(lst.back(), *(names.end() - 1)) << "back() should return the last element";
    }

    TEST(list_accessors, at) {
        saxion::list lst(names);
        auto name = names.begin();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (names.size()-1));

        auto count = 50;
        while (count--) {
            auto ind = dis(gen);
            ASSERT_EQ(lst.at(ind), *(name + ind)) << "It looks like wrong item is returned when using at()";
        }
    }

    TEST(list_accessors, indexed) {
        saxion::list lst(names);
        auto name = names.begin();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (names.size()-1));

        auto count = 50;
        while (count--) {
            auto ind = dis(gen);
            ASSERT_EQ(lst[ind], *(name+ind)) << "It looks like wrong item is returned when using operator[]";
        }
    }

    TEST(list_accessors, at_throws) {

        saxion::list lst(names);

        ASSERT_ANY_THROW((void) lst.at(-1)) << "Accessing list element with negative index should throw.";
        ASSERT_ANY_THROW((void) lst.at(lst.size())) << "Accessing list element beyond its size should throw.";

        saxion::list<int> empty;

        ASSERT_ANY_THROW((void) empty.at(0)) << "Accessing 0-th element of an empty list should throw";
    }

    TEST(list_modifiers, push_back_cpy) {
        saxion::list lst(names);
        auto size = lst.size();

        lst.push_back(more_names.front());
        ++size;

        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.back(), more_names.front()) << "The last element of the list should be the one recently added";

        lst.push_back(more_names.back());
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.back(), more_names.back()) << "The last element of the list should be the one recently added";
        ASSERT_EQ(lst.front(), *names.begin())
                                    << "The first element of the list should be the one constructed from names";
        ASSERT_EQ(lst.at(names.size() - 1), *(names.end() - 1)) << "The last constructed element should be unchanged";
    }

    TEST(list_modifiers, push_back_mv) {
        saxion::list lst(names);
        auto size = lst.size();

        lst.push_back("frank");
        ++size;

        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.back(), "frank") << "The last element of the list should be the one recently added";

        std::string name("gia");

        lst.push_back(std::move(name));
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.back(), "gia") << "The last element of the list should be the one recently added";
        ASSERT_EQ(name.length(), 0) << "The string <name> should have been moved from";
        ASSERT_EQ(lst.front(), *names.begin())
                                    << "The first element of the list should be the one constructed from names";
        ASSERT_EQ(lst.at(names.size() - 1), *(names.end() - 1)) << "The last constructed element should be unchanged";
    }

    TEST(list_modifiers, push_front_cpy) {
        saxion::list lst(names);
        auto size = lst.size();

        lst.push_front(more_names.front());
        ++size;

        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.front(), more_names.front()) << "The first element of the list should be the one recently added";

        lst.push_front(more_names.back());
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.front(), more_names.back()) << "The first element of the list should be the one recently added";
        ASSERT_EQ(lst[1], more_names.front()) << "The second element of the list should be the one added previously";
        ASSERT_EQ(lst[2], *names.begin()) << "The third element of the list should be the one constructed from names";
        ASSERT_EQ(lst.back(), *(names.end() - 1)) << "The last element of the list should be unchanged";
    }

    TEST(list_modifiers, push_front_mv) {
        saxion::list lst(names);
        auto size = lst.size();

        lst.push_front("frank");
        ++size;

        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.front(), "frank") << "The first element of the list should be the one recently added";

        std::string name("gia");

        lst.push_front(std::move(name));
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.front(), "gia") << "The first element of the list should be the one recently added";
        ASSERT_EQ(lst[1], "frank") << "The second element of the list should be the one added previously";
        ASSERT_EQ(name.length(), 0) << "The string <name> should have been moved from";
        ASSERT_EQ(lst[2], *names.begin()) << "The third element of the list should be the one constructed from names";
        ASSERT_EQ(lst.back(), *(names.end() - 1)) << "The last element of the list should be unchanged";
    }


    TEST(list_modifiers, pop_front) {
        saxion::list lst(names);
        auto size = lst.size();

        auto names_element(names.begin());
        ++names_element;

        while (names_element != names.end()) {
            lst.pop_front();
            --size;


            ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;

            if (size) {
                ASSERT_EQ(lst.front(), *names_element) << "The first element of the list should be " << *names_element;
                ASSERT_EQ(lst.back(), *(names.end() - 1)) << "The last element of the list should be unchanged";
            }

            ++names_element;
        }
    }

    TEST(list_modifiers, pop_back) {
        saxion::list lst(names);
        auto size = lst.size();

        auto names_element(names.end() - 1);
        --names_element;

        while (size) {
            lst.pop_back();
            --size;

            ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;

            if (size) {
                ASSERT_EQ(lst.back(), *names_element) << "The last element of the list should be " << *names_element;
                ASSERT_EQ(lst.front(), *names.begin()) << "The first element of the list should be unchanged";
            }

            --names_element;
        }
    }

    TEST(list_modifiers, pop_push) {
        saxion::list lst(names);
        auto size = lst.size();

        auto names_element(names.begin());
        ++names_element;
        lst.pop_front();
        --size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;
        ASSERT_EQ(lst.front(), *names_element) << "The first element of the list should be " << *names_element;
        ASSERT_EQ(lst.back(), *(names.end() - 1)) << "The last element of the list should be unchanged";

        lst.push_back(more_names.front());
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.back(), more_names.front()) << "The last element of the list should be " << more_names.front();
        ASSERT_EQ(lst.front(), *names_element) << "The first element of the list should be " << *names_element;

        lst.push_front("frank");
        ++size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should increment and be " << size;
        ASSERT_EQ(lst.front(), "frank") << "The first element of the list should be " << "frank";
        ASSERT_EQ(lst.back(), more_names.front()) << "The last element of the list should be unchanged";

        lst.push_back(more_names.back());
        lst.push_front(more_names.front());
        lst.pop_back();
        lst.pop_front();
        ASSERT_EQ(lst.size(), size) << "The size of the list should be " << size;
        ASSERT_EQ(lst.front(), "frank") << "The first element of the list should be unchanged";
        ASSERT_EQ(lst.back(), more_names.front()) << "The last element of the list should be unchanged";

        lst.pop_front();
        --size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;
        ASSERT_EQ(lst.front(), *names_element) << "The first element of the list should be " << *names_element;
        ASSERT_EQ(lst.back(), more_names.front()) << "The last element of the list should be unchanged";

        lst.pop_back();
        --size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should be " << size;

        auto name = names.begin() + 1;
        for (std::size_t i = 0; i < lst.size(); ++i) {
            ASSERT_EQ(lst[i], *name) << "The list should be back to original (minus first el.), but isn't at element at index: "
                                     << i;
            ++name;
        }
    }

    TEST(list_modifiers, emplace) {
        saxion::list lst(names);
        auto element = lst.emplace_back(25, 'a');
        ASSERT_EQ(lst.size(), names.size() + 1) << "Size should increment";
        ASSERT_EQ(lst.back(), std::string(25, 'a')) << "The last element should be constructed in-place and equal to: "
                                                    << std::string(25, 'a');
        ++element;
        ASSERT_EQ(element, lst.end()) << "The returned iterator should point to the last element";

        element = lst.emplace(lst.begin(), 10, 'z');
        ASSERT_EQ(lst.size(), names.size() + 2) << "Size should increment";
        ASSERT_EQ(lst.front(), std::string(10, 'z')) << "The last element should be constructed in-place and equal to: "
                                                     << std::string(10, 'z');

        ASSERT_EQ(element, lst.begin()) << "The returned iterator should point to the first element";
    }


    TEST(list_iterators, iterators) {
        saxion::list lst(names);
        decltype(lst)::iterator element = lst.begin();

        ASSERT_EQ(*element, *names.begin()) << "The begin iterator should point to the first element";
        auto size(lst.size());
        while (size--) {
            ++element;
        }

        ASSERT_EQ(element, lst.end()) << "After traversing the list iterator should be equal to end";
        ASSERT_EQ(element, lst.cend()) << "It should be possible to compare const and non-const iterators";

#if 0   // disabled - conversions are difficultish
        element = decltype(lst)::iterator(lst.cend()); //just check if possible to convert
#else
        element = lst.end();
#endif
        --element;
        ASSERT_EQ(*element, lst.back()) << "iterator should point to the last element";

        element = lst.begin();
        for (auto el_names = names.begin(); el_names != names.end(); ++el_names, ++element) {
            ASSERT_EQ(*element, *el_names) << "Iterators traversal returned wrong item";
        }

        ASSERT_EQ(element, lst.end()) << "After traversing the list iterator should be equal to end";

#if 0   //disabled - non-standard treatment of reverse iteration
        // and the other way around:
        auto el_names = names.end();
        element = lst.end();
        do {
            --element;
            --el_names;
            ASSERT_EQ(*element, *el_names) << "Iterators traversal returned wrong item";

        } while (el_names != names.begin());
#endif
    }

    TEST(list_modifiers, erase) {
        saxion::list lst(names);
        auto size = lst.size();

        auto element(lst.begin());
        auto names_element(names.begin());
        element = lst.erase(element);
        --size;

        ++names_element;

        ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;
        ASSERT_EQ(lst.front(), *names_element) << "The first element of the list should be " << *names_element;

        while (element != lst.end()) {
            ASSERT_EQ(*element, *names_element) << "Elements mismatch after erasing head using iterator";
            ++element;
            ++names_element;
        }

        lst = names;
        size = lst.size();
        element = lst.begin();
        ++element;
        //removing all the insides:
        auto last_name = *(names.end() - 1);
        while (*element != last_name) {
            element = lst.erase(element);
            --size;
        }
        ASSERT_EQ(lst.size(), size) << "The size of the list should decrease and be " << size;
        ASSERT_EQ(lst.front(), *names.begin()) << "The first element of the list should be " << *names.begin();
        ASSERT_EQ(lst.back(), *(names.end() - 1)) << "The last element of the list should be " << *(names.end() - 1);

        lst = names;
        size = lst.size();
        element = lst.end();
        --element;

        element = lst.erase(element);
        --size;
        ASSERT_EQ(lst.size(), size) << "The size of the list should decrement and be " << size;
        ASSERT_EQ(element, lst.end()) << "The element iterator should point to the end()";

        --element;
        names_element = names.end() - 1;
        --names_element;
        while (element != lst.end()) {
            ASSERT_EQ(*element, *names_element) << "Elements mismatch after erasing tail using iterator";
            --element;
            --names_element;
        }
    }

    TEST(list_modifiers, insert) {
        saxion::list lst(names);
        auto pos = lst.begin();
        for (auto& name : more_names) {
            pos = lst.insert(pos, name);
            ASSERT_EQ(*pos, name) << "The returned iterator should point to inserted element";
            ++pos;
            ++pos;
        }

        ASSERT_EQ(lst.size(), names.size() + more_names.size()) << "The size of the list should be " << (names.size() + more_names.size());

        auto elst = lst.begin();
        auto enms = names.begin();
        auto emor = more_names.begin();

        while (emor != more_names.end()) {
            ASSERT_EQ(*elst, *emor) << "Inserted element mismatch";
            ++elst;
            ++emor;
            ASSERT_EQ(*elst, *enms) << "Original element mismatch";
            ++elst;
            ++enms;
        }

        while (enms != names.end()) {
            ASSERT_EQ(*elst, *enms) << "Original element mismatch";
            ++elst;
            ++enms;
        }

        pos = lst.insert(lst.end(), "zack");
        ASSERT_EQ(*pos, "zack") << "The inserted element should be zack";
        ++pos;
        ASSERT_EQ(pos, lst.end());
        std::string name("xenia");
        pos = lst.insert(pos, std::move(name));
        ASSERT_EQ(*pos, "xenia") << "The inserted element should be zack";
        ++pos;
        ASSERT_EQ(pos, lst.end());
        ASSERT_TRUE(name.empty()) << "The moved from object should be empty";

    }
}