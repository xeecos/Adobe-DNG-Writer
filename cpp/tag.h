#pragma once
#include "types.h"
struct DataBuffer
{
    unsigned char* data;
    int size;
};
class Tag
{
    public:
        TagType Type;
        int TagId;
        DataTypeEnum DataType;
        int DataCount;
        int DataOffset;
        bool selfContained;
        vector<double> rawValue;
        int DataLength;
        char* subIFD;
        DataBuffer DataValue;
        Tag(TagType tagType, vector<double> value)
        {
            Type = tagType;
            TagId = tagType.id;
            DataType = tagType.dataType;
            DataCount = value.size();
            DataOffset = 0;
            subIFD = NULL;
            setValue(value);
            rawValue = value;
            DataLength = DataValue.size;
            selfContained = DataLength <= 4;                   
        };
        void setValue(vector<double> value)
        {
            int len = value.size();
            if(DataType == DataTypeEnum::Byte)
            {
                DataValue.data = (unsigned char*)malloc(len);
                DataValue.size = len;
                for(int i=0;i<len;i++)
                {
                    DataValue.data[i] =value[i];
                }
            }
            else if(DataType == DataTypeEnum::Short)
            {
                DataValue.data = (unsigned char*)malloc(len*2);
                DataValue.size = len*2;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeUInt16LE(value[i],i*2);
                }
            }
            else if(DataType == DataTypeEnum::Long)
            {
                DataValue.data = (unsigned char*)malloc(len*4);
                DataValue.size = len*4;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeUInt32LE(value[i],i*4);
                }
            }
            else if(DataType == DataTypeEnum::Sbyte)
            {
                DataValue.data = (unsigned char*)malloc(len);
                DataValue.size = len;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeInt8(value[i],i);
                }
            }
            else if(DataType == DataTypeEnum::Undefined)
            {   
                DataValue.data = (unsigned char*)malloc(len);
                DataValue.size = len;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeUInt8(value[i],i);
                }
            }
            else if(DataType == DataTypeEnum::Sshort)
            {    
                DataValue.data = (unsigned char*)malloc(len*2);
                DataValue.size = len*2;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeInt16LE(value[i],i*2);
                }
            }
            else if(DataType == DataTypeEnum::Slong)
            {   
                DataValue.data = (unsigned char*)malloc(len*4);
                DataValue.size = len*4;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeInt32LE(value[i],i*4);
                }
            }
            else if(DataType == DataTypeEnum::Float)
            {   
                DataValue.data = (unsigned char*)malloc(len*4);
                DataValue.size = len*4;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeFloatLE(value[i],i*4);
                }
            }
            else if(DataType == DataTypeEnum::Double)
            {   
                DataValue.data = (unsigned char*)malloc(len*8);
                DataValue.size = len*8;
                for(int i=0;i<len;i++)
                {
                    DataValue.writeDoubleLE(value[i],i*8);
                }
            }
            else if(DataType == DataTypeEnum::Rational)
            {   
                DataValue.data = (unsigned char*)malloc(len*2*4);
                DataValue.size = len*2*4;
                int idx = 0;
                for(int i=0;i<len;i++)
                {
                    for(int j=0;j<2;j++)
                    {
                        DataValue.writeUInt32LE(value[i][j],idx);
                        idx+=4;
                    }
                }
            }
            else if(DataType == DataTypeEnum::Srational)
            {   
                DataValue.data = (unsigned char*)malloc(len*2*4);
                DataValue.size = len*2*4;
                let idx = 0;
                for(int i=0;i<len;i++)
                {
                    for(int j=0;j<2;j++)
                    {
                        DataValue.writeInt32LE(value[i][j],idx);
                        idx+=4;
                    }
                }
            }
            else if(DataType == DataTypeEnum::Ascii)
            {   
                DataValue = Buffer.from(value+"\0");
                DataCount += 1;
            }
            else if(DataType == DataTypeEnum::IFD)
            {   
                DataValue.data = (unsigned char*)malloc(4);
                DataValue.size = 4;
                subIFD = value[0];
            }
            else
            {
                printf("unknow data type:%d",DataType);
            }
            int count = ((((DataValue.size+3) & 0xFFFFFFFC) - (DataValue.size)));
            int offset = DataValue.size;
            if(count>0)
            {
                unsigned char* tmp = (unsigned char*)malloc(offset+count);
                int byteLength = offset+count;
                for(int i=0;i<byteLength;i++)
                {
                    tmp.writeUInt8(0,i);
                }
                DataValue.data.copy(tmp,0,0,offset);
                DataValue.data = tmp;
            }
        }

        void setBuffer(buf, tagOffset, dataOffset)
        {
            buf = buf;
            TagOffset = tagOffset;
            DataOffset = dataOffset;
            if(subIFD)
            {
                subIFD.setBuffer(buf, DataOffset)
            }            
        }
        int dataLen()
        {
            if(subIFD)
            {
                return subIFD.dataLen()
            }
                
            if(selfContained)
            {
                return 0
            }
            return (DataValue.byteLength + 3) & 0xFFFFFFFC
        } 
        void write()
        {
            if(subIFD)
            {
                subIFD.write();
                let buf = Buffer.alloc(12);
                buf.writeUInt16LE(TagId,0);
                buf.writeUInt16LE(DataType.Long[0],2);
                buf.writeUInt32LE(DataCount,4);
                buf.writeUInt32LE(DataOffset,8);
                buf.copy(buf,TagOffset,0,12);
            }
            else
            {
                if(selfContained)
                {
                    let buf = Buffer.alloc(12);
                    buf.writeUInt16LE(TagId,0);
                    buf.writeUInt16LE(DataType[0],2);
                    buf.writeUInt32LE(DataCount,4);
                    DataValue.copy(buf,8,0,4)
                    buf.copy(buf,TagOffset,0,12);
                }    
                else
                {
                    let buf = Buffer.alloc(12);
                    buf.writeUInt16LE(TagId,0);
                    buf.writeUInt16LE(DataType[0],2);
                    buf.writeUInt32LE(DataCount,4);
                    buf.writeUInt32LE(DataOffset,8);
                    buf.copy(buf,TagOffset,0,12);
                    DataValue.copy(buf,DataOffset,0,DataLength);
                }
            }
        };
};
