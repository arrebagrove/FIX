
#put some tests here. expect_message(), etc.
buyStock("AMZN",56.3,10)
sellStock("AMZN",56.3,10)
expect_equivalent(getPortfolio(),"AMZN,56.3,10\nAMZN.56.3,-10\n")
