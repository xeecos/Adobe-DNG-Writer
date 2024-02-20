#pragma once
#include <stdint.h>
union float2byte
{
    float val;
    unsigned char b[4];
};

typedef union short2byte
{
    short val;
    unsigned char b[2];
}short2byte;

typedef union ushort2byte
{
    unsigned short val;
    unsigned char b[2];
} ushort2byte;

typedef union long2byte
{
    long val;
    unsigned char b[4];
} long2byte;

typedef union sbyte2byte
{
    char val;
    unsigned char b;
} sbyte2byte;

typedef union int2byte
{
    int32_t val;
    unsigned char b[4];
} int2byte;

typedef union uint2byte
{
    uint32_t val;
    unsigned char b[4];
} uint2byte;

typedef union double2byte
{
    double val;
    unsigned char b[8];
} double2byte;