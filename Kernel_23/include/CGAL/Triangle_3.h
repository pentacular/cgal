// Copyright (c) 1999
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Andreas Fabri, Stefan Schirra

#ifndef CGAL_TRIANGLE_3_H
#define CGAL_TRIANGLE_3_H

#include <CGAL/assertions.h>
#include <CGAL/Kernel/Return_base_tag.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Dimension.h>

#include <type_traits>

namespace CGAL {

template <class R_>
class Triangle_3 : public R_::Kernel_base::Triangle_3
{
  typedef typename R_::Boolean               Boolean;
  typedef typename R_::RT                    RT;
  typedef typename R_::FT                    FT;
  typedef typename R_::Point_3               Point_3;
  typedef typename R_::Plane_3               Plane_3;
  typedef typename R_::Aff_transformation_3  Aff_transformation_3;

  typedef Triangle_3                         Self;
  static_assert(std::is_same<Self, typename R_::Triangle_3>::value);

public:

  typedef Dimension_tag<3>  Ambient_dimension;
  typedef Dimension_tag<2>  Feature_dimension;

  typedef typename R_::Kernel_base::Triangle_3  Rep;

  const Rep& rep() const
  {
    return *this;
  }

  Rep& rep()
  {
    return *this;
  }

  typedef          R_                       R;

  Triangle_3() {}

  Triangle_3(const Rep& t)
      : Rep(t) {}

  Triangle_3(const Point_3& p, const Point_3& q, const Point_3& r)
    : Rep(typename R::Construct_triangle_3()(Return_base_tag(), p, q, r)) {}

  Triangle_3 transform(const Aff_transformation_3 &t) const
  {
    return Triangle_3(t.transform(this->vertex(0)),
                      t.transform(this->vertex(1)),
                      t.transform(this->vertex(2)));
  }

  Plane_3 supporting_plane() const
  {
    return R().construct_supporting_plane_3_object()(*this);
  }

  Boolean has_on(const Point_3 &p) const
  {
    return R().has_on_3_object()(*this, p);
  }

  decltype(auto)
  vertex(int i) const
  {
    return R().construct_vertex_3_object()(*this, i);
  }

  decltype(auto)
  operator[](int i) const
  {
    return vertex(i);
  }

  Boolean is_degenerate() const
  {
    return R().is_degenerate_3_object()(*this);
  }

  Bbox_3
  bbox() const
  {
    return R().construct_bbox_3_object()(*this);
  }

  decltype(auto) squared_area() const
  {
    return R().compute_squared_area_3_object()(*this);
  }

};


template < class R >
std::ostream &
operator<<(std::ostream &os, const Triangle_3<R> &t)
{
    switch(IO::get_mode(os)) {
    case IO::ASCII :
        return os << t[0] << ' ' << t[1] << ' ' << t[2];
    case IO::BINARY :
        return os << t[0]  << t[1]  << t[2];
    default:
        os << "Triangle_3(" << t[0] <<  ", " << t[1] <<   ", " << t[2] <<")";
        return os;
    }
}

template < class R >
std::istream &
operator>>(std::istream &is, Triangle_3<R> &t)
{
    typename R::Point_3 p, q, r;

    is >> p >> q >> r;

    if (is)
        t = Triangle_3<R>(p, q, r);
    return is;
}

} //namespace CGAL

#endif // CGAL_TRIANGLE_3_H
