import gzip
import numpy

from tensorflow.python.framework import dtypes
from tensorflow.python.platform import gfile

def _read32(bytestream):
    dt=numpy.dtype(numpy.uint32).newbyteorder('>')
    return numpy.frombuffer(bytestream.read(4),dtype=dt)[0]


def _extract_image(f):
    with gzip.GzipFile(fileobj=f) as bytestream:
        magic=_read32(bytestream)
        #print(magic)
        num_images=_read32(bytestream)#10000
        #print(num_images)
        rows=_read32(bytestream)#28
        cols=_read32(bytestream)#28
        buf=bytestream.read(rows*cols*num_images)
        data=numpy.frombuffer(buf,dtype=numpy.uint8)
        data=data.reshape(num_images,row,cols,1)
        return data
def _extract_label(f):
    with gzip.GzipFile(fileobj=f) as bytestream:
        magic=_read32(bytestream)
        num_items=_read32(bytestream)
        #print(num_items)
        buf=bytestream.read(num_items)
        labels=numpy.frombuffer(buf,dtype=numpy.uint8)
        #one_hot_encoding_X(??)
        return labels
        
        
    

'''    
train_images="train-images-idx3-ubyte.gz"
f=gfile.Open(train_images,'rb')
train_data=_extract_image(f)
'''
train_labels="train-labels-idx1-ubyte.gz"
f2=gfile.Open(train_labels,'rb')
train_label=_extract_label(f2)

