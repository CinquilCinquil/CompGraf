#include "background.h"

using namespace std;

namespace CG
{
	Background::Background(const vector<RGB>& colors) {
		// clock-wise starting at bottom_left
		for (int i = 0;i < 4;i ++) {
			corners[i] = colors[i];
		}
	}

	Background::~Background(){}

	float Background::lerp(float a, float b, float t) const {	
		return (1 - t)*a + t*b;
	}

	RGB Background::lerp(const RGB &A, const RGB &B, float t) const {
		const RGB what = RGB(lerp(A.r, B.r, t), lerp(A.g, B.g, t), lerp(A.b, B.b, t));
		return what;
	}

	RGB Background::sampleUV(float u, float v) const {
		RGB a = lerp(corners[bl], corners[br], u);
		RGB b = lerp(corners[tl], corners[tr], u);
		return lerp(b, a, v);
	}
}