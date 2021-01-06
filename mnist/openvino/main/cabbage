#include <iostream>
using namespace std;


int main() {
	unsigned short int a[] = { 11673,12067,12006,11928,11752,12218,11757,11879,11730,11767 };
	float* b;
	//float* b = reinterpret_cast<float*>(&a);
	for (int i = 0; i < 10; i++)
	{
		b= reinterpret_cast<float*>(&(a[i]));
		printf("%d) %f\n", i, *b);
	}


	//printf("%d %f\n", a, *b);

}
