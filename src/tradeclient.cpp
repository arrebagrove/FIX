#include "Application.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include <Rcpp.h>

using namespace Rcpp;
Application *application;
// [[Rcpp::export]]
void initFIX(std::string FileLogPath, std::string FileStorePath, int ReconnectInterval,
              std::string SenderCompID, std::string DefaultApplVerID, std::string ResetOnLogon,
              std::string BeginString, std::string TargetCompID, int HeartBtInt, 
              int SocketConnectPort, std::string SocketConnectHost, std::string DataDictionary,
              std::string StartTime, std::string EndTime)
{
  application = new Application(FileLogPath, FileStorePath, ReconnectInterval,
              SenderCompID, DefaultApplVerID, ResetOnLogon,
              BeginString, TargetCompID, HeartBtInt, 
              SocketConnectPort, SocketConnectHost, DataDictionary,
              StartTime, EndTime);
  application->init();
}

// [[Rcpp::export]]
void destroyFIX()
{
  application->destroy();
  delete application;
  application = NULL;
}

// [[Rcpp::export]]
void buyStock(std::string ticker, double price, double quantity)
{
  try
  {
    application->buy(ticker, price, quantity);
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
    application->sell(ticker, price, quantity);
  }
  catch ( std::exception & e )
  {
    std::cout << e.what();
  }
}

// [[Rcpp::export]]
std::vector<std::string> getPortfolio()
{
  Application::PortfolioType pf =  application->getPortfolio();
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

