#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Triangulation_vertex_base_with_info_2<K::FT, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef K::Point_2 Point;

using namespace std;

double ceil_to_double(EK::FT &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

EK::FT sqrt_rho_inv(EK::FT d) {
    // Square root of the inverse of rho(t)
    return CGAL::sqrt((CGAL::sqrt(d) - 1) / 2.0);
}


void testcase(int n) {
    int l; cin >> l;
    int b; cin >> b;
    int r; cin >> r;
    int t; cin >> t;
    
    vector<Point> bacteria(n);
    for (int i = 0 ; i < n ; i++) {
        int x; cin >> x;
        int y; cin >> y;
        bacteria[i] = Point(x, y);
    }
    
    Triangulation tr;
    tr.insert(bacteria.begin(), bacteria.end());
    
    // Minimum distance to boundaries
    for (auto it = tr.finite_vertices_begin() ; it != tr.finite_vertices_end() ; it++) {
        Point p = it->point();
        
        K::FT d_shortest = INT_MAX;
        d_shortest = min(d_shortest, p.x() - l);
        d_shortest = min(d_shortest, p.y() - b);
        d_shortest = min(d_shortest, r - p.x());
        d_shortest = min(d_shortest, t - p.y());
        
        it->info() = 4 * d_shortest * d_shortest;
    }
    
    // Minimum distance to another bacteria
    for (auto it = tr.finite_edges_begin() ; it != tr.finite_edges_end() ; it++) {
        auto v1 = it->first->vertex((it->second + 1) % 3);
        auto v2 = it->first->vertex((it->second + 2) % 3);
        K::FT d = CGAL::squared_distance(v1->point(), v2->point());
        
        v1->info() = min(v1->info(), d);
        v2->info() = min(v2->info(), d);
    }
    
    vector<K::FT> ds;
    for (auto it = tr.finite_vertices_begin() ; it != tr.finite_vertices_end() ; it++) {
        ds.push_back(it->info());
    }
    
    sort(ds.begin(), ds.end());
    
    EK::FT first = sqrt_rho_inv(EK::FT(ds[0]));
    EK::FT medium = sqrt_rho_inv(EK::FT(ds[n / 2]));
    EK::FT last = sqrt_rho_inv(EK::FT(ds[n - 1]));
    
    cout << ceil_to_double(first) << " " << ceil_to_double(medium) << " " << ceil_to_double(last) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
    return 0;
}
