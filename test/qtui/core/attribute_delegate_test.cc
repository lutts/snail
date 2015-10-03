//-*- TestCaseName: AttributeDelegateTest;-*-
// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include <QApplication>
#include <QWidget>

#include "test/testutils/gmock_common.h"
#include "src/qtui/core/attribute_delegate.h"

#include "qtui/i_attribute_editor_view.h"
#include "test/testutils/qmodelindex_generator.h"

static int dummy_argc { 0 };
static QApplication app(dummy_argc, nullptr);

class AttributeDelegateTest : public ::testing::Test {
 protected:
  AttributeDelegateTest() {
    // const string saved_flag = GMOCK_FLAG(verbose);
    GMOCK_FLAG(verbose) = kErrorVerbosity;
  }
  // ~AttributeDelegateTest() { }
  virtual void SetUp() {
  }
  // virtual void TearDown() { }

  // region: objects test subject depends on
  QModelIndexGenerator index_generator;
  // endregion

  // region: test subject
  AttributeDelegate delegate;
  // endregion

  // region: object depends on test subject
  // endregion
};

TEST_F(AttributeDelegateTest, should_construct_properly) { // NOLINT
  // See SetUp()
  SUCCEED();
}

BEGIN_MOCK_LISTENER_DEF(MockListener, IAttributeDelegate)

MOCK_METHOD1(CreateEditorFor,
               IAttributeEditorView*(int row));
MOCK_METHOD1(CloseEditor,
               void(IAttributeEditorView*));

BEGIN_BIND_SIGNAL(IAttributeDelegate)

BIND_SIGNAL1(CreateEditorFor,
             IAttributeEditorView*, int, row);
BIND_SIGNAL1(CloseEditor,
             void, IAttributeEditorView*, view);

END_BIND_SIGNAL()
END_MOCK_LISTENER_DEF()

class AttributeEditorViewTestStub : public IAttributeEditorView
                                  , public QWidget {
 public:
  AttributeEditorViewTestStub() { }
  virtual ~AttributeEditorViewTestStub() { }

  QWidget* getWidget() override {
    auto this_ptr = const_cast<AttributeEditorViewTestStub*>(this);
    return static_cast<QWidget*>(this_ptr);
  }

 private:
  SNAIL_DISABLE_COPY(AttributeEditorViewTestStub)
};

TEST_F(AttributeDelegateTest,
       should_createEditor_emit_CreateEditor_signal) { // NOLINT
  // Setup fixture
  int row = std::rand();
  QModelIndex index = index_generator.index(row, 0);
  QStyleOptionViewItem option;
  AttributeEditorViewTestStub editor_view;

  // Expectations
  auto mock_listener = MockListener::attachTo(&delegate);
  EXPECT_CALL(*mock_listener, CreateEditorFor(row))
      .WillOnce(Return(&editor_view));

  // Exercise system
  auto widget = delegate.createEditor(nullptr, option, index);

  // Verify results
  auto actual_editor_view = dynamic_cast<AttributeEditorViewTestStub*>(widget);
  ASSERT_EQ(&editor_view, actual_editor_view);
}

TEST_F(AttributeDelegateTest,
       should_createEditor_return_nullptr_if_CreateEditor_slot_return_nullptr) { // NOLINT
  // Setup fixture
  int row = std::rand();
  QModelIndex index = index_generator.index(row, 0);
  QStyleOptionViewItem option;

  // Expectations
  auto mock_listener = MockListener::attachTo(&delegate);
  EXPECT_CALL(*mock_listener, CreateEditorFor(row)).WillOnce(Return(nullptr));

  // Exercise system
  auto widget = delegate.createEditor(nullptr, option, index);

  // Verify results
  ASSERT_EQ(nullptr, widget);
}

TEST_F(AttributeDelegateTest,
       should_destroyEditor_emit_CloseEditor_signal) { // NOLINT
  // Setup fixture
  AttributeEditorViewTestStub editor_view;

  // Expectations
  auto mock_listener = MockListener::attachTo(&delegate);
  EXPECT_CALL(*mock_listener, CloseEditor(&editor_view));

  // Exercise system
  delegate.destroyEditor(editor_view.getWidget(), QModelIndex());
}
