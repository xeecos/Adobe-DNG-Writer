const {TagType} = require("./tag");

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
        out = np.zeros((data.shape[0], int(data.shape[1]*(1.25))), dtype=np.uint8)
        out[:, ::5] = data[:, ::4] >> 2
        out[:, 1::5] = ((data[:, ::4] & 0b0000000000000011) << 6)
        out[:, 1::5] += data[:, 1::4] >> 4
        out[:, 2::5] = ((data[:, 1::4] & 0b0000000000001111) << 4)
        out[:, 2::5] += data[:, 2::4] >> 6
        out[:, 3::5] = ((data[:, 2::4] & 0b0000000000111111) << 2)
        out[:, 3::5] += data[:, 3::4] >> 8
        out[:, 4::5] = data[:, 3::4] & 0b0000000011111111
        return out
    }


    static pack12(data)
    {
        out = np.zeros((data.shape[0], int(data.shape[1]*(1.5))), dtype=np.uint8)
        out[:, ::3] = data[:, ::2] >> 4
        out[:, 1::3] = ((data[:, ::2] & 0b0000000000001111) << 4)
        out[:, 1::3] += data[:, 1::2] >> 8
        out[:, 2::3] = data[:, 1::2] & 0b0000001111111111
        return out
    }
    static pack14(data)
    {
        out = np.zeros((data.shape[0], int(data.shape[1]*(1.75))), dtype=np.uint8)
        out[:, ::7] = data[:, ::6] >> 6
        out[:, 1::7] = ((data[:, ::6] & 0b0000000000000011) << 6)
        out[:, 1::7] += data[:, 1::6] >> 8
        out[:, 2::7] = ((data[:, 1::6] & 0b0000000000001111) << 4)
        out[:, 2::7] += data[:, 2::6] >> 6
        out[:, 3::7] = ((data[:, 2::6] & 0b0000000000111111) << 2)
        out[:, 3::7] += data[:, 3::6] >> 8
        out[:, 4::7] = ((data[:, 3::6] & 0b0000000000001111) << 4)
        out[:, 4::7] += data[:, 4::6] >> 6
        out[:, 5::7] = ((data[:, 4::6] & 0b0000000000111111) << 2)
        out[:, 5::7] += data[:, 5::6] >> 8
        out[:, 6::7] = data[:, 5::6] & 0b0000000011111111
    }
    static convert(image,tags,name,path)
    {
        let dngTemplate = new DNG()
        let rawFrame = self.__process__(image)
        let file_output = True
        let width = tags.get(TagType.ImageWidth)[0]
        let length = tags.get(TagType.ImageLength)[0]
        let bpp = tags.get(TagType.BitsPerSample)[0]
        if(bpp == 8)
        {
            tile = rawFrame.astype('uint8').tobytes()
        }
        else if(bpp == 10)
        {
            tile = pack10(rawFrame).tobytes()
        }
        else if(bpp == 12)
        {
            tile = pack12(rawFrame).tobytes()
        }
        else if(bpp == 14)
        {
            tile = pack14(rawFrame).tobytes()
        }
        else if(bpp == 16)
        {
            tile = rawFrame.tobytes()
        }

        dngTemplate.ImageDataStrips.append(tile)
        mainIFD = new IFD()
        mainTagStripOffset = new Tag(TagType.TileOffsets, [0 for tile in dngTemplate.ImageDataStrips])
        mainIFD.tags.append(mainTagStripOffset)
        mainIFD.tags.append(new Tag(TagType.NewSubfileType, [0]))
        mainIFD.tags.append(new Tag(TagType.TileByteCounts, [len(tile) for tile in dngTemplate.ImageDataStrips]))
        mainIFD.tags.append(new Tag(TagType.Compression, [compression_scheme]))
        mainIFD.tags.append(new Tag(TagType.Software, "PyDNG"))

        for tag in tags.list():
            try:
                mainIFD.tags.append(new Tags(tag[0], tag[1]))
            except Exception as e:
                print("TAG Encoding Error!", e, tag)

        dngTemplate.IFDs.append(mainIFD)

        totalLength = dngTemplate.dataLen()

        mainTagStripOffset.setValue([k for offset, k in dngTemplate.StripOffsets.items()])

        buf = bytearray(totalLength)
        dngTemplate.setBuffer(buf)
        dngTemplate.write()

        if file_output:
            if not filename.endswith(".dng"):
                filename = filename + '.dng'
            outputDNG = os.path.join(path, filename)
            with open(outputDNG, "wb") as outfile:
                outfile.write(buf)
            return outputDNG
        else:
            return buf
    }
    

    setBuffer(buf)
    {
        this.buf = buf
        let currentOffset = 8
        for(ifd in this.IFDs)
        {
            ifd.setBuffer(buf, currentOffset)
            currentOffset += ifd.dataLen()
        }
    }
        
    dataLen(self)
    {
        totalLength = 8
        for ifd in self.IFDs:
            totalLength += (ifd.dataLen() + 3) & 0xFFFFFFFC

        for i in range(len(self.ImageDataStrips)):
            self.StripOffsets[i] = totalLength
            strip = self.ImageDataStrips[i]
            totalLength += (len(strip) + 3) & 0xFFFFFFFC
            
        return (totalLength + 3) & 0xFFFFFFFC
    }
    write()
    {
        struct.pack_into("<ccbbI", self.buf, 0, b'I', b'I', 0x2A, 0x00, 8) 
        for ifd in self.IFDs:
            ifd.write()
        for i in range(len(self.ImageDataStrips)):
            self.buf[self.StripOffsets[i]:self.StripOffsets[i]+len(self.ImageDataStrips[i])] = self.ImageDataStrips[i]
    }
}
module.exports = DNG;