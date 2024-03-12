#include <vector>

using namespace std;
using byte = unsigned char;

struct RGB {
	
	byte r = 0, g = 0, b = 0;
	
	RGB() {}
	
	RGB(byte r, byte g, byte b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
};

class Background {
  private:
    
    RGB corners[4] = {RGB(), RGB(), RGB(), RGB()};
    
	// clock-wise starting at bottom_left
    enum corners_e {bl = 0, tl, tr, br};
	
	float lerp(float a, float b, float t) const {	
		return (1 - t)*a + t*b;
	}
	
    RGB lerp(const RGB &A, const RGB &B, float t) const {
		const RGB what = RGB(lerp(A.r, B.r, t), lerp(A.g, B.g, t), lerp(A.b, B.b, t));
		return what;
	}

  public:
    Background(const vector<RGB>& colors) {
		
		// clock-wise starting at bottom_left
		for (int i = 0;i < 4;i ++) {
			corners[i] = colors[i];
		}
	}
	
    ~Background() {};

    RGB sampleUV(float u, float v) const {
		
		RGB a = lerp(corners[bl], corners[br], u);
		RGB b = lerp(corners[tl], corners[tr], u);
		return lerp(b, a, v);
		
	}
};