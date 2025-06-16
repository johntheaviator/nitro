#include "IntersectionFinder.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

IntersectionFinder::IntersectionFinder() = default;

void IntersectionFinder::loadRectanglesFromFile(const std::string& filename) 
{
    m_inputRectangles = Rectangle::loadFromFile(filename);
    
    if (m_inputRectangles.size() < 2)
    {
        throw std::runtime_error("Not Possible to find intersections with only one valid rectangle.");
    }
}

/* Creates a unique sorted key for a group of rectangles */
std::string IntersectionFinder::createKey(const std::vector<int>& ids) 
{
    std::vector<int> sorted = ids;
    std::sort(sorted.begin(), sorted.end());

    std::string key = std::to_string(sorted[0]);

    for (size_t i = 1; i < sorted.size(); ++i) 
    { 
        key += "-";
        key += std::to_string(sorted[i]);
    }
    
    return key;
}

bool IntersectionFinder::recordIntersectionIfUnique(const Rectangle& rect, const std::vector<int>& parent_ids) 
{
    bool boReturn = false;

    std::string key = createKey(parent_ids);

    if (std::find(m_processedKeys.begin(), m_processedKeys.end(), key) == m_processedKeys.end()) 
    {
        m_processedKeys.push_back(key);
        m_intersections.push_back({rect, parent_ids});

        boReturn = true;
    }

    return boReturn;
}


void IntersectionFinder::processIntersections() 
{
    for (size_t i = 0; i < m_inputRectangles.size(); ++i) 
    {
        for (size_t j = i + 1; j < m_inputRectangles.size(); ++j) 
        {
            const auto& r1 = m_inputRectangles[i];
            const auto& r2 = m_inputRectangles[j];

            Rectangle intersection(-1, 0, 0, 0, 0);
            if (Rectangle::calculate_intersection(r1, r2, intersection)) 
            {
                std::vector<int> parent_ids = {r1.id(), r2.id()};
                if (recordIntersectionIfUnique(intersection, parent_ids)) 
                {
                    find_intersections_recursive(intersection, parent_ids, j + 1);
                }
            }
        }
    }
}

void IntersectionFinder::find_intersections_recursive(const Rectangle& current_intersection, const std::vector<int>& parent_ids, size_t start_index) 
{
    for (size_t i = start_index; i < m_inputRectangles.size(); ++i) 
    {
        const auto& next_rect = m_inputRectangles[i];
        Rectangle new_intersection(-1, 0, 0, 0, 0);

        if (Rectangle::calculate_intersection(current_intersection, next_rect, new_intersection)) 
        {
            std::vector<int> new_parent_ids = parent_ids;
            new_parent_ids.push_back(next_rect.id());

            if (recordIntersectionIfUnique(new_intersection, new_parent_ids)) 
            {
                find_intersections_recursive(new_intersection, new_parent_ids, i + 1);
            }
        }
    }
}

void IntersectionFinder::printResults() 
{
    std::cout << "Input:\n";
    for (const auto& rect : m_inputRectangles) 
    {
        std::cout << "\t" << rect.id() << ": Rectangle at ("
                  << rect.x() << "," << rect.y() << "), "
                  << "w=" << rect.w() << ", h=" << rect.h() << ".\n";
    }

    std::cout << "Intersections:\n";

    if (m_intersections.empty()) 
    {
        std::cout << "No intersections found.\n";
        return;
    }

    std::vector<IntersectionResult> sorted = m_intersections;

    std::sort(sorted.begin(), sorted.end(), [](const IntersectionResult& a, const IntersectionResult& b) {
    /* First: by number of rectangles involved (ascending) */
    if (a.parent_ids.size() != b.parent_ids.size()) 
    {
        return a.parent_ids.size() < b.parent_ids.size();
    }

    /* Second: by the smallest rectangle ID in the intersection */
    auto sorted_a = a.parent_ids, sorted_b = b.parent_ids;
    std::sort(sorted_a.begin(), sorted_a.end());
    std::sort(sorted_b.begin(), sorted_b.end());

    return sorted_a < sorted_b;
    });

    for (size_t i = 0; i < sorted.size(); ++i) 
    {
        const auto& result = sorted[i];

        std::cout << "\tBetween rectangle ";

        for (size_t j = 0; j < result.parent_ids.size(); ++j) 
        {
            std::cout << result.parent_ids[j];
            if (j + 2 == result.parent_ids.size()) 
            {
                std::cout << " and ";
            } 
            else if (j + 1 < result.parent_ids.size()) 
            {
                std::cout << ", ";
            }
        }

        const auto& rect = result.rect;
        std::cout << " at (" << rect.x() << "," << rect.y()
                  << "), w=" << rect.w() << ", h=" << rect.h() << ".\n";
    }
}
    