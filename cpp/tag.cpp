#include "tag.h"
Tag::Tag(TagType tagType, vector<any> value)
{
    Type = tagType;
    TagId = tagType.id;
    dataType = DataTypeList[tagType.dataType];
    DataCount = value.size();
    DataOffset = 0;
    setValue(value);
    rawValue = value;
    DataLength = DataValue.size;
    selfContained = DataLength <= 4;
};
void Tag::setValue(vector<any> value)
{
    int len = value.size();
    if (dataType.type == DataTypeEnum::Byte)
    {
        DataValue.data = (unsigned char *)malloc(len);
        DataValue.size = len;
        for (int i = 0; i < len; i++)
        {
            DataValue.data[i] = any_cast<char>(value.at(i));
        }
    }
    else if (dataType.type == DataTypeEnum::Short)
    {
        DataValue.data = (unsigned char *)malloc(len * 2);
        DataValue.size = len * 2;
        for (int i = 0; i < len; i++)
        {
            unsigned val = any_cast<unsigned short>(value[i]);
            ushort2byte s2b;
            s2b.val = val;
            DataValue.data[i * 2] = s2b.b[0];
            DataValue.data[i * 2 + 1] = s2b.b[1];
        }
    }
    else if (dataType.type == DataTypeEnum::Long)
    {
        DataValue.data = (unsigned char *)malloc(len * 4);
        DataValue.size = len * 4;
        for (int i = 0; i < len; i++)
        {
            short val = any_cast<long>(value[i]);
            long2byte l2b;
            l2b.val = val;
            DataValue.data[i * 4] = l2b.b[0];
            DataValue.data[i * 4 + 1] = l2b.b[1];
            DataValue.data[i * 4 + 2] = l2b.b[2];
            DataValue.data[i * 4 + 3] = l2b.b[3];
        }
    }
    else if (dataType.type == DataTypeEnum::Sbyte)
    {
        DataValue.data = (unsigned char *)malloc(len);
        DataValue.size = len;
        for (int i = 0; i < len; i++)
        {
            sbyte2byte b2b;
            b2b.val = any_cast<char>(value[i]);
            DataValue.data[i] = b2b.b;
        }
    }
    else if (dataType.type == DataTypeEnum::Undefined)
    {
        DataValue.data = (unsigned char *)malloc(len);
        DataValue.size = len;
        for (int i = 0; i < len; i++)
        {
            DataValue.data[i] = any_cast<uint8_t>(value[i]);
        }
    }
    else if (dataType.type == DataTypeEnum::Sshort)
    {
        DataValue.data = (unsigned char *)malloc(len * 2);
        DataValue.size = len * 2;
        for (int i = 0; i < len; i++)
        {
            short val = any_cast<short>(value[i]);
            short2byte s2b;
            s2b.val = val;
            DataValue.data[i * 2] = s2b.b[0];
            DataValue.data[i * 2 + 1] = s2b.b[1];
        }
    }
    else if (dataType.type == DataTypeEnum::Slong)
    {
        DataValue.data = (unsigned char *)malloc(len * 4);
        DataValue.size = len * 4;
        for (int i = 0; i < len; i++)
        {
            long val = any_cast<long>(value[i]);
            long2byte l2b;
            l2b.val = val;
            DataValue.data[i * 4] = l2b.b[0];
            DataValue.data[i * 4 + 1] = l2b.b[1];
            DataValue.data[i * 4 + 2] = l2b.b[2];
            DataValue.data[i * 4 + 3] = l2b.b[3];
        }
    }
    else if (dataType.type == DataTypeEnum::Float)
    {
        DataValue.data = (unsigned char *)malloc(len * 4);
        DataValue.size = len * 4;
        for (int i = 0; i < len; i++)
        {
            float val = any_cast<float>(value[i]);
            float2byte f2b;
            f2b.val = val;
            DataValue.data[i * 4] = f2b.b[0];
            DataValue.data[i * 4 + 1] = f2b.b[1];
            DataValue.data[i * 4 + 2] = f2b.b[2];
            DataValue.data[i * 4 + 3] = f2b.b[3];
        }
    }
    else if (dataType.type == DataTypeEnum::Double)
    {
        DataValue.data = (unsigned char *)malloc(len * 8);
        DataValue.size = len * 8;
        for (int i = 0; i < len; i++)
        {
            double val = any_cast<double>(value[i]);
            double2byte d2b;
            d2b.val = val;
            DataValue.data[i * 8] = d2b.b[0];
            DataValue.data[i * 8 + 1] = d2b.b[1];
            DataValue.data[i * 8 + 2] = d2b.b[2];
            DataValue.data[i * 8 + 3] = d2b.b[3];
            DataValue.data[i * 8 + 4] = d2b.b[4];
            DataValue.data[i * 8 + 5] = d2b.b[5];
            DataValue.data[i * 8 + 6] = d2b.b[6];
            DataValue.data[i * 8 + 7] = d2b.b[7];
        }
    }
    else if (dataType.type == DataTypeEnum::Rational)
    {
        DataValue.data = (unsigned char *)malloc(len * 2 * 4);
        DataValue.size = len * 2 * 4;
        for (int i = 0; i < len * 2; i++)
        {
            uint2byte i2b;
            i2b.val = any_cast<uint32_t>(value[i]);
            DataValue.data[i * 4] = i2b.b[0];
            DataValue.data[i * 4 + 1] = i2b.b[1];
            DataValue.data[i * 4 + 2] = i2b.b[2];
            DataValue.data[i * 4 + 3] = i2b.b[3];
        }
    }
    else if (dataType.type == DataTypeEnum::Srational)
    {
        DataValue.data = (unsigned char *)malloc(len * 2 * 4);
        DataValue.size = len * 2 * 4;
        for (int i = 0; i < len; i++)
        {
            int2byte i2b;
            i2b.val = any_cast<int32_t>(value[i]);
            DataValue.data[i * 4] = i2b.b[0];
            DataValue.data[i * 4 + 1] = i2b.b[1];
            DataValue.data[i * 4 + 2] = i2b.b[2];
            DataValue.data[i * 4 + 3] = i2b.b[3];
        }
    }
    else if (dataType.type == DataTypeEnum::Ascii)
    {
        string val = any_cast<string>(value[0]);
        DataValue.data = (unsigned char *)malloc(val.length());
        DataValue.size = val.length();
        memcpy(DataValue.data, val.c_str(), val.length());
        DataCount += 1;
    }
    else if (dataType.type == DataTypeEnum::IFD)
    {
        DataValue.data = (unsigned char *)malloc(4);
        DataValue.size = 4;
        subIFD = any_cast<ifd::IFD *>(value[0]);
    }
    else
    {
        printf("unknow data type:%d", dataType.type);
    }
    int count = ((((DataValue.size + 3) & 0xFFFFFFFC) - (DataValue.size)));
    int offset = DataValue.size;
    if (count > 0)
    {
        unsigned char *tmp = (unsigned char *)malloc(offset + count);
        int byteLength = offset + count;
        for (int i = 0; i < byteLength; i++)
        {
            tmp[i] = 0;
        }
        for (int i = 0; i < offset; i++)
        {
            tmp[i] = DataValue.data[i];
        }
        free(DataValue.data);
        DataValue.data = tmp;
    }
}

void Tag::setBuffer(VBuf *buf, int tagOffset, int dataOffset)
{
    mBuf = buf;
    TagOffset = tagOffset;
    DataOffset = dataOffset;
    if (subIFD)
    {
        subIFD->setBuffer(mBuf, DataOffset);
    }
}
int Tag::dataLen()
{
    if (subIFD)
    {
        return subIFD->dataLen();
    }

    if (selfContained)
    {
        return 0;
    }
    return (DataValue.size + 3) & 0xFFFFFFFC;
}
void Tag::write()
{
    if (subIFD)
    {
        subIFD->write();
        VBuf *buf = new VBuf(12);
        buf->writeUInt16LE(TagId, 0);
        buf->writeUInt16LE(DataTypeList[DataTypeEnum::Long].size, 2);
        buf->writeUInt32LE(DataCount, 4);
        buf->writeUInt32LE(DataOffset, 8);
        mBuf->copy(buf->raw(), TagOffset, 0, 12);
        free(buf);
    }
    else
    {
        if (selfContained)
        {
            VBuf *buf = new VBuf(12);
            buf->writeUInt16LE(TagId, 0);
            buf->writeUInt16LE(dataType.size, 2);
            buf->writeUInt32LE(DataCount, 4);
            buf->copy(DataValue.data, 8, 0, 4);
            mBuf->copy(buf->raw(), TagOffset, 0, 12);
            free(buf);
        }
        else
        {
            VBuf *buf = new VBuf(12);
            buf->writeUInt16LE(TagId, 0);
            buf->writeUInt16LE(dataType.size, 2);
            buf->writeUInt32LE(DataCount, 4);
            buf->writeUInt32LE(DataOffset, 8);
            mBuf->copy(buf->raw(), TagOffset, 0, 12);
            mBuf->copy(DataValue.data, DataOffset, 0, DataLength);
            free(buf);
        }
    }
};