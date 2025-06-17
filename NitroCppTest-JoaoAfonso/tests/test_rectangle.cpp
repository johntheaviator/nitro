#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include "../Rectangle.h"
#include "test_helpers.h"

TEST_CASE("Rectangle::loadFromFile loads valid rectangles", "[RectangleLoadFromFile]") {
    std::string json = R"({
        "rects": [
            {"x": 1, "y": 2, "w": 3, "h": 4},
            {"x": 5, "y": 6, "w": 7, "h": 8}
        ]
    })";
    std::string filename = writeTempJson(json);

    auto rects = Rectangle::loadFromFile(filename);
    REQUIRE(rects.size() == 2);
    CHECK(rects[0].x() == 1);
    CHECK(rects[0].y() == 2);
    CHECK(rects[0].w() == 3);
    CHECK(rects[0].h() == 4);
    CHECK(rects[1].x() == 5);
    CHECK(rects[1].y() == 6);
    CHECK(rects[1].w() == 7);
    CHECK(rects[1].h() == 8);

    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile throws if file not found", "[RectangleLoadFromFile]") {
    REQUIRE_THROWS_AS(Rectangle::loadFromFile("nonexistent_file.json"), std::runtime_error);
}

TEST_CASE("Rectangle::loadFromFile throws if invalid JSON", "[RectangleLoadFromFile]") {
    std::string filename = writeTempJson("{ invalid json ");
    REQUIRE_THROWS_AS(Rectangle::loadFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile throws if no rects array", "[RectangleLoadFromFile]") {
    std::string filename = writeTempJson(R"({"foo": []})");
    REQUIRE_THROWS_AS(Rectangle::loadFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile throws if missing keys", "[RectangleLoadFromFile]") {
    std::string json = R"({
        "rects": [
            {"x": 1, "y": 2, "w": 3}
        ]
    })";
    std::string filename = writeTempJson(json);
    REQUIRE_THROWS_AS(Rectangle::loadFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile throws if extra keys", "[RectangleLoadFromFile]") {
    std::string json = R"({
        "rects": [
            {"x": 1, "y": 2, "w": 3, "h": 4, "foo": 5}
        ]
    })";
    std::string filename = writeTempJson(json);
    REQUIRE_THROWS_AS(Rectangle::loadFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile throws if negative dimensions", "[RectangleLoadFromFile]") {
    std::string json = R"({
        "rects": [
            {"x": 1, "y": 2, "w": -3, "h": 4}
        ]
    })";
    std::string filename = writeTempJson(json);
    REQUIRE_THROWS_AS(Rectangle::loadFromFile(filename), std::runtime_error);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile ignores zero width or height", "[RectangleLoadFromFile]") {
    std::string json = R"({
        "rects": [
            {"x": 1, "y": 2, "w": 0, "h": 4},
            {"x": 2, "y": 3, "w": 3, "h": 0},
            {"x": 3, "y": 4, "w": 5, "h": 6}
        ]
    })";
    std::string filename = writeTempJson(json);
    auto rects = Rectangle::loadFromFile(filename);
    REQUIRE(rects.size() == 1);
    CHECK(rects[0].x() == 3);
    CHECK(rects[0].y() == 4);
    CHECK(rects[0].w() == 5);
    CHECK(rects[0].h() == 6);
    removeTempFile(filename);
}

TEST_CASE("Rectangle::loadFromFile respects MAX_RECTANGLES", "[RectangleLoadFromFile]") {
    std::string json = R"({"rects":[)";
    for (int i = 0; i < MAX_RECTANGLES + 5; ++i) {
        json += "{\"x\":0,\"y\":0,\"w\":1,\"h\":1}";
        if (i != MAX_RECTANGLES + 4) json += ",";
    }
    json += "]}\n";
    std::string filename = writeTempJson(json);
    auto rects = Rectangle::loadFromFile(filename);
    REQUIRE(rects.size() == MAX_RECTANGLES);
    removeTempFile(filename);
}


TEST_CASE("Rectangle::Rectangle intersection", "[rectangle]") {
    Rectangle r1(1, 0, 0, 10, 10);
    Rectangle r2(2, 5, 5, 10, 10);
    Rectangle result(-1, 0, 0, 0, 0);

    bool intersects = Rectangle::calculate_intersection(r1, r2, result);

    REQUIRE(intersects == true);
    REQUIRE(result.x() == 5);
    REQUIRE(result.y() == 5);
    REQUIRE(result.w() == 5);
    REQUIRE(result.h() == 5);
}

TEST_CASE("Rectangle::No intersection", "[rectangle]") {
    Rectangle r1(1, 0, 0, 5, 5);
    Rectangle r2(2, 10, 10, 5, 5);
    Rectangle result(-1, 0, 0, 0, 0);

    bool intersects = Rectangle::calculate_intersection(r1, r2, result);

    REQUIRE(intersects == false);
}