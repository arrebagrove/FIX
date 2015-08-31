#ifndef TRADECLIENT_APPLICATION_H
#define TRADECLIENT_APPLICATION_H

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix40/NewOrderSingle.h"
#include "quickfix/fix40/ExecutionReport.h"
#include "quickfix/fix40/OrderCancelRequest.h"
#include "quickfix/fix40/OrderCancelReject.h"
#include "quickfix/fix40/OrderCancelReplaceRequest.h"

#include "quickfix/fix41/NewOrderSingle.h"
#include "quickfix/fix41/ExecutionReport.h"
#include "quickfix/fix41/OrderCancelRequest.h"
#include "quickfix/fix41/OrderCancelReject.h"
#include "quickfix/fix41/OrderCancelReplaceRequest.h"

#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"

#include "quickfix/fix43/NewOrderSingle.h"
#include "quickfix/fix43/ExecutionReport.h"
#include "quickfix/fix43/OrderCancelRequest.h"
#include "quickfix/fix43/OrderCancelReject.h"
#include "quickfix/fix43/OrderCancelReplaceRequest.h"
#include "quickfix/fix43/MarketDataRequest.h"

#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/OrderCancelRequest.h"
#include "quickfix/fix44/OrderCancelReject.h"
#include "quickfix/fix44/OrderCancelReplaceRequest.h"
#include "quickfix/fix44/MarketDataRequest.h"

#include "quickfix/fix50/NewOrderSingle.h"
#include "quickfix/fix50/ExecutionReport.h"
#include "quickfix/fix50/OrderCancelRequest.h"
#include "quickfix/fix50/OrderCancelReject.h"
#include "quickfix/fix50/OrderCancelReplaceRequest.h"
#include "quickfix/fix50/MarketDataRequest.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/SessionSettings.h"

#include <queue>

class Settings
{
public:
std::string FileLogPath;
std::string FileStorePath;
int ReconnectInterval;
std::string SenderCompID;
std::string DefaultApplVerID;
std::string ResetOnLogon;

std::string BeginString;
std::string TargetCompID;
int HeartBtInt;
int SocketConnectPort;
std::string SocketConnectHost;
std::string DataDictionary;
std::string StartTime;
std::string EndTime;

std::string toString()
{
std::ostringstream oss;

oss << "[DEFAULT]\n";
oss << "ConnectionType=initiator\n";
oss << "FileLogPath=" << FileLogPath << "\n";
oss << "FileStorePath=" << FileStorePath << "\n";
oss << "ReconnectInterval=" << ReconnectInterval << "\n";
oss << "SenderCompID=" << SenderCompID << "\n";
oss << "DefaultApplVerID="<< DefaultApplVerID <<"\n";
oss << "ResetOnLogon=" << ResetOnLogon << "\n";
oss << "[SESSION]\n";
oss << "BeginString=" << BeginString << "\n";
oss << "TargetCompID=" << TargetCompID << "\n";
oss << "HeartBtInt=" << HeartBtInt << "\n";
oss << "SocketConnectPort=" << SocketConnectPort << "\n";
oss << "SocketConnectHost=" << SocketConnectHost << "\n";
oss << "DataDictionary=" << DataDictionary << "\n";
oss << "StartTime=" << StartTime << "\n";
oss << "EndTime=" << EndTime << "\n";
return oss.str();
}

};

class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
  typedef std::map< std::string, std::vector< std::pair<double, double> > > PortfolioType;
  void sell(std::string ticker, double price, double quantity);
  void buy(std::string ticker, double price, double quantity);
  PortfolioType getPortfolio() {return portfolio;}

  volatile bool isLoggedOut;
  volatile bool isLoggedOn;
  Application();
  Application(std::string FileLogPath, std::string FileStorePath, int ReconnectInterval,
              std::string SenderCompID, std::string DefaultApplVerID, std::string ResetOnLogon,
              std::string BeginString, std::string TargetCompID, int HeartBtInt, 
              int SocketConnectPort, std::string SocketConnectHost, std::string DataDictionary,
              std::string StartTime, std::string EndTime);
  void init();
  void destroy();
private:
  void onCreate( const FIX::SessionID& ) {}
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& ) {}
  void toApp( FIX::Message&, const FIX::SessionID& )
  throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );
  FIX::SocketInitiator *initiator;
  void onMessage( const FIX40::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX40::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX41::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX41::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX42::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX42::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX43::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX43::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX44::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX44::OrderCancelReject&, const FIX::SessionID& );
  void onMessage( const FIX50::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX50::OrderCancelReject&, const FIX::SessionID& );

  Settings settings;
  PortfolioType portfolio;
};

#endif
