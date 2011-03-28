sinc <- function(x) {
  sin(x)/x
}

sigma <- function(u) {
  u/(1+abs(u))
}

sigmad <- function(u) {
  u/((1+abs(u))**2)
}

compz <- function(x, d, M, w) {
  z <- vector(,length=M+1)
  z[1] = 1
  for (i in 1:d) {
    z[i+1] = x[i]
  }
  for (i in (d+1):M) {
    z[i+1] = 0
    for (j in 1:(i-1)) {
      z[i+1] <- z[i+1] + w[i,j]*z[j]
    }
    z[i+1] = sigma(z[i+1])
  }
  z
}

yk <- function(x, d, M, m, w) {
  z <- compz(x, d, M, w)
  z[(M+1-m+1):(M+1)]
}

randomw <- function(M) {
  w <- matrix(,M,M+1)
  for (i in 1:M) {
    for (j in 1:(M+1)) {
      w[i,j] <- runif(1, min=-0.001, max=0.001);
    }
  }
  w
}

error <- function(d, M, m, w, x, t) {
  sum(apply(array(1:dim(x)[1]), 1, function(n) {
    1/2 * (yk(x[n,], d, M, m, w)-t[n])**2
  }))
}

compDhidden <- function(w, z, i) {
  M <- length(z)-1
  if (i == M) {
    z[i+1]
  } else {
    z[i+1] + sum(apply(array((i+1):M), 1, function(k) {
      w[k,i+1]*compDhidden(w, z, k)
    }))
  }
}

diffEn <- function(z, M, m, w, i, j, y) {
  if (i >= M - m + 1) {
    (z[i+1] - y) * z[j+1]
  } else {
    compDhidden(w, z, i) * z[j+1]
  }
}

diffE <- function(zs, d, M, m, w, x, t, i, j) {
  sum(apply(array(1:dim(x)[1]), 1, function(n) {
    z <- zs[n,]
    diffEn(z,M,m,w,i,j,t[n,])
  }))
}

pertube <- function(w, epsilon, i, j) {
  ei <- matrix(0,dim(w)[1],dim(w)[2])
  ei[i,j+1] <- 1
  w+epsilon*ei
}

diffEAnal <- function(d, M, m, w, x, t, i, j) {
  epsilon <- 0.0001
  (error(d, M, m, pertube(w, epsilon, i, j), x, t) - error(d, M, m, w, x, t)) / epsilon
}

data <- read.table("../Data/sincTrain50.dt", stringsAsFactors=FALSE)
x <- matrix(data[,1], dim(data)[1], 1)
t <- matrix(data[,2], dim(data)[1], 1)
d <- 1
M <- 5
m <- 1
w <- randomw(M)
zs <- t(apply(x, 1, function(x) { compz(x, d, M, w) }))
for (i in (d+1):M) {
  print("i")
  print(i)
  for (j in 1:(i-1)) {
    print("j")
    print(j)
    print("comp")
    print(diffE(zs, d, M, m, w, x, t, i, j))
    print("numerical")
    print(diffEAnal(d, M, m, w, x, t, i, j))
  }
}

