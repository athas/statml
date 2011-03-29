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

visualise <- function(file, plotfile, C) {
  frame <- readKnollData(file)
  data <- tomatrix(frame)
  pos <- data[data[,3]==1,c(1,2)]
  neg <- data[data[,3]==-1,c(1,2)]
  gamma <- 1.907632
  filter <- ksvm(V3~., data=data, kernel="rbfdot", kpar=list(sigma=gamma), C=C, cross=5, type="C-svc")
  index <- SVindex(filter)
  pdf(plotfile)
  vectors <- data[index,c(1,2)]
  alpha <- alpha(filter)[[1]]
  plot(type="p", data[,c(1,2)], col="black")
  points(vectors[alpha<C,], col="blue")
  points(vectors[alpha==C,], col="red")
  legend("topright", c("Not a support vector",
                       "Free support vectors",
                       "Bounded support vectors"),
         title="Support vectors", lwd=1, pch=1, col=c("black", "blue", "red"))
  dev.off()
}


countvectors <- function(file, gamma, C) {
  frame <- readKnollData(file)
  data <- tomatrix(frame)
  pos <- data[data[,3]==1,c(1,2)]
  neg <- data[data[,3]==-1,c(1,2)]
  filter <- ksvm(V3~., data=data, kernel="rbfdot", kpar=list(sigma=gamma), C=C, cross=5, type="C-svc")
  print("Number of support vectors")
  print(nSV(filter))
}
