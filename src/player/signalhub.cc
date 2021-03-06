// signalhub.h
// 10/16/2011
#include "signalhub.h"
#include "lib/player/player.h"
#include <QtCore>

//#define DEBUG "signalhub"
//#include "qtx/qxdebug.h"

// - Constructions -
SignalHub::SignalHub(Player *player, QObject *parent)
  : Base(parent),
    player_(player),
    tokenMode_(MediaTokenMode),
    playMode_(NormalPlayMode),
    playerMode_(NormalPlayerMode),
    windowMode_(NormalWindowMode),
    playing_(false), paused_(false), stopped_(true)
{ Q_ASSERT(player_); }

// - Properties -

qreal
SignalHub::volume() const
{
  qreal ret = 0;
  switch (tokenMode_) {
  case MediaTokenMode:
    ret = player_->isValid() ? player_->volume() : 0;
    break;

  case SignalTokenMode:
    break;

  default: Q_ASSERT(0);
  }

#ifdef Q_OS_LINUX
  // de-expand percentage
  if (ret > 0)
    ret = qSqrt(ret);
#endif // Q_OS_LINUX
  return ret;
}

void
SignalHub::setVolume(qreal percentage)
{
#ifdef Q_OS_LINUX
  // expand percentage
  percentage = percentage * percentage;
#endif // Q_OS_LINUX

  if (percentage < 0)
    percentage = 0;
  else if (percentage > 1)
    percentage = 1;

  switch (tokenMode_) {
  case MediaTokenMode:
    if (player_->isValid()) {
      player_->setVolume(percentage);
      emit volumeChanged(percentage);
    } break;

  case LiveTokenMode:
  case SignalTokenMode:
#ifdef Q_OS_WIN
    //QtWin::setWaveVolume(percentage);
    emit volumeChanged(percentage);
#endif // Q_OS_WIN
    break;
  }
}

// - States -

// + TokenMode +

void
SignalHub::setTokenMode(TokenMode mode)
{
  if (tokenMode_ != mode) {
    if (!isStopped())
      stop();

    tokenMode_ = mode;
    switch (mode) {
    case MediaTokenMode:  emit message(tr("enter media mode")); break;
    case LiveTokenMode:   Q_ASSERT(0); break;
    case SignalTokenMode: emit message(tr("enter game mode")); break;
    }
    emit tokenModeChanged(mode);
  }
}

void
SignalHub::setMediaTokenMode(bool t)
{ setTokenMode(t ? MediaTokenMode : SignalTokenMode); }

void
SignalHub::setLiveTokenMode(bool t)
{ setTokenMode(t ? LiveTokenMode : MediaTokenMode); }

void
SignalHub::setSignalTokenMode(bool t)
{ setTokenMode(t ? SignalTokenMode : MediaTokenMode); }

// + PlayMode +

void
SignalHub::setPlayerMode(PlayerMode mode)
{
  if (playerMode_ != mode) {
    playerMode_ = mode;

#ifdef DEBUG
    switch (mode) {
    case NormalPlayerMode: emit message(tr("switched to normal player mode")); break;
    case EmbeddedPlayerMode: emit message(tr("switched to embedded player mode")); break;
    case MiniPlayerMode: emit message(tr("switched to mini player mode")); break;
    }
#endif // DEBUG
    emit playerModeChanged(mode);
  }
}

void
SignalHub::setNormalPlayerMode()
{ setPlayerMode(NormalPlayerMode); }

void
SignalHub::setMiniPlayerMode(bool t)
{
  if (t)
    setPlayerMode(MiniPlayerMode);
  else
    setPlayerMode(isFullScreenWindowMode() ? EmbeddedPlayerMode : NormalPlayerMode);
}

void
SignalHub::setEmbeddedPlayerMode(bool t)
{ setPlayerMode(t ? EmbeddedPlayerMode : NormalPlayerMode); }

// + WindowMode +

void
SignalHub::setWindowMode(WindowMode mode)
{
  if (windowMode_ != mode) {
    windowMode_ = mode;

#ifdef DEBUG
    switch (mode) {
    case NormalWindowMode: emit message(tr("switched to normal video mode")); break;
    case FullScreenWindowMode: emit message(tr("switched to full screen video mode")); break;
    }
#endif // DEBUG
    emit windowModeChanged(mode);
  }
}

void
SignalHub::setNormalWindowMode(bool t)
{ setWindowMode(t ? NormalWindowMode : FullScreenWindowMode); }

void
SignalHub::setFullScreenWindowMode(bool t)
{ setWindowMode(t ? FullScreenWindowMode : NormalWindowMode); }

// + PlayMode +

void
SignalHub::setPlayMode(PlayMode mode)
{
  if (playMode_ != mode) {
    playMode_ = mode;

    switch (mode) {
    case NormalPlayMode: emit message(tr("switched to normal play mode")); break;
    case SyncPlayMode: emit message(tr("switched to synchronized play mode")); break;
    case LivePlayMode: emit message(tr("switched to live play mode")); break;
    }
    emit playModeChanged(mode);
  }
}

void
SignalHub::setSyncPlayMode(bool t)
{ setPlayMode(t ? SyncPlayMode : NormalPlayMode); }

void
SignalHub::setLivePlayMode(bool t)
{ setPlayMode(t ? LivePlayMode : NormalPlayMode); }

// - Actions -

void
SignalHub::play()
{
  //if (!playing_) {
    playing_ = true;
    stopped_ = false;
    paused_ = false;
    emit played();
  //}
}

void
SignalHub::pause()
{
  //if (!paused_) {
    paused_ = true;
    playing_ = false;
    stopped_ = false;
    emit paused();
  //}
}

void
SignalHub::stop()
{
  //if (!stopped_) {
    stopped_ = true;
    playing_ = false;
    paused_ = false;
    emit stopped();
  //}
}

void
SignalHub::open()
{ emit opened(); }

// EOF
