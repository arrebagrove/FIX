#sajid code begins here
#' Estimate model of pair trade
#' Assumption is that difference between log(price) of two stocks is random walk
#' @param price.pair A data frame with two columns of prices, one for each stock
#' @param method method of estimation. usually lm
#' @return list of mean, std. dev.,spread, hedge ratio, and premium
#' @export
#' @example /examples/estimateex.R

EstimateModel <- function(price.pair, method = lm)
{
  x <- log(price.pair)
  
  reg <- method(x[, 2] ~ x[, 1])
  hedge.ratio <- as.numeric(reg$coef[2])
  premium     <- as.numeric(reg$coef[1])
  spread      <- x[, 2] - (hedge.ratio * x[, 1] + premium)
  meanT    <- as.numeric(mean(spread,na.rm=TRUE))
  sdT      <- as.numeric(sd(spread,na.rm=TRUE))
  list(meanT=meanT,sdT=sdT,spread=spread,
       hedge.ratio = hedge.ratio, premium = premium)
}

#' Determine if difference between stocks is stationary and if trade is recommended
#' Uses two tests: Phillips-Perron test for unit root, and Augmented Dickey Fuller test
#' If residual between two stationary stocks is more than threshold standard deviations away from mean,
#' then recommend trade. Otherwise return NA
#' @param model  model list returned by EstimateModel
#' @param todaysPrices data frame of todays prices for the stocks in model
#' @param threshold std deviation threshold for trade 
#' @return stock to buy if trade is recommended, or NA if not
#' @export
#' @example /examples/recommendex.R

RecommendTrade <- function(model,todaysPrices, threshold)
{
  Is.passed.PP.test  <- PP.test(as.numeric(model$spread))$p.value <= 0.1
  Is.passed.adf.test <- tseries::adf.test(as.numeric(model$spread))$p.value <= 0.1
  upperThr <- model$meanT + threshold * model$sdT
  lowerThr <- model$meanT - threshold * model$sdT
  todaySpread=todaysPrices[,2]-(model$hedge.ratio*todaysPrices[,1]+model$premium)
  if (Is.passed.adf.test || Is.passed.PP.test) {
    if (todaySpread >= model$meanT + model$sdT){
      return(names(todaysPrices[,1]))
    }
    else if (todaySpread <= model$meanT - model$sdT){
      return(names(todaysPrices[,2]))
    }
    else {
      return(NA)
    }
  }
  else{
    return(NA)
  }
}
#sajid code ends here
#' @import xts,zoo,tseries