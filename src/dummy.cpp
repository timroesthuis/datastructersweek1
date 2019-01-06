//
// Created by Saxion ACS.
//

#include <iostream>
#include "list.h"
#include "forward_list.h"

int main() {
    [[maybe_unused]] saxion::list<int> lst;
    [[maybe_unused]] saxion::forward_list<std::string> flst;
    std::cout << "dummy created and doing well: ";
    std::cout << lst.size();
    std::cout << " ";
    std::cout << flst.size();

    std::string str(25, 'a');
    std::cout << str;

    flst.emplace_back(25, 'b');
    std::cout << "\n" << flst.front();
}