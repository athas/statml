## This function is for creating a copy of a matrix/table without
## metadata.  For some reason, transposing the matrix returned from
## read.table leads to all kinds of messes.
tomatrix <- function(a) {
  ret = matrix(,dim(a)[1], dim(a)[2])
  for (i in 1:dim(a)[1]) {
    for (j in 1:dim(a)[2]) {
      ret[i,j] = a[i,j]
    }
  }
  ret
}

readKnollData <- function (file) {
  read.table(file, stringsAsFactors=FALSE)
}

mytranspose <- function (a) {
  ret = matrix(,dim(a)[2], dim(a)[1])
  for (i in 1:dim(a)[1]) {
    for (j in 1:dim(a)[2]) {
      ret[j,i] = a[i,j]
    }
  }
  ret
}

pointsByClass <- function (data, class) {
  data[data[3]==class,c(1,2)]
}

fisherCovarMatrix <- function (x1s, x2s) {
  f <- function (vec, mean) {
    res = matrix(0,length(mean), length(mean))
    for (i in 1:dim(vec)[1]) {
      res = res + tomatrix(t(vec[i,]-mean)) %*% tomatrix(vec[i,]-mean)
    }
    res
  }
  f(x1s, mean(x1s)) + f(x2s, mean(x2s))
}

fisherLDA <- function (data) {
  x1s <- pointsByClass(data, 1)
  x2s <- pointsByClass(data, -1)
  solve(fisherCovarMatrix(x1s, x2s)) %*% (mean(x1s) - mean(x2s))
}

correctness <- function (ts, ys) {
  mean(apply(array(1:length(ts)), 1, function(n) {
    if (ts[n] == ys[n]) { 1 } else { 0 }
  }))
}

testFisherLDA <- function (w, testdata) {
  correctness(testdata[,3], apply(testdata[,c(1,2)], 1, function (x) {
    if (t(w) %*% x > 0) { 1 } else { -1 }
  }))
}

visualiseKnoll <- function (file,title,data,w=NULL,xlim=c(-3,3),ylim=c(-3,3)) {
  pdf(file, width=4.5, height=4.5)
  plot.new()
  par(mai=c(0.5, 0.5, 0.5, 0.5))
  plot.window(xlim=xlim, ylim=ylim)
  box()
  axis(1, at=seq(xlim[1], xlim[2], length=xlim[2]-xlim[1]+1))
  axis(2, at=seq(ylim[1], ylim[2], length=ylim[2]-ylim[1]+1))
  title(main=title)
  for (i in 1:dim(data)[1]) {
    if (data[i,3] == -1) {
      points(data[i,1], data[i,2], col="blue", lwd=1, pch=1)
    } else {
      points(data[i,1], data[i,2], col="red", lwd=1, pch=1)
    }
  }
  if (!is.null(w)) {
    f <- function (x) {
      return (-(w[1]*x/w[2]))
    }
    linepts <- seq(xlim[1]-1, xlim[2]+1, len=10)
    lines(linepts, apply(array(linepts), 1, f), col="purple")
  }
  legend("topright", c("-1", "1"),
         title="Selections", lwd=1, pch=1, col=c("blue", "red"))
  dev.off()
}

knollAtrain=readKnollData("../Data/knollA-train.dt")
knollAtest=readKnollData("../Data/knollA-test.dt")
knollAw=fisherLDA(knollAtrain)
knollBtrain=readKnollData("../Data/knollB-train.dt")
knollBtest=readKnollData("../Data/knollB-test.dt")
knollBw=fisherLDA(knollBtrain)
knollCtrain=readKnollData("../Data/knollC-train.dt")
knollCtest=readKnollData("../Data/knollC-test.dt")
knollCw=fisherLDA(knollCtrain)
print("knollA on training set:")
print(testFisherLDA(knollAw, knollAtrain))
print("knollA on testing set:")
print(testFisherLDA(knollAw, knollAtest))
print("knollB on training set:")
print(testFisherLDA(knollBw, knollBtrain))
print("knollB on testing set:")
print(testFisherLDA(knollBw, knollBtest))
print("knollC on training set:")
print(testFisherLDA(knollCw, knollCtrain))
print("knollC on testing set:")
print(testFisherLDA(knollCw, knollCtest))

visualiseKnoll("../img/question2-plot-a.pdf",
               "knollA training data", knollAtrain, knollAw)
visualiseKnoll("../img/question2-plot-b.pdf",
               "knollB training data", knollBtrain, knollBw)
visualiseKnoll("../img/question2-plot-c.pdf",
               "knollC training data", knollCtrain, knollCw)
visualiseKnoll("../img/question2-plot-c-close.pdf",
               "knollC training data closeup", knollCtrain, knollCw, xlim=c(-0.03,0.03))

visualiseKnoll("../img/question2-plot-test-a.pdf",
               "knollA test data", knollAtest, knollAw)
visualiseKnoll("../img/question2-plot-test-b.pdf",
               "knollB test data", knollBtest, knollBw)
