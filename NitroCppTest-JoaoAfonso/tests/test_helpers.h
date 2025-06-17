#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP
#include <string>

// Writes JSON content to a temp file and returns its filename
std::string writeTempJson(const std::string& jsonContent);

// Removes a temp file by filename
void removeTempFile(const std::string& filename);

#endif // TEST_HELPERS_HPP