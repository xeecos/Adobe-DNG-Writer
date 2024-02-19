#pragma once
#include <vector>
#include "tag.h"
#include "ifd.h"
using namespace std;
class Header
{
    public:
        Header()
        {
            IFDOffset = 8
        }
        const char* raw()
        {
            return {'I','I',0x2A,0x0,0x8,0,0,0};
        };
        int IFDOffset;
};
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
        DNG()
        {
        }
        void convert(unsigned char* image, vector<Tag*> tags, int width, int height, int bpp)
        {
            DNG* dngTemplate = new DNG();
            // let rawFrame = self.__process__(image)
            unsigned char* rawFrame = image;
            
            unsigned char* tile = rawFrame;
            ImageDataStrip strip;
            strip.data = tile;
            strip.length = width * height * (bpp>8?2:1);
            dngTemplate->ImageDataStrips.push_back(strip);
            IFD *mainIFD = new IFD();
            vector<int> tileOffsets;
            vector<int> tileLengths;
            for(int i=0;i<dngTemplate->ImageDataStrips.size();i++)
            {
                tileLengths.push_back(dngTemplate->ImageDataStrips[i].length);
                tileOffsets.push_back(0);
            }
            Tag *mainTagStripOffset = new Tag(TagType.TileOffsets, tileOffsets);
            mainIFD->tags.push_back(mainTagStripOffset);
            mainIFD->tags.push_back(new Tag(TagType.NewSubfileType, [0]));
            mainIFD->tags.push_back(new Tag(TagType.TileByteCounts,tileLengths));
            mainIFD->tags.push_back(new Tag(TagType.Compression, [1]));
            mainIFD->tags.push_back(new Tag(TagType.Software, "PyDNG"));

            for(let i in tags)
            {
                mainIFD->tags.push_back(tags[i]);
            }
            dngTemplate.IFDs.push(mainIFD);

            let totalLength = dngTemplate.dataLen()
            let offsets = [];
            for(let i in dngTemplate.StripOffsets)
            {
                offsets.push(dngTemplate.StripOffsets[i]);
            }
            mainTagStripOffset.setValue(offsets);

            let buf = Buffer.alloc(totalLength);
            dngTemplate.setBuffer(buf);
            dngTemplate.write();
            return buf;
        }
        

        void setBuffer(buf)
        {
            this.buf = buf
            let currentOffset = 8
            this.IFDs.forEach(ifd=>{
                ifd.setBuffer(buf, currentOffset);
                currentOffset += ifd.dataLen();
            });
        }
            
        int dataLen()
        {
            let totalLength = 8;
            this.IFDs.forEach(ifd=>{
                totalLength += (ifd.dataLen() + 3) & 0xFFFFFFFC
            });
            for(let i=0,len= this.ImageDataStrips.length;i<len;i++)
            {
                this.StripOffsets[i] = totalLength
                let strip = this.ImageDataStrips[i]
                totalLength += ((strip.length) + 3) & 0xFFFFFFFC
            }
            return (totalLength + 3) & 0xFFFFFFFC
        }
        void write()
        {
            this.buf.writeUInt8(0x49,0);
            this.buf.writeUInt8(0x49,1);
            this.buf.writeUInt8(0x2A,2);
            this.buf.writeUInt8(0,3);
            this.buf.writeUInt32LE(8,4);
            this.IFDs.forEach(ifd=>{
                ifd.write();
            });
            for(let i=0,len= this.ImageDataStrips.length;i<len;i++)
            {
                this.ImageDataStrips[i].copy(this.buf,this.StripOffsets[i],0,(this.ImageDataStrips[i].length))
            }
        }
    private:
        vector<IFD*> IFDs;
}