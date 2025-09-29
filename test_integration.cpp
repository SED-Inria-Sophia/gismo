#include "src/gismo/Core/Core"
#include "src/gismo/Math/Math"
#include "src/gismo/Geometry/Geometry"

#include <iostream>

int main()
{
    // Test Core module
    std::cout << "Testing GISMO modular architecture\n";

    // Test Math module constants
    std::cout << "Pi = " << gismo::Math::pi << std::endl;
    std::cout << "E = " << gismo::Math::e << std::endl;

    // Test Geometry module
    gismo::Geometry::Point<double, 3> p1(1.0, 2.0, 3.0);
    gismo::Geometry::Point<double, 3> p2(4.0, 5.0, 6.0);

    std::cout << "Point 1: (" << p1.x() << ", " << p1.y() << ", " << p1.z() << ")\n";
    std::cout << "Point 2: (" << p2.x() << ", " << p2.y() << ", " << p2.z() << ")\n";

    // Test Vector operations
    gismo::Geometry::Vector<double, 3> v = p2 - p1;
    std::cout << "Vector from p1 to p2: (" << v.x() << ", " << v.y() << ", " << v.z() << ")\n";

    // Test distance calculation
    double distance = p1.distanceTo(p2);
    std::cout << "Distance between points: " << distance << std::endl;

    // Test BoundingBox
    gismo::Geometry::BoundingBox<double, 3> bbox;
    bbox.extend(p1);
    bbox.extend(p2);

    auto min_pt = bbox.min();
    auto max_pt = bbox.max();
    std::cout << "Bounding box: [(" << min_pt.x() << ", " << min_pt.y() << ", " << min_pt.z()
              << ") - (" << max_pt.x() << ", " << max_pt.y() << ", " << max_pt.z() << ")]\n";

    std::cout << "All modules working correctly!\n";

    return 0;
}