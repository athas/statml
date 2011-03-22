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

test <- function(file) {
  data <- tomatrix(readKnollData(file))
  pos <- data[data[,3]==1,c(1,2)]
  neg <- data[data[,3]==-1,c(1,2)]
  sigma <- median(gendiff(pos, neg))
}

test("../Data/knollC-train100.dt")
