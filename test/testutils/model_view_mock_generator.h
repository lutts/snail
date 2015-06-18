// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef TEST_TESTUTILS_MODEL_VIEW_MOCK_GENERATOR_H_
#define TEST_TESTUTILS_MODEL_VIEW_MOCK_GENERATOR_H_

#include <utility>  // std::pair
#include <memory>
#include <list>

class IModelViewPair {
 public:
  virtual ~IModelViewPair() = default;
};

template <typename TModel, typename TView>
class ModelViewPair : public IModelViewPair {
 public:
  ~ModelViewPair() {
    reset();
  }

  TModel* model() {
    createModelIfNotExist();
    return model_.get();
  }

  std::shared_ptr<TModel> shared_model() {
    createModelIfNotExist();
    return model_;
  }

  TView* view() {
    createViewIfNotExist();
    return view_.get();
  }

  std::shared_ptr<TView> shared_view() {
    createViewIfNotExist();
    return view_;
  }

  std::pair<TModel*, TView*> toStdPair() {
    return std::make_pair(model(), view());
  }

  void reset() {
    model_.reset();
    view_.reset();
  }

 private:
  void createModelIfNotExist() {
    if (!model_)
      model_ = std::make_shared<TModel>();
  }

  void createViewIfNotExist() {
    if (!view_)
      view_ = std::make_shared<TView>();
  }

  std::shared_ptr<TModel> model_;
  std::shared_ptr<TView> view_;
};

template <typename TModel, typename TView,
          template<typename, typename> class Tmvpair = ModelViewPair>
class ModelViewMockGenerator {
 public:
  ModelViewMockGenerator() = default;

  std::shared_ptr<Tmvpair<TModel, TView> > make_mvpair() {
    auto mvpair = std::make_shared<Tmvpair<TModel, TView> >();
    mvpair_list.push_back(mvpair);
    return mvpair;
  }

 private:
  ModelViewMockGenerator(const ModelViewMockGenerator&) = delete;
  ModelViewMockGenerator& operator=(const ModelViewMockGenerator&) = delete;

  std::list<std::shared_ptr<IModelViewPair> > mvpair_list;
};

#endif  // TEST_TESTUTILS_MODEL_VIEW_MOCK_GENERATOR_H_
