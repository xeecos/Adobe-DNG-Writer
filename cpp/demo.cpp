#include "dng.h"
int main()
{
    DNG dng();
    const int width = 2304;
    const int height = 1296;
    vector<Tag*> tags;
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::ImageWidth], {width}));
    tags.push_back(new Tag(TagType.ImageLength, height));
    tags.push_back(new Tag(TagType.TileWidth, width));
    tags.push_back(new Tag(TagType.TileLength, height));
    tags.push_back(new Tag(TagType.Orientation, 1));
    tags.push_back(new Tag(TagType.PhotometricInterpretation, 32803));
    tags.push_back(new Tag(TagType.SamplesPerPixel, 1));
    tags.push_back(new Tag(TagType.BitsPerSample, bpp));
    tags.push_back(new Tag(TagType.CFARepeatPatternDim, [2,2]));
    tags.push_back(new Tag(TagType.CFAPattern, [2, 1, 1, 0]));
    tags.push_back(new Tag(TagType.BlackLevel, 0));
    tags.push_back(new Tag(TagType.WhiteLevel, ((1 << cmos_bits) -1) ));
    tags.push_back(new Tag(TagType.ColorMatrix1, ccm1));
    tags.push_back(new Tag(TagType.ColorMatrix2, ccm2));
    tags.push_back(new Tag(TagType.CalibrationIlluminant1, 21));
    tags.push_back(new Tag(TagType.AsShotNeutral, [[1,1],[1,1],[1,1]]));
    tags.push_back(new Tag(TagType.DNGVersion, [1, 4, 0, 0]));
    tags.push_back(new Tag(TagType.DNGBackwardVersion, [1, 2, 0, 0]));
    tags.push_back(new Tag(TagType.Make, "Camera Brand"));
    tags.push_back(new Tag(TagType.Model, "Camera Model"));
    tags.push_back(new Tag(TagType.PreviewColorSpace, [2]));

    unsigned char* dngBuffer = dng.convert(rawData);
    return 0;
}