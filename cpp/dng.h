#pragma once
#include <vector>
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

class DNG
{
    public:
        DNG()
        {
            this.IFDs = []
            this.ImageDataStrips = []
            this.StripOffsets = {}
        }
        void convert(image,tags,name,path)
        {
            let dngTemplate = new DNG()
            // let rawFrame = self.__process__(image)
            let rawFrame = image;
            let width = tags.ImageWidth.rawValue[0];
            let length = tags.ImageLength.rawValue[0];
            let bpp = tags.BitsPerSample.rawValue[0];
            let tile;
            if(bpp == 8)
            {
                tile = rawFrame;
            }
            else if(bpp == 10)
            {
                tile = DNG.pack10(rawFrame);
            }
            else if(bpp == 12)
            {
                tile = DNG.pack12(rawFrame);
            }
            else if(bpp == 14)
            {
                tile = DNG.pack14(rawFrame);
            }
            else if(bpp == 16)
            {
                tile = rawFrame;
            }

            dngTemplate.ImageDataStrips.push(tile)
            let mainIFD = new IFD()
            let tileOffsets = [];
            let tileLengths = [];
            for(let i=0;i<dngTemplate.ImageDataStrips.length;i++)
            {
                tileLengths.push(dngTemplate.ImageDataStrips[i].length)
                tileOffsets.push(0);
            }
            console.log(tileOffsets)
            let mainTagStripOffset = new Tag(TagType.TileOffsets, tileOffsets)
            mainIFD.tags.push(mainTagStripOffset)
            mainIFD.tags.push(new Tag(TagType.NewSubfileType, [0]))
            mainIFD.tags.push(new Tag(TagType.TileByteCounts,tileLengths))
            mainIFD.tags.push(new Tag(TagType.Compression, [1]))
            mainIFD.tags.push(new Tag(TagType.Software, "PyDNG"))

            for(let i in tags)
            {
                mainIFD.tags.push(tags[i]);
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
        vector<IFD> IFDs;
}