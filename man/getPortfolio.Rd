\name{getPortfolio}
\alias{getPortfolio}
\title{returns a list of buy and sell orders that were filled by the FIX server}
\usage{
getPortfolio()
}
\arguments{
}
\value{
a string representing all the orders that were placed
}
\description{
It returns a list of orders that were filled by the server
}
\examples{
  sellStock("MSFT", 10, 10)
  getPortfolio()
  # "MSFT,10,10"
}

