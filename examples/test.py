import struct
l = [[19549, 10000], [-7877, 10000], [-2582, 10000],[-5724, 10000], [10121, 10000], [1917, 10000],[-1267, 10000], [ -110, 10000], [ 6621, 10000]]
# # print([len(item) for item in l])
print(''.join('{:02x}'.format(x) for x in struct.pack('<%sl' % (len(l)*2), *[item for sublist in l for item in sublist])))
# lst = [0 for tile in [2,2]]
# print(lst)
# str = "Camera Brand"
# print(len(struct.pack('<%ssx0L' % len(str), bytearray(str.encode("ascii")))))