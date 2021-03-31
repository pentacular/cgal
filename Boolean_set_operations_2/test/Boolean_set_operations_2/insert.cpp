#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_set_2.h>
#include <list>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef CGAL::Polygon_set_2<Kernel>                       Polygon_set_2;

int main()
{
  Polygon_set_2                   ps;

  Polygon_2 a;
  a.push_back({-8.9141860000000310293, -66.424003912648714731});
  a.push_back({-8.9946985979838824932, -50.549783483396339534});
  a.push_back({-9.07569859798388201, -46.999881208142973321});

  Polygon_2 b;
  b.push_back({-9.4311023374436047106, -36.223810326036300467});
  b.push_back({-9.5002883374435960206, -92.226123214562647945});
  b.push_back({-9.2947883374435953385, -85.53073791060376152});

  Polygon_2 c;
  c.push_back({-9.3602273374436038011, -37.633675733464556856});
  c.push_back({-9.2947883374435953385, -85.53073791060376152});
  c.push_back({-9.07569859798388201, -46.999881208142973321});

  Polygon_2 d;
  d.push_back({-9.8931519384873141121, -27.591865923719421261});
  d.push_back({-9.9623379384873018694, -100.85806761687955202});
  d.push_back({-9.5002883374435960206, -92.226123214562647945});

  Polygon_2 e;
  e.push_back({-9.9816519384873139131, -26.451643304304283788});
  e.push_back({-10.603466094067306003, -18.885680756879807518});
  e.push_back({-10.142151938487298679, -103.55979788268778918});

  Polygon_2 f;
  f.push_back({-10.603466094067306003, -18.885680756879807518});
  f.push_back({-10.763966094067288992, -111.12576043011229388});
  f.push_back({-10.142151938487298679, -103.55979788268778918});

  ps.insert(Polygon_with_holes_2(a));
  ps.insert(Polygon_with_holes_2(b));
  ps.insert(Polygon_with_holes_2(c));
  ps.insert(Polygon_with_holes_2(d));
  ps.insert(Polygon_with_holes_2(e));
  ps.insert(Polygon_with_holes_2(f));
  
  return 0;
}
