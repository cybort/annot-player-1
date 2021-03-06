#ifndef ANNOTATIONGRAPHICSITEMSCHEDULER_H
#define ANNOTATIONGRAPHICSITEMSCHEDULER_H

// annotationgraphicsitemstyle.h
// 7/16/2011

#include "annotationgraphicsitem.h"
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QSet>
#include <QtCore/QSize>
#include <utility>

QT_FORWARD_DECLARE_CLASS(QWidget)

QT_BEGIN_NAMESPACE
inline uint qHash(const QPoint& p)
{
  // Assume that p.x() and p.y() are less than INT16_MAX (2^15)
  return qHash(p.x() << 16 | p.y());
}
QT_END_NAMESPACE

class SignalHub;
class AnnotationGraphicsItemScheduler : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(AnnotationGraphicsItemScheduler)
  typedef AnnotationGraphicsItemScheduler Self;
  typedef QObject Base;

public:
  struct Item { ///< \internal
    qint64 time; // timestamp of appearance
    int life;   // visible time
    int width;  // item width
  };
  explicit AnnotationGraphicsItemScheduler(SignalHub *hub, QObject *parent = nullptr)
    : Base(parent), hub_(hub), scale_(1.0), pauseTime_(0), resumeTime_(0)
  { Q_ASSERT(hub); clear(); }

  qreal scale() const { return scale_; }

signals:
  void message(const QString &text);

public slots:
  void pause();
  void resume();
  void clear();
  void clearSubtitles();

  void setScale(qreal value) { scale_ = value; }
  void setViewSize(const QSize &value) { viewSize_ = value; }

  // - Float scheduling, lane by lane -
public:
  int nextMovingY(const QSizeF &itemSize, int visibleMsecs);
  int nextStationaryY(int itemHeight, int visibleMsecs, AnnotationGraphicsItem::Style style, bool sub);

  // - Float scheduling, cell by cell -
public:
  QPointF nextFloatPos(const QSizeF &itemSize, int visibleMsecs, bool sub);

private:
  SignalHub *hub_;

  qreal scale_;
  QSize viewSize_;

  qint64 pauseTime_,
         resumeTime_;

  // Schedule by cell
  QHash<QPoint, qint64> cells_; // timestamps for each cell
  std::pair<QPoint, qint64> lastCell_;
  QSet<QPoint> subCells_;

  // Schedule by lane
  enum { LaneCount = 200 }; // number of vertical lanes, large enough
  Item floatLane_[LaneCount];

  qint64 topLaneTime_[LaneCount],
         bottomLaneTime_[LaneCount];
  qint8 subLaneStyle_[LaneCount]; // qint8 must be able to hold AnnotationGraphicsItem::Style
};

#endif // ANNOTATIONGRAPHICSITEMSCHEDULER_H
