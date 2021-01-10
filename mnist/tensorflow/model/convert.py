import tensorflow as tf

#load_model
#model=tf.keras.models.load_model('saved_model_quant')

#convert the model
converter=tf.lite.TFLiteConverter.from_saved_model('saved_model_X_quant')
#converter.post_training_quantize = True
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model=converter.convert()

#save the model

with open('model.tflite','wb') as f:
    f.write(tflite_model)
