#' Stock prices for 4 stocks for the last year
#'
#' AAPL, AMZN, MSFT, GOOG
#'
#' @format An xts object with 244 rows and 4 variables:
#' \describe{
#'   \item{AAPL.Adjusted}{AAPL daily adjusted price}
#'   \item{GOOG.Adjusted}{GOOG daily adjusted price}
#'   \item{MSFT.Adjusted}{MSFT daily adjusted price}
#'   \item{AMZN.Adjusted}{AMZN daily adjusted price}
#' }
#' @source quantmod package
"stock.prices.hist"

#' Stock prices for 4 stocks for one day (March 17 2015)
#'
#' AAPL, AMZN, MSFT, GOOG
#'
#' @format An xts object with 1 rows and 4 variables:
#' \describe{
#'   \item{AAPL.Adjusted}{AAPL daily adjusted price}
#'   \item{GOOG.Adjusted}{GOOG daily adjusted price}
#'   \item{MSFT.Adjusted}{MSFT daily adjusted price}
#'   \item{AMZN.Adjusted}{AMZN daily adjusted price}
#' }
#' @source quantmod package
"stock.prices.today"