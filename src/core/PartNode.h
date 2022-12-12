#pragma once

#include "node.h"
#include "linalg.h"

class PartNode : public AbstractNode
{
public:
  VISITABLE();
  PartNode(const ModuleInstantiation *mi) : AbstractNode(mi), color(-1.0f, -1.0f, -1.0f, 1.0f) { }
  std::string toString() const override;
  std::string name() const override;

  Color4f color;
};
