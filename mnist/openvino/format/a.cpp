#include <iostream>
#include <fstream>
#include <string>

int reverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = (unsigned char)(i & 255);
    ch2 = (unsigned char)((i >> 8) & 255);
    ch3 = (unsigned char)((i >> 16) & 255);
    ch4 = (unsigned char)((i >> 24) & 255);
    return (static_cast<int>(ch1) << 24) + (static_cast<int>(ch2) << 16) + (static_cast<int>(ch3) << 8) + ch4;
}

int main() {

	//read_input_data
	std::ifstream file("t10k-images-idx3-ubyte", std::ios::binary);
	//file.open("t10k-images-idx3-ubyte");
	if (!file.is_open()) {
		printf("file not opened\n");
		return 0;
	}

	int magic_number = 0;
	int number_of_images = 0;
	int n_rows = 0;
	int n_cols = 0;
	size_t height = 0;
	size_t width = 0;
	//std::shared_ptr<unsigned char> data[10000];
	std::shared_ptr<float> data[10000];

	file.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	//printf("%d\n",magic_number);
	file.read(reinterpret_cast<char*>(&number_of_images), sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	file.read(reinterpret_cast<char*>(&n_rows), sizeof(n_rows));
	n_rows = reverseInt(n_rows);
	height = (size_t)n_rows;
	file.read(reinterpret_cast<char*>(&n_cols), sizeof(n_cols));
	n_cols = reverseInt(n_cols);
	width = (size_t)n_cols;


	//printf("%d\n",number_of_images);
	size_t size = width * height * 1;
	for (int d = 0; d < 100; d++)
	{
		//data[d].reset(new unsigned char[size], std::default_delete<unsigned char[]>());
		data[d].reset(new float[size], std::default_delete<float[]>());
	}
	size_t count = 0;
	size_t count2 = 0;
	float temp2 = 0;
	while (count2 != 100) {
		for (int r = 0; r < n_rows; ++r) {
			for (int c = 0; c < n_cols; ++c) {
				unsigned char temp = 0;
				file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				temp2 = (float)temp / 255.0;
				//printf("%d\n", temp2);
				
				//data[count2].get()[count++] = temp;
				data[count2].get()[count++] = temp2;
				//temp2 = (2.0 * ((float)temp / 255.0)) - 1.0;
				//printf("%d\n", temp2);
			}
		}
		count = 0;
		count2++;

	}
	//read_input_data
	//printf("%f\n", data[1].get());
	//printf("count2: %d\n",count2);
	
	for (int d = 0; d < 100; d++) {
		printf("%lf\n", (data[1].get())[d]);
	}

	//float temp3 = 159;
	//temp3= ((((float)temp3+255.0) / 255.0)) - 1.0;
	//temp3 = (((temp3 + 255.0) / 255.0)) - 1.0;
	//temp3 = temp3/ 255.0;
	//printf("temp3: %lf\n", temp3);

	file.close();

}