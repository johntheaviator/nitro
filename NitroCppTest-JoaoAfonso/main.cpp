#include <iostream>
#include "IntersectionFinder.h"

/**
 * @brief Entry point of the application.
 *
 * Expects a single command-line argument specifying the JSON input file.
 * Loads rectangles, computes all intersections, and prints the results.
 */
int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <json_file>\n";
        return 1;
    }

    try 
    {
        IntersectionFinder finder;
        finder.loadRectanglesFromFile(argv[1]);
        finder.processIntersections();
        finder.printResults();
    } 
    
    catch (const std::runtime_error& e) 
    {
        std::cerr << "Runtime error: " << e.what() << "\n";
        return 1;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    } 
    catch (...) 
    {
        std::cerr << "An unknown error occurred.\n";
        return 1;
    }

    return 0; 
}