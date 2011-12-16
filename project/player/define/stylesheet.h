#ifndef STYLESHEET_H
#define STYLESHEET_H

// stylesheet.h
// 7/15/2011
// See: http://doc.qt.nokia.com/stable/stylesheet-examples.html
// See: http://doc.qt.nokia.com/4.7/stylesheet-reference.html

#include "rc.h"
#include "core/ss.h"
#include <QtGlobal>

// - Sliders -

// See: http://blog.sina.com.cn/s/blog_791f544a0100s2ml.html
//#define SS_SLIDER "QSlider::groove:horizontal { border: 1px solid #bbb; background: white; height: 10px; border-radius: 4px; } QSlider::sub-page:horizontal { background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1, stop: 0 #66e, stop: 1 #bbf); background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 #bbf, stop: 1 #55f); border: 1px solid #777; height: 10px; border-radius: 4px; } QSlider::add-page:horizontal { background: #fff; border: 1px solid #777; height: 10px; border-radius: 4px; } QSlider::handle:horizontal { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc); border: 1px solid #777; width: 13px; margin-top: -2px; margin-bottom: -2px; border-radius: 4px; } QSlider::handle:horizontal:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd); border: 1px solid #444; border-radius: 4px; } QSlider::sub-page:horizontal:disabled { background: #bbb; border-color: #999; } QSlider::add-page:horizontal:disabled { background: #eee; border-color: #999; } QSlider::handle:horizontal:disabled { background: #eee; border: 1px solid #aaa; border-radius: 4px; } "
// - groove
// - handle
// - sub-page
// - add-page
#define SS_SLIDER_ \
  SS_BEGIN(QSlider::groove:horizontal) \
    SS_BORDER(1px) \
    SS_HEIGHT(2px) \
    SS_NO_MARGIN \
    SS_LEFT(5px) SS_RIGHT(5px) \
  SS_END \
  SS_BEGIN(QSlider::sub-page:horizontal) \
    SS_BACKGROUND( \
      qlineargradient(spread:pad, \
        x1:0, y1:1, x2:0, y2:0, \
        stop:0    rgba(27, 5, 27, 255), \
        stop:0.25 rgba(99, 20, 102, 255), \
        stop:0.5  rgba(154, 30, 158, 255), \
        stop:1    rgba(173, 57, 176, 255))) \
  SS_END \
  SS_BEGIN(QSlider::add-page:horizontal) \
    SS_BACKGROUND(#555) \
    SS_BORDER(1px solid #777) \
    SS_HEIGHT(1px) \
    SS_LEFT(5px) SS_RIGHT(5px) \
  SS_END \
  SS_BEGIN(QSlider::handle:horizontal) \
    SS_TRANSPARENT \
    SS_MARGIN(-4px) \
    SS_WIDTH(8px) \
    SS_BORDER_RADIUS(4px) \
  SS_END \
  SS_BEGIN(QSlider::handle:horizontal:hover) \
    SS_BACKGROUND( \
      qlineargradient( \
        x1:0, y1:0, x2:1, y2:1, \
        stop:0 #bbf, \
        stop:1 #66e)) \
  SS_END \
  SS_BEGIN(QSlider::handle:horizontal:disabled) \
    SS_TRANSPARENT \
    SS_NO_MARGIN \
    SS_NO_WIDTH \
  SS_END \
  SS_BEGIN(QSlider::sub-page:horizontal:disabled) \
    SS_BACKGROUND(#bbb) \
    SS_BORDER_COLOR(#999) \
  SS_END \
  SS_BEGIN(QSlider::add-page:horizontal:disabled) \
    SS_BACKGROUND(#eee) \
    SS_BORDER_COLOR(#999) \
  SS_END \
  SS_BEGIN(QSlider::handle:horizontal:disabled) \
    SS_BACKGROUND(#eee) \
    SS_BORDER(1px solid #aaa) \
  SS_END

#define SS_SLIDER_POSITION      SS_SLIDER_
#define SS_SLIDER_VOLUME        SS_SLIDER_

// - Buttons -

// Radio buttons

#define SS_RADIOBUTTON_TEXT_(_color) \
  SS_BEGIN(QRadioButton) \
    SS_TRANSPARENT \
    SS_COLOR(_color) \
  SS_END \

#define SS_RADIOBUTTON_TEXT     SS_RADIOBUTTON_TEXT_(blue)

// Tool buttons

#define SS_TOOLBUTTON_(_id) \
  SS_BEGIN(QToolButton) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id) \
  SS_END \
  SS_BEGIN(QToolButton::hover) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id##_HOVER) \
  SS_END \
  SS_BEGIN(QToolButton::pressed) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id##_PRESSED) \
  SS_END \
  SS_BEGIN(QToolButton::checked) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id##_CHECKED) \
  SS_END \
  SS_BEGIN(QToolButton::checked:hover) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id##_CHECKED_HOVER) \
  SS_END \
  SS_BEGIN(QToolButton::disabled) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_##_id##_DISABLED) \
  SS_END

#define SS_TOOLBUTTON_TEXT_(_normal, _hover, _pressed, _checked, _checked_hover, _disabled) \
  SS_BEGIN(QToolButton) \
    SS_TRANSPARENT \
    SS_COLOR(_normal) \
  SS_END \
  SS_BEGIN(QToolButton:hover) \
    SS_COLOR(_hover) \
  SS_END \
  SS_BEGIN(QToolButton:pressed) \
    SS_COLOR(_pressed) \
  SS_END \
  SS_BEGIN(QToolButton:checked) \
    SS_COLOR(_checked) \
  SS_END \
  SS_BEGIN(QToolButton:checked:hover) \
    SS_COLOR(_checked_hover) \
  SS_END \
  SS_BEGIN(QToolButton:disabled) \
    SS_COLOR(_disabled) \
  SS_END

#define SS_TOOLBUTTON_TEXT_NORMAL       SS_TOOLBUTTON_TEXT_(blue, red, red, orange, red, gray)
#define SS_TOOLBUTTON_TEXT_HIGHLIGHT    SS_TOOLBUTTON_TEXT_(red, blue, red, orange, red, gray)
#define SS_TOOLBUTTON_TEXT              SS_TOOLBUTTON_TEXT_NORMAL

#define SS_TOOLBUTTON_PLAY      SS_TOOLBUTTON_(PLAY)
#define SS_TOOLBUTTON_PAUSE     SS_TOOLBUTTON_(PAUSE)
#define SS_TOOLBUTTON_STOP      SS_TOOLBUTTON_(STOP)
#define SS_TOOLBUTTON_OPEN      SS_TOOLBUTTON_(OPEN)
#define SS_TOOLBUTTON_NEXTFRAME SS_TOOLBUTTON_(NEXTFRAME)
#define SS_TOOLBUTTON_MINI      SS_TOOLBUTTON_(MINI)
#define SS_TOOLBUTTON_EMBED     SS_TOOLBUTTON_(EMBED)
#define SS_TOOLBUTTON_FULLSCREEN SS_TOOLBUTTON_(FULLSCREEN)
#define SS_TOOLBUTTON_SNAPSHOT  SS_TOOLBUTTON_(SNAPSHOT)
#define SS_TOOLBUTTON_MENU      SS_TOOLBUTTON_(MENU)
#define SS_TOOLBUTTON_MAXIMIZE  SS_TOOLBUTTON_(MAXIMIZE)
#define SS_TOOLBUTTON_MINIMIZE  SS_TOOLBUTTON_(MINIMIZE)
#define SS_TOOLBUTTON_RESTORE   SS_TOOLBUTTON_(RESTORE)
#define SS_TOOLBUTTON_PREVIOUS  SS_TOOLBUTTON_(PREVIOUS)
#define SS_TOOLBUTTON_NEXT      SS_TOOLBUTTON_(NEXT)

#define SS_TOOLBUTTON_ANNOT     SS_TOOLBUTTON_(ANNOT)
#define SS_TOOLBUTTON_SHOWANNOT SS_TOOLBUTTON_(SHOWANNOT)
#define SS_TOOLBUTTON_HIDEANNOT SS_TOOLBUTTON_(HIDEANNOT)

#define SS_TOOLBUTTON_USER      SS_TOOLBUTTON_TEXT
#define SS_TOOLBUTTON_SEEK      SS_TOOLBUTTON_TEXT

// - Labels -
#define SS_LABEL \
  SS_BEGIN(QLabel) \
    SS_COLOR(blue) \
  SS_END \
  SS_BEGIN(QLabel:hover) \
    SS_COLOR(red) \
  SS_END \
  SS_BEGIN(QLabel:disabled) \
    SS_COLOR(gray) \
  SS_END

// - Lines -

#define SS_LINEEDIT \
  SS_BEGIN(QLineEdit) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_LINEEDIT) \
    SS_BORDER(1px groove gray) \
    SS_BORDER_WIDTH(4px) \
    SS_BORDER_RADIUS(3px) \
    SS_COLOR(black) \
  SS_END

//#define SS_LINEEDIT_OSD
//  SS_BEGIN(QLineEdit)
//    SS_TRANSPARENT
//    SS_BORDER(1px groove purple)
//    SS_COLOR(blue)
//  SS_END
//  SS_BEGIN(QLineEdit::hover)
//    SS_BORDER_IMAGE_URL(RC_IMAGE_LINEEDIT)
//    SS_BORDER(1px groove)
//    SS_BORDER_WIDTH(4px)
//    SS_COLOR(black)
//  SS_END

// - Boxes -

// See: http://doc.qt.nokia.com/stable/stylesheet-examples.html#customizing-qcombobox
// TODO: Change QAbstractItemView style as well!
#define SS_COMBOBOX \
  SS_BEGIN(QComboBox) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_BACKGROUND) \
    SS_BORDER_WIDTH(4px) \
    SS_BORDER_RADIUS(3px) \
  SS_END \
  SS_BEGIN(QComboBox::drop-down) \
    SS_TRANSPARENT \
  SS_END \
  SS_BEGIN(QComboBox::down-arrow) \
    SS_TRANSPARENT \
  SS_END \
  SS_BEGIN(QComboBox::down-arrow:pressed) \
    SS_TRANSPARENT \
  SS_END \
  SS_BEGIN(QComboBox QAbstractItemView) \
  SS_END

//#define SS_COMBOBOX_OSD
//  SS_LINEEDIT_OSD
//  SS_BEGIN(QComboBox)
//    SS_TRANSPARENT
//    SS_BORDER(1px groove purple)
//    SS_COLOR(blue)
//  SS_END
//  SS_BEGIN(QComboBox::drop-down)
//    SS_TRANSPARENT
//  SS_END
//  SS_BEGIN(QComboBox::down-arrow)
//    SS_TRANSPARENT
//  SS_END
//  SS_BEGIN(QComboBox QAbstractItemView)
//    SS_BORDER_IMAGE_URL(RC_IMAGE_BACKGROUND)
//  SS_END

// - Item views -

#define SS_TREEVIEW \
  SS_BEGIN(QTreeView) \
    SS_BACKGROUND_IMAGE_URL(RC_IMAGE_BACKGROUND) \
    SS_BORDER_WIDTH(4px) \
    SS_BORDER_RADIUS(3px) \
  SS_END

// - Graphics views -

#define SS_GRAPHICSVIEW \
  SS_BEGIN(QGraphicsView) \
    SS_TRANSPARENT \
    SS_BORDERLESS \
  SS_END

// - Menus -
#define SS_MENU SS_NULL

// See: http://doc.qt.nokia.com/latest/stylesheet-examples.html#customizing-qmenu
// See: http://drpeterjones.com/colorcalc/
#define SS_CONTEXTMENU \
  SS_BEGIN(QMenu::item) \
    SS_COLOR(orange) \
  SS_END \
  SS_BEGIN(QMenu::item::selected) \
    SS_BACKGROUND( \
      qlineargradient(spread:pad, \
        x1:0, y1:0, x2:1, y2:0, \
        stop:0    rgba(255, 204, 0, 64), \
        stop:0.25 rgba(255, 204, 51, 128), \
        stop:0.5  rgba(255, 204, 102, 64), \
        stop:1    rgba(255, 204, 153, 16))) \
  SS_END
//SS_BEGIN(QMenu)
//SS_BACKGROUND_IMAGE_URL(RC_IMAGE_CONTEXTMENU)
//SS_EN

// - Text edits -

#define SS_TEXTEDIT \
  SS_BEGIN(QTextEdit) \
    SS_BORDER_IMAGE_URL(RC_IMAGE_TEXTEDIT) \
  SS_END

// - Windows -

//#define SS_WINDOW
//  SS_BEGIN(QWidget)
//    SS_BACKGROUND_IMAGE_URL(RC_IMAGE_AERO)
//  SS_END
#define SS_WINDOW     SS_NULL

//#ifdef USE_WIN_DWM
//  #define SS_WINDOW_DWM
//    SS_BEGIN(QWidget)
//      SS_BORDER_RADIUS(8px)
//    SS_END
//#endif // USE_WIN_DWM
//
#define SS_WINDOW_DWM   SS_NULL

// - Customized -

#define SS_BACKGROUND_CLASS(_class) \
  SS_BEGIN(_class) \
    SS_BACKGROUND_IMAGE_URL(RC_IMAGE_AERO) \
  SS_END


#endif // STYLESHEET_H
