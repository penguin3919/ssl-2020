//mnist_openvino
//main9.cpp

#include <iostream>
#include <string>
#include <cmath>
#include <samples/common.hpp>

using namespace InferenceEngine;

int reverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = (unsigned char) (i & 255);
    ch2 = (unsigned char) ((i >> 8) & 255);
    ch3 = (unsigned char) ((i >> 16) & 255);
    ch4 = (unsigned char) ((i >> 24) & 255);
    return (static_cast<int>(ch1) << 24) + (static_cast<int>(ch2) << 16) + (static_cast<int>(ch3) << 8) + ch4;
}

float* tofp16(const short int* i){
	float* arr=new float[10];
	unsigned char sign;
	int expo;
	float mant;
	int  temp;
	for(int k=0;k<10;k++)
	{
		sign=(i[k]>>14)&1;
		expo=((i[k]>>10)&31)-15;
		mant=1;
		temp=i[k]%1024;
		for(int d=9;d>-1;d--)
		{
			mant+=((temp>>d)&1)*(1>>(10-d));
		}
		if(expo==16){std::cout<<"Nan or Inf or 0.00+"<<std::endl;arr[k]=-1;continue;}
		if(expo==-15) 
		{
			expo=-14;
			mant-=1;
		}
		arr[k]=pow(2,expo)*mant*pow(-1,sign+2);
	}
	return arr;
}

int main(int argc, char* argv[]){

	Core ie;
	size_t num=std::stoi(argv[3]);

	std::string binFileName=fileNameNoExt(argv[1])+".bin";
	CNNNetwork network=ie.ReadNetwork(argv[1]);
	
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
	std::vector<std::shared_ptr<float>> imagedata(10000);

	file.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
	magic_number=reverseInt(magic_number);
	file.read(reinterpret_cast<char *>(&number_of_images), sizeof(number_of_images));
	number_of_images=reverseInt(number_of_images);
	file.read(reinterpret_cast<char *>(&n_rows), sizeof(n_rows));
	n_rows=reverseInt(n_rows);
	height=(size_t)n_rows;
	file.read(reinterpret_cast<char *>(&n_cols), sizeof(n_cols));
	n_cols=reverseInt(n_cols);
	width=(size_t)n_cols;
	size_t size= width*height*1;
	
	for(int d=0;d<10000;d++)
	{
		imagedata[d].reset(new float[size], std::default_delete<float[]>());
	}

	size_t count=0;
	size_t count2=0;
	float temp2=0;
	
	while(count2!=10000){
		for(int r=0;r<n_rows;++r){
			for(int c=0;c<n_cols;++c){
				unsigned char temp=0;
				file.read(reinterpret_cast<char *>(&temp),sizeof(temp));
				temp2=(float)temp/255.0;
				imagedata[count2].get()[count++]=temp2;
			}
		}
		count=0;
		count2++;

	}

	//prepare_input_blobs
        

        /** Taking information about all topology inputs **/
        InputsDataMap inputsInfo(network.getInputsInfo());

        if (inputsInfo.size() != 1 && inputsInfo.size() != 2) throw std::logic_error("Sample supports topologies only with 1 or 2 inputs");
        std::string imageInputName;

        InputInfo::Ptr inputInfo = nullptr;

        SizeVector inputImageDims;
        /** Stores input image **/

        /** Iterating over all input blobs **/
        for (auto & item : inputsInfo) {
            /** Working with first input tensor that stores image **/
            if (item.second->getInputData()->getTensorDesc().getDims().size() == 4) {
		imageInputName = item.first;

                inputInfo = item.second;

                
                Precision inputPrecision = Precision::FP32;
                item.second->setPrecision(inputPrecision);
            } 
        }

        if (inputInfo == nullptr) {
            inputInfo = inputsInfo.begin()->second;
        }
        // --------------------------- 6. Prepare output blobs -------------------------------------------------

	OutputsDataMap outputsInfo(network.getOutputsInfo());
	std::string output_name;
	DataPtr outputInfo;

	for(const auto& out : outputsInfo){
		output_name=out.first;
		outputInfo=out.second;
	}
	std::cout<<"outputname"<<output_name<<std::endl;

	const SizeVector outputDims=outputInfo->getTensorDesc().getDims();

	outputInfo->setPrecision(Precision::FP16);
//loading model to the device
	ExecutableNetwork executable_network=ie.LoadNetwork(network,"MYRIAD");
	InferRequest infer_request=executable_network.CreateInferRequest();

//prepare input: data -> Memory Blob

	Blob::Ptr imageInput=infer_request.GetBlob(imageInputName);

	MemoryBlob::Ptr mimage=as<MemoryBlob>(imageInput);
	if(!mimage){printf("cast imageinput to memory blob failed");return 1;}
	auto minputHolder=mimage->wmap();

	size_t num_channels=mimage->getTensorDesc().getDims()[1];
	size_t image_size=mimage->getTensorDesc().getDims()[3]*mimage->getTensorDesc().getDims()[2];
	float* data=minputHolder.as<float*>();
	
            for (size_t pid = 0; pid < image_size; pid++) {
                    /**          [images stride + channels stride + pixel id ] all in bytes            **/
                    data[pid] = imagedata[num].get()[pid];   
            }
	//creating input blob
	infer_request.Infer();
	const Blob::Ptr output_blob=infer_request.GetBlob(output_name);
	MemoryBlob::CPtr moutput=as<MemoryBlob>(output_blob);
	if(!moutput){printf("cast output to memory blob failed\n");}

	auto moutputHolder=moutput->rmap();
	auto* detection=moutputHolder.as<const PrecisionTrait<Precision::FP16>::value_type*>();
	float* detection2=tofp16(detection);
	//tofp16(detection);
	for(int dd=0;dd<10;dd++){
		std::cout<<"detection["<<dd<<"]:"<<detection2[dd]<<std::endl;
	}
	//input_data_close
	delete[] detection2;
	file.close();
	return 0;
}



