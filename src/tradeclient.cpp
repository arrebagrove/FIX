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

static std::string _settings =
"\n\
[DEFAULT]\n\
ConnectionType=initiator\n\
FileLogPath=/tmp/fix/log\n\
FileStorePath=/tmp/fix/store\n\
ReconnectInterval=5\n\
SenderCompID=IB\n\
DefaultApplVerID=FIX.4.2\n\
ResetOnLogon=Y\n\
\n\
[SESSION]\n\
BeginString=FIX.4.2\n\
TargetCompID=IB\n\
HeartBtInt=5\n\
SocketConnectPort=4000\n\
#SocketAcceptPort=56156\n\
SocketConnectHost=172.16.55.1\n\
DataDictionary=/root/transportData/spec/FIX42.xml\n\
StartTime=00:00:05\n\
EndTime=23:55:00\n\
";

Application application;
// [[Rcpp::export]]
void initFIX(std::string settings)
{
   _settings = settings;
}
// [[Rcpp::export]]
void buyStock(std::string ticker, double price, double quantity)
{
  try
  {
    std::istringstream settingsStream(_settings);
    FIX::SessionSettings sessionSettings( settingsStream );

    application.isLoggedOn = false;
    application.isLoggedOut = false;
    FIX::FileStoreFactory storeFactory( sessionSettings );
    FIX::FileLogFactory logFactory(sessionSettings);
    FIX::SocketInitiator initiatr( application, storeFactory, sessionSettings, logFactory );
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
    std::istringstream settingsStream(_settings);
    FIX::SessionSettings sessionSettings( settingsStream );

    application.isLoggedOn = false;
    application.isLoggedOut = false;
    FIX::FileStoreFactory storeFactory( sessionSettings );
    FIX::FileLogFactory logFactory(sessionSettings);
    FIX::SocketInitiator initiator( application, storeFactory, sessionSettings, logFactory);
    initiator.start();
    while(!initiator.isLoggedOn()) {}
    application.sell(ticker, price, quantity);
    initiator.stop();
    // wait for logout to come
    while(!initiator.isStopped()) {}
  }
  catch ( std::exception & e )
  {
    //std::cout << e.what();
  }
}

// [[Rcpp::export]]
std::vector<std::string> getPortfolio()
{
  Application::PortfolioType pf =  application.getPortfolio();
  std::vector<std::string> allOrders;
  for(Application::PortfolioType::iterator it1 = pf.begin(); it1 != pf.end(); it1++) {
      std::ostringstream oss;
      std::string ticker = it1->first;
      for (std::vector<std::pair<double, double> >::iterator it2 = it1->second.begin();
          it2 != it1->second.end();
          it2++)
      {
          oss << ticker << "," << it2->first << "," << it2->second << std::endl;
      }
      allOrders.push_back(oss.str());
  }
  return allOrders;
}

