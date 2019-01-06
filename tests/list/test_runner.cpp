//
// Created by Saxion ACS.
//

#include <gtest/gtest.h>
#include "../test_runner.h"

void append_after_each(std::string& str, const std::string& pattern, const std::string& to_append) {
    std::string copy(to_append);
    copy.reserve(2 * str.length());

    std::string::size_type last_pos = 0;
    std::string::size_type found_at;

    while (std::string::npos != (found_at = str.find(pattern, last_pos))) {

        copy.append(str, last_pos, found_at - last_pos + 1);
        copy += to_append;
        last_pos = found_at + pattern.length();
    }

    copy.append(str, last_pos, found_at - last_pos + 1);
    str.swap(copy);
}

int main(int argc, char** argv) {


    ::testing::InitGoogleTest(&argc, argv);

    ::testing::UnitTest& unit_test = *::testing::UnitTest::GetInstance();

    ::testing::TestEventListeners& listeners = unit_test.listeners();
//    delete listeners.Release(listeners.default_result_printer());

    // not needed now, doesn't do anything yet
    listeners.Append(new test_runner());

    auto ret_val = RUN_ALL_TESTS();


    // print detailed output diagnostics
    if (unit_test.failed_test_count()) {

        std::ostream& out = std::cout;


        out << "\nFailed " << unit_test.failed_test_count() << (unit_test.failed_test_count() == 1 ? " test" : " tests")
            << ":\n\n";

        for (auto i = 0; i < unit_test.total_test_case_count(); ++i) { // for each suite
            const auto& test_case = *unit_test.GetTestCase(i);
            for (auto j = 0; j < test_case.total_test_count(); ++j) { // for each test case in a suite
                const auto& test_info = *test_case.GetTestInfo(j);
                if (test_info.result()->Failed()) {
                    out << "[Test suite]:[Test case] => [" << test_info.test_case_name() << "]:[" << test_info.name()
                        << "]\n";
                    out << "   The following assertions failed:\n";

                    const auto& test_result = *test_info.result();

                    auto count = 1;
                    for (auto k = 0; k < test_result.total_part_count(); ++k) {

                        if (test_result.GetTestPartResult(k).failed()) {
                            const auto& part_result = test_result.GetTestPartResult(k);
                            std::string message(part_result.message());
                            append_after_each(message, "\n", "       > ");
                            out << "    " << count << ": > in '" << part_result.file_name() << ":"
                                << part_result.line_number() << "\n";

                            out << message << "\n";
                            count++;
                        }
                    }
                }
            }
        }
    }

    return ret_val;
}