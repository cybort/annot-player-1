#ifndef QTCOCOA_H
#define QTCOCOA_H

// qtcocoa.h
// 7/30/2011

#include <QtCore/QPoint>
#include <QtCore/QString>

// - Type cast -

template <typename To, typename From>
inline To
nsobject_cast(From x)
{ return __undefined_cast__(x); }

#define NSOBJECT_TYPE_REGISTER(_type, _obj) \
  template <> \
  inline _type *nsobject_cast<_type *>(objc_object *obj) \
  { return reinterpret_cast<_type *>(obj); } \
  template <> \
  inline objc_object *nsobject_cast<objc_object *>(_type *obj) \
  { return reinterpret_cast<objc_object *>(obj); } \
  template <> \
  inline objc_object *nsobject_cast<objc_object *>(const _type *obj) \
  { return reinterpret_cast<objc_object *>(const_cast<_type *>(obj)); } \
  template <> \
  inline _type *nsobject_cast<_type *>(_obj *obj) \
  { return reinterpret_cast<_type *>(obj); } \
  template <> \
  inline _obj *nsobject_cast<_obj *>(_type *obj) \
  { return reinterpret_cast<_obj *>(obj); } \
  template <> \
  inline _obj *nsobject_cast<_obj *>(const _type *obj) \
  { return reinterpret_cast<_obj *>(const_cast<_type *>(obj)); }

#define NSBOOL(_cond)   ((_cond) ? YES : NO)

// - NSObject -

struct nsobject_t { };

nsobject_t *nsobject_new();
void nsobject_release(nsobject_t *obj);

// - NSResponder -

struct nsresponder_t : public nsobject_t{ };

nsresponder_t *nsresponder_new();
void nsresponder_release(nsresponder_t *obj);

// - NSString -

struct nsstring_t : public nsobject_t{ };

nsstring_t *nsstring_new();
void nsstring_release(nsstring_t *obj);

//nsstring_t *nsstring_string();
nsstring_t *nsstring_new_with_characters(const ushort *chars, ulong len);

// - NSWindow -

struct nswindow_t : public nsresponder_t { };

nswindow_t *nswindow_new();
void nswindow_release(nswindow_t *obj);

bool nswindow_accepts_mouse_moved_events(const nswindow_t *obj);
void nswindow_set_accepts_mouse_moved_events(nswindow_t *obj, bool t);

// - NSView -

struct nsview_t : public nsresponder_t { };

nsview_t *nsview_new();
void nsview_release(nsview_t *obj);

bool nsview_is_hidden(const nsview_t *obj);
void nsview_set_hidden(nsview_t *obj, bool hidden);

nswindow_t *nsview_window(nsview_t *obj);
const nswindow_t *nsview_window(const nsview_t *obj);

// - QtCocoa -

namespace QtCocoa {

  // - Type Conversion -

  QString NSStringToQString(const nsstring_t *ns);
  nsstring_t *QStringToNSString(const QString &qs);

  // - Mouse and Keyboards -

  ///  Send mouse click event
  void mouseClickEvent(nsview_t *view, const QPoint &pos);

  // - Events -

  bool isModifierKeyPressed(uint mask);
  bool isCommandKeyPressed();
  bool isControlKeyPressed();
  bool isAlternateKeyPressed();
  bool isShiftKeyPressed();
  bool isHelpKeyPressed();
  bool isFunctionKeyPressed();
  bool isCapsLockKeyPressed();

  // - Shell Interaction -

  bool trashFile(const QString &fileName);
  bool trashFileWithFullPath(const QString &fullPath);

} // namespace QtCocoa

#endif // QTCOCOA_H
