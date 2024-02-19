#include "dng.h"
int main()
{
    DNG dng();
    dng.addTag();
    unsigned char* dngBuffer = dng.convert(rawData);
    return 0;
}