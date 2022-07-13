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
    eng.seed(10);
    
    
    int Lw = 0;
    int Hw = 800;
    
    int Lh = 0;
    int Hh = 600;
    float adjusting_space = 0.9;
    float Limit_space = (float(Hw-Lw) * float(Hh-Lh)* adjusting_space)/numberPoints;
    float Limit_distance = sqrt((Limit_space/3.14));
    
    std::uniform_real_distribution<double> dist_w(Lw, Hw);
    std::uniform_real_distribution<double> dist_h(Lh, Hh);
    
    std::cout << "Generating " << numberPoints << " random points" << std::endl;

    std::vector<dt::Vector2<double>> points;
    
    int itr = 0;
    int itr_s = 0;
    const int itr_limit = 50000;
    dt::Vector2<double> tmp(-1,-1,-1);
    double dist = -1;
    bool find = false;
    
    
    
    
    while(itr < numberPoints && itr_s< itr_limit)
    {
        tmp.In = itr;
        tmp.x = dist_w(eng);
        tmp.y = dist_w(eng);
        find = false;
        for (const auto p : points)
        {
            dist = p.dist(tmp);
            if (dist < Limit_distance)
            {
                find = true;
                break;
            }
        }
        
        if (!find)
        {
            points.push_back(tmp);
            itr++;
        }
        
        itr_s++;
    }
    
    if (itr_s == itr_limit)
    {
        cout<<"cannot creat the instance!!"<<endl;
        exit(2);
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
    out.open("/Users/hesamshaelaie/Documents/Research/Delaunay-triangulation/Delaunay-triangulation/TInstance.txt");
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
