/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "MaterialNode.h"
#include "module.h"
#include "ModuleInstantiation.h"
#include "Builtins.h"
#include "Children.h"
#include "Parameters.h"
#include "printutils.h"
#include <cctype>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/regex.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope

static std::shared_ptr<AbstractNode> builtin_material(const ModuleInstantiation *inst, Arguments arguments, Children children)
{
  auto node = std::make_shared<MaterialNode>(inst);

  Parameters parameters = Parameters::parse(std::move(arguments), inst->location(), {"name"});
  if (parameters["name"].type() == Value::Type::STRING) {
    auto materialName = parameters["name"].toString();
    boost::algorithm::to_lower(materialName);
    node->materialName = materialName;
  }
  else if (parameters["name"].type() == Value::Type::NUMBER) {
    node->materialName = parameters["name"].toString();
  }
  boost::regex regex("[^A-Za-z0-9_]");
  std::string cleanName = boost::regex_replace(node->materialName, regex, "");
  if(cleanName != node->materialName) {
        LOG(message_group::Warning, inst->location(), parameters.documentRoot(), "material() expects strings containing alphanumeric characters and underscores only - \"%1s\" will be changed to \"%2s\"", node->materialName, cleanName);
    node->materialName = cleanName;
  }

  return children.instantiate(node);
}

std::string MaterialNode::toString() const
{
  return STR("material(", QuotedString(this->materialName), ")");
}

std::string MaterialNode::name() const
{
  return "material";
}

void register_builtin_material()
{
  Builtins::init("material", new BuiltinModule(builtin_material),
  {
    "material(\"materialname\")",
  });
}
