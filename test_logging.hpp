#pragma once

#include <cstdio>
#include <string>
#include <vector>

class TestLogging {
   public:
    struct test_result {
        std::string test_name{};
        std::string message{};
        bool passed;

        test_result()
            : passed(true) {}

        test_result(const std::string& message_, bool passed_ = false)
            : message(message_), passed(passed_) {}
    };

    struct suite {
        std::string name{};
        int tests_passed{}, tests_failed{};
        std::vector<test_result> fails;
    };

    static std::vector<suite> s_suites;
    static std::string s_current_test_name;

    static void start_suite(std::string name) {
        s_suites.push_back({});

        s_suites.back().name = name;
    }

    static void test_start(const std::string& name) {
        s_current_test_name = name;
        printf("\n\n%s::%s \033[33mSTART\033[0m\n", s_suites.back().name.c_str(), s_current_test_name.c_str());
    }

    static void test_pass() {
        s_suites.back().tests_passed++;
        printf("\n%s::%s \033[32mPASSED\033[0m\n", s_suites.back().name.c_str(), s_current_test_name.c_str());
    }

    static void test_failed(test_result result) {
        s_suites.back().tests_failed++;
        s_suites.back().fails.push_back(result);
        printf("\n%s::%s \033[31mFAILED [%s]\033[0m\n", s_suites.back().name.c_str(), s_current_test_name.c_str(), result.message.c_str());
    }

// NOTE: These are convenience functions for a test framework, secure formats strings are ignored here
//       This is clang specific, as gcc seems to not warn about it
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

    template <typename... Args>
    static void test_printf(Args... args) {
        printf("\t");
        printf(args...);
        printf("\n");
    }

    template <typename... Args>
    static void test_printf_start(Args... args) {
        printf("\t");
        printf(args...);
    }

    template <typename... Args>
    static void test_printf_append(Args... args) {
        printf(args...);
    }

    template <typename... Args>
    static void test_printf_end(Args... args) {
        printf(args...);
        printf("\n");
    }

#pragma clang diagnostic pop

    static int results() {
        bool failed = false;
        ;
        for (auto& suite : s_suites) {
            if (suite.tests_failed > 0) failed = true;

            printf("\n\nsummary for %s: %s (%d/%d passed; %d/%d failed)\n", suite.name.c_str(), suite.tests_failed == 0 ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m", suite.tests_passed, suite.tests_passed + suite.tests_failed, suite.tests_failed, suite.tests_passed + suite.tests_failed);
            for (auto& fail_reason : suite.fails)
                printf("\t\033[31m%s::%s failed\033[0m - reason: %s\n", suite.name.c_str(), fail_reason.test_name.c_str(), fail_reason.message.c_str());
        }

        return failed ? 1 : 0;
    }

    template <typename T, typename... Args>
    static void run(std::string name, T executor, Args... args) {
        test_start(name);
        test_result res = executor(args...);

        if (res.passed) {
            test_pass();
        } else {
            res.test_name = s_current_test_name;
            test_failed(res);
        }
    }
};
