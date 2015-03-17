PairTrade <- R6Class("PairTrade",
    public = list(
        tickerPair = NA,
        prices = NA,
        beta = NA,
        pValue = NA,
        
        initialize = function(tickerPair, prices) {
            if (!missing(tickerPair)) self$tickerPair <- tickerPair
            private$adfOnPair(prices[[1]], prices[[2]])
        },
        recommend = function(portfolio) {
          
        }
    )
    ,
    private = list(
        adfOnPair = function(ticker1, ticker2)
        {
          ticker1 <- zoo(ticker1[,7], as.Date(ticker1[,1]))
          ticker2 <- zoo(ticker2[,7], as.Date(ticker2[,1]))
          
          t.zoo <- merge(ticker1, ticker2, all=FALSE)
          t <- as.data.frame(t.zoo)
          
          m <- lm(t$ticker1 ~ t$ticker2 + 0, data=t)
          beta <- coef(m)[1]
          
          sprd <- t$ticker1 - beta*t$ticker2
          ht <- adf.test(sprd, alternative="stationary", k=0)
          
          self$pValue = ht$p.value
          self$beta = beta
          
        }
    )
)

PairTradeModel <- R6Class("PairTradeModel",
                     public = list(
                       tickerList = NA,
                       tickerPairs = NA,
                       tickerPairAdfs = NA,
                       initialize = function(tickerList) {
                         # if prices is missing we will take from data directory
                         if (!missing(tickerList))
                           self$tickerList <- tickerList
                         self$tickerPairs <- combn(tickerList, 2)
                         lapply(1:NCOL(self$tickerPairs), function(i)(private$addToModel(self$tickerPairs[,i])))
                         
                       }
                     )
                     ,
                     private = list(
                       addToModel = function(tickerPair)
                       {
                         x <- PairTrade$new(tickerPair, mget(tickerPair, inherits = T))
                         if (is.na(self$tickerPairAdfs)[1])
                           self$tickerPairAdfs = x
                         else
                           self$tickerPairAdfs = c(self$tickerPairAdfs, x)
                         
                       }
                     )
)
