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

class BackgroundColor {
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
    BackgroundColor(const vector<RGB>& colors) {
		
		// clock-wise starting at bottom_left
		for (int i = 0;i < 4;i ++) {
			corners[i] = colors[i];
		}
	}
	
    ~BackgroundColor() {};

    RGB sampleUV(float u, float v) const {
		
		RGB a = lerp(corners[bl], corners[br], u);
		RGB b = lerp(corners[tl], corners[tr], u);
		return lerp(b, a, v);
		
	}
	
	/*
	vector<RGB> do_coolstuff() {
	
		vector<RGB> pixels;
		
		byte b = (int) lerp(0, 255, 0.2);
		
		for (int i = 0;i < h;i ++) {
			for (int j = 0;j < w;j ++) {
				
				byte g = (int) lerp(255, 0, ((float) i) / h);
				byte r = (int) lerp(0, 255, ((float) j) / w);
				
				pixels.push_back(RGB(r, g, b));
			}
		}
		
		return pixels;
	
	}
	*/
};