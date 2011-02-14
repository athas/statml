## Constants
N <- 100
mean <- c(1,1)
covariance <- matrix(c(0.3, 0.2, 0.2, 0.2), 2, 2)

## Compute L such that t(L) %*% L == covariance.
L <- chol(covariance)

## Generate N random samples in xs
xs <- c()
for (i in 1:N) {
  z <- rnorm(2)
  y <- mean + L %*% z
  xs <- c(xs, y)
}

## Compute maximum likelyhood mean from sample data using 2.121.
s <- 0
for (i in 1:N) {
  xn <- c(xs[i*2-1], xs[i*2])
  s <- s + xn
}
meanML <- 1/N * s

## Compute maximum likelyhood covariance using 2.125.
s <- 0
for (i in 1:N) {
  xn <- c(xs[i*2-1], xs[i*2])
  s <- s + (xn-meanML) %*% t(xn-meanML)
}
covarianceML <- 1/(N-1)*s

print "Sample covariance:"
print(covarianceML)
print "Sample mean:"
print(meanML)

## Question 1.3 plot
pdf("../img/question13-plot.pdf")
plot.new()
plot.window(xlim=c(-3,3), ylim=c(-3,3))
box()
axis(1, at=seq(-10, 10, length=21))
axis(2)
title(main="Two-dimensional gaussian distribution")
for (i in 1:N) {
  xn <- c(xs[i*2-1], xs[i*2])
  points(xn[1], xn[2], col="red")
}
dev.off()

## Question 1.4 plot
pdf("../img/question14-plot.pdf")
plot.new()
plot.window(xlim=c(-3,3), ylim=c(-3,3))
box()
axis(1, at=seq(-10, 10, length=21))
axis(2)
title(main="Two-dimensional gaussian distribution")
for (i in 1:N) {
  xn <- c(xs[i*2-1], xs[i*2])
  points(xn[1], xn[2], col="red")
}
points(meanML[1], meanML[2], col="blue", pch=1, lwd=4)
points(mean[1], mean[2], col="darkgreen", pch=1, lwd=4)
legend("topright",c("Maximum likelyhood mean", "Real mean"),
       title="Means", lwd=4, pch=1, col=c("blue", "darkgreen"))
dev.off()

## Question 1.5 plots
x1s=xs[seq(1,length(xs),by=2)]
x2s=xs[seq(2,length(xs),by=2)]

pdf("../img/question15-plot-1-a.pdf", width=3, height=3)
hist(x1s,1)
dev.off()

pdf("../img/question15-plot-1-b.pdf", width=3, height=3)
hist(x1s,5)
dev.off()

pdf("../img/question15-plot-1-c.pdf", width=3, height=3)
hist(x1s,10)
dev.off()


pdf("../img/question15-plot-2-a.pdf", width=3, height=3)
hist(x2s,1)
dev.off()

pdf("../img/question15-plot-2-b.pdf", width=3, height=3)
hist(x2s,5)
dev.off()

pdf("../img/question15-plot-2-c.pdf", width=3, height=3)
hist(x2s,10)
dev.off()

## Question 1.6.

pdf("../img/question16-plot-analytical.pdf", width=5, height=4)

ys <- c()
# The x1s have to be sorted for the line drawing to work properly.
for (i in 1:N) {
  ys <- c(ys, dnorm(sort(x1s)[i], mean[1], covariance[1][1]))
}
hist(x1s,5,freq=FALSE,ylim=c(0,2))
lines(sort(x1s), ys, type="l")
dev.off()
