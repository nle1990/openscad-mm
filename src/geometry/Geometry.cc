#include "Geometry.h"
#include "CGAL_Nef_polyhedron.h"
#include "PolySet.h"
#include "CGALHybridPolyhedron.h"
#include "Polygon2d.h"
#include "printutils.h"
#include "node.h"
#include <boost/foreach.hpp>
#include <sstream>

GeometryList::GeometryList()
{
}

GeometryList::GeometryList(const Geometry::Geometries& geometries) : children(geometries)
{
}

GeometryList::~GeometryList()
{
}

size_t GeometryList::memsize() const
{
  size_t sum = 0;
  for (const auto& item : this->children) {
    if(!item.second) continue;
    sum += item.second->memsize();
  }
  return sum;
}

BoundingBox GeometryList::getBoundingBox() const
{
  BoundingBox bbox;
  for (const auto& item : this->children) {
    if(!item.second) continue;
    bbox.extend(item.second->getBoundingBox());
  }
  return bbox;
}

std::string GeometryList::dump() const
{
  std::stringstream out;
  for (const auto& item : this->children) {
    if(!item.second) continue;
    out << item.second->dump();
  }
  return out.str();
}

unsigned int GeometryList::getDimension() const
{
  unsigned int dim = 0;
  for (const auto& item : this->children) {
    if (!item.second) continue;
    if (!dim) dim = item.second->getDimension();
    else if (dim != item.second->getDimension()) {
      LOG(message_group::Warning, Location::NONE, "", "Mixing 2D and 3D objects is not supported.");
      break;
    }
  }
  return dim;
}

bool GeometryList::isEmpty() const
{
  for (const auto& item : this->children) {
    if(!item.second) continue;
    if (!item.second->isEmpty()) return false;
  }
  return true;
}

void GeometryList::transform(const Transform3d& matrix)
{
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->transform(matrix);
    item.second.reset(geom);
  }
}

void GeometryList::resize(const Vector3d& newsize,
                                 const Eigen::Matrix<bool, 3, 1>& autosize)
{
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->resize(newsize, autosize);
    item.second.reset(geom);
  }
}



void GeometryList::setColor(Color4f color) {
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->setColor(color);
    item.second.reset(geom);
  }
}

void GeometryList::setMaterial(std::string material) {
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->setMaterial(material);
    item.second.reset(geom);
  }
}

void GeometryList::setPart(std::string part) {
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->setPart(part);
    item.second.reset(geom);
  }
}

unsigned int GeometryList::getConvexity() const {
  if (this->isEmpty()) return this->convexity;

  int maxConvexity = this->convexity;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    int childConvexity = item.second->getConvexity();
    if(childConvexity > maxConvexity) maxConvexity = childConvexity;
  }

  return maxConvexity;
}


void GeometryList::setAttributes(Geometry::Attributes attr) {
  if (this->isEmpty()) return;

  for (auto& item : this->children)
  {
    if(!item.second) continue;
    Geometry* geom = item.second->copy();
    geom->setAttributes(attr);
    item.second.reset(geom);
  }
}

void flatten(const GeometryList& geomlist, GeometryList::Geometries& childlist)
{
  for (const auto& item : geomlist.getChildren()) {
    if (const auto chlist = dynamic_pointer_cast<const GeometryList>(item.second)) {
      flatten(*chlist, childlist);
    } else {
      childlist.push_back(item);
    }
  }
}

/*!
   Creates a new GeometryList which has a flat hierarchy (all
   children directly reachable GeometryLists are collected in a flat
   list)
 */
Geometry::Geometries GeometryList::flatten() const
{
  Geometries newchildren;
  ::flatten(*this, newchildren);
  return newchildren;
}

std::string GeometryList::toString() const
{
  std::ostringstream os;
  os << "GeometryList<\n";
  bool start = true;
  for (const auto& item : this->children) {
    if(!start) os << ",\n";
    if(item.second) os << "    " << item.second->toString() << "\n";
    else os << "    No geometry\n";
    if(item.first) os << "      (" << item.first->toString() << ")";
    start = false;
  }
  os << ">";
  return os.str();
}

