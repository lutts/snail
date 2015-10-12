// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "src/qtui/ui/kbnode_provider_view.h"

#include <QDialog>
#include <QFormLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>

#include "utils/basic_utils.h"
#include "qtui/i_kbnode_tree_qmodel.h"

class KbNodeProviderViewImpl : public QDialog {
  Q_OBJECT

 public:
  KbNodeProviderViewImpl();
  virtual ~KbNodeProviderViewImpl() = default;

  void setProviderName(const QString& provider_name) {
    setWindowTitle(tr("Add %1").arg(provider_name));
  }

 private:
  SNAIL_DISABLE_COPY(KbNodeProviderViewImpl);

  QTreeView* tree_view_;
  QLineEdit* kbnode_name_editor_;
  QPushButton *add_button;

  KbNodeProviderView* q_ptr;
  friend class KbNodeProviderView;
};

#include "kbnode_provider_view.moc"

KbNodeProviderViewImpl::KbNodeProviderViewImpl()
    : QDialog(nullptr) {
  auto layout_ = new QFormLayout(this);

  tree_view_ = new QTreeView(this);
  tree_view_->header()->hide();
  layout_->addRow(tr("Parent Node: "), tree_view_);

  kbnode_name_editor_ = new QLineEdit(this);
  connect(kbnode_name_editor_, &QLineEdit::textEdited,
          [this](const QString &text) {
            q_ptr->NewKbNodeNameChanged(text);
          });
  layout_->addRow(tr("Node Name: "), kbnode_name_editor_);

  auto category_checkbox = new QCheckBox(this);
  connect(category_checkbox, &QCheckBox::stateChanged,
          [this](int state) {
            q_ptr->UserToggleCategoryCheckbox(state == Qt::Checked);
          });
  layout_->addRow(tr("Category: "), category_checkbox);

  // v_layout->addStretch();

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
  buttonBox->setObjectName(QStringLiteral("buttonBox"));
  buttonBox->setOrientation(Qt::Horizontal);
  buttonBox->setStandardButtons(
      QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
  buttonBox->setCenterButtons(true);
  add_button = buttonBox->button(QDialogButtonBox::Ok);
  add_button->setText(tr("Add"));
  add_button->setEnabled(true);

  QObject::connect(buttonBox, &QDialogButtonBox::accepted,
                   [this]() {
                     q_ptr->UserClickAddButton();
                   });
  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  layout_->addRow(buttonBox);
}

KbNodeProviderView::KbNodeProviderView()
    : impl(utils::make_unique<KbNodeProviderViewImpl>()) {
  impl->q_ptr = this;
}

KbNodeProviderView::~KbNodeProviderView() = default;

bool KbNodeProviderView::showView(bool modal) {
  (void)modal;
  return impl->exec() == QDialog::Accepted;
}

void KbNodeProviderView::setProviderName(const QString& provider_name) {
  impl->setProviderName(provider_name);
}

void KbNodeProviderView::setNewKbNodeName(const QString& name) {
  impl->kbnode_name_editor_->setText(name);
}

void KbNodeProviderView::setKbNodeTreeQModel(IKbNodeTreeQModel* tree_model) {
  auto model = dynamic_cast<QAbstractItemModel*>(tree_model);
  if (!model)
    return;

  impl->tree_view_->setModel(model);
  impl->tree_view_->expandToDepth(0);
  QObject::connect(
      impl->tree_view_->selectionModel(),
      &QItemSelectionModel::currentChanged,
      [this](const QModelIndex & current, const QModelIndex & previous) {
        (void)previous;
        UserSelectIndex(current);
      });
}

void KbNodeProviderView::selectIndex(const QModelIndex& index) {
  impl->tree_view_->setCurrentIndex(index);
}

void KbNodeProviderView::setNameValidateResult(bool result) {
  impl->add_button->setEnabled(result);
}
