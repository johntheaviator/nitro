#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <vector>
#include <string>

/* Defines the maximum rectangles to be processed */
#define MAX_RECTANGLES 10

/**
* @class Rectangle
* @brief Represents a rectangle in 2D space with an ID, position, and dimensions.
*
* Provides methods to access rectangle properties and calculate intersections
* between rectangles. This class is designed for use in geometric algorithms
* where spatial relationships are important.
*/
class Rectangle 
{
private:
    int m_id;   /* Unique ID of the rectangle. Negative IDs are used for derived/intersected rectangles.*/
    int m_x;    /* X-coordinate of the top-left corner. */
    int m_y;    /* Y-coordinate of the top-left corner. */
    int m_w;    /* Width of the rectangle. */
    int m_h;    /* Height of the rectangle. */

public:
    /**
    * @brief Constructs a rectangle with given ID, position, and size.
    * @param id Unique identifier for the rectangle.
    * @param x X-coordinate of the top-left corner.
    * @param y Y-coordinate of the top-left corner.
    * @param w Width of the rectangle.
    * @param h Height of the rectangle.
    */
    Rectangle(int id, int x, int y, int w, int h);

    virtual ~Rectangle();

    inline int id() const { return m_id; }      /* Returns the rectangle ID. */
    inline int x() const { return m_x; }       /* Returns the X-coordinate. */
    inline int y() const { return m_y; }       /* Returns the Y-coordinate. */
    inline int w() const { return m_w; }       /* Returns the width. */
    inline int h() const { return m_h; }       /* Returns the height. */
    inline int right() const { return m_x + m_w; }   /* Returns the X-coordinate of the right edge (x + w). */
    inline int bottom() const { return m_y + m_h; }  /* Returns the Y-coordinate of the bottom edge (y + h). */

    /**
    * @brief Loads rectangles from a JSON file.
    *
    * This static factory method parses a JSON file and constructs up to MAX_RECTANGLES Rectangle objects
    * based on the array found under the "rects" key. Each object must contain "x", "y", "w", and "h" fields.
    * 
    * Validation rules:
    * - Rectangles must contain the fields: x, y, w, h.
    * - x and y can be any integer (including negative values).
    * - Width and height (w, h) must be positive integers:
    *     - If width or height is zero, the rectangle is skipped with a warning.
    *     - If width or height is negative, parsing is aborted with an error.
    *
    * The resulting vector contains rectangles with assigned unique IDs starting from 1.
    *
    * @param filename Path to the input JSON file.
    * @return std::vector<Rectangle> A list of parsed Rectangle objects.
    * @throws std::runtime_error if the file cannot be read or the JSON format is invalid.
    */
    static std::vector<Rectangle> loadFromFile(const std::string& filename);

    /**
     * @brief Calculates the intersection of two rectangles.
     *
     * If the rectangles intersect, the result is stored in the 'result' parameter.
     * Returns true if the rectangles overlap, false otherwise.
     *
     * @param r1 First rectangle.
     * @param r2 Second rectangle.
     * @param result Rectangle to store the intersection result (if any).
     * @return True if rectangles intersect; false otherwise.
     */
    static bool calculate_intersection(const Rectangle& r1, const Rectangle& r2, Rectangle& result);
};

#endif // RECTANGLE_HPP
