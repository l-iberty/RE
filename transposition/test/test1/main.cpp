#include <iostream>
using namespace std;

char input[20] = "5{m_51sa_0rn_9c0s7}";

int wy(int a1, int a2) {
	int result;
	input[a1] ^= input[a2];
	input[a2] ^= input[a1];
	result = a1;
	input[a1] ^= input[a2];
	return result;
}

void encode() {
	wy(13, 16);
	wy(10, 15);
	wy(11, 14);
	wy(10, 16);
	wy(16, 3);
	wy(9, 10);
	wy(8, 2);
	wy(4, 9);
	wy(17, 13);
	wy(9, 11);
	wy(0, 14);
	wy(6, 8);
	wy(3, 10);
	wy(11, 1);
	wy(0, 15);
}

void display() {
	for (int i = 0;i < 19;i++)
		cout << input[i] << " ";
	cout << endl << endl;
}

void loop(int count) {
	cout << "#" << count << endl;
	for (int i = 0;i < count;i++) {
		encode();
	}
	display();
}

void main() {
	for (int i = 0;i < 10;i++)
		loop(i);
}