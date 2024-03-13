#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

namespace CG
{
    using byte = unsigned char;
	
	struct RGB {
	
		byte r = 0;
		byte g = 0;
		byte b = 0;

        RGB();

		RGB(byte r, byte g, byte b);
		
	};
}

#endif