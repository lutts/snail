// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef QTABSTRACTPROPERTYMANAGER_H_
#define QTABSTRACTPROPERTYMANAGER_H_

#include "common.h"
#include <QLineEdit>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

typedef QLineEdit::EchoMode EchoMode;

class QtAbstractPropertyManager;
class QtPropertyPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtProperty
{
public:
  virtual ~QtProperty();

  QList<QtProperty *> subProperties() const;

  QtAbstractPropertyManager *propertyManager() const;

  QString toolTip() const;
  QString statusTip() const;
  QString whatsThis() const;
  QString propertyName() const;
  bool isEnabled() const;
  bool isModified() const;

  bool hasValue() const;
  QIcon valueIcon() const;
  QString valueText() const;
  QString displayText() const;

  void setToolTip(const QString &text);
  void setStatusTip(const QString &text);
  void setWhatsThis(const QString &text);
  void setPropertyName(const QString &text);
  void setEnabled(bool enable);
  void setModified(bool modified);

  void addSubProperty(QtProperty *property);
  void insertSubProperty(QtProperty *property, QtProperty *afterProperty);
  void removeSubProperty(QtProperty *property);
protected:
  explicit QtProperty(QtAbstractPropertyManager *manager);
  void propertyChanged();
private:
  friend class QtAbstractPropertyManager;
  QtPropertyPrivate *d_ptr;
};

class QtAbstractPropertyManagerPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtAbstractPropertyManager : public QObject
{
  Q_OBJECT
public:

  explicit QtAbstractPropertyManager(QObject *parent = 0);
  ~QtAbstractPropertyManager();

  QSet<QtProperty *> properties() const;
  void clear() const;

  QtProperty *addProperty(const QString &name = QString());
Q_SIGNALS:

  void propertyInserted(QtProperty *property,
                        QtProperty *parent, QtProperty *after);
  void propertyChanged(QtProperty *property);
  void propertyRemoved(QtProperty *property, QtProperty *parent);
  void propertyDestroyed(QtProperty *property);
protected:
  virtual bool hasValue(const QtProperty *property) const;
  virtual QIcon valueIcon(const QtProperty *property) const;
  virtual QString valueText(const QtProperty *property) const;
  virtual QString displayText(const QtProperty *property) const;
  virtual EchoMode echoMode(const QtProperty *) const;
  virtual void initializeProperty(QtProperty *property) = 0;
  virtual void uninitializeProperty(QtProperty *property);
  virtual QtProperty *createProperty();
private:
  friend class QtProperty;
  QtAbstractPropertyManagerPrivate *d_ptr;
  Q_DECLARE_PRIVATE(QtAbstractPropertyManager)
  Q_DISABLE_COPY(QtAbstractPropertyManager)
      };

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif  // QTABSTRACTPROPERTYMANAGER_H_
