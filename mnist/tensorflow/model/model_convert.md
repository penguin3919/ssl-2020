## model convert1
(saved_model.pb -> .tflite -> _edgetpu.tflite)

1. tensorflow model
(saved_model.pb)

2. post quantization
- [x] !ADD(quantize -.py)

3. tensorflow lite convert(in tensorflow)
(convert -> DEFAULT option -> quantize weight activation)
(mnist.tflite)

4. edgetpu compiler ( quantized model only) (in linux)
(mnist_edgetpu.tflite)
- [x] (result -> !ADD log file) 

*compare edgetpu model <-> non-tpu model 

??? with schema.fbs (in linux) -> convert to json file 

*compare tensorflow model <-> quantized tensorflow model 

??? (in tensorflow) layer+dim 
#fake quantize layer -> not supported in openvino 

## model convert2
(saved_model.pb -> .xml /.bin)

mo_tf.py [1 28 28 1] ([-1 28 28 1])
(set N W H C order in older openvino version???)

model optimizer 사용시 input data는 FP16, FP32만 지원된다.

openvino에 있는 format reader 파일에는 mnist data를 U8로 읽어온다.

PQ는 안 되지만, QAT는 지원이 된다. 그러나 tf2에서 QAT가 experimental 이라서 사용가능한지 확인해봐야한다.
openvino 에서 지원하는 quantize는? (<-> fake quantize)

saved_model.pb(saved_model_X_quant)

saved_model(saved_model/variable, saved_model/saved_model.pb, ...)

Openvino 모델 변환
Tensorflow quantize 한 것으로 mo_tf.py 돌리는 건 됐는데
막상 예제를 만들어서 사용하려고 하면 fake quantize를 지원하지 않는다고
실행이 되지 않는다.

Openvino에도 fake quantize 란게 있는데 같은건지 봐야하낟





