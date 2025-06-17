#include "test_helpers.h"
#include <fstream>
#include <cstdio>

// Helper to write JSON to a temp file and return its name
std::string writeTempJson(const std::string& jsonContent) {
    static int counter = 0;
    std::string filename = "temp_rectangles_" + std::to_string(counter++) + ".json";
    std::ofstream ofs(filename);
    ofs << jsonContent;
    ofs.close();
    return filename;
}

// Helper to remove temp file
void removeTempFile(const std::string& filename) {
    std::remove(filename.c_str());
}