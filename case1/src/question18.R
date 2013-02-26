## Exponential distribution as curried function.
expdist <- function(lambda) {
  function(z) {
    -lambda^(-1)*log(1-z)
  }
}

plotexp <- function(lambda,L,col) {
  ys=sapply(1:L, absdiff(lambda))
  lines(1:L, ys, col=col)
}

absdiff <- function(lambda) {
  muL = 1/lambda
  function(L) {
    x = 0
    repetitions=100
    for (i in 1:repetitions) {
      x = x + mean(sapply(runif(L), expdist(lambda)))
    }
    abs(muL - x / repetitions)
  }
}

lambdas <- c(0.5, 1.0, 1.5)
labels <- c("lamba=0.5", "lambda=1.0", "lambda=1.5")
cols <- c("red", "blue", "green")
Lmax=1000
pdf("../img/question18-plot-1.pdf")
plot.new()
plot.window(xlim=c(1,Lmax), ylim=c(0,0.5))
box()
axis(1)
axis(2)
for (i in 1:length(lambdas)) {
  plotexp(lambdas[i], Lmax, cols[i])
}
title(main="Absolute difference")
legend("topright", labels, col=cols, lwd=1)
dev.off()
