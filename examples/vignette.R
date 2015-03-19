library("pairFIX")
data(stock.prices.today)
data(stock.prices.hist)

#select 2 stocks
price.pair.history <- stock.prices.hist[,c("AAPL.Adjusted","AMZN.Adjusted")]
#could also get todays prices from FIX engine
price.pair.today   <- stock.prices.today[,c("AAPL.Adjusted","AMZN.Adjusted")]

#Estimate parameters & plot spread
model <- EstimateModel(price.pair.history, method = lm)
plot(model$spread)

#check if trade is recommended
trade=RecommendTrade(model,price.pair.today,0.05)
print(trade)

#for sake of example, assume AAPL is recommended buy
#example code to buy and sell from FIX engine

buyStock(AAPL,stock.prices.today[,"AAPL.Adjusted"],100)
sellStock(AMZN,stock.prices.today[,"AMZN.Adjusted"],100)
