#include <iostream>
#include "utils/vectoru.cpp"
#include <string>

using namespace std;

int main() {
	
	vectoru<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);	
	
	cout << v << '\n';
	
	cout << "THE END\n";
	
	return 0;
}