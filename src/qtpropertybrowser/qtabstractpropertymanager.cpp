// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]
#include "qtabstractpropertymanager.h"

#include <QSet>
#include <QList>
#include <QIcon>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtPropertyPrivate
{
 public:
  QtPropertyPrivate(QtAbstractPropertyManager *manager)
      : m_enabled(true),
        m_modified(false),
        m_manager(manager) {}
  QtProperty *q_ptr;

  QSet<QtProperty *> m_parentItems;
  QList<QtProperty *> m_subItems;

  QString m_toolTip;
  QString m_statusTip;
  QString m_whatsThis;
  QString m_name;
  bool m_enabled;
  bool m_modified;

  QtAbstractPropertyManager * const m_manager;
};

class QtAbstractPropertyManagerPrivate
{
  QtAbstractPropertyManager *q_ptr;
  Q_DECLARE_PUBLIC(QtAbstractPropertyManager)
 public:
  void propertyDestroyed(QtProperty *property);
  void propertyChanged(QtProperty *property) const;
  void propertyRemoved(QtProperty *property,
                       QtProperty *parentProperty) const;
  void propertyInserted(QtProperty *property, QtProperty *parentProperty,
                        QtProperty *afterProperty) const;

  QSet<QtProperty *> m_properties;
};

/*!
  \class QtProperty

  \brief The QtProperty class encapsulates an instance of a property.

  Properties are created by objects of QtAbstractPropertyManager
  subclasses; a manager can create properties of a given type, and
  is used in conjunction with the QtAbstractPropertyBrowser class. A
  property is always owned by the manager that created it, which can
  be retrieved using the propertyManager() function.

  QtProperty contains the most common property attributes, and
  provides functions for retrieving as well as setting their values:

  \table
  \header \o Getter \o Setter
  \row
  \o propertyName() \o setPropertyName()
  \row
  \o statusTip() \o setStatusTip()
  \row
  \o toolTip() \o setToolTip()
  \row
  \o whatsThis() \o setWhatsThis()
  \row
  \o isEnabled() \o setEnabled()
  \row
  \o isModified() \o setModified()
  \row
  \o valueText() \o Nop
  \row
  \o valueIcon() \o Nop
  \endtable

  It is also possible to nest properties: QtProperty provides the
  addSubProperty(), insertSubProperty() and removeSubProperty() functions to
  manipulate the set of subproperties. Use the subProperties()
  function to retrieve a property's current set of subproperties.
  Note that nested properties are not owned by the parent property,
  i.e. each subproperty is owned by the manager that created it.

  \sa QtAbstractPropertyManager, QtBrowserItem
*/

/*!
  Creates a property with the given \a manager.

  This constructor is only useful when creating a custom QtProperty
  subclass (e.g. QtVariantProperty). To create a regular QtProperty
  object, use the QtAbstractPropertyManager::addProperty()
  function instead.

  \sa QtAbstractPropertyManager::addProperty()
*/
QtProperty::QtProperty(QtAbstractPropertyManager *manager)
{
  d_ptr = new QtPropertyPrivate(manager);
  d_ptr->q_ptr = this;
}

/*!
  Destroys this property.

  Note that subproperties are detached but not destroyed, i.e. they
  can still be used in another context.

  \sa QtAbstractPropertyManager::clear()

*/
QtProperty::~QtProperty()
{
  QSetIterator<QtProperty *> itParent(d_ptr->m_parentItems);
  while (itParent.hasNext()) {
    QtProperty *property = itParent.next();
    property->d_ptr->m_manager->d_ptr->propertyRemoved(this, property);
  }

  d_ptr->m_manager->d_ptr->propertyDestroyed(this);

  QListIterator<QtProperty *> itChild(d_ptr->m_subItems);
  while (itChild.hasNext()) {
    QtProperty *property = itChild.next();
    property->d_ptr->m_parentItems.remove(this);
  }

  itParent.toFront();
  while (itParent.hasNext()) {
    QtProperty *property = itParent.next();
    property->d_ptr->m_subItems.removeAll(this);
  }
  delete d_ptr;
}

/*!
  Returns the set of subproperties.

  Note that subproperties are not owned by \e this property, but by
  the manager that created them.

  \sa insertSubProperty(), removeSubProperty()
*/
QList<QtProperty *> QtProperty::subProperties() const
{
  return d_ptr->m_subItems;
}

/*!
  Returns a pointer to the manager that owns this property.
*/
QtAbstractPropertyManager *QtProperty::propertyManager() const
{
  return d_ptr->m_manager;
}

/*!
  Returns the property's  tool tip.

  \sa setToolTip()
*/
QString QtProperty::toolTip() const
{
  return d_ptr->m_toolTip;
}

/*!
  Returns the property's status tip.

  \sa setStatusTip()
*/
QString QtProperty::statusTip() const
{
  return d_ptr->m_statusTip;
}

/*!
  Returns the property's "What's This" help text.

  \sa setWhatsThis()
*/
QString QtProperty::whatsThis() const
{
  return d_ptr->m_whatsThis;
}

/*!
  Returns the property's name.

  \sa setPropertyName()
*/
QString QtProperty::propertyName() const
{
  return d_ptr->m_name;
}

/*!
  Returns whether the property is enabled.

  \sa setEnabled()
*/
bool QtProperty::isEnabled() const
{
  return d_ptr->m_enabled;
}

/*!
  Returns whether the property is modified.

  \sa setModified()
*/
bool QtProperty::isModified() const
{
  return d_ptr->m_modified;
}

/*!
  Returns whether the property has a value.

  \sa QtAbstractPropertyManager::hasValue()
*/
bool QtProperty::hasValue() const
{
  return d_ptr->m_manager->hasValue(this);
}

/*!
  Returns an icon representing the current state of this property.

  If the given property type can not generate such an icon, this
  function returns an invalid icon.

  \sa QtAbstractPropertyManager::valueIcon()
*/
QIcon QtProperty::valueIcon() const
{
  return d_ptr->m_manager->valueIcon(this);
}

/*!
  Returns a string representing the current state of this property.

  If the given property type can not generate such a string, this
  function returns an empty string.

  \sa QtAbstractPropertyManager::valueText()
*/
QString QtProperty::valueText() const
{
  return d_ptr->m_manager->valueText(this);
}

/*!
  Returns the display text according to the echo-mode set on the editor.

  When the editor is a QLineEdit, this will return a string equal to what
  is displayed.

  \sa QtAbstractPropertyManager::valueText()
*/
QString QtProperty::displayText() const
{
  return d_ptr->m_manager->displayText(this);
}

/*!
  Sets the property's tool tip to the given \a text.

  \sa toolTip()
*/
void QtProperty::setToolTip(const QString &text)
{
  if (d_ptr->m_toolTip == text)
    return;

  d_ptr->m_toolTip = text;
  propertyChanged();
}

/*!
  Sets the property's status tip to the given \a text.

  \sa statusTip()
*/
void QtProperty::setStatusTip(const QString &text)
{
  if (d_ptr->m_statusTip == text)
    return;

  d_ptr->m_statusTip = text;
  propertyChanged();
}

/*!
  Sets the property's "What's This" help text to the given \a text.

  \sa whatsThis()
*/
void QtProperty::setWhatsThis(const QString &text)
{
  if (d_ptr->m_whatsThis == text)
    return;

  d_ptr->m_whatsThis = text;
  propertyChanged();
}

/*!
  \fn void QtProperty::setPropertyName(const QString &name)

  Sets the property's  name to the given \a name.

  \sa propertyName()
*/
void QtProperty::setPropertyName(const QString &text)
{
  if (d_ptr->m_name == text)
    return;

  d_ptr->m_name = text;
  propertyChanged();
}

/*!
  Enables or disables the property according to the passed \a enable value.

  \sa isEnabled()
*/
void QtProperty::setEnabled(bool enable)
{
  if (d_ptr->m_enabled == enable)
    return;

  d_ptr->m_enabled = enable;
  propertyChanged();
}

/*!
  Sets the property's modified state according to the passed \a modified value.

  \sa isModified()
*/
void QtProperty::setModified(bool modified)
{
  if (d_ptr->m_modified == modified)
    return;

  d_ptr->m_modified = modified;
  propertyChanged();
}

/*!
  Appends the given \a property to this property's subproperties.

  If the given \a property already is added, this function does
  nothing.

  \sa insertSubProperty(), removeSubProperty()
*/
void QtProperty::addSubProperty(QtProperty *property)
{
  QtProperty *after = 0;
  if (d_ptr->m_subItems.count() > 0)
    after = d_ptr->m_subItems.last();
  insertSubProperty(property, after);
}

/*!
  \fn void QtProperty::insertSubProperty(QtProperty *property, QtProperty *precedingProperty)

  Inserts the given \a property after the specified \a
  precedingProperty into this property's list of subproperties.  If
  \a precedingProperty is 0, the specified \a property is inserted
  at the beginning of the list.

  If the given \a property already is inserted, this function does
  nothing.

  \sa addSubProperty(), removeSubProperty()
*/
void QtProperty::insertSubProperty(QtProperty *property,
                                   QtProperty *afterProperty)
{
  if (!property)
    return;

  if (property == this)
    return;

  // traverse all children of item. if this item is a child of item then cannot add.
  QList<QtProperty *> pendingList = property->subProperties();
  QMap<QtProperty *, bool> visited;
  while (!pendingList.isEmpty()) {
    QtProperty *i = pendingList.first();
    if (i == this)
      return;
    pendingList.removeFirst();
    if (visited.contains(i))
      continue;
    visited[i] = true;
    pendingList += i->subProperties();
  }

  pendingList = subProperties();
  int pos = 0;
  int newPos = 0;
  QtProperty *properAfterProperty = 0;
  while (pos < pendingList.count()) {
    QtProperty *i = pendingList.at(pos);
    if (i == property)
      return; // if item is already inserted in this item then cannot add.
    if (i == afterProperty) {
      newPos = pos + 1;
      properAfterProperty = afterProperty;
    }
    pos++;
  }

  d_ptr->m_subItems.insert(newPos, property);
  property->d_ptr->m_parentItems.insert(this);

  d_ptr->m_manager->d_ptr->propertyInserted(property, this, properAfterProperty);
}

/*!
  Removes the given \a property from the list of subproperties
  without deleting it.

  \sa addSubProperty(), insertSubProperty()
*/
void QtProperty::removeSubProperty(QtProperty *property)
{
  if (!property)
    return;

  d_ptr->m_manager->d_ptr->propertyRemoved(property, this);

  QList<QtProperty *> pendingList = subProperties();
  int pos = 0;
  while (pos < pendingList.count()) {
    if (pendingList.at(pos) == property) {
      d_ptr->m_subItems.removeAt(pos);
      property->d_ptr->m_parentItems.remove(this);

      return;
    }
    pos++;
  }
}

/*!
  \internal
*/
void QtProperty::propertyChanged()
{
  d_ptr->m_manager->d_ptr->propertyChanged(this);
}

////////////////////////////////

void QtAbstractPropertyManagerPrivate::propertyDestroyed(QtProperty *property)
{
  if (m_properties.contains(property)) {
    emit q_ptr->propertyDestroyed(property);
    q_ptr->uninitializeProperty(property);
    m_properties.remove(property);
  }
}

void QtAbstractPropertyManagerPrivate::propertyChanged(QtProperty *property) const
{
  emit q_ptr->propertyChanged(property);
}

void QtAbstractPropertyManagerPrivate::propertyRemoved(QtProperty *property,
                                                       QtProperty *parentProperty) const
{
  emit q_ptr->propertyRemoved(property, parentProperty);
}

void QtAbstractPropertyManagerPrivate::propertyInserted(QtProperty *property,
                                                        QtProperty *parentProperty, QtProperty *afterProperty) const
{
  emit q_ptr->propertyInserted(property, parentProperty, afterProperty);
}

/*!
  \class QtAbstractPropertyManager

  \brief The QtAbstractPropertyManager provides an interface for
  property managers.

  A manager can create and manage properties of a given type, and is
  used in conjunction with the QtAbstractPropertyBrowser class.

  When using a property browser widget, the properties are created
  and managed by implementations of the QtAbstractPropertyManager
  class. To ensure that the properties' values will be displayed
  using suitable editing widgets, the managers are associated with
  objects of QtAbstractEditorFactory subclasses. The property browser
  will use these associations to determine which factories it should
  use to create the preferred editing widgets.

  The QtAbstractPropertyManager class provides common functionality
  like creating a property using the addProperty() function, and
  retrieving the properties created by the manager using the
  properties() function. The class also provides signals that are
  emitted when the manager's properties change: propertyInserted(),
  propertyRemoved(), propertyChanged() and propertyDestroyed().

  QtAbstractPropertyManager subclasses are supposed to provide their
  own type specific API. Note that several ready-made
  implementations are available:

  \list
  \o QtBoolPropertyManager
  \o QtColorPropertyManager
  \o QtDatePropertyManager
  \o QtDateTimePropertyManager
  \o QtDoublePropertyManager
  \o QtEnumPropertyManager
  \o QtFlagPropertyManager
  \o QtFontPropertyManager
  \o QtGroupPropertyManager
  \o QtIntPropertyManager
  \o QtPointPropertyManager
  \o QtRectPropertyManager
  \o QtSizePropertyManager
  \o QtSizePolicyPropertyManager
  \o QtStringPropertyManager
  \o QtTimePropertyManager
  \o QtVariantPropertyManager
  \endlist

  \sa QtAbstractEditorFactoryBase, QtAbstractPropertyBrowser, QtProperty
*/

/*!
  \fn void QtAbstractPropertyManager::propertyInserted(QtProperty *newProperty,
  QtProperty *parentProperty, QtProperty *precedingProperty)

  This signal is emitted when a new subproperty is inserted into an
  existing property, passing pointers to the \a newProperty, \a
  parentProperty and \a precedingProperty as parameters.

  If \a precedingProperty is 0, the \a newProperty was inserted at
  the beginning of the \a parentProperty's subproperties list.

  Note that signal is emitted only if the \a parentProperty is created
  by this manager.

  \sa QtAbstractPropertyBrowser::itemInserted()
*/

/*!
  \fn void QtAbstractPropertyManager::propertyChanged(QtProperty *property)

  This signal is emitted whenever a property's data changes, passing
  a pointer to the \a property as parameter.

  Note that signal is only emitted for properties that are created by
  this manager.

  \sa QtAbstractPropertyBrowser::itemChanged()
*/

/*!
  \fn void QtAbstractPropertyManager::propertyRemoved(QtProperty *property, QtProperty *parent)

  This signal is emitted when a subproperty is removed, passing
  pointers to the removed \a property and the \a parent property as
  parameters.

  Note that signal is emitted only when the \a parent property is
  created by this manager.

  \sa QtAbstractPropertyBrowser::itemRemoved()
*/

/*!
  \fn void QtAbstractPropertyManager::propertyDestroyed(QtProperty *property)

  This signal is emitted when the specified \a property is about to
  be destroyed.

  Note that signal is only emitted for properties that are created
  by this manager.

  \sa clear(), uninitializeProperty()
*/

/*!
  \fn void QtAbstractPropertyBrowser::currentItemChanged(QtBrowserItem *current)

  This signal is emitted when the current item changes. The current item is specified by \a current.

  \sa QtAbstractPropertyBrowser::setCurrentItem()
*/

/*!
  Creates an abstract property manager with the given \a parent.
*/
QtAbstractPropertyManager::QtAbstractPropertyManager(QObject *parent)
    : QObject(parent)
{
  d_ptr = new QtAbstractPropertyManagerPrivate;
  d_ptr->q_ptr = this;

}

/*!
  Destroys the manager. All properties created by the manager are
  destroyed.
*/
QtAbstractPropertyManager::~QtAbstractPropertyManager()
{
  clear();
  delete d_ptr;
}

/*!
  Destroys all the properties that this manager has created.

  \sa propertyDestroyed(), uninitializeProperty()
*/
void QtAbstractPropertyManager::clear() const
{
  while (!properties().isEmpty()) {
    QSetIterator<QtProperty *> itProperty(properties());
    QtProperty *prop = itProperty.next();
    delete prop;
  }
}

/*!
  Returns the set of properties created by this manager.

  \sa addProperty()
*/
QSet<QtProperty *> QtAbstractPropertyManager::properties() const
{
  return d_ptr->m_properties;
}

/*!
  Returns whether the given \a property has a value.

  The default implementation of this function returns true.

  \sa QtProperty::hasValue()
*/
bool QtAbstractPropertyManager::hasValue(const QtProperty *property) const
{
  Q_UNUSED(property)
      return true;
}

/*!
  Returns an icon representing the current state of the given \a
  property.

  The default implementation of this function returns an invalid
  icon.

  \sa QtProperty::valueIcon()
*/
QIcon QtAbstractPropertyManager::valueIcon(const QtProperty *property) const
{
  Q_UNUSED(property)
      return QIcon();
}

/*!
  Returns a string representing the current state of the given \a
  property.

  The default implementation of this function returns an empty
  string.

  \sa QtProperty::valueText()
*/
QString QtAbstractPropertyManager::valueText(const QtProperty *property) const
{
  Q_UNUSED(property)
      return QString();
}

/*!
  Returns a string representing the current state of the given \a
  property.

  The default implementation of this function returns an empty
  string.

  \sa QtProperty::valueText()
*/
QString QtAbstractPropertyManager::displayText(const QtProperty *property) const
{
  Q_UNUSED(property)
      return QString();
}

/*!
  Returns the echo mode representing the current state of the given \a
  property.

  The default implementation of this function returns QLineEdit::Normal.

  \sa QtProperty::valueText()
*/
EchoMode QtAbstractPropertyManager::echoMode(const QtProperty *property) const
{
  Q_UNUSED(property)
      return QLineEdit::Normal;
}

/*!
  Creates a property with the given \a name which then is owned by this manager.

  Internally, this function calls the createProperty() and
  initializeProperty() functions.

  \sa initializeProperty(), properties()
*/
QtProperty *QtAbstractPropertyManager::addProperty(const QString &name)
{
  QtProperty *property = createProperty();
  if (property) {
    property->setPropertyName(name);
    d_ptr->m_properties.insert(property);
    initializeProperty(property);
  }
  return property;
}

/*!
  Creates a property.

  The base implementation produce QtProperty instances; Reimplement
  this function to make this manager produce objects of a QtProperty
  subclass.

  \sa addProperty(), initializeProperty()
*/
QtProperty *QtAbstractPropertyManager::createProperty()
{
  return new QtProperty(this);
}

/*!
  \fn void QtAbstractPropertyManager::initializeProperty(QtProperty *property) = 0

  This function is called whenever a new valid property pointer has
  been created, passing the pointer as parameter.

  The purpose is to let the manager know that the \a property has
  been created so that it can provide additional attributes for the
  new property, e.g. QtIntPropertyManager adds \l
  {QtIntPropertyManager::value()}{value}, \l
  {QtIntPropertyManager::minimum()}{minimum} and \l
  {QtIntPropertyManager::maximum()}{maximum} attributes. Since each manager
  subclass adds type specific attributes, this function is pure
  virtual and must be reimplemented when deriving from the
  QtAbstractPropertyManager class.

  \sa addProperty(), createProperty()
*/

/*!
  This function is called just before the specified \a property is destroyed.

  The purpose is to let the property manager know that the \a
  property is being destroyed so that it can remove the property's
  additional attributes.

  \sa clear(), propertyDestroyed()
*/
void QtAbstractPropertyManager::uninitializeProperty(QtProperty *property)
{
  Q_UNUSED(property);
}

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#include "moc_qtabstractpropertymanager.cpp"
