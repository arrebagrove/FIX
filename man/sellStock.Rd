\name{sellStock}
\alias{sellStock}
\title{Sell Stocks over a fix connection}
\usage{
sellStock(ticker, price, quantity)
}
\arguments{
\item{ticker}{A string representing the ticker of the security}
\item{price}{A double value representing the price}
\item{quantity}{ A double value representing the quantity }
}
\value{
Nonde
}
\description{
It executes a Sell Order on fix connection. For now, it assumes that a FIX server on the local machine 
with a server config that is provided in the package
}
\examples{
  sellStock("MSFT", 10, 10)
  getPortfolio()
  # "MSFT,10,10"
}

