#define private public // For testing purposes, make private methods public
#include "../IntersectionFinder.h"
#include "../Rectangle.h"
#undef private // Restore private access after testing
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include "test_helpers.h"

TEST_CASE("IntersectionFinder::LoadRectanglesFromFileThrowsOnSingleRectangle", "[IntersectionFinder]") {
    std::string json = R"({
        "rects": [
            {"x": 0, "y": 0, "w": 10, "h": 10}
        ]
    })";
    std::string filename = writeTempJson(json);
    IntersectionFinder finder;
    REQUIRE_THROWS_AS(finder.loadRectanglesFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("IntersectionFinder::LoadRectanglesFromFileLoadsMultipleRectangles", "[IntersectionFinder]") {
    std::string json = R"({
        "rects": [
            {"x": 0, "y": 0, "w": 10, "h": 10},
            {"x": 5, "y": 5, "w": 10, "h": 10}
        ]
    })";
    std::string filename = writeTempJson(json);
    IntersectionFinder finder;
    REQUIRE_NOTHROW(finder.loadRectanglesFromFile(filename));
    removeTempFile(filename);
}

TEST_CASE("IntersectionFinder::CreateKeySortsAndJoinsIds", "[IntersectionFinder]") {
    IntersectionFinder finder;
    std::vector<int> ids = {3, 1, 2};
    std::string key = finder.createKey(ids);
    REQUIRE(key == "1-2-3");
}

TEST_CASE("IntersectionFinder::RecordIntersectionIfUniqueWorks", "[IntersectionFinder]") {
    IntersectionFinder finder;
    Rectangle r1(1, 0, 0, 10, 10);
    std::vector<int> ids = {2, 1};
    REQUIRE(finder.recordIntersectionIfUnique(r1, ids));
    // Should not record again
    REQUIRE_FALSE(finder.recordIntersectionIfUnique(r1, ids));
}

TEST_CASE("IntersectionFinder::ProcessIntersectionsFindsCorrectIntersections", "[IntersectionFinder]") {
    std::string json = R"({
        "rects": [
            {"x": 0, "y": 0, "w": 10, "h": 10},
            {"x": 5, "y": 5, "w": 10, "h": 10}
        ]
    })";
    std::string filename = writeTempJson(json);
    IntersectionFinder finder;
    finder.loadRectanglesFromFile(filename);
    finder.processIntersections();
    bool found = false;
    for (const auto& res : finder.m_intersections) {
        if (res.parent_ids.size() == 2 &&
            ((res.parent_ids[0] == 1 && res.parent_ids[1] == 2) ||
             (res.parent_ids[0] == 2 && res.parent_ids[1] == 1))) {
            REQUIRE(res.rect.x() == 5);
            REQUIRE(res.rect.y() == 5);
            REQUIRE(res.rect.w() == 5);
            REQUIRE(res.rect.h() == 5);
            found = true;
        }
    }
    REQUIRE(found);
    removeTempFile(filename);
}

TEST_CASE("IntersectionFinder::NoIntersectionsFound", "[IntersectionFinder]") {
    std::string json = R"({
        "rects": [
            {"x": 0, "y": 0, "w": 2, "h": 2},
            {"x": 10, "y": 10, "w": 2, "h": 2}
        ]
    })";
    std::string filename = writeTempJson(json);
    IntersectionFinder finder;
    finder.loadRectanglesFromFile(filename);
    finder.processIntersections();
    REQUIRE(finder.m_intersections.empty());
    removeTempFile(filename);
}

TEST_CASE("IntersectionFinder::RecursiveIntersections", "[IntersectionFinder]") {

    std::string json = R"({
        "rects": [
            {"x": 0, "y": 0, "w": 5, "h": 5},
            {"x": 2, "y": 2, "w": 5, "h": 5},
            {"x": 2, "y": 2, "w": 1, "h": 1}
        ]
    })";
    std::string filename = writeTempJson(json);
    IntersectionFinder finder;
    finder.loadRectanglesFromFile(filename);
    finder.processIntersections();
    bool found = false;
    for (const auto& res : finder.m_intersections) {
        if (res.parent_ids.size() == 3) {
            std::vector<int> ids = res.parent_ids;
            std::sort(ids.begin(), ids.end());
            if (ids == std::vector<int>({1,2,3})) {
                REQUIRE(res.rect.x() == 2);
                REQUIRE(res.rect.y() == 2);
                REQUIRE(res.rect.w() == 1);
                REQUIRE(res.rect.h() == 1);
                found = true;
            }
        }
    }
    REQUIRE(found);
    removeTempFile(filename);
}
   