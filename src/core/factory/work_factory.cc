// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/core/factory/work_factory.h"

#include <vector>

#include "src/core/work.h"
#include "src/core/kbnode_manager.h"
#include "src/core/kbnode_attribute.h"

namespace snailcore {

std::vector<IKbNode*> addTestKbNodes(fto::KbNodeManager* kbnode_manager) {
  static bool added = false;
  static std::vector<IKbNode*> test_root_kbnodes;

  if (!added) {
    added = true;

    auto test_root_kbnode1 =
        kbnode_manager->addKbNode("TestAttr1", nullptr, true);
    kbnode_manager->addKbNode("just", test_root_kbnode1);
    kbnode_manager->addKbNode("for", test_root_kbnode1);
    kbnode_manager->addKbNode("test", test_root_kbnode1);
    kbnode_manager->addKbNode("only", test_root_kbnode1);
    kbnode_manager->addKbNode("dummies", test_root_kbnode1);

    test_root_kbnodes.push_back(test_root_kbnode1);

    auto test_root_kbnode2 =
        kbnode_manager->addKbNode("TestAttr2", nullptr, true);
    kbnode_manager->addKbNode("tree test", nullptr);
    auto test_root_kbnode2_subnode1 =
        kbnode_manager->addKbNode("SubTree1", test_root_kbnode2, true);
    kbnode_manager->addKbNode("just", test_root_kbnode2_subnode1);
    kbnode_manager->addKbNode("for", test_root_kbnode2_subnode1);
    auto test_root_kbnode2_subnode2 =
        kbnode_manager->addKbNode("SubTree2", test_root_kbnode2, true);
    kbnode_manager->addKbNode("test", test_root_kbnode2_subnode2);
    kbnode_manager->addKbNode("only", test_root_kbnode2_subnode2);
    kbnode_manager->addKbNode("dummies", test_root_kbnode2_subnode2);

    test_root_kbnodes.push_back(test_root_kbnode2);
  }

  return test_root_kbnodes;
}

std::vector<std::unique_ptr<IAttributeSupplier> > createAttrSuppliers(
    KbNodeManager* kbnode_manager) {
  auto root_kbnodes = addTestKbNodes(kbnode_manager);

  std::vector<std::unique_ptr<IAttributeSupplier> > attr_suppliers;

  int max_attr = root_kbnodes.size();
  for (auto root_kbnode : root_kbnodes) {
    attr_suppliers.push_back(
        utils::make_unique<KbNodeAttributeSupplier>(root_kbnode, max_attr--));
  }

  return attr_suppliers;
}

WorkFactory::WorkFactory(KbNodeManager* kbnode_manager)
    : kbnode_manager_{kbnode_manager} {}

fto::Work* WorkFactory::createWork(const utils::U8String& work_name) {
  auto work = new Work;
  work->set_name(work_name);

  work->setAttributeSuppliers(createAttrSuppliers(kbnode_manager_));

  return work;
}

}  // namespace snailcore
