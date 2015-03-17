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
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"
#include "Application.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <Rcpp.h>
using namespace Rcpp;

static std::string settings =
"\
# default settings for sessions \
  [DEFAULT] \
  ConnectionType=initiator \
  ReconnectInterval=60 \
  SenderCompID=TW \
\
# session definition \
  [SESSION] \
# inherit ConnectionType, ReconnectInterval and SenderCompID from default \
  BeginString=FIX.4.1 \
  TargetCompID=ARCA \
  StartTime=12:30:00 \
  EndTime=23:30:00 \
  HeartBtInt=20 \
  SocketConnectPort=9823 \
  SocketConnectHost=123.123.123.123 \
  DataDictionary=somewhere/FIX41.xml \
";
// [[Rcpp::export]]
void buyStock(std::string ticker, int quantity)
{
  try
  {
    std::istringstream settingsStream(settings);
    FIX::SessionSettings settings( settingsStream );

    Application application;
    FIX::FileStoreFactory storeFactory( settings );
    FIX::SocketInitiator initiator( application, storeFactory, settings );

    initiator.start();
    application.run();
    initiator.stop();

  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
  }

}
// [[Rcpp::export]]
bool sellStock(std::string ticker, int quantity)
{
  return true;
}

// [[Rcpp::export]]
bool getPortfolio()
{
  return true;
}

#if 0
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
