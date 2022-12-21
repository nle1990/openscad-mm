#pragma once

#include "node.h"
#include "linalg.h"

class MaterialNode : public AbstractNode
{
public:
  VISITABLE();
  MaterialNode(const ModuleInstantiation *mi) : AbstractNode(mi), materialName() { }
  std::string toString() const override;
  std::string name() const override;

  std::string materialName;
};
