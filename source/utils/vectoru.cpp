#include <vector>
#include <iostream>

template<class T>
class vectoru : public std::vector<T> {
	
	public:
	
	void fill_with(T a, int n = 1) {
		(*this).clear();
		for (int i{0}; i < n;i ++) {
			(*this).push_back(a);
		}
	}
	
	vectoru &operator=(const std::initializer_list<T> &il)
	{
		typename std::initializer_list<T>::iterator it = il.begin();
		
		// Copy the elements from the il into the array.
		while(it != il.end())
		{
			(*this).push_back(*it);
			it ++;
		}

		return *this;
	}
};

/// Printing

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

/// Arithimetical Operations

template<typename T>
vectoru<T>& operator+=(vectoru<T>& vec_l, vectoru<T>& vec_r) {
	
	// vec_l = vec_l + vec_r
	
	typename vectoru<T>::iterator it_l = vec_l.begin();
	typename vectoru<T>::iterator it_r = vec_r.begin();
	
	while (it_l != vec_l.end()) {
		
		*it_l = *it_l + (it_r != vec_r.end() ? *it_r : 0);
		
		it_l ++;
		it_r ++;
	}
	
	return vec_l;
}

template<typename T>
vectoru<T>& operator*=(vectoru<T>& vec_l, vectoru<T>& vec_r) {
	
	// multiplying vec_l by vec_r
	
	typename vectoru<T>::iterator it_l = vec_l.begin();
	typename vectoru<T>::iterator it_r = vec_r.begin();
	
	while (it_l != vec_l.end() && it_r != vec_r.end()) {
		
		*it_l = *it_l * (it_r != vec_r.end() ? *it_r : 1);
		
		it_l ++;
		it_r ++;
	}
	
	return vec_l;
}

template<typename T>
vectoru<T> operator+(vectoru<T>& vec_l, vectoru<T>& vec_r) {
	vectoru<T> v = vec_l;
	v += vec_r;
	return v;
}

template<typename T>
vectoru<T> operator*(vectoru<T>& vec_l, vectoru<T>& vec_r) {
	vectoru<T> v = vec_l;
	v *= vec_r;
	return v;
}

// with numbers

template<typename T>
vectoru<T>& operator+=(vectoru<T>& vec_l, T a) {
	vectoru<T> v;
	v.fill_with(a, vec_l.size());
	return vec_l += v;
}

template<typename T>
vectoru<T> operator+(vectoru<T>& vec_l, T a) {
	vectoru<T> v = vec_l;
	return v += a;
}

template<typename T>
vectoru<T>& operator-=(vectoru<T>& vec_l, T a) {
	return vec_l += -a;
}

template<typename T>
vectoru<T> operator-(vectoru<T>& vec_l, T a) {
	return vec_l + -a;
}

template<typename T>
vectoru<T>& operator*=(vectoru<T>& vec_l, T a) {
	vectoru<T> v;
	v.fill_with(a, vec_l.size());
	return vec_l *= v;
}

template<typename T>
vectoru<T> operator*(vectoru<T>& vec_l, T a) {
	vectoru<T> v = vec_l;
	return v *= a;
}
