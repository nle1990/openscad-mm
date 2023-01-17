#pragma once

#include "Cache.h"
#include "memory.h"

class Geometry;

/*!
 */
class CGALCache
{
public:
  CGALCache(size_t limit = 100 *1024 *1024);

  static CGALCache *instance() { if (!inst) inst = new CGALCache; return inst; }
  static bool acceptsGeometry(const shared_ptr<const Geometry>& geom);

  bool contains(const std::string& id) const { return false; return this->cache.contains(id); } //FIXME-MM: reenable, but taking into account attributes
  shared_ptr<const Geometry> get(const std::string& id) const;
  bool insert(const std::string& id, const shared_ptr<const Geometry>& N);
  size_t size() const;
  size_t totalCost() const;
  size_t maxSizeMB() const;
  void setMaxSizeMB(size_t limit);
  void clear();
  void print();

private:
  static CGALCache *inst;

  struct cache_entry {
    shared_ptr<const Geometry> N;
    std::string msg;
    cache_entry(const shared_ptr<const Geometry>& N);
    ~cache_entry() { }
  };

  Cache<std::string, cache_entry> cache;
};
