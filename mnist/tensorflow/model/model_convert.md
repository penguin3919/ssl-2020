## model convert1
(saved_model.pb -> .tflite -> _edgetpu.tflite)

1. tensorflow model
(saved_model.pb)

2. post quantization
!ADD(quantize -.py)

3. tensorflow lite convert(in tensorflow)
(convert -> DEFAULT option -> quantize weight activation)
(mnist.tflite)

4. edgetpu compiler ( quantized model only) (in linux)
(mnist_edgetpu.tflite)
(result -> !ADD log file) 

*compare edgetpu model <-> non-tpu model 

??? with schema.fbs (in linux) -> convert to json file 

*compare tensorflow model <-> quantized tensorflow model 

??? (in tensorflow) layer+dim 
#fake quantize layer -> not supported in openvino 

## model convert2
(saved_model.pb -> .xml /.bin)

mo_tf.py [1 28 28 1] ([-1 28 28 1])
(set N W H C order in order openvino version???)

saved_model.pb(saved_model_X_quant)

saved_model(saved_model/variable, saved_model/saved_model.pb, ...)






