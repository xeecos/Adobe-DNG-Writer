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
        let currentDataOffset = offset + 2 + this.tags.length*12 + 4
        let currentTagOffset = offset + 2
        this.tags.forEach(tag=>{
            tag.setBuffer(buf, currentTagOffset, currentDataOffset);
            currentTagOffset += 12;
            currentDataOffset += tag.dataLen();
            currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC;
        })
    }
    dataLen()
    {
        let totalLength = 2 + this.tags.length*12 + 4
        this.tags.forEach(tag=>{
            totalLength += tag.dataLen()
        });
        return (totalLength + 3) & 0xFFFFFFFC
    }

    write()
    {
        this.buf.writeUInt16LE(this.tags.length,this.offset);
        this.tags.forEach(tag=>{
            tag.write();
        });
        this.buf.writeUInt32LE(this.offset + 2 + (this.tags.length)*12,this.NextIFDOffset);
    }
}
module.exports = {IFD};