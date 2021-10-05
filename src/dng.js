const {Tag} = require("./tag");
const {TagType} = require('./types')
const {IFD} = require('./ifd')
class Header
{
    constructor()
    {
        this.IFDOffset = 8
    }
    raw()
    {
        return ['I','I',0x2A,0x0,0x8,0,0,0];
    }
}
module.exports = Header;

class DNG
{
    constructor()
    {
        this.IFDs = []
        this.ImageDataStrips = []
        this.StripOffsets = {}
    }
    
    static pack10(data)
    {
        return data;
    }
    static pack12(data)
    {
        return data;
    }
    static pack14(data)
    {
        return data;
    }
    static convert(image,tags,name,path)
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
        console.log("totalLength:",totalLength,width,length)
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
    

    setBuffer(buf)
    {
        this.buf = buf
        let currentOffset = 8
        this.IFDs.forEach(ifd=>{
            ifd.setBuffer(buf, currentOffset);
            currentOffset += ifd.dataLen();
        });
    }
        
    dataLen()
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
    write()
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
}
module.exports = {DNG};