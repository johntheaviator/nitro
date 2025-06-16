#ifndef INTERSECTION_FINDER_HPP
#define INTERSECTION_FINDER_HPP

#include <vector>
#include <string>
#include "Rectangle.h"


/**
* @struct IntersectionResult
* @brief Stores the result of a rectangle intersection.
*
* Contains the resulting intersected rectangle and the IDs of the rectangles involved.
*/
struct IntersectionResult 
{
    Rectangle rect;               /* The intersected rectangle */
    std::vector<int> parent_ids;  /* The original rectangles involved in the intersection */
};

/**
* @class IntersectionFinder
* @brief Orchestrates loading rectangles, finding intersections, and reporting results.
*
* This class encapsulates:
* - Reading input rectangles from a JSON file
* - Performing 2-way and recursive N-way intersection detection
* - Storing and printing the results
*/
class IntersectionFinder 
{
private:
    std::vector<Rectangle> m_inputRectangles;     /* Rectangles loaded from input */
    std::vector<IntersectionResult> m_intersections; /* Detected intersections */
    std::vector<std::string> m_processedKeys;   /* Used to track processed intersection keys (unused currently, reserved for deduplication). */

    /**
    * @brief Recursively detects intersections involving 3 or more rectangles.
    * @param current_intersection The current intersected rectangle.
    * @param parent_ids IDs of rectangles involved so far.
    * @param start_index Index to continue iteration from.
    */
    void find_intersections_recursive(
        const Rectangle& current_intersection,
        const std::vector<int>& parent_ids,
        size_t start_index
    );

    /**
    * @brief Generates a sorted string key from rectangle IDs.
    * 
    * Used for identification or deduplication of intersection groups.
    *
    * @param ids Vector of rectangle IDs involved in an intersection.
    * @return std::string A hyphen-separated, sorted string representation of the IDs.
    */
    std::string createKey(const std::vector<int>& ids);

    /**
    * @brief Records an intersection if its key has not been seen before.
    * 
    * @param rect The intersected rectangle.
    * @param parent_ids The IDs of rectangles that form this intersection.
    * @return true if this is a new (unique) intersection and it was recorded.
    * @return false if this intersection was already recorded.
    */
    bool recordIntersectionIfUnique(const Rectangle& rect, const std::vector<int>& parent_ids);

public:
    /**
    * @brief Constructs an IntersectionFinder instance.
    */
    IntersectionFinder();

    /**
    * @brief Loads rectangles from a JSON file.
    * 
    * Reads and parses rectangles from a JSON input file. Validates format and field presence.
    * 
    * @param filename Path to the JSON input file.
    * @throws std::runtime_error if the file is missing, invalid, or contains malformed rectangles.
    */
    void loadRectanglesFromFile(const std::string& filename);

    /**
    * @brief Computes all pairwise and higher-order intersections.
    * 
    * Iteratively compares rectangles to find intersections, including recursive intersections involving
    * three or more rectangles.
    */
    void processIntersections();

    /**
    * @brief Prints the original rectangles and all found intersections to stdout.
    * 
    * Outputs a formatted list of input rectangles followed by detailed intersection results,
    * indicating which rectangles contributed to each intersection.
    */
    void printResults();
};

#endif // INTERSECTION_FINDER_HPP

