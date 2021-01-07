//mnist_openvino

#include <iostream>
#include <string>
#include <samples/common.hpp>
//#include <gflags/gflags.h>
#include <format_reader/MnistUbyte.h>

using namespace InferenceEngine;
//using namespace FormatReader;

int reverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = (unsigned char) (i & 255);
    ch2 = (unsigned char) ((i >> 8) & 255);
    ch3 = (unsigned char) ((i >> 16) & 255);
    ch4 = (unsigned char) ((i >> 24) & 255);
    return (static_cast<int>(ch1) << 24) + (static_cast<int>(ch2) << 16) + (static_cast<int>(ch3) << 8) + ch4;
}

int main(int argc, char* argv[]){

	Core ie;
//	const std::string data=argv[2];

	std::string binFileName=fileNameNoExt(argv[1])+".bin";
	CNNNetwork network=ie.ReadNetwork(argv[1]);

//input
	InputsDataMap inputsInfo(network.getInputsInfo());
	//printf("%d\n",inputsInfo.size());
       
	//printf("%s\n",argv[2]);
	//FormatReader::MnistUbyte* mnistdata=new FormatReader::MnistUbyte(argv[2]);
	//printf("%d\n",mnistdata->size());
	
	//read_input_data
	std::ifstream file(argv[2],std::ios::binary);
	if(!file.is_open()){
		printf("file not opened\n");
		return 0;
	}

	int magic_number=0;
	int number_of_images=0;
	int n_rows=0;
	int n_cols=0;
	size_t height=0;
	size_t width=0;
	std::shared_ptr<unsigned char> data[10000];

	file.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
	magic_number=reverseInt(magic_number);
	//printf("%d\n",magic_number);
	file.read(reinterpret_cast<char *>(&number_of_images), sizeof(number_of_images));
	number_of_images=reverseInt(number_of_images);
	file.read(reinterpret_cast<char *>(&n_rows), sizeof(n_rows));
	n_rows=reverseInt(n_rows);
	height=(size_t)n_rows;
	file.read(reinterpret_cast<char *>(&n_cols), sizeof(n_cols));
	n_cols=reverseInt(n_cols);
	width=(size_t)n_cols;

	//printf("%d\n",number_of_images);
	size_t size= width*height*1;
	for(int d=0;d<10000;d++)
	{
		data[d].reset(new unsigned char[size], std::default_delete<unsigned char[]>());
	}
	size_t count=0;
	size_t count2=0;
	while(count2!=10000){
		for(int r=0;r<n_rows;++r){
			for(int c=0;c<n_cols;++c){
				unsigned char temp=0;
				file.read(reinterpret_cast<char *>(&temp),sizeof(temp));
				data[count2].get()[count++]=temp;
			}
		}
		count=0;
		count2++;

	}
	//read_input_data

	//printf("count2: %d\n",count2);

	file.close();

//	delete mnistdata;

	return 0;
}



