#ifndef CLIENTAGENT_H
#define CLIENTAGENT_H

// module/clientagent/clientagent.h
// 10/6/2011
// Publish client side ClientAgentService for server callback.
// See: http://blog.csdn.net/tingsking18/article/details/5456831
// See: http://hi.baidu.com/2sky2sea/blog/item/40ec5555680279c1b745ae9b.html
//
// Note: since soap timeout is not slow, synchronized approach is used.
// TODO: Deal with offline case here??

#include <QObject>
#include "clientdelegate.h"
#include "module/annotcloud/user.h"
#include "module/annotcloud/token.h"
#include "module/annotcloud/annotation.h"

class ClientService;
class ServerAgent;

class ClientAgent: public QObject, public ClientDelegate
{
  Q_OBJECT
  typedef ClientAgent Self;
  typedef QObject Base;

  typedef Core::Universe::User User;
  typedef Core::Universe::Token Token;
  typedef Core::Universe::TokenList TokenList;
  typedef Core::Universe::Annotation Annotation;
  typedef Core::Universe::AnnotationList AnnotationList;

  ClientService *service_;
  bool authorized_;
  bool connected_;

  long key_;

#ifdef WITH_MODULE_SERVERAGENT
  ServerAgent *server_;
#endif // WITH_MODULE_SERVERAGENT

public:
  explicit ClientAgent(QObject *parent = 0);

#ifdef WITH_MODULE_SERVERAGENT
  void setServerAgent(ServerAgent *server);
#endif // WITH_MODULE_SERVERAGENT

  // - Properties -
public:

  long privateKey() const;
  void setPrivateKey(long key);
  void randomizePrivateKey();

  long nextPublicKey() const; ///< get next possible public key

  int port() const;
protected:
  void randomizeServicePort();
  void setPort(int port);

  // - Service -
public:
  bool isReady() const;

  // - Signals -
signals:
  void authorized();
  void deauthorized();
  void authorizationError();
  void chatReceived(const QString text);

  // - Constrols -
public slots:
  void setAuthorized(bool t);
  void setConnected(bool t);

  // - Implementations -
public:
  virtual bool authorize(long key); ///< \override
  virtual void deauthorize(); ///< \override
  virtual bool isAuthorized() const; ///< \override
  virtual bool isConnected() const; ///< \override
  virtual void chat(const QString &text); ///< \override

};

#endif // CLIENTAGENT_H
