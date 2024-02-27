#include <iostream>
#include "utils/vectoru.cpp"
#include <string>

using namespace std;

int main() {
	
	vectoru<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);	
	
	vectoru<int> v2;
	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(30);
	
	vectoru<int> v3;
	
	v3 = v1 + v2;
	cout << v3 << '\n';
	
	v3 = v1 * v2;
	cout << v3 << '\n';
	
	v3 = v1 * 5;
	cout << v3 << '\n';
	
	v3 -= 1;
	cout << v3 << '\n';
	
	cout << v1 << '\n';
	cout << v2 << '\n';
	
	cout << "THE END\n";
	
	return 0;
}