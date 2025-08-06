#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>
#include <string_view>

using namespace std;

// Compile-time constants for better performance
namespace Constants {
    constexpr double PI = 3.14159265358979323846;
    constexpr int PRECISION = 2;
}

// Base class (Abstract) - Optimized
class Shape {
protected:
    string_view name;  // Use string_view for better performance
    mutable double cached_area = -1.0;      // Cache for expensive calculations
    mutable double cached_perimeter = -1.0;
    
public:
    explicit Shape(string_view shapeName) noexcept : name(shapeName) {}
    
    // Virtual destructor for proper cleanup
    virtual ~Shape() = default;
    
    // Pure virtual functions for polymorphism
    virtual double calculateArea() const = 0;
    virtual double calculatePerimeter() const = 0;
    virtual void displayInfo() const = 0;
    
    // Common function - return by value is fine for string_view
    string_view getName() const noexcept { return name; }
    
    // Efficient area calculation with caching
    double getArea() const {
        if (cached_area < 0) {
            cached_area = calculateArea();
        }
        return cached_area;
    }
    
    // Efficient perimeter calculation with caching
    double getPerimeter() const {
        if (cached_perimeter < 0) {
            cached_perimeter = calculatePerimeter();
        }
        return cached_perimeter;
    }
};

// Derived class: Rectangle - Optimized
class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) noexcept : Shape("Rectangle"), width(w), height(h) {}
    
    double calculateArea() const override {
        return width * height;
    }
    
    double calculatePerimeter() const override {
        return 2.0 * (width + height);  // Use 2.0 for consistent double arithmetic
    }
    
    void displayInfo() const override {
        // Use single cout with stringstream-like approach for better performance
        cout << fixed << setprecision(Constants::PRECISION)
             << "Shape: " << name
             << "\nDimensions: " << width << " x " << height
             << "\nArea: " << getArea()
             << "\nPerimeter: " << getPerimeter()
             << "\n------------------------\n";
    }
};

// Derived class: Circle - Optimized
class Circle : public Shape {
private:
    double radius;
    double radius_squared;  // Cache radius squared for efficiency
    
public:
    Circle(double r) noexcept : Shape("Circle"), radius(r), radius_squared(r * r) {}
    
    double calculateArea() const override {
        return Constants::PI * radius_squared;  // Use cached value
    }
    
    double calculatePerimeter() const override {
        return 2.0 * Constants::PI * radius;
    }
    
    void displayInfo() const override {
        cout << fixed << setprecision(Constants::PRECISION)
             << "Shape: " << name
             << "\nRadius: " << radius
             << "\nArea: " << getArea()
             << "\nCircumference: " << getPerimeter()
             << "\n------------------------\n";
    }
};

// Derived class: Triangle - Optimized
class Triangle : public Shape {
private:
    double side1, side2, side3;
    double semi_perimeter;  // Cache semi-perimeter for Heron's formula
    
public:
    Triangle(double s1, double s2, double s3) noexcept 
        : Shape("Triangle"), side1(s1), side2(s2), side3(s3),
          semi_perimeter((s1 + s2 + s3) * 0.5) {}
    
    double calculateArea() const override {
        // Using Heron's formula with cached semi-perimeter
        const double term = semi_perimeter * (semi_perimeter - side1) * 
                           (semi_perimeter - side2) * (semi_perimeter - side3);
        return sqrt(term);
    }
    
    double calculatePerimeter() const override {
        return 2.0 * semi_perimeter;  // Use cached value
    }
    
    void displayInfo() const override {
        cout << fixed << setprecision(Constants::PRECISION)
             << "Shape: " << name
             << "\nSides: " << side1 << ", " << side2 << ", " << side3
             << "\nArea: " << getArea()
             << "\nPerimeter: " << getPerimeter()
             << "\n------------------------\n";
    }
};

// Function that demonstrates polymorphism - Optimized
void processShape(const Shape& shape) {
    cout << "Processing " << shape.getName() << ":\n";
    shape.displayInfo();
}

// Function to calculate total area and perimeter - Optimized
struct ShapeStats {
    double total_area = 0.0;
    double total_perimeter = 0.0;
    size_t count = 0;
};

ShapeStats calculateShapeStats(const vector<unique_ptr<Shape>>& shapes) {
    ShapeStats stats;
    stats.count = shapes.size();
    
    // Reserve capacity to avoid reallocation during iteration
    for (const auto& shape : shapes) {
        stats.total_area += shape->getArea();        // Use cached values
        stats.total_perimeter += shape->getPerimeter();
    }
    
    return stats;
}

int main() {
    // Set up output formatting once
    cout << fixed << setprecision(Constants::PRECISION);
    cout << "=== Optimized Polymorphism Demo: Shape Calculator ===\n\n";
    
    // Create a vector of shape pointers with reserved capacity
    vector<unique_ptr<Shape>> shapes;
    shapes.reserve(5);  // Reserve capacity to avoid reallocations
    
    // Add different shapes to the container using emplace_back for efficiency
    shapes.emplace_back(make_unique<Rectangle>(5.0, 3.0));
    shapes.emplace_back(make_unique<Circle>(4.0));
    shapes.emplace_back(make_unique<Triangle>(3.0, 4.0, 5.0));
    shapes.emplace_back(make_unique<Rectangle>(2.5, 6.0));
    shapes.emplace_back(make_unique<Circle>(2.5));
    
    // Demonstrate polymorphism - same interface, different behaviors
    cout << "Individual Shape Information:\n";
    for (const auto& shape : shapes) {
        // Polymorphic call - the correct derived class method is called
        shape->displayInfo();
    }
    
    // Calculate and display comprehensive statistics
    const auto stats = calculateShapeStats(shapes);
    cout << "\n=== Summary Statistics ===\n"
         << "Total shapes: " << stats.count
         << "\nTotal area: " << stats.total_area << " square units"
         << "\nTotal perimeter: " << stats.total_perimeter << " units"
         << "\nAverage area: " << (stats.total_area / stats.count) << " square units\n\n";
    
    // Demonstrate polymorphism with function parameter
    cout << "Demonstrating polymorphism with function calls:\n";
    Rectangle rect(7.0, 2.0);
    Circle circle(3.0);
    
    processShape(rect);   // Calls Rectangle's methods
    processShape(circle); // Calls Circle's methods
    
    return 0;
}
