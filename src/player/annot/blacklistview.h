#ifndef BLACKLISTVIEW_H
#define BLACKLISTVIEW_H

// blacklistview.h
// 11/16/2011

#include "src/common/acwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QLineEdit;
class QStackedLayout;
class QToolButton;
QT_END_NAMESPACE

class AnnotationFilter;

class TextFilterView;
class UserFilterView;
class AnnotationFilterView;

class BlacklistView : public AcWindow
{
  Q_OBJECT
  Q_DISABLE_COPY(BlacklistView)
  typedef BlacklistView Self;
  typedef AcWindow Base;

  // - Types -
protected:
  enum Tab {
    TextTab = 0,
    UserTab,
    AnnotationTab,
    TabCount
  };

  // - Constructions -
public:
  explicit BlacklistView(AnnotationFilter *filter, QWidget *parent = nullptr);

  // - Properties -
protected:
  bool active() const { return active_; }
protected slots:
  void setActive(bool t);
  void connectFilter();
  void disconnectFilter();

  // - Events -
public:
  void setVisible(bool visible) override;

  // - Slots -
public slots:
  void refresh();
protected slots:
  void add();
  void remove();
  void clearCurrentText();

  void setFilterEnabled(bool enabled);

  void setTabToText();
  void setTabToUser();
  void setTabToAnnotation();
  void invalidateTab();

  void invalidateFilter();

  // - Implementations -
protected:
  QLineEdit *currentLineEdit();

private:
  void createTabs();
  void createLayout();
  void createActions();

private:
  bool active_;
  Tab tab_;
  AnnotationFilter *filter_;

  QStackedLayout *stackedLayout_;
  QToolButton *textTabButton_,
              *userTabButton_,
              *annotationTabButton_;
  QToolButton *enableButton_,
              *addButton_,
              *clearButton_,
              *removeButton_;

  TextFilterView *textTab_;
  UserFilterView *userTab_;
  AnnotationFilterView *annotationTab_;
};

#endif // BLACKLISTVIEW_H
