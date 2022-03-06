#include <tice.h>
#include <stdlib.h>
#include <stdint.h>

union VRAMPixel
{
	uint16_t v;
	struct
	{
		uint16_t b:5,g:6,r:5;
	};
};

static VRAMPixel* VRAM = (VRAMPixel*)0xD40000;

/* Main function, called first */
int main(void)
{
	for (int i = 0; i < 320*240; i += 1)
	{
		VRAM[i].v = 0;
		VRAM[i].r = 63;
	}

    /* Return 0 for success */
    return 0;
}
