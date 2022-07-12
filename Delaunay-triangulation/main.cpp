#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"
#include <fstream>
#include <iomanip>

using std::setw;
using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
    int numberPoints = 40;
    if (argc>1)
    {
        numberPoints = atoi(argv[1]);
    }

    std::default_random_engine eng(std::random_device{}());
    //eng.seed(10);
    std::uniform_real_distribution<double> dist_w(0, 800);
    std::uniform_real_distribution<double> dist_h(0, 600);

    std::cout << "Generating " << numberPoints << " random points" << std::endl;

    std::vector<dt::Vector2<double>> points;
    for(int i = 0; i < numberPoints; ++i) {
        points.push_back(dt::Vector2<double>{dist_w(eng), dist_h(eng), i});
    }

    dt::Delaunay<double> triangulation;
    const auto start = std::chrono::high_resolution_clock::now();
    const std::vector<dt::Triangle<double>> triangles = triangulation.triangulate(points);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> diff = end - start;

    std::cout << triangles.size() << " triangles generated in " << diff.count()
            << "s\n";
    const std::vector<dt::Edge<double>> edges = triangulation.getEdges();

    
    
    // ============== output ============================
    // Transform each points of each vector as a rectangle
    std::ofstream out;
    out.open("/Users/hesamshaelaie/Documents/Research/others-code/testing/Testing-graph/Testing-graph/TInstance.txt");
    if (!out) {
        cout<<"cannot open the output file!!"<<endl;
        exit(31);
    }
    
    
    out<<numberPoints<<endl;
    out<<edges.size();
    out<<endl<<endl;
    for(const auto p : points)
    {
        out<< setw(20) << p.In;
        out<< setw(20) << p.x;
        out<< setw(20) << p.y<<endl;
    }
    
    out<<endl<<endl<<endl;
    
    int cnt =0;
    for(const auto &e : edges)
    {
        cnt++;
        out<<setw(5)<<e.v->In;
        out<<setw(5)<<e.w->In<<endl;
        
    }
    cout<<"number of edges: "<< cnt <<endl;
    return 0;
}
