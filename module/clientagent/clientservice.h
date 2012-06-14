#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

// module/clientagent/clientservice.h
// 10/7/2011

#include "clientsoap.h"
#include "clientdelegate.h"
#include "module/annotcloud/user.h"
#include "module/annotcloud/token.h"
#include "module/annotcloud/annotation.h"
#include <QtCore/QThread>

/**
 *  \brief  Extend client soap service and represent service thread.
 */
class ClientServant : public QThread, public ClientSoap::ClientSoapService
{
  Q_OBJECT
  Q_DISABLE_COPY(ClientServant)
  typedef ClientServant Self;
  typedef QThread QtBase;
  typedef ClientSoap::ClientSoapService SoapBase;

  typedef Core::Universe::User User;
  typedef Core::Universe::Token Token;
  typedef Core::Universe::TokenList TokenList;
  typedef Core::Universe::Annotation Annotation;
  typedef Core::Universe::AnnotationList AnnotationList;

  ClientDelegate *delegate_;
  int port_;

public:
  explicit ClientServant(QObject *parent = 0);

  // - Properties -
public:
  ClientDelegate *delegate() const;
  void setDelegate(ClientDelegate *delegate);

  int port() const;
  void setPort(int port);

  // - Methods overriding ClientSoapService -
public:
  virtual int authorize(ClientSoap::tns__authorize *request, ClientSoap::tns__authorizeResponse *response); /// \reimp
  virtual int deauthorize(ClientSoap::tns__deauthorize *request, ClientSoap::tns__deauthorizeResponse *response); ///< \reimp
  virtual int isAuthorized(ClientSoap::tns__isAuthorized *request, ClientSoap::tns__isAuthorizedResponse *response); ///< \reimp
  virtual int isConnected(ClientSoap::tns__isConnected *request, ClientSoap::tns__isConnectedResponse *response); ///< \reimp
  virtual int chat(ClientSoap::tns__chat *request, ClientSoap::tns__chatResponse *response); ///< \reimp

  // - Methods overriding QThread -
protected:
  virtual void run(); ///< \reimp
};

/**
 *  \brief  Client agent service manager and controller.
 */
class ClientService: public QObject
{
  Q_OBJECT
  typedef ClientService Self;
  typedef QObject Base;

  ClientServant *servant_;
  ClientDelegate *delegate_;
  int port_;

public:
  explicit ClientService(QObject *parent = 0);
  ~ClientService();

  // - Properties -
  int port() const;
  void setPort(int port);

  ClientDelegate *delegate() const;
  void setDelegate(ClientDelegate *delegate);

  bool isRunning() const;
  // - Slots -
public slots:
  void start();
  void restart();
  void stop(long msecs = 2000);
};

#endif // CLIENTSERVICE_H
