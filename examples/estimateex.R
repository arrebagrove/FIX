x    <- rnorm(1000)+50
y    <- (x - 2) + rnorm(1000)+50
price.pair.history<-as.data.frame(cbind(x,y))
model <- EstimateModel(price.pair.history, method = lm)
