#include "Rectangle.h"
#include <algorithm>
#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

Rectangle::Rectangle(int id, int x, int y, int w, int h):   m_id(id), 
                                                            m_x(x), 
                                                            m_y(y), 
                                                            m_w(w), 
                                                            m_h(h) {}

Rectangle::~Rectangle(){}

/* Loads and validates data from JSON file*/
std::vector<Rectangle> Rectangle::loadFromFile(const std::string& filename) 
{
    int id_counter = 1;
    std::ifstream file_stream(filename);
    std::vector<Rectangle> rectangles;

    if (!file_stream.is_open()) 
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    json data;
    try 
    {
        data = json::parse(file_stream);
    } 
    catch (const json::parse_error& e) 
    {
        throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
    }

    if (!data.contains("rects") || !data["rects"].is_array()) 
    {
        throw std::runtime_error("JSON file must contain a 'rects' array.");
    }

    for (const auto& item : data["rects"]) 
    {
        if (id_counter > MAX_RECTANGLES) 
        {
            std::cout << "Info: JSON file contains more than "<< MAX_RECTANGLES << " rectangles. Processing the first" << MAX_RECTANGLES << ".\n";
            break;
        }

        /* Check that all required keys exist */
        if (!(item.contains("x") && item.contains("y") &&
              item.contains("w") && item.contains("h"))) 
              {
            throw std::runtime_error("Each rectangle must contain exactly 'x', 'y', 'w', and 'h' fields.");
        }

        /* Check for no extra keys */
        const std::vector<std::string> allowed_keys = {"x", "y", "w", "h"};
        for (auto it = item.begin(); it != item.end(); ++it) 
        {
            if (std::find(allowed_keys.begin(), allowed_keys.end(), it.key()) == allowed_keys.end()) 
            {
                throw std::runtime_error("Invalid key '" + it.key() + "' found in rectangle definition.");
            }
        }

        /* Extract values */
        int x = item.at("x").get<int>();
        int y = item.at("y").get<int>();
        int w = item.at("w").get<int>();
        int h = item.at("h").get<int>();

        /* Validate dimensions */
        if (w < 0 || h < 0) 
        {
            throw std::runtime_error("Rectangle with negative width or height found: "
                                    "w=" + std::to_string(w) + ", h=" + std::to_string(h));
        }

        if (w == 0 || h == 0)
        {
            std::cout << "Info: Ignoring rectangle with zero width or height: "
                    << "x=" << x << ", y=" << y << ", w=" << w << ", h=" << h << ".\n";
            continue;
        }

        rectangles.emplace_back(id_counter++, x, y, w, h);
    }

    return rectangles;
}


/**
 * Computes the overlapping region (if any) between two rectangles.
 * The result is returned via the 'result' reference parameter.
 */
bool Rectangle::calculate_intersection(const Rectangle& r1, const Rectangle& r2, Rectangle& result) 
{
    bool boReturn = false;
    /* Determine the overlap boundaries */
    int intersect_x = std::max(r1.x(), r2.x());
    int intersect_y = std::max(r1.y(), r2.y());
    int intersect_right = std::min(r1.right(), r2.right());
    int intersect_bottom = std::min(r1.bottom(), r2.bottom());

    /* Calculate width and height of the overlapping area */
    int intersect_w = intersect_right - intersect_x;
    int intersect_h = intersect_bottom - intersect_y;

    /* Valid intersection only if both width and height are positive */
    if (intersect_w > 0 && intersect_h > 0) 
    {
        result = Rectangle(-1, intersect_x, intersect_y, intersect_w, intersect_h);
        boReturn = true;
    }

    return boReturn;
}
