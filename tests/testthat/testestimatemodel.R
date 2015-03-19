library(testthat)
library(pairFIX)

x    <- rnorm(1000)+50
y    <- (x - 2) + rnorm(1000)+50
price.pair.history<-as.data.frame(cbind(x,y))
model=EstimateModel(price.pair.history, method = lm)

#check each element of output
expect_more_than(model$meanT,-1)
expect_more_than(model$sdT,0)
expect_true(is.numeric(model$hedge.ratio))
expect_true(is.numeric(model$premium))

#check that spread is equal to residuals
x <- log(price.pair.history)

reg <- lm(x[, 2] ~ x[, 1])
expect_equivalent(reg$residuals,model$spread)