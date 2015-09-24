// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef KBNODEPROVIDER_PRESENTER_H_
#define KBNODEPROVIDER_PRESENTER_H_

#include <memory>  // std::shared_ptr

#include "pfmvp/pf_presenter.h"
#include "snail/i_kbnode_provider_model.h"
#include "qtui/i_kbnode_provider_view.h"

using KbNodeProviderPresenterBase =
    pfmvp::PfPresenterT<snailcore::IKbNodeProviderModel,
                        IKbNodeProviderView>;

class KbNodeProviderPresenter : public KbNodeProviderPresenterBase {
 public:
  KbNodeProviderPresenter(
      std::shared_ptr<model_type> model,
      std::shared_ptr<view_type> view,
      std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel);
  ~KbNodeProviderPresenter();

  void initialize() override;

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderPresenter);

  std::unique_ptr<IKbNodeTreeQModel> kbnode_qmodel_;
};

#endif  // KBNODEPROVIDER_PRESENTER_H_
