#include "Application.h"
#include "quickfix/Session.h"
#include <iostream>
#include <utility>
#include <sstream>
#include <unistd.h>

Application::Application()
{
    isLoggedOn = false;
    isLoggedOut = false;
}

void Application::onLogon( const FIX::SessionID& sessionID )
{
  isLoggedOn = true;
  //std::cout << std::endl << "Logon - " << sessionID << std::endl;
}

void Application::onLogout( const FIX::SessionID& sessionID )
{
  isLoggedOut = true;
  //std::cout << std::endl << "Logout - " << sessionID << std::endl;
}
void Application::fromAdmin( const FIX::Message& message, const FIX::SessionID& sessionID)
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon )
{
  crack(message, sessionID);
  //std::cout << std::endl << "IN: ADMIN "<< message << std::endl;
}

void Application::fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
  std::map <int, std::string> headerVals;
  crack( message, sessionID );
  //std::cout << std::endl << "IN: " << message.toXML() << std::endl;
  for(FIX::FieldMap::iterator i = message.begin(); i != message.end(); ++i)
  {
    int field = i->first;
    std::string value = i->second.getString();
    headerVals[field] = value;
  }
  for(FIX::FieldMap::iterator i = message.getHeader().begin(); i != message.getHeader().end(); ++i)
  {
    int field = i->first;
    std::string value = i->second.getString();
    headerVals[field] = value;
  }

  if (headerVals.count(35) == 1 && headerVals[35] == std::string("8"))
  {
    // This is a order execution
    //std::cout << "Received an order execution report \n" << std::endl;
    if (headerVals.count(39) == 1 && headerVals[39] == std::string("2"))
    {
      //std::cout << "Received a fill\n" << std::endl;
      if (headerVals.count(54) == 1)
      {
        if (headerVals.count(32) == 1 &&
            headerVals.count(31) == 1 &&
            headerVals.count(55) == 1)
        {
             std::istringstream quantityStream(headerVals[32]);
             double quantity;
             quantityStream >> quantity;
             std::istringstream priceStream(headerVals[31]);
             double price;
             priceStream >> price;
             std::string ticker(headerVals[55]);
             // buy or sell
             if (headerVals[54] == std::string("1"))
                 portfolio[ticker].push_back(std::make_pair(quantity, price));
             else if (headerVals[54] == std::string("2"))
                 portfolio[ticker].push_back(std::make_pair(-1 * quantity, price));
        }
      }
      // There was a fill
      // 32 is last shares
      // 31 is last price
      // 54 is buy or sell
      // 55 is symbol
    }
  }
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID )
throw( FIX::DoNotSend )
{
  try
  {
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField( possDupFlag );
    if ( possDupFlag ) throw FIX::DoNotSend();
  }
  catch ( FIX::FieldNotFound& ) {}

  //std::cout << std::endl << "OUT: " << message << std::endl;
}

void Application::onMessage
( const FIX40::ExecutionReport& er, const FIX::SessionID& sid) {}
void Application::onMessage
( const FIX40::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX41::ExecutionReport&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX41::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX42::ExecutionReport&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX42::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX43::ExecutionReport&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX43::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX44::ExecutionReport&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX44::OrderCancelReject&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX50::ExecutionReport&, const FIX::SessionID& ) {}
void Application::onMessage
( const FIX50::OrderCancelReject&, const FIX::SessionID& ) {}

static int clientOrderIdCounter = 0;

void Application::buy(std::string ticker, double price, double quantity)
{
    std::string clientOrderId("PK");
    std::ostringstream oss;
    oss << "PK" << clientOrderIdCounter++;
    FIX42::NewOrderSingle newOrderSingle(
        FIX::ClOrdID(oss.str()), FIX::HandlInst('1'), FIX::Symbol(ticker), FIX::Side( FIX::Side_BUY ),
        FIX::TransactTime(), FIX::OrdType_LIMIT );

    newOrderSingle.set( FIX::OrderQty(quantity));
    newOrderSingle.set( FIX::TimeInForce(FIX::TimeInForce_DAY ));
    newOrderSingle.set( FIX::Price(price) );

    FIX::Header& header = newOrderSingle.getHeader();
    header.setField( FIX::SenderCompID("SENDER") );
    header.setField( FIX::TargetCompID("EXEC") );
    header.setField( FIX::BeginString("FIX.4.2"));
    FIX::Session::sendToTarget( newOrderSingle );
}

void Application::sell(std::string ticker, double price, double quantity)
{
    std::string clientOrderId("PK");
    std::ostringstream oss;
    oss << "PK" << clientOrderIdCounter++;
    FIX42::NewOrderSingle newOrderSingle(
        FIX::ClOrdID(oss.str()), FIX::HandlInst('1'), FIX::Symbol(ticker), FIX::Side( FIX::Side_SELL ),
        FIX::TransactTime(), FIX::OrdType_LIMIT );

    newOrderSingle.set( FIX::OrderQty(quantity));
    newOrderSingle.set( FIX::TimeInForce(FIX::TimeInForce_DAY ));
    newOrderSingle.set( FIX::Price(price) );

    FIX::Header& header = newOrderSingle.getHeader();
    header.setField( FIX::SenderCompID("SENDER") );
    header.setField( FIX::TargetCompID("EXEC") );
    header.setField( FIX::BeginString("FIX.4.2"));
    FIX::Session::sendToTarget( newOrderSingle );
}


