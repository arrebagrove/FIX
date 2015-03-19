
#put some tests here. expect_message(), etc.
buyStock("AMZN",56.3,10)
sellStock("AMZN",56.3,10)
expect_equivalent(getPortfolio(),"AMZN,10,56.3\nAMZN,-10,56.3\n")
