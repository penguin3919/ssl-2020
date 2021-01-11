//mnist_openvino

#include <iostream>
#include <string>
#include <cmath>
#include <samples/common.hpp>
#include <samples/slog.hpp>
//#include <gflags/gflags.h>
//#include <format_reader/MnistUbyte.h>
#include <ngraph/ngraph.hpp>

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

float tofp16(unsigned short int i){
	unsigned char sign=(i>>14)&1;
	int expo=((i>>10)&31)-15;
	float mant=1;
	int  temp=i;
	temp=temp%1024;
	for(int d=9;d>-1;d--)
	{
		mant+=((temp>>d)&1)*(1>>(10-d));
	}
	if(expo==16){std::cout<<"Nan or Inf or 0.00+"<<std::endl;return -1;}
	if(expo==-15) 
	{
		expo=-14;
		mant-=1;
	}
	return pow(2,expo)*mant*pow(-1,sign+2);
}

int main(int argc, char* argv[]){

	Core ie;
//	const std::string data=argv[2];
	size_t num=std::stoi(argv[3]);

	std::string binFileName=fileNameNoExt(argv[1])+".bin";
	CNNNetwork network=ie.ReadNetwork(argv[1]);

	//printf("%d\n",inputsInfo.size());
       
	//printf("%s\n",argv[2]);
	
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
	//std::shared_ptr<unsigned char> data[10000];
	//std::shared_ptr<float> data[10000];
	std::vector<std::shared_ptr<float>> imagedata(10000);
	//std::shared_ptr<float> imagedata;

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
		//data[d].reset(new unsigned char[size], std::default_delete<unsigned char[]>());
		imagedata[d].reset(new float[size], std::default_delete<float[]>());
		//imagedata[d].reset(new float[size]);
	}
	//imagedata.reset(new float[size], std::default_delete<float[]>());

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
				//imagedata[count2].get()[count++]=temp;
			}
		}
		count=0;
		count2++;

	}
	/*
		for(int r=0;r<n_rows;++r){
			for(int c=0;c<n_cols;++c){
				unsigned char temp=0;
				file.read(reinterpret_cast<char *>(&temp),sizeof(temp));
				temp2=(float)temp/255.0;
				imagedata.get()[count++]=temp2;
			}
		}*/

	//read_input_data

	//printf("count2: %d\n",count2);
/*	
	for(int d=0;d<100;d++){
		printf("%lf\n",(imagedata[1].get())[d]);
	}
*/

	//prepare_input_blobs
        // --------------------------- 5. Prepare input blobs --------------------------------------------------
        slog::info << "Preparing input blobs" << slog::endl;

        /** Taking information about all topology inputs **/
        InputsDataMap inputsInfo(network.getInputsInfo());
	//printf("inputs: %d\n",inputsInfo.size());

        /** SSD network has one input and one output **/
        if (inputsInfo.size() != 1 && inputsInfo.size() != 2) throw std::logic_error("Sample supports topologies only with 1 or 2 inputs");
        /**
         * Some networks have SSD-like output format (ending with DetectionOutput layer), but
         * having 2 inputs as Faster-RCNN: one for image and one for "image info".
         *
         * Although object_datection_sample_ssd's main task is to support clean SSD, it could score
         * the networks with two inputs as well. For such networks imInfoInputName will contain the "second" input name.
         */
        std::string imageInputName;
	//	, imInfoInputName;

        InputInfo::Ptr inputInfo = nullptr;

        SizeVector inputImageDims;
        /** Stores input image **/

        /** Iterating over all input blobs **/
        for (auto & item : inputsInfo) {
            /** Working with first input tensor that stores image **/
	//auto item=*inputsInfo;
	//auto &item=inputsInfo;
            if (item.second->getInputData()->getTensorDesc().getDims().size() == 4) {
                //printf("inputsInfo input blob -> 4\n");
		imageInputName = item.first;

                inputInfo = item.second;

                slog::info << "Batch size is " << std::to_string(network.getBatchSize()) << slog::endl;
		//printf("%s %s\n",imageInputName,inputInfo);
                /** Creating first input blob **/
                //Precision inputPrecision = Precision::U8;
                Precision inputPrecision = Precision::FP32;
                item.second->setPrecision(inputPrecision);
            } 
	    /*
	    else if (item.second->getInputData()->getTensorDesc().getDims().size() == 2) {
                imInfoInputName = item.first;

                Precision inputPrecision = Precision::FP32;
                item.second->setPrecision(inputPrecision);
                if ((item.second->getTensorDesc().getDims()[1] != 3 && item.second->getTensorDesc().getDims()[1] != 6)) {
                    throw std::logic_error("Invalid input info. Should be 3 or 6 values length");
                }
            }*/
        }

        if (inputInfo == nullptr) {
            inputInfo = inputsInfo.begin()->second;
        }
        // -----------------------------------------------------------------------------------------------------

        // --------------------------- 6. Prepare output blobs -------------------------------------------------
/*
	DataPtr output_info=network.getOutputsInfo().begin()->second;
	std::string output_name=network.getOutputsInfo().begin()->first;
	output_info->setPrecision(Precision::FP16);
	printf("output blob!\n");
*/
	OutputsDataMap outputsInfo(network.getOutputsInfo());
	std::string output_name;
	DataPtr outputInfo;

	for(const auto& out : outputsInfo){
		output_name=out.first;
		outputInfo=out.second;
	}

//	output_name=outputsInfo.begin()->first;
	std::cout<<"outputname"<<output_name<<std::endl;

	const SizeVector outputDims=outputInfo->getTensorDesc().getDims();

	//const int maxproposalcount=outputDims[2];
	//const int objectsize=outputDims[3];

	outputInfo->setPrecision(Precision::FP16);
	//printf("max: %d, objectsize: %d\n",maxproposalcount,objectsize);
	/*std::cout<<"outputDims[0]: "<<outputDims[0]<<std::endl;
	std::cout<<"outputDims[1]: "<<outputDims[1]<<std::endl;
	std::cout<<"outputDims[2]: "<<outputDims[2]<<std::endl;
	std::cout<<"outputDims[3]: "<<outputDims[3]<<std::endl;
*/
//loading model to the device
	ExecutableNetwork executable_network=ie.LoadNetwork(network,"MYRIAD");
	InferRequest infer_request=executable_network.CreateInferRequest();

//prepare input: data -> Memory Blob

	//size_t batchsize=network.getBatchSize();
	Blob::Ptr imageInput=infer_request.GetBlob(imageInputName);

	MemoryBlob::Ptr mimage=as<MemoryBlob>(imageInput);
	if(!mimage){printf("cast imageinput to memory blob failed");return 1;}
	auto minputHolder=mimage->wmap();

	size_t num_channels=mimage->getTensorDesc().getDims()[1];
	size_t image_size=mimage->getTensorDesc().getDims()[3]*mimage->getTensorDesc().getDims()[2];
/*
	std::cout<<"input[0]:"<<mimage->getTensorDesc().getDims()[0]<<std::endl;
	std::cout<<"input[1]:"<<mimage->getTensorDesc().getDims()[1]<<std::endl;
	std::cout<<"input[2]:"<<mimage->getTensorDesc().getDims()[2]<<std::endl;
	std::cout<<"input[3]:"<<mimage->getTensorDesc().getDims()[3]<<std::endl;
*/
	printf("channel: %d, image_size: %d\n",num_channels,image_size);
	float* data=minputHolder.as<float*>();

       
            for (size_t pid = 0; pid < image_size; pid++) {
                /** Iterate over all channels **/
                
                    /**          [images stride + channels stride + pixel id ] all in bytes            **/
                    data[pid] = imagedata[num].get()[pid];
                
            }
        
printf("num:%d\n",num);

	//creating input blob
	/*
	Blob::Ptr imageInput=infer_request.SetBlob(imageInputName,data[0]);
	infer_request.Infer();
	Blob::Ptr output=infer_request.GetBlob(output_name);
	printf("output:%d\n",*output);
*/
	infer_request.Infer();
printf("hi\n");	
	//Blob::Ptr output=infer_request.GetBlob(output_name);

	//InferenceEngine::SizeVector blobsize=output->getTensorDesc().getDims();
	//std::cout<<"blobsize W H C"<<blobsize[3]<<" "<<blobsize[2]<<" "<<blobsize[1]<<std::endl;
	//float* blob_data=output->buffer().as<float*>();
	/*
	for(int ddd=0;ddd<10;ddd++){
	std::cout<<"data: "<<blob_data[ddd]<<std::endl;}
	std::cout<<"blob_data size: "<<sizeof(blob_data)<<std::endl;
	//process output -> float 16 * 10
	*/
	
	const Blob::Ptr output_blob=infer_request.GetBlob(output_name);
	MemoryBlob::CPtr moutput=as<MemoryBlob>(output_blob);
	if(!moutput){printf("cast output to memory blob failed\n");}

	auto moutputHolder=moutput->rmap();
	//const short unsigned int* detection=moutputHolder.as<const PrecisionTrait<Precision::U16>::value_type*>();
	auto* detection=moutputHolder.as<const PrecisionTrait<Precision::FP16>::value_type*>();
	
	std::cout<<"detection[0]:"<<tofp16(detection[0])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[1])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[2])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[3])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[4])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[5])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[6])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[7])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[8])<<std::endl;
	std::cout<<"detection[0]:"<<tofp16(detection[9])<<std::endl;
/*	std::cout<<"detection[0]:"<<detection[1]<<std::endl;
	std::cout<<"detection[0]:"<<detection[2]<<std::endl;
	std::cout<<"detection[0]:"<<detection[3]<<std::endl;
	std::cout<<"detection[0]:"<<detection[4]<<std::endl;
	std::cout<<"detection[0]:"<<detection[5]<<std::endl;
	std::cout<<"detection[0]:"<<detection[6]<<std::endl;
	std::cout<<"detection[0]:"<<detection[7]<<std::endl;
	std::cout<<"detection[0]:"<<detection[8]<<std::endl;
	std::cout<<"detection[0]:"<<detection[9]<<std::endl;
*/	
	//std::cout<<"precision name: "<<detection.name()<<std::endl;
	//float* detection=moutputHolder.as<float*>();
	//static_cast<float>
	/*
	printf("detection[0]: %lf \n",static_cast<float>(detection[0]));	
	printf("detection[1]: %lf \n",detection[1]);	
	printf("detection[2]: %lf \n",detection[2]);	
	printf("detection[3]: %lf \n",detection[3]);	
	printf("detection[4]: %lf \n",detection[4]);	
	printf("detection[5]: %lf \n",detection[5]);	
	printf("detection[6]: %lf \n",detection[6]);	
	printf("detection[7]: %lf \n",detection[7]);	
	printf("detection[8]: %lf \n",detection[8]);	
	printf("detection[9]: %lf \n",detection[9]);	
	//printf("result: %f\n",detection[6]);
	*/
/*	
	printf("result: %f\n",static_cast<float>(detection[0]));
	printf("result: %f\n",static_cast<float>(detection[1]));
	printf("result: %f\n",static_cast<float>(detection[2]));
	printf("result: %f\n",static_cast<float>(detection[3]));
	printf("result: %f\n",static_cast<float>(detection[4]));
	printf("result: %f\n",static_cast<float>(detection[5]));
	printf("result: %d\n",detection[6]);
	printf("result: %d\n",detection[7]);
	printf("result: %d\n",detection[8]);
	printf("result: %d\n",detection[9]);
	//printf("result: %f\n",reinterpret_cast<float>(detection[0]));

*/
	//input_data_close
//	file.close();
/*	for(int d=0;d<10000;d++)
	{
	imagedata[d].reset();
	}
*/
	return 0;
}



