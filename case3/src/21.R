require("kernlab")

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

gendiff <- function(pos, neg) {
  eucnorm <- function(v) {
    sqrt(v[1]**2+v[2]**2)
  }
  res <- matrix(,dim(pos)[1]*dim(neg)[1])
  n <- dim(pos)[1]
  m <- dim(neg)[1]
  for (i in 1:n) {
    for (j in 1:m) {
      res[(i-1)*n+j] <- eucnorm(pos[i,]-pos[j,])
    }
  }
  res
}

compsigma <- function(gamma) {
  sqrt(1/2*gamma)
}

test <- function(file) {
  frame <- readKnollData(file)
  data <- tomatrix(frame)
  pos <- data[data[,3]==1,c(1,2)]
  neg <- data[data[,3]==-1,c(1,2)]
  sigmaJ <- median(gendiff(pos, neg))
  bandwidthJ <- 1/(2*sigmaJ**2)
  Cs <- c(0.1, 1, 10, 100, 1000, 10000)
  gammas <- apply(array(c(-3, -1, 0, 1, 3)), 1, function(i) { bandwidthJ * 2**i })
  bestC <- Cs[1]
  bestgamma <- gammas[1]
  bestcross = 999
  for (C in Cs) {
    for (gamma in gammas) {
      filter <- ksvm(V3~., data=data, kernel="rbfdot", kpar=list(sigma=gamma), C=C, cross=5, type="C-svc")
      if (cross(filter) < bestcross) {
        bestC <- C
        bestgamma <- gamma
        bestcross <- cross(filter)
      }
    }
  }
  print("Best C:")
  print(bestC)
  print("Best gamma:")
  print(bestgamma)
  print("Cross-examination error of best combination:")
  print(bestcross)
  frame <- readKnollData("../Data/knollC-test.dt")
  data <- tomatrix(frame)
  filter <- ksvm(V3~., data=data, kernel="rbfdot", kpar=list(sigma=bestgamma), C=bestC, cross=5, type="C-svc")
  res <- predict(filter, data[,-3])
  print("Correct classifications in test set:")
  print(length(res[res==data[,3]]))
}

#test("../Data/knollC-train100.dt")
