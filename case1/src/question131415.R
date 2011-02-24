require("gplots")
## Constants
N <- 100
mean <- c(1,1)
covariance <- matrix(c(0.3, 0.2, 0.2, 0.2), 2, 2)

## Compute L such that t(L) %*% L == covariance.
L <- chol(covariance)

gendata <- function(N) {
  xs <- c()
  for (i in 1:N) {
    z <- rnorm(2)
    y <- mean + L %*% z
    xs <- c(xs, y)
  }
  array(xs, dim=c(2,length(xs)/2))
}

## Generate 100 random samples in xs
xs <- gendata(100)
x1s=xs[1,]
x2s=xs[2,]

## Compute maximum likelyhood mean from sample data using 2.121.
s <- 0
for (i in 1:N) {
  s <- s + xs[,i]
}
meanML <- 1/N * s

## Compute maximum likelyhood covariance using 2.125.
s <- 0
for (i in 1:N) {
  xn <- xs[,i]
  s <- s + (xn-meanML) %*% t(xn-meanML)
}
covarianceML <- 1/(N-1)*s

print("Sample covariance:")
print(covarianceML)
print("Sample mean:")
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
  points(xs[1,i], xs[2,i], col="red")
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
  points(xs[1,i], xs[2,i], col="red")
}
points(meanML[1], meanML[2], col="blue", pch=1, lwd=4)
points(mean[1], mean[2], col="darkgreen", pch=1, lwd=4)
legend("topright",c("Maximum likelyhood mean", "Real mean"),
       title="Means", lwd=4, pch=1, col=c("blue", "darkgreen"))
dev.off()

## Question 1.5 plots

pdf("../img/question15-plot-1-a.pdf", width=2, height=3)
hist(x1s,breaks=1, main="2 cells")
dev.off()

pdf("../img/question15-plot-1-b.pdf", width=2, height=3)
hist(x1s,breaks="Sturges", main="Sturges")
dev.off()

pdf("../img/question15-plot-1-c.pdf", width=2, height=3)
hist(x1s,breaks=seq(min(x1s), max(x1s), length=20), main="20 cells")
dev.off()


pdf("../img/question15-plot-2-a.pdf", width=2, height=3)
hist(x2s,breaks=1, main="2 cells")
dev.off()

pdf("../img/question15-plot-2-b.pdf", width=2, height=3)
hist(x2s,breaks="Sturges", main="Sturges")
dev.off()

pdf("../img/question15-plot-2-c.pdf", width=2, height=3)
hist(x2s,breaks=seq(min(x2s), max(x2s), length=20), main="20 cells")
dev.off()

## Question 1.6.

pdf("../img/question16-plot-analytical.pdf", width=5, height=4)

ys <- c()
## The x1s have to be sorted for the line drawing to work properly.
for (i in 1:N) {
  ys <- c(ys, dnorm(sort(x1s)[i], mean[1], covariance[1][1]))
}
hist(x1s,5,freq=FALSE,ylim=c(0,2))
lines(sort(x1s), ys, type="l")
dev.off()

## Question 1.7

plothist2 <- function(filename, data, bins, text="") {
  pdf(filename, width=3, height=3)
hist2d(data[1,], data[2,], nbins=bins, main=text);
dev.off()
}

plothist2("../img/question17-plot-100.pdf", gendata(100), c(10, 10), "N=100, 10x10 bins")
plothist2("../img/question17-plot-1000-10x10.pdf", gendata(1000), c(10, 10), "N=1000, 10x10 bins")
plothist2("../img/question17-plot-1000-15x15.pdf", gendata(1000), c(15, 15), "N=1000, 15x15 bins")
plothist2("../img/question17-plot-1000-20x20.pdf", gendata(1000), c(20, 20), "N=1000, 20x20 bins")
plothist2("../img/question17-plot-10000.pdf", gendata(10000), c(10, 10), "N=10000, 10x10 bins")
