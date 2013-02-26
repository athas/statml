import Image
from numpy import *

im = Image.open("kande1.ppm")
#box = (150, 264, 330, 328)
box = (150, 264, 155, 328)
train = im.crop(box)
#train.show()
pix = train.load()

R = []
G = []
B = []
for p in train.getdata():
	R.append(p[0])
	G.append(p[1])
	B.append(p[2])
R = array(R)
G= array(G)
B = array(B)

print (cov(column_stack([R,G,B])))
print cov(R,G,B)
#variance = cov(t_a)