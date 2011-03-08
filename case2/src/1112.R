##
##  Reads the body fat data set from the bodyfat.mat file.
##

load( '../Data/bodyfat.RData' )

ridx <- sample( 1:dim( data )[1], dim( data )[1] )
test <- data[ridx[201:length( ridx )],]
train <- data[ridx[1:200],]

linearBasis <- function (j,x) {
  x[j]**1
}

designMatrix <- function (M,xs) {
  N <- dim(xs)[1]
  res <- matrix(,N,M)
  for (j in 1:M) {
    for (i in 1:N) {
      res[i,j] <- linearBasis(j,xs[i,])
    }
  }
  res
}

pseudoinverse <- function (M) {
  solve(t(M)%*%M)%*%t(M)
}

paramML <- function (M, xs, t) {
  D <- designMatrix(M,xs)
  pseudoinverse(D)%*%t
}

MAPest <- function (precision, M, xs, ts) {
  D <- dim(xs)[2]
  noiseprec <- 1
  des <- designMatrix(M, xs)
  Sn = solve(precision * diag(D)+noiseprec*t(des)%*%des)
  noiseprec * Sn %*% t(des) %*% ts
}

linearY <- function (x, ws) {
  D <- length(ws)
  ret <- 0
  for (i in 1:D) {
    ret <- ret + ws[i] * x[i]
  }
  ret
}

RMS <- function (ts, xs, ws) {
  N <- dim(xs)[1]
  sqrt(1/N * sum(apply(array(1:N), 1, function(n) {
    (ts[n] - linearY(xs[n,], ws))**2
  })))
}

avgParamMLErr <- function (selx, seltest) {
  ret <- 0
  D <- dim(selx)[2]
  wss <- apply(array(1:10), 1, function(i) {
    paramML <- paramML(D, selx, train[,2])
    RMS(array(test[,2]), seltest, paramML)
  })
  sum(wss) / length(wss)
}

testPrec <- function (precision, xs, txs) {
  RMS(test[,2], txs,
      MAPest(precision, dim(xs)[2], xs, train[,2]))
}

avgComp <- function (n, f) {
  res=apply(1:n, 1, f)
  sum(res)/length(res)
}

linearBasis <- function (j,x) {
  x[j]**1
}

designMatrix <- function (M,xs) {
  N <- dim(xs)[1]
  res <- matrix(,N,M)
  for (j in 1:M) {
    for (i in 1:N) {
      res[i,j] <- linearBasis(j,xs[i,])
    }
  }
  res
}

pseudoinverse <- function (M) {
  solve(t(M)%*%M)%*%t(M)
}

paramML <- function (M, xs, t) {
  D <- designMatrix(M,xs)
  pseudoinverse(D)%*%t
}

MAPest <- function (precision, M, xs, ts) {
  D <- dim(xs)[2]
  noiseprec <- 1
  des <- designMatrix(M, xs)
  Sn = solve(precision * diag(D)+noiseprec*t(des)%*%des)
  noiseprec * Sn %*% t(des) %*% ts
}

linearY <- function (x, ws) {
  D <- length(ws)
  ret <- 0
  for (i in 1:D) {
    ret <- ret + ws[i] * x[i]
  }
  ret
}

RMS <- function (ts, xs, ws) {
  N <- dim(xs)[1]
  sqrt(1/N * sum(apply(array(1:N), 1, function(n) {
    (ts[n] - linearY(xs[n,], ws))**2
  })))
}

paramMLErr <- function (selx, seltest) {
  D <- dim(selx)[2]
  paramML <- paramML(D, selx, train[,2])
  RMS(array(test[,2]), seltest, paramML)
}

testPrec <- function (precision, xs, txs) {
  RMS(test[,2], txs,
      MAPest(precision, dim(xs)[2], xs, train[,2]))
}

avgComp <- function (n, f) {
  res=apply(array(1:n), 1, f)
  sum(res)/length(res)
}

precs <- array(seq(-1000,1000,length=30))

sel1MAPres <- apply(precs, 1, function (prec) {
    selx    <- train[,c(4,7,8,9)]
    seltest <- test[,c(4,7,8,9)]
    testPrec(prec, selx, seltest)
})
sel2MAPres <- apply(precs, 1, function (prec) {
    selx <- train[,c(1)]
    dim(selx) <- c(length(selx),1)
    seltest <- test[,c(1)]
    dim(seltest) <- c(length(seltest),1)
    testPrec(prec, selx, seltest)
})
perr1 <- paramMLErr(train[,c(4,7,8,9)], test[,c(4,7,8,9)])
sel2x <- train[,c(8)]
sel2test <- test[,c(8)]
dim(sel2x) <- c(length(sel2x),1)
dim(sel2test) <- c(length(sel2test),1)
perr2 <- paramMLErr(sel2x, sel2test)

print("Selection 1 maximum likelyhood error:")
print(perr1)
print("Selection 2 maximum likelyhood error:")
print(perr2)

pdf("../img/question12-plot.pdf")
plot.new()
plot.window(xlim=c(-1000,1000), ylim=c(4,10))
box()
axis(1, at=seq(-1000, 1000, length=51))
axis(2)
title(main="RMS error of MAP estimation")
lines(precs,sel1MAPres, lwd=1, pch=1, col="blue")
lines(precs,sel2MAPres, lwd=1, pch=1, col="red")
lines(c(-1000,1000), c(perr1, perr1), lwd=1, pch=1, col="darkgreen")
lines(c(-1000,1000), c(perr2,perr2), lwd=1, pch=1, col="brown")
legend("topright", c("Selection 1 (MAP)",
                     "Selection 2 (MAP)",
                     "Selection 1 (ML)",
                     "Selection 2 (ML)"),
       title="Selections", lwd=1, pch=1, col=c("blue", "red", "darkgreen", "brown"))
dev.off()
pdf("../img/question12-plot-b.pdf")
plot.new()
plot.window(xlim=c(-1000,1000), ylim=c(4.5,5.5))
box()
axis(1, at=seq(-1000, 1000, length=51))
axis(2)
title(main="RMS error of MAP estimation (closeup)")
lines(precs,sel1MAPres, lwd=1, pch=1, col="blue")
lines(c(-1000,1000), c(perr1, perr1), lwd=1, pch=1, col="darkgreen")
legend("topright", c("Selection 1 (MAP)",
                     "Selection 1 (ML)"),
       title="Selections", lwd=1, pch=1, col=c("blue", "darkgreen"))
dev.off()
