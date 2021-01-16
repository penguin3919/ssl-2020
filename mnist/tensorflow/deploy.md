deploy

https://coral.ai/docs/dev-board/get-started/?gclid=EAIaIQobChMIqLK436yg7gIVjjMqCh2_TQwbEAAYASAAEgJ66vD_BwE#run-pycoral
-> bird

1. Download the example code from github:

sudo apt-get install git
mkdir coral && cd coral
git clone https://github.com/google-coral/pycoral.git
cd pycoral

(pycoral update!!)

wget -P test_data/ https://github.com/google-coral/test_data/raw/master/mobilenet_v2_1.0_224_inat_bird_quant_edgetpu.tflite

wget -P test_data/ https://github.com/google-coral/test_data/raw/master/inat_bird_labels.txt

wget -P test_data/ https://github.com/google-coral/test_data/raw/master/parrot.jpg

python3 examples/classify_image.py \
--model test_data/mobilenet_v2_1.0_224_inat_bird_quant_edgetpu.tflite \
--labels test_data/inat_bird_labels.txt \
--input test_data/parrot.jpg
