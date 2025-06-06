#include <CGAL/Exact_rational.h>
#include <CGAL/Cartesian.h>

#include <CGAL/Algebraic_kernel_for_circles_2_2.h>
#include <CGAL/Circular_kernel_2/Intersection_traits.h>
#include <CGAL/Circular_kernel_2.h>
#include <CGAL/Hyperbolic_surface_traits_2.h>
#include <CGAL/Hyperbolic_fundamental_domain_factory_2.h>
#include <CGAL/Triangulation_on_hyperbolic_surface_2.h>
#include <CGAL/Triangulation_on_hyperbolic_surface_2_IO.h>
#include <CGAL/Hyperbolic_Delaunay_triangulation_CK_traits_2.h>

#include <iostream>
#include <sstream>
#include <vector>

typedef CGAL::Circular_kernel_2<CGAL::Cartesian<CGAL::Exact_rational>,CGAL::Algebraic_kernel_for_circles_2_2<CGAL::Exact_rational>>               Kernel;
typedef CGAL::Hyperbolic_Delaunay_triangulation_CK_traits_2<Kernel>         ParentTraits;
typedef CGAL::Hyperbolic_surface_traits_2<ParentTraits>                     Traits;
typedef CGAL::Hyperbolic_fundamental_domain_2<Traits>                       Domain;
typedef CGAL::Hyperbolic_fundamental_domain_factory_2<Traits>               Factory;
typedef CGAL::Triangulation_on_hyperbolic_surface_2<Traits>                 Triangulation;

typedef typename Traits::Hyperbolic_point_2                                 Point;

int main()
{
  Factory factory;
  Domain domain = factory.make_hyperbolic_fundamental_domain_g2(3459);
  Triangulation triangulation0 = Triangulation(domain);

  assert(triangulation0.is_valid());

  Triangulation triangulation (triangulation0);
  assert(triangulation.has_anchor());

  std::stringstream buffer;
  buffer << triangulation;
  buffer >> triangulation;

  std::vector<std::tuple<typename Triangulation::Combinatorial_map_with_cross_ratios::Dart_const_handle,Point,Point,Point>> input_not_centered;
  std::vector<std::tuple<typename Triangulation::Combinatorial_map_with_cross_ratios::Dart_const_handle,Point,Point,Point>> input_centered;

  input_not_centered = triangulation.lift(false);
  input_centered = triangulation.lift();

  triangulation.make_Delaunay();

  assert(triangulation.is_Delaunay());

  std::vector<std::tuple<typename Triangulation::Combinatorial_map_with_cross_ratios::Dart_const_handle,Point,Point,Point>> output_not_centered;
  std::vector<std::tuple<typename Triangulation::Combinatorial_map_with_cross_ratios::Dart_const_handle,Point,Point,Point>> output_centered;

  output_not_centered = triangulation.lift(false);
  output_centered = triangulation.lift();


  Triangulation::Combinatorial_map_with_cross_ratios& cmap = triangulation.combinatorial_map();
  Triangulation::Anchor& anchor = triangulation.anchor();
  assert(cmap.is_dart_used(anchor.dart));

  std::cout << "printing triangulation for test purposes: " << std::endl << triangulation;

  return 0;
}
