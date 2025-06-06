// Copyright (c) 2010,2011 Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL$
// $Id$
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Ron Wein          <wein@post.tau.ac.il>

#ifndef CGAL_ARR_FACE_INDEX_MAP_H
#define CGAL_ARR_FACE_INDEX_MAP_H

#include <CGAL/license/Arrangement_on_surface_2.h>

#include <CGAL/disable_warnings.h>

/*! \file
 * Definition of the Arr_face_index_map<Arrangement> class.
 */

#include <CGAL/Unique_hash_map.h>
#include <CGAL/property_map.h>

#include <boost/graph/properties.hpp>

namespace CGAL {

/*! \class
 * An auxiliary class that automatically maintains a mapping of the
 * arrangement faces to the indices 0, ..., (n -1), where n is the number
 * of faces in the arrangement.
 */
template <typename Arrangement_>
class Arr_face_index_map : public Arrangement_::Observer {
public:
  using Arrangement_2 = Arrangement_;
  using Base_aos = typename Arrangement_2::Base_aos;

  using Halfedge_handle = typename Base_aos::Halfedge_handle;
  using Face_handle = typename Base_aos::Face_handle;

  // Boost property type definitions:
  using category = boost::readable_property_map_tag;
  using value_type = unsigned int;
  using reference = value_type;
  using key_type = Face_handle;

private:
  using Self = Arr_face_index_map<Arrangement_2>;
  using Base = typename Arrangement_2::Observer;

  using Index_map = Unique_hash_map<Face_handle, unsigned int>;

  // Data members:
  unsigned int n_faces;                    // The current number of faces.
  Index_map index_map;                     // Mapping faces to indices.
  std::vector<Face_handle> rev_map;        // Mapping indices to faces.

  enum {MIN_REV_MAP_SIZE = 32};

public:
  /*! constructs default. */
  Arr_face_index_map() :
    Base(),
    n_faces(0),
    rev_map(MIN_REV_MAP_SIZE)
  {}

  /*! constructs from an associated arrangement. */
  Arr_face_index_map(const Base_aos& arr) :
    Base(const_cast<Base_aos&>(arr))
  { _init(); }

  /*! constructs copy. */
  Arr_face_index_map(const Self& other) :
    Base(const_cast<Base_aos&>(*(other.arrangement())))
  { _init(); }

  /*! assigns. */
  Self& operator= (const Self& other) {
    if (this == &other) return (*this);

    this->detach();
    this->attach(const_cast<Base_aos&>(*(other.arrangement())));

    return (*this);
  }

  /*! obtains the index of a given face.
   * \param f A handle to the face.
   * \pre f is a valid face in the arrangement.
   */
  unsigned int operator[](Face_handle f) const { return (index_map[f]); }

  /*! obtains the face given its index.
   * \param i The index of the face.
   * \pre i is less than the number of faces in the arrangement.
   */
  Face_handle face(const int i) const {
    CGAL_precondition((unsigned int) i < n_faces);
    return (rev_map[i]);
  }

  /// \name Notification functions, to keep the mapping up-to-date.
  //@{

  /*! updates the mapping after the arrangement has been assigned with another
   * arrangement.
   */
  virtual void after_assign() override { _init(); }

  /*! updates the mapping after the arrangement is cleared.
   */
  virtual void after_clear() override { _init(); }

  /*! updates the mapping after attaching to a new arrangement.
   */
  virtual void after_attach() override { _init(); }

  /*! updates the mapping after detaching the arrangement.
   */
  virtual void after_detach() override {
    n_faces = 0;
    index_map.clear();
  }

  /*! updates the mapping after the creation of a new face is split from another
   * face.
   * \param f A handle to the existing face.
   * \param new_f A handle to the newly created face.
   */
  virtual void after_split_face(Face_handle /* f */,
                                Face_handle new_f,
                                bool /* is_hole */) override {
    // Update the number of vertices.
    ++n_faces;

    // If necessary, allocate memory for the reverse mapping.
    if (rev_map.size() < n_faces)
      rev_map.resize(2 * n_faces);

    // Update the mapping of the newly created face.
    index_map[new_f] = n_faces - 1;
    rev_map[n_faces - 1] = new_f;
  }

  /*! updates the mapping before the merge of two faces.
   * \param f1 A handle to the face that is going to remain.
   * \param f2 A handle to the face that is about to be removed.
   */
  virtual void before_merge_face(Face_handle /* f1 */,
                                 Face_handle f2,
                                 Halfedge_handle /* e */) override {
    // Update the number of faces.
    --n_faces;

    // Reduce memory consumption in case the number of faces has
    // drastically decreased.
    if (2*n_faces+1 < rev_map.size() &&
        rev_map.size() / 2 >= MIN_REV_MAP_SIZE)
    {
      rev_map.resize(rev_map.size() / 2);
    }

    // Get the current face index, and assign this index to the face
    // currently indexed (n - 1).
    unsigned int index = index_map[f2];

    if (index == n_faces) return;

    Face_handle last_f = rev_map[n_faces];
    index_map[last_f] = index;
    rev_map[index] = last_f;

    // Clear the reverse mapping for the last face.
    rev_map[n_faces] = Face_handle();
  }
  //@}

private:
  /*! initializes the map for the given arrangement. */
  void _init() {
    // Get the number of faces and allocate the reverse map accordingly.
    n_faces = static_cast<unsigned int>(this->arrangement()->number_of_faces());

    if (n_faces < MIN_REV_MAP_SIZE) rev_map.resize (MIN_REV_MAP_SIZE);
    else rev_map.resize (n_faces);

    // Clear the current mapping.
    index_map.clear();

    // Create the initial mapping.
    Face_handle fh;
    unsigned int index = 0;

    for (auto fit = this->arrangement()->faces_begin();
         fit != this->arrangement()->faces_end(); ++fit, ++index) {
      // Map the current face to the current index.
      fh = fit;
      index_map[fh] = index;
      rev_map[index] = fh;
    }
  }
};

/*! obtains the index property-map function. Provided so that boost is able to
 * access the Arr_face_index_map above.
 * \param index_map The index map.
 * \param f A face handle.
 * \return The face index.
 */
template <typename Arrangement>
unsigned int get(const CGAL::Arr_face_index_map<Arrangement>& index_map,
                 typename Arrangement::Face_handle f)
{ return (index_map[f]); }

} // namespace CGAL

#include <CGAL/enable_warnings.h>

#endif
