x    <- rnorm(1000)+50
y    <- (x - 2) + rnorm(1000)+50
price.pair.history<-as.data.frame(cbind(x,y))
model <- EstimateModel(price.pair.history, method = lm)
x    <- rnorm(1)
y    <- (x - 2) + rnorm(1)
price.pair.today<-as.data.frame(cbind(x,y))

trade=RecommendTrade(model,price.pair.today,0.05)
