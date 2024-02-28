#pragma once
#include <vector>
#include "tag.h"
#include "ifd.h"
#include "types.h"
#include "vbuf.h"
using namespace std;
using namespace types;
struct ImageDataStrip
{
    unsigned char* data;
    int length;
};
class DNG
{
    public:
        vector<ImageDataStrip> ImageDataStrips;
        vector<int> StripOffsets;
        vector<ifd::IFD*> IFDs;
        DNG()
        {
            StripOffsets.resize(1);
        }
        VBuf* convert(unsigned char* image, vector<Tag*> tags, int width, int height, int bpp)
        {
            DNG* dngTemplate = new DNG();
            // let rawFrame = self.__process__(image)
            unsigned char* rawFrame = image;
            
            unsigned char* tile = rawFrame;
            ImageDataStrip strip;
            strip.data = tile;
            strip.length = width * height * (bpp>8?2:1);
            dngTemplate->ImageDataStrips.push_back(strip);
            ifd::IFD* mainIFD = new ifd::IFD();
            vector<any> tileOffsets;
            vector<any> tileLengths;
            for(int i=0;i<dngTemplate->ImageDataStrips.size();i++)
            {
                tileLengths.push_back(dngTemplate->ImageDataStrips[i].length);
                tileOffsets.push_back(0);
            }
            Tag *mainTagStripOffset = new Tag(TagTypeList[TagTypeEnum::TileOffsets], tileOffsets);
            mainIFD->tags.push_back(mainTagStripOffset);
            mainIFD->tags.push_back(new Tag(TagTypeList[TagTypeEnum::NewSubfileType], {0}));
            mainIFD->tags.push_back(new Tag(TagTypeList[TagTypeEnum::TileByteCounts],tileLengths));
            mainIFD->tags.push_back(new Tag(TagTypeList[TagTypeEnum::Compression], {1}));
            mainIFD->tags.push_back(new Tag(TagTypeList[TagTypeEnum::Software], {"PyDNG"}));
            for(int i = 0; i<tags.size(); i++)
            {
                mainIFD->tags.push_back(tags[i]);
            }
            dngTemplate->IFDs.push_back(mainIFD);
            int totalLength = dngTemplate->dataLen();
            vector<any> offsets;
            for(int i = 0; i < dngTemplate->StripOffsets.size(); i++)
            {
                offsets.push_back(dngTemplate->StripOffsets[i]);
            }
            mainTagStripOffset->setValue(offsets);

            VBuf *buf = new VBuf(totalLength);
            dngTemplate->setBuffer(buf);
            dngTemplate->write();
            return buf;
        }
        

        void setBuffer(VBuf*buf)
        {
            mBuf = buf;
            int currentOffset = 8;
            for(int i=0; i < IFDs.size(); i++)
            {
                ifd::IFD* _ifd= IFDs[i];
                _ifd->setBuffer(mBuf, currentOffset);
                currentOffset += _ifd->dataLen();
            }
        }
            
        int dataLen()
        {
            int totalLength = 8;
            for(int i=0; i < IFDs.size(); i++)
            {
                ifd::IFD* _ifd= IFDs[i];
                totalLength += (_ifd->dataLen() + 3) & 0xFFFFFFFC;
            };
            StripOffsets.resize(ImageDataStrips.size());
            for(int i=0,len = ImageDataStrips.size();i<len;i++)
            {
                StripOffsets[i] = totalLength;
                ImageDataStrip strip = ImageDataStrips[i];
                totalLength += ((strip.length) + 3) & 0xFFFFFFFC;
            }
            return (totalLength + 3) & 0xFFFFFFFC;
        }
        void write()
        {
            mBuf->writeUInt8(0x49,0);
            mBuf->writeUInt8(0x49,1);
            mBuf->writeUInt8(0x2A,2);
            mBuf->writeUInt8(0,3);
            mBuf->writeUInt32LE(8,4);
            
            for(int i=0; i < IFDs.size(); i++)
            {
                ifd::IFD* _ifd= IFDs[i];
                _ifd->write();
            }
            for(int i=0,len = ImageDataStrips.size();i<len;i++)
            {
                mBuf->copy(ImageDataStrips[i].data, StripOffsets[i], 0, ImageDataStrips[i].length);
            }
        }
    private:
        VBuf* mBuf;
};