#include "test_logging.hpp"

std::string TestLogging::s_current_test_name{};
std::vector<TestLogging::suite> TestLogging::s_suites{};