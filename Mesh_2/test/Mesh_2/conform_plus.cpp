#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Triangulation_conformer_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2                                          Point;
typedef CGAL::Constrained_Delaunay_triangulation_2<K>  CDTB;
typedef CGAL::Constrained_triangulation_plus_2<CDTB> CDT;

typedef CDT::Vertex_handle                                  Vertex_handle;

int main()
{
  CDT cdt;
  cdt.insert_constraint(Point(2, 5), Point(8, 5));
  cdt.insert(Point(5, 6));
  cdt.insert(Point(5, 4));

 std::pair<Vertex_handle,Vertex_handle> p;

  for(const auto& [vh1, vh2] : cdt.subconstraints()) {
    std::cerr << "subconstraint: " << vh1->point() << " -- " << vh2->point() << std::endl;
  }

  std::cerr << "\nMake Gabriel" << std::endl;

  CGAL::make_conforming_Gabriel_2(cdt);

  int counter = 0;

  for(const auto& [vh1, vh2] : cdt.subconstraints()) {
    ++counter;
    std::cerr << "subconstraint: " << vh1->point() << " -- " << vh2->point() << std::endl;
  }

  assert(counter>1);

  return 0;
}
