#include <vector>

template<class T>
class vectoru : public std::vector<T> {
	
	public:
	
	// 
};

template<typename T>
std::ostream& operator<<(std::ostream& out,  vectoru<T>& vec) {
	
	typename vectoru<T>::iterator it = vec.begin();
	
	// printing all of the contents of the vector in the following manner:
	// [x, y, z, ..., w]
	
	out << "[";
	
	while (it != vec.end()) {
		out << *it;
		
		it ++;
		out << (it != vec.end() ? ", " : "");
	}
	
	out << "]";
	
	return out;
}
	