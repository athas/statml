## Constants
means <- c(-1,0,2)
deviations <- c(1,2,3)
labels <- c("mu=-1, sigma=1", "mu=0, sigma=2", "mu=2, sigma=3")
colours <- c("red", "blue", "darkgreen")

x <- seq(-10, 10, length=1000) # 1000 points should be enough

pdf("../img/question11-plot.pdf")
plot.new()
plot.window(xlim=c(-10,10), ylim=c(0,0.5))
box()
axis(1, at=seq(-10, 10, length=21))
axis(2)
for (i in 1:length(means)) {
  y <- dnorm(x, means[i], deviations[i])
  lines(x,y,col=colours[i])
}
legend("topright", labels, col=colours, title="Distributions", lwd=1)
title(main="Gaussian distributions")
dev.off()
