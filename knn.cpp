#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

struct Point
{
    int val;
    double x, y;
    double distance;
};

static int64_t 
PointClassification(std::vector<Point>& points, size_t k, const Point& p)
{
    for (auto& point : points)
    {
        point.distance = sqrt((point.x - p.x) * (point.x - p.x) +
                        (point.y - p.y) * (point.y - p.y));
    }

    std::ranges::sort(points, [](const auto& a, const auto& b) {
        return a.distance < b.distance;
        });

    std::vector<int> frequency_count(3, 0); // Number of groups
    std::span<Point> point_span{ points.data(), std::min(k, points.size()) };
    for (auto& point : point_span)
    {
        if (point.val < frequency_count.size())
        {
            ++frequency_count[point.val];
        }
    }


    auto max_iterator = std::ranges::max_element(frequency_count);
    return std::distance(frequency_count.begin(), max_iterator);
}

static std::vector<std::string> 
ParseLine(const std::string& str, char delimiter)
{
    std::istringstream iss(str);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(iss, token, delimiter))
    {
        tokens.push_back(token);
    }
    
    return tokens;
}

int main()
{
    std::vector<Point> points;
    std::ifstream file("data_points.txt");
    std::string line;

    while (getline(file, line))
    {
        auto tokens = ParseLine(line, ',');
        Point temp{};

        temp.x = std::stod(tokens[0]);
        temp.y = std::stod(tokens[1]);
        temp.val = std::stoi(tokens[2]);
        
        points.push_back(temp);
    }

    Point p{};
    std::cout << "Enter x and y coordinates of the point to classify: ";
    std::cin >> p.x >> p.y;

    size_t k;
    std::cout << "Enter the number of nearest neighbours to consider: ";
    std::cin >> k;

    int64_t group = PointClassification(points, k, p);
    std::cout << "The point belongs to group " << group << std::endl;

    return 0;
}

