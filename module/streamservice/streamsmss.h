#ifndef STREAMSMSS_H
#define STREAMSMSS_H

// streamsmss.h
// 2/11/2012

#include <OnDemandServerMediaSubsession.hh>

class InputStream;
class MediaToc;

class StreamSMSS : public OnDemandServerMediaSubsession
{
  typedef StreamSMSS Self;
  typedef OnDemandServerMediaSubsession Base;

  InputStream *in_;
  MediaToc *toc_;
  float duration_; // in seconds

protected:
  StreamSMSS(UsageEnvironment &env, InputStream *in, MediaToc *toc, float duration)
    : Base(env, false), in_(in), toc_(toc), duration_(duration)
  { } // reuseFirstSource = false

  InputStream *in() const { return in_; }
  MediaToc *toc() const { return toc_; }

protected:
  ///  \override
  virtual void seekStreamSource(FramedSource* inputSource,
                                double &seekNPT,
                                double streamDuration,
                                u_int64_t &numBytes);
  virtual float duration() const { return duration_; }  ///< \override
};

#endif // STREAMSMSS_H
