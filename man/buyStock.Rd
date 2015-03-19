\name{buyStock}
\alias{buyStock}
\title{Buy Stocks over a fix connection}
\usage{
buyStock(ticker, price, quantity)
}
\arguments{
\item{ticker}{A string representing the ticker of the security}
\item{price}{A double value representing the price}
\item{quantity}{ A double value representing the quantity }
}
\value{
None
}
\description{
It executes a buy Order on fix connection. For now, it assumes that a FIX server on the local machine 
with a server config that is provided in the package
}
\examples{
  buyStock("MSFT", 10, 10)
  getPortfolio()
  # "MSFT,10,-10"
}

