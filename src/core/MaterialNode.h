#pragma once

#include "node.h"
#include "linalg.h"

class MaterialNode : public AbstractNode
{
public:
  VISITABLE();
  MaterialNode(const ModuleInstantiation *mi) : AbstractNode(mi), color(-1.0f, -1.0f, -1.0f, 1.0f) { }
  std::string toString() const override;
  std::string name() const override;

  Color4f color;
};
