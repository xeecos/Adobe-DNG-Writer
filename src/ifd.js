      
class IFD
{
    constructor()
    {

        this.tags = []
        this.NextIFDOffset = 0
    }

    setBuffer(buf, offset)
    {
        this.buf = buf
        this.offset = offset
        currentDataOffset = offset + 2 + this.tags.length*12 + 4
        currentTagOffset = offset + 2
        for tag in sorted(self.tags, key=lambda x: x.TagId)
        {
            tag.setBuffer(buf, currentTagOffset, currentDataOffset)
            currentTagOffset += 12
            currentDataOffset += tag.dataLen()
            currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC
        }
    }
    dataLen(self)
    {
        totalLength = 2 + len(self.tags)*12 + 4
        for tag in sorted(self.tags, key=lambda x: x.TagId):
            totalLength += tag.dataLen()
        return (totalLength + 3) & 0xFFFFFFFC
    }

    write()
    {
        if not self.buf:
            raise RuntimeError("buffer not initialized")

        struct.pack_into("<H", self.buf, self.offset, len(self.tags))

        for tag in sorted(self.tags, key=lambda x: x.TagId):
            tag.write()

        struct.pack_into("<I", self.buf, self.offset + 2 + len(self.tags)*12, self.NextIFDOffset)
    }
}
module.exports = IFD;