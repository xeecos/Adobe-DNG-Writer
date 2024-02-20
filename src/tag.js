const {TagType,DataType} = require("./types")
class Tag
{
    constructor(tagType=TagType.Invalid, value=[])
    {
        if(typeof value=='number')
        {
            value = [value];
        }
        this.Type = tagType;
        this.TagId = tagType[0];
        this.DataType = tagType[1];
        this.DataCount = value.length;
        this.DataOffset = 0;
        this.subIFD = null;
        this.setValue(value)
        this.rawValue = value;
        this.DataLength = this.Value.length;
        this.selfContained = this.DataLength <= 4;                   
    }
    setValue(value)
    {
        let len = value.length;
        if(this.DataType == DataType.Byte)
        {
            this.Value = Buffer.alloc(len);
            for(let i=0;i<len;i++)
            {
                this.Value.writeUInt8(value[i],i);
            }
        }
        else if(this.DataType == DataType.Short)
        {
            this.Value = Buffer.alloc(len*2);
            for(let i=0;i<len;i++)
            {
                this.Value.writeUInt16LE(value[i],i*2);
            }
        }
        else if(this.DataType == DataType.Long)
        {
            this.Value = Buffer.alloc(len*4);
            for(let i=0;i<len;i++)
            {
                this.Value.writeUInt32LE(value[i],i*4);
            }
        }
        else if(this.DataType == DataType.Sbyte)
        {
            this.Value = Buffer.alloc(len);
            for(let i=0;i<len;i++)
            {
                this.Value.writeInt8(value[i],i);
            }
        }
        else if(this.DataType == DataType.Undefined)
        {   
            this.Value = Buffer.alloc(len);
            for(let i=0;i<len;i++)
            {
                this.Value.writeUInt8(value[i],i);
            }
        }
        else if(this.DataType == DataType.Sshort)
        {    
            this.Value = Buffer.alloc(len*2);
            for(let i=0;i<len;i++)
            {
                this.Value.writeInt16LE(value[i],i*2);
            }
        }
        else if(this.DataType == DataType.Slong)
        {   
            this.Value = Buffer.alloc(len*4);
            for(let i=0;i<len;i++)
            {
                this.Value.writeInt32LE(value[i],i*4);
            }
        }
        else if(this.DataType == DataType.Float)
        {   
            this.Value = Buffer.alloc(len*4);
            for(let i=0;i<len;i++)
            {
                this.Value.writeFloatLE(value[i],i*4);
            }
        }
        else if(this.DataType == DataType.Double)
        {   
            this.Value = Buffer.alloc(len*8);
            for(let i=0;i<len;i++)
            {
                this.Value.writeDoubleLE(value[i],i*8);
            }
        }
        else if(this.DataType == DataType.Rational)
        {   
            this.Value = Buffer.alloc(len*2*4);
            let idx = 0;
            for(let i=0;i<len;i++)
            {
                for(let j=0;j<2;j++)
                {
                    this.Value.writeUInt32LE(value[i][j],idx);
                    idx+=4;
                }
            }
        }
        else if(this.DataType == DataType.Srational)
        {   
            this.Value = Buffer.alloc(len*2*4);
            let idx = 0;
            for(let i=0;i<len;i++)
            {
                for(let j=0;j<2;j++)
                {
                    this.Value.writeInt32LE(value[i][j],idx);
                    idx+=4;
                }
            }
        }
        else if(this.DataType == DataType.Ascii)
        {   
            this.Value = Buffer.from(value+"\0");
            this.DataCount += 1
        }
        else if(this.DataType == DataType.IFD)
        {   
            this.Value = Buffer.from([0,0,0,0]);
            this.subIFD = value[0]
        }
        else
        {
            console.log("unknow data type:",this.DataType);
        }
        let count = ((((this.Value.byteLength+3) & 0xFFFFFFFC) - (this.Value.byteLength)));
        let offset = this.Value.byteLength;
        if(count>0)
        {
            let tmp = Buffer.alloc(offset+count);
            
            for(let i=0;i<tmp.byteLength;i++)
            {
               tmp.writeUInt8(0,i);
            }
            this.Value.copy(tmp,0,0,offset);
            this.Value = tmp;

        }
    }

    setBuffer(buf, tagOffset, dataOffset)
    {
        this.buf = buf;
        this.TagOffset = tagOffset;
        this.DataOffset = dataOffset;
        if(this.subIFD)
        {
            this.subIFD.setBuffer(buf, this.DataOffset)
        }            
    }
    dataLen()
    {
        if(this.subIFD)
        {
            return this.subIFD.dataLen()
        }
            
        if(this.selfContained)
        {
            return 0
        }
        return (this.Value.byteLength + 3) & 0xFFFFFFFC
    } 
    write()
    {
        if(this.subIFD)
        {
            this.subIFD.write();
            let buf = Buffer.alloc(12);
            buf.writeUInt16LE(this.TagId,0);
            buf.writeUInt16LE(DataType.Long[0],2);
            buf.writeUInt32LE(this.DataCount,4);
            buf.writeUInt32LE(this.DataOffset,8);
            buf.copy(this.buf,this.TagOffset,0,12);
        }
        else
        {
            if(this.selfContained)
            {
                let buf = Buffer.alloc(12);
                buf.writeUInt16LE(this.TagId,0);
                buf.writeUInt16LE(this.DataType[0],2);
                buf.writeUInt32LE(this.DataCount,4);
                this.Value.copy(buf,8,0,4)
                buf.copy(this.buf,this.TagOffset,0,12);
            }    
            else
            {
                let buf = Buffer.alloc(12);
                buf.writeUInt16LE(this.TagId,0);
                buf.writeUInt16LE(this.DataType[0],2);
                buf.writeUInt32LE(this.DataCount,4);
                buf.writeUInt32LE(this.DataOffset,8);
                buf.copy(this.buf,this.TagOffset,0,12);
                this.Value.copy(this.buf,this.DataOffset,0,this.DataLength);
            }
        }
    }  
}

module.exports = {Tag};