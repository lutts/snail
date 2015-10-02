#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include <QTimer>
#include <QWidget>
#include <QBoxLayout>


namespace QSint
{


class ActionLabel;
class ActionPanelScheme;


/**
    \brief Class representing a single group of actions similar to Windows XP task panels.
    \since 0.2

    \image html ActionGroup.png An example of ActionGroup

    ActionGroup consists from optional header and set of actions represented by ActionLabel.
    It can contain arbitrary widgets as well.
*/

class ActionGroup : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool expandable READ isExpandable WRITE setExpandable)
    Q_PROPERTY(bool header READ hasHeader WRITE setHeader)
    Q_PROPERTY(QString headerText READ headerText WRITE setHeaderText)

public:
    /** Constructor. Creates ActionGroup without header.
      */
    explicit ActionGroup(QWidget *parent = 0);

    /** Constructor. Creates ActionGroup with header's
        text set to \a title, but with no icon.

        If \a expandable set to \a true (default), the group can be expanded/collapsed by the user.
      */
    explicit ActionGroup(const QString& title,
                         bool expandable = true,
                         QWidget *parent = 0);

    /** Constructor. Creates ActionGroup with header's
        text set to \a title and icon set to \a icon.

        If \a expandable set to \a true (default), the group can be expanded/collapsed by the user.
      */
    explicit ActionGroup(const QPixmap& icon,
                         const QString& title,
                         bool expandable = true,
                         QWidget *parent = 0);

    /** Creates action item from the \a action and returns it.

      If \a addToLayout is set to \a true (default),
      the action is added to the default vertical layout, i.e. subsequent
      calls of this function will create several ActionLabels arranged vertically,
      one below another.

      Set \a addToLayout to \a false if you want to add the action to the specified layout manually.
      This allows to do custom actions arrangements, i.e. horizontal etc.

      If \a addStretch is set to \a true (default),
      ActionLabel will be automatically aligned to the left side of the ActionGroup.
      Set \a addStretch to \a false if you want ActionLabel to occupy all the horizontal space.
      */
    ActionLabel* addAction(QAction *action, bool addToLayout = true, bool addStretch = true);

    /** Adds \a label to the group.

      \sa addAction() for the description.
      */
    ActionLabel* addActionLabel(ActionLabel *label, bool addToLayout = true, bool addStretch = true);

    /** Adds \a widget to the group. Returns \a true if it has been added successfully.

      \sa addAction() for the description.
      */
    bool addWidget(QWidget *widget, bool addToLayout = true, bool addStretch = true);

    /** Returns group's layout (QVBoxLayout by default).
      */
  QBoxLayout* groupLayout();

  /** Return header's layout (QHBoxLayout by default).
   *
   * By default, the first layout item is an title actionlabel, if the group is
   * expandable, the second layout item is the up/down button
   */
  QBoxLayout* headerLayout();

    /** Sets the scheme of the panel and all the child groups to \a scheme.

        By default, ActionPanelScheme::defaultScheme() is used.
      */
    void setScheme(ActionPanelScheme *pointer);

    /** Returns \a true if the group is expandable.

      \sa setExpandable().
      */
    bool isExpandable() const;

    /** Returns \a true if the group has header.

      \sa setHeader().
      */
    bool hasHeader() const;

    /** Returns text of the header.
        Only valid if the group has header (see hasHeader()).

      \sa setHeaderText().
      */
    QString headerText() const;

    QSize minimumSizeHint() const;

 signals:
  void editButtonClicked();
  void headerTextChanged(const QString& text);

public slots:
    /** Expands/collapses the group.
        Only valid if the group has header (see hasHeader()).
      */
    void showHide();

    /** Makes the group expandable if \a expandable is set to \a true.

      \sa isExpandable().
      */
    void setExpandable(bool expandable = true);

    /** Enables/disables group's header according to \a enable.

      \sa hasHeader().
      */
    void setHeader(bool enable = true);

    /** Sets text of the header to \a title.
        Only valid if the group has header (see hasHeader()).

      \sa headerText().
      */
  void setHeaderText(const QString & title);

  /** show [edit] button if \a has_edit_button is set to \a true
   */
  void setHasEditButton(bool has_edit_button);

  /** set header title editable by click edit button

      NOTE: should has edit button be true

      \sa setHadEditButton
   */
  void setHeaderTextEditable(bool header_text_editable);

protected slots:
    void processHide();
    void processShow();

protected:
    void init(bool header);

    virtual void paintEvent ( QPaintEvent * event );

    double m_foldStep, m_foldDelta, m_fullHeight, m_tempHeight;
    int m_foldDirection;

    QPixmap m_foldPixmap;

    class TaskHeader *myHeader;
    class TaskGroup *myGroup;
    QWidget *myDummy;

    ActionPanelScheme *myScheme;
};


} // namespace

#endif // ACTIONGROUP_H
