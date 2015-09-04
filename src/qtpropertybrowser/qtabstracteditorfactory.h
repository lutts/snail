// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef QTABSTRACTEDITORFACTORY_H_
#define QTABSTRACTEDITORFACTORY_H_

#include "common.h"
#include "qtabstractpropertymanager.h"
#include <QSet>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QT_QTPROPERTYBROWSER_EXPORT QtAbstractEditorFactoryBase : public QObject
{
  Q_OBJECT
public:
  virtual QWidget *createEditor(QtProperty *property, QWidget *parent) = 0;
protected:
  explicit QtAbstractEditorFactoryBase(QObject *parent = 0)
      : QObject(parent) {}

  virtual void breakConnection(QtAbstractPropertyManager *manager) = 0;
protected Q_SLOTS:
  virtual void managerDestroyed(QObject *manager) = 0;

  friend class QtAbstractPropertyBrowser;
};

template <class PropertyManager>
class QtAbstractEditorFactory : public QtAbstractEditorFactoryBase
{
 public:
  explicit QtAbstractEditorFactory(QObject *parent) : QtAbstractEditorFactoryBase(parent) {}
  QWidget *createEditor(QtProperty *property, QWidget *parent)
  {
    QSetIterator<PropertyManager *> it(m_managers);
    while (it.hasNext()) {
      PropertyManager *manager = it.next();
      if (manager == property->propertyManager()) {
        return createEditor(manager, property, parent);
      }
    }
    return 0;
  }
  void addPropertyManager(PropertyManager *manager)
  {
    if (m_managers.contains(manager))
      return;
    m_managers.insert(manager);
    connectPropertyManager(manager);
    connect(manager, SIGNAL(destroyed(QObject *)),
            this, SLOT(managerDestroyed(QObject *)));
  }
  void removePropertyManager(PropertyManager *manager)
  {
    if (!m_managers.contains(manager))
      return;
    disconnect(manager, SIGNAL(destroyed(QObject *)),
               this, SLOT(managerDestroyed(QObject *)));
    disconnectPropertyManager(manager);
    m_managers.remove(manager);
  }
  QSet<PropertyManager *> propertyManagers() const
  {
    return m_managers;
  }
  PropertyManager *propertyManager(QtProperty *property) const
  {
    QtAbstractPropertyManager *manager = property->propertyManager();
    QSetIterator<PropertyManager *> itManager(m_managers);
    while (itManager.hasNext()) {
      PropertyManager *m = itManager.next();
      if (m == manager) {
        return m;
      }
    }
    return 0;
  }
 protected:
  virtual void connectPropertyManager(PropertyManager *manager) = 0;
  virtual QWidget *createEditor(PropertyManager *manager, QtProperty *property,
                                QWidget *parent) = 0;
  virtual void disconnectPropertyManager(PropertyManager *manager) = 0;
  void managerDestroyed(QObject *manager)
  {
    QSetIterator<PropertyManager *> it(m_managers);
    while (it.hasNext()) {
      PropertyManager *m = it.next();
      if (m == manager) {
        m_managers.remove(m);
        return;
      }
    }
  }
 private:
  void breakConnection(QtAbstractPropertyManager *manager)
  {
    QSetIterator<PropertyManager *> it(m_managers);
    while (it.hasNext()) {
      PropertyManager *m = it.next();
      if (m == manager) {
        removePropertyManager(m);
        return;
      }
    }
  }
 private:
  QSet<PropertyManager *> m_managers;
  friend class QtAbstractPropertyEditor;
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif  // QTABSTRACTEDITORFACTORY_H_
