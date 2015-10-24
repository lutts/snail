// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef SRC_QTUI_CORE_SIMPLE_KBNODE_ADDER_PRESENTER_H_
#define SRC_QTUI_CORE_SIMPLE_KBNODE_ADDER_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_simple_kbnode_adder_model.h"
#include "qtui/ui/i_simple_kbnode_adder_view.h"
#include "qtui/core/fto_tree_item_qmodel.h"

namespace snailcore {
class IKbNode;
}  // namespace snailcore

using SimpleKbNodeAdderPresenterBase =
    pfmvp::PfPresenterT<snailcore::ISimpleKbNodeAdderModel,
                        ISimpleKbNodeAdderView>;

class SimpleKbNodeAdderPresenter : public SimpleKbNodeAdderPresenterBase {
 public:
  SimpleKbNodeAdderPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::shared_ptr<fto::TreeItemQModel<snailcore::IKbNode>> kbnode_qmodel);
  ~SimpleKbNodeAdderPresenter();

  void initialize() override;

 private:
  SNAIL_DISABLE_COPY(SimpleKbNodeAdderPresenter);

  std::shared_ptr<fto::TreeItemQModel<snailcore::IKbNode>> kbnode_qmodel_;
};

#endif  // SRC_QTUI_CORE_SIMPLE_KBNODE_ADDER_PRESENTER_H_
