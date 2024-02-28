#include "dng.h"
using namespace tag;
int main()
{
    DNG dng;
    const int width = 2304;
    const int height = 1296;
    const int bpp = 16;
    const int cmos_bits = 10;
    vector<Tag*> tags;
    
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::ImageWidth], {width}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::ImageLength], {height}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::TileWidth], {width}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::TileLength], {height}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::Orientation], {1}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::PhotometricInterpretation], {32803}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::SamplesPerPixel], {1}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::BitsPerSample], {bpp}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::CFARepeatPatternDim], {2,2}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::CFAPattern], {2, 1, 1, 0}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::BlackLevel], {0}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::WhiteLevel], {(1 << cmos_bits) - 1}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::ColorMatrix1], {18491, 10000, -5959, 10000, -2531, 10000,	
        -4145, 10000, 17069, 10000, -2923, 10000,
        -2402, 10000,  -13060, 10000,  25462, 10000}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::ColorMatrix2], {18491, 10000, -5959, 10000, -2531, 10000,	
        -4145, 10000, 17069, 10000, -2923, 10000,
        -2402, 10000,  -13060, 10000,  25462, 10000}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::CalibrationIlluminant1], {21}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::AsShotNeutral], {1,1,1,1,1,1}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::DNGVersion], {1, 4, 0, 0}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::DNGBackwardVersion], {1, 2, 0, 0}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::Make], {"Camera Brand"}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::Model], {"Camera Model"}));
    tags.push_back(new Tag(TagTypeList[TagTypeEnum::PreviewColorSpace], {2}));
    FILE* raw_fp = fopen("../assets/sc3336.raw16","rb");
    fseek(raw_fp, 0 , SEEK_END);
    size_t len = ftell(raw_fp);
    fseek(raw_fp, 0 , SEEK_SET);
    
    unsigned char * rawData = (unsigned char*)malloc(width*height*2);
    fread(rawData, len, 1, raw_fp);
    fclose(raw_fp);
    VBuf* buf = dng.convert(rawData, tags, width, height, bpp);
    
    FILE* fp = fopen("out.dng","w");
    fwrite(buf->raw(), buf->size(), 1, fp);
    fflush(fp);
    free(rawData);
    fclose(fp);
    return 0;
}