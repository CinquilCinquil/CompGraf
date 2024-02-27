#include <vector>
#include <iostream>

template<class T>
class vectoru : public std::vector<T> {
	
	public:
	
	//
};

// Printing

template<typename T>
std::ostream& operator<<(std::ostream& out,  vectoru<T>& vec) {
	
	// printing all of the contents of the vector in the following manner:
	// [x, y, z, ..., w]
	
	typename vectoru<T>::iterator it = vec.begin();
	
	out << "[";
	
	while (it != vec.end()) {
		out << *it;
		
		it ++;
		out << (it != vec.end() ? ", " : "");
	}
	
	out << "]";
	
	return out;
}

// Arithimetical Operations

template<typename T>
vectoru<T>& operator+(vectoru<T>& vec_l, vectoru<T>& vec_r) {
	
	// adding the contents of vec_r to vec_l
	
	typename vectoru<T>::iterator it_l = vec_l.begin();
	typename vectoru<T>::iterator it_r = vec_r.begin();
	
	while (it_l != vec_l.end() && it_r != vec_r.end()) {
		
		*it_l = *it_l + *it_r;
		
		it_l ++;
		it_r ++;
	}
	
	return vec_l;
}

template<typename T>
vectoru<T>& operator+(vectoru<T>& vec_l, T a) {
	
	// adding 'a' to the vector
	
	typename vectoru<T>::iterator it_l = vec_l.begin();
	
	while (it_l != vec_l.end()) {
		
		*it_l = *it_l + a;
		
		it_l ++;
	}
	
	return vec_l;
}

template<typename T>
vectoru<T>& operator-(vectoru<T>& vec_l, T a) {
	return (vec_l + -a);
}

template<typename T>
vectoru<T>& operator*(vectoru<T>& vec_l, T a) {
	
	// multiplying the vector by 'a'
	
	typename vectoru<T>::iterator it_l = vec_l.begin();
	
	while (it_l != vec_l.end()) {
		
		*it_l = *it_l * a;
		
		it_l ++;
	}
	
	return vec_l;
}

	