1. mnist6.tflite:
  coral dev board나 coral accelerator에 사용할 tflite model은 quantize가 필요하고, edgetpu compiler를 거친 다음 사용하는데,
  raspberry pi 에서 TPU의 도움 없이 사용할 모델은 quantize가 필요없다. 
  (-> mnist_cpu.py, mnist.py)
 
