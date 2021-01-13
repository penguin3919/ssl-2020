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
	std::ifstream file("t10k-labels-idx1-ubyte", std::ios::binary);

	
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
	int* data = new int[10000];
	//std::shared_ptr<float> data[10000];

	file.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	printf("%d\n",magic_number);
	file.read(reinterpret_cast<char*>(&number_of_images), sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	printf("%d\n", number_of_images);

	size_t count = 0;
	while (count != 10000) {
		unsigned char temp = 0;
		file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		data[count++] = temp;
	}

	for (int d = 0; d < 10; d++) {
		printf("%d\n", data[d]);
	}

	delete[] data;
	file.close();

}