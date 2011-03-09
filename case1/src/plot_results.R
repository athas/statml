#plot_results <- function( prob_map, centroid, covariance, npoints = 100 ){
# plot_contour(prob_im,centroid,covariance): plot the centroid and the
# contours of the corresponding gaussian on top of the model
# probabability map
#
#
# centroid, and the contours of the probability map (prob_map)
# prob_map: a M x N data matrix
# centroid: 2 x 1 vector
# covariance: 2 x 2 matrix
w <- 640
h <- 480
prob_map <- matrix(vector("numeric",w*h), nrow = h, ncol = w)

dx <- seq( 1, dim( prob_map )[1], length.out = 100 )
dy <- seq( 1, dim( prob_map )[2], length.out = 100 )
gr <- list( x = outer( dy*0, dx, FUN = '+' ), y = outer( dy, dx*0, FUN = '+' ) )

mu <- c(236.669843, 302.225900)
centroid <- mu
Sigma <- matrix(c(2743.075724,-217.633352,-217.633352,8723.156267),2,2)



C <- chol( Sigma )
iSigma <- chol2inv( C )
Z <- 1/( 2*pi*sqrt( prod( diag( C ) ) ) )
print("Z:")
print(Z)
print("iSigma:")
print(iSigma)



dens <- rep( 0, prod( dim( gr$x ) ) )
for( i in 1:prod( dim( gr$x ) ) )
	dens[i] <- exp( -0.5*t( c( gr$x[i], gr$y[i] ) - mu )%*%iSigma%*%( c( gr$x[i], gr$y[i] ) - mu ) )



#pdf("../img/question13-plot.pdf")
plot.new()
image( 1:dim( prob_map )[1], 1:dim( prob_map )[2], prob_map, xlab = "x", ylab = "y" )
points( centroid[1], centroid[2], col = "red",cex = 2 )
contour( dx, dy, matrix( dens, nrow = length( dx ), ncol = length( dy ), byrow = TRUE ), add = TRUE )
#}