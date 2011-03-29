sinc <- function(x) {
  sin(x)/x
}

sigma <- function(u) {
  u/(1+abs(u))
}

sigmad <- function(u) {
  u/((1+abs(u))**2)
}

## Compute the a and z values as a two-column matrix where the first
## column is the a's and the second column the z's.  All indexes are
## shifted one to make room for a[1]/z[1] - the bias parameter (always
## 1).
compaz <- function(x, M, m, w) {
  z <- vector(,length=M+1)
  a <- vector(,length=M+1)
  d <- length(x)
  z[1] = 1
  a[1] = 1
  for (i in 1:d) {
    z[i+1] = x[i]
    a[i+1] = x[i]
  }
  ## Compute hidden units.
  for (i in (d+1):(M-m)) {
    a[i+1] = 0
    for (j in 1:(i-1)) {
      a[i+1] <- a[i+1] + w[i,j]*z[j]
    }
    z[i+1] = sigma(a[i+1])
  }
  ## Compute output units.
  for (i in (M-m+1):M) {
    a[i+1] = 0
    for (j in 1:(i-1)) {
      a[i+1] <- a[i+1] + w[i,j]*z[j]
    }
    z[i+1] = a[i+1]
  }
  matrix(c(a, z), M+1, 2)
}

## Extract the m output neurons.
yk <- function(z, m) {
  M <- length(z)-1
  z[(M+1-m+1):(M+1)]
}

## Create random initial weights.
randomw <- function(M) {
  w <- matrix(,M,M+1)
  for (i in 1:M) {
    for (j in 1:(M+1)) {
      w[i,j] <- runif(1, min=-0.001, max=0.001);
    }
  }
  w
}

error <- function(zs, m, t) {
  sum(apply(array(1:(dim(t)[1])), 1, function(n) {
    1/2 * (yk(zs[m,], m)-t[n])**2
  }))
}

compDels <- function(a, z, m, w, y, d) {
  M <- length(a)-1
  del <- vector(,M)
  ## Output units.
  for (i in M:(M-m+1)) {
    del[i] <- z[i+1] - y[i-M+m]
  }
  ## Hidden units.
  for (i in (M-m):(d+1)) {
    del[i] <- sigmad(a[i+1]) * sum(apply(array((i+1):M), 1, function (k) {
      w[k,i+1]*del[k]
    }))
  }
  del
}

diffE <- function(as, zs, m, w, x, t, i, j) {
  d <- dim(x)[2]
  sum(apply(array(1:dim(x)[1]), 1, function(n) {
    z <- zs[n,]
    a <- as[n,]
    del <- compDels(a, z, m, w, t[n,], d)
    del[i] * z[j+1]
  }))
}

pertube <- function(w, epsilon, i, j) {
  ei <- matrix(0,dim(w)[1],dim(w)[2])
  ei[i,j+1] <- 1
  w+epsilon*ei
}

diffEAnal <- function(zs, M, m, w, x, t, i, j) {
  epsilon <- 0.0001
  zsi <- t(apply(x, 1, function(x) {
    compaz(x, M, m, pertube(w, epsilon, i, j))[,2]
  }))
  (error(zsi, m, t) - error(zs, m, t)) / epsilon
}

compareDiffs <- function(file, M) {
  data <- read.table(file, stringsAsFactors=FALSE)
  x <- matrix(data[,1], dim(data)[1], 1)
  t <- matrix(data[,2], dim(data)[1], 1)
  d <- 1
  m <- 1
  w <- randomw(M)
  azs <- t(apply(x, 1, function(x) { compaz(x, M, m, w) }))
  as <- azs[,1:6]
  zs <- azs[,7:12]
  for (i in (d+1):M) {
    print("i")
    print(i)
    for (j in 1:(i-1)) {
      print("j")
      print(j)
      print("comp")
      print(diffE(as, zs, m, w, x, t, i, j))
      print("numerical")
      print(diffEAnal(zs, M, m, w, x, t, i, j))
    }
  }
}

test <- function(file, M) {
  data <- read.table(file, stringsAsFactors=FALSE)
  x <- matrix(data[,1], dim(data)[1], 1)
  t <- matrix(data[,2], dim(data)[1], 1)
  d <- 1
  m <- 1
  w <- randomw(M)
  azs <- t(apply(x, 1, function(x) { compaz(x, M, m, w) }))
  zs <- azs[,2]
  as <- azs[,1]
  for (i in (d+1):M) {
    print("i")
    print(i)
    for (j in 1:(i-1)) {
      print("j")
      print(j)
      print(diffE(zs, m, w, x, t, i, j))
    }
  }
}
