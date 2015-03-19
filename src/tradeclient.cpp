/****************************************************************************
** Copyright (c) quickfixengine.org  All rights reserved.
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "Application.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include <Rcpp.h>

using namespace Rcpp;

static std::string settings =
"\n\
[DEFAULT]\n\
ConnectionType=initiator\n\
FileLogPath=/tmp/fix/log\n\
FileStorePath=/tmp/fix/store\n\
ReconnectInterval=5\n\
SenderCompID=SENDER\n\
DefaultApplVerID=FIX.4.2\n\
ResetOnLogon=Y\n\
\n\
[SESSION]\n\
BeginString=FIX.4.2\n\
TargetCompID=EXEC\n\
HeartBtInt=5\n\
SocketConnectPort=56156\n\
#SocketAcceptPort=56156\n\
SocketConnectHost=127.0.0.1\n\
DataDictionary=/root/transportData/spec/FIX42.xml\n\
StartTime=00:00:05\n\
EndTime=23:55:00\n\
";

Application application;
// [[Rcpp::export]]
void buyStock(std::string ticker, double price, double quantity)
{
  try
  {
    std::istringstream settingsStream(settings);
    FIX::SessionSettings settings( settingsStream );

    application.isLoggedOn = false;
    application.isLoggedOut = false;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::FileLogFactory logFactory(settings);
    FIX::SocketInitiator initiator( application, storeFactory, settings, logFactory );
    initiator.start();
    while(!initiator.isLoggedOn()) {}
    application.buy(ticker, price, quantity);
    initiator.stop();
    // wait for logout to come
    while(!initiator.isStopped()) {}
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
  }
}

// [[Rcpp::export]]
void sellStock(std::string ticker, double price, double quantity)
{
  try
  {
    std::istringstream settingsStream(settings);
    FIX::SessionSettings settings( settingsStream );

    application.isLoggedOn = false;
    application.isLoggedOut = false;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::FileLogFactory logFactory(settings);
    FIX::SocketInitiator initiator( application, storeFactory, settings, logFactory);
    initiator.start();
    while(!initiator.isLoggedOn()) {}
    application.sell(ticker, price, quantity);
    initiator.stop();
    // wait for logout to come
    while(!initiator.isStopped()) {}
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
  }
}

// [[Rcpp::export]]
bool getPortfolio()
{
  return true;
}

#if 0
int main()
{
//  buyStock(std::string("MSFT"), 10);
  /*    buyStock(std::string("MSFT"), 40.56, 10);
  buyStock(std::string("GOOG"), 400.56, 10);
 buyStock(std::string("MSFT"), 40.56, 10);
  buyStock(std::string("GOOG"), 400.56, 10);
 buyStock(std::string("MSFT"), 40.56, 10);
  buyStock(std::string("GOOG"), 400.56, 10);
 buyStock(std::string("MSFT"), 40.56, 10);
  buyStock(std::string("GOOG"), 400.56, 10);*/
 buyStock(std::string("MSFT"), 40.56, 10);
  buyStock(std::string("GOOG"), 400.56, 10);
  sellStock(std::string("GOOG"), 400.90, 20);
  sellStock(std::string("MSFT"), 60, 20);
  return 0;
}
class FixConnection
{
public:
FixConnection(std::string compId, std::string pass) {}
void buyStock() {}
void sellStock() {}
void getPortfolio() {}
};

RCPP_MODULE(FIX){
    class_<FixConnection>("FixConnection")
        .constructor<std::string,std::string>()
        .method("buyStock", &FixConnection::buyStock)
        .method("sellStock", &FixConnection::sellStock)
        .method("getPortfolio", &FixConnection::getPortfolio)
        ;        
};

int main( int argc, char** argv )
{
  if ( argc != 2 )
  {
    std::cout << "usage: " << argv[ 0 ]
    << " FILE." << std::endl;
    return 0;
  }
  std::string file = argv[ 1 ];

  try
  {
    FIX::SessionSettings settings( file );

    Application application;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::SocketInitiator initiator( application, storeFactory, settings );

    initiator.start();
    application.run();
    initiator.stop();

    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
    return 1;
  }
}
#endif
