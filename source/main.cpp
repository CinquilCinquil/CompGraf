#include <iostream>
#include "utils/vectoru.cpp"
#include <string>

using namespace std;

int main() {
	
	vectoru<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);	
	
	vectoru<int> v2;
	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(30);	
	
	cout << v << '\n';
	v += 1;
	cout << v << '\n';
	v *= 2;
	
	cout << v << '\n';
	v = v2 + 1;
	cout << v << '\n';
	v = v2 * 2;
	cout << v << '\n';
	
	cout << v2 << '\n';
	
	//vectoru<int> v3 = (v + v2);
	
	//cout << v3 << '\n';
	//cout << v << '\n';
	//cout << v2 << '\n';
	
	cout << "THE END\n";
	
	return 0;
}