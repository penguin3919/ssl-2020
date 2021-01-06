#read_data

import gzip
import numpy

from tensorflow.python.framework import dtypes
from tensorflow.python.platform import gfile

#tensorflow

#   --normalize

#from tensorflow.python.ops import math_ops

from tensorflow import keras
import tensorflow as tf

def _read32(bytestream):
    dt=numpy.dtype(numpy.uint32).newbyteorder('>')
    return numpy.frombuffer(bytestream.read(4),dtype=dt)[0]

def _extract_image(f):
    with gzip.GzipFile(fileobj=f) as bytestream:
        magic=_read32(bytestream)
        #print(magic)
        num_images=_read32(bytestream)#10000
        #num_images=1
        #print(num_images)
        rows=_read32(bytestream)#28
        cols=_read32(bytestream)#28
        buf=bytestream.read(rows*cols*num_images)
        data=numpy.frombuffer(buf,dtype=numpy.uint8)
        data=data.reshape(num_images,rows,cols,1)
        return data
    
def _extract_label(f):
    with gzip.GzipFile(fileobj=f) as bytestream:
        magic=_read32(bytestream)
        num_items=_read32(bytestream)
        #num_items=1
        #print(num_items)
        buf=bytestream.read(num_items)
        labels=numpy.frombuffer(buf,dtype=numpy.uint8)
        #one_hot_encoding_X(??)
        return labels

#------------------------/data
    
test_images="t10k-images-idx3-ubyte.gz"
f=gfile.Open(test_images,'rb')
data=_extract_image(f)

test_labels="t10k-labels-idx1-ubyte.gz"
f2=gfile.Open(test_labels,'rb')
labels=_extract_label(f2)

#--normalize
#type -> tensor???

#print(data[1][3])

#data=math_ops.cast(data,dtypes.float32)
data=numpy.float32(data)
#data=(2.0*(data/255.0))-1.0
data=data/255.0

#print(data[1][3])

#print(labels[0])
#print(labels[0].dtype)
#labels=math_ops.cast(labels,dtypes.int32)
labels=numpy.int32(labels)
#print(labels[0])
#print(labels[0].dtype)

#------------------------data/

#------------------------/eval

#model=tf.saved_model.load('saved_model')
model=tf.keras.models.load_model('saved_model2')

#output=model.evaluate(x=data,y=labels,verbose=2)
#output=model.predict(x=data,verbose=2)
output=model.predict(x=data)
a=output[0]
a=list(a)
print(a)
#print(a.index(max(a)))
#print(labels[0])

