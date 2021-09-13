      
class IFD
{
    constructor(object)
    {

    }
    def __init__(self):
        self.tags = []
        self.NextIFDOffset = 0

    def setBuffer(self, buf, offset):
        self.buf = buf
        self.offset = offset
        currentDataOffset = offset + 2 + len(self.tags)*12 + 4
        currentTagOffset = offset + 2
        for tag in sorted(self.tags, key=lambda x: x.TagId):
            tag.setBuffer(buf, currentTagOffset, currentDataOffset)
            currentTagOffset += 12
            currentDataOffset += tag.dataLen()
            #currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC
            

    def dataLen(self):
        totalLength = 2 + len(self.tags)*12 + 4
        for tag in sorted(self.tags, key=lambda x: x.TagId):
            totalLength += tag.dataLen()
        return (totalLength + 3) & 0xFFFFFFFC

    def write(self):
        if not self.buf:
            raise RuntimeError("buffer not initialized")

        struct.pack_into("<H", self.buf, self.offset, len(self.tags))

        for tag in sorted(self.tags, key=lambda x: x.TagId):
            tag.write()

        struct.pack_into("<I", self.buf, self.offset + 2 + len(self.tags)*12, self.NextIFDOffset)

}
module.exports = IFD;