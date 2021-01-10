import gzip

f2=gzip.open("t10k-labels-idx1-ubyte.gz",'rb')
f2.read(8)

text=f2.read(1)
data=text.decode('utf-8')

print((data[2:]))
