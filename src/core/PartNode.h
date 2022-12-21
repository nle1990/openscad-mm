#pragma once

#include "node.h"
#include "linalg.h"

class PartNode : public AbstractNode
{
public:
  VISITABLE();
  PartNode(const ModuleInstantiation *mi) : AbstractNode(mi), partName() { }
  std::string toString() const override;
  std::string name() const override;

  std::string partName;
};
