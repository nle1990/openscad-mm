#pragma once

#include "cgal.h"
#include "Geometry.h"
#include "memory.h"
#include <string>
#include "linalg.h"

class CGAL_Nef_polyhedron : public Geometry
{
public:
  VISITABLE_GEOMETRY();
  CGAL_Nef_polyhedron(const CGAL_Nef_polyhedron3 *p, Geometry::Attributes attr);
  CGAL_Nef_polyhedron(shared_ptr<const CGAL_Nef_polyhedron3> p, Geometry::Attributes attr) : p3(p) { attributes = attr; }
  CGAL_Nef_polyhedron(const CGAL_Nef_polyhedron& src);
  ~CGAL_Nef_polyhedron() {}

  size_t memsize() const override;
  // FIXME: Implement, but we probably want a high-resolution BBox..
  BoundingBox getBoundingBox() const override;
  std::string dump() const override;
  unsigned int getDimension() const override { return 3; }
  // Empty means it is a geometric node which has zero area/volume
  bool isEmpty() const override;
  Geometry *copy() const override { return new CGAL_Nef_polyhedron(*this); }
  size_t numFacets() const override {
    if(!p3/* || !p3->is_valid()*/) return 0;
    return p3->number_of_facets();
  }

  void reset() { p3.reset(); }
  CGAL_Nef_polyhedron operator+(const CGAL_Nef_polyhedron& other) const;
  CGAL_Nef_polyhedron& operator+=(const CGAL_Nef_polyhedron& other);
  CGAL_Nef_polyhedron& operator*=(const CGAL_Nef_polyhedron& other);
  CGAL_Nef_polyhedron& operator-=(const CGAL_Nef_polyhedron& other);
  CGAL_Nef_polyhedron& minkowski(const CGAL_Nef_polyhedron& other);
  virtual void transform(const Transform3d& matrix) override;
  virtual void resize(const Vector3d& newsize, const Eigen::Matrix<bool, 3, 1>& autosize) override;
  std::string toString() const override;

  shared_ptr<const CGAL_Nef_polyhedron3> p3;
};
