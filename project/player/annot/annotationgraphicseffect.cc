// annotationgraphicseffect.cc
// 5/3/2012

#include "annotationgraphicseffect.h"

// - Construction -

AnnotationGraphicsEffect::AnnotationGraphicsEffect(QObject *parent)
  : Base(parent)
{
  // Halo
#ifdef Q_WS_WIN
  enum { offset = 1, radius = 18 };
#else
  enum { offset = 1, radius = 12 };
#endif // Q_WS_WIN
  setBlurRadius(radius); // in pixels
  setOffset(offset); // in pixels
  setColor(Qt::black);

  // Transparency
  //setOpacity(0.8);
}

// EOF
