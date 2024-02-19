const fs = require('fs');
const {Tag} = require('../src/tag');
const {TagType} = require('../src/types')
const {DNG} = require('../src/dng')
let width = 2304;
let height = 1296;
let bpp = 16;
let cmos_bits = 10;
let rawFile = '../assets/sc3336.raw16';
let rawData = fs.readFileSync(rawFile);
let ccm_d65 = [1.93671, -0.813726, -0.122981, -0.304568, 1.67905, -0.374485, -0.0774701, -0.551157, 1.62863];
let ccm_a = [1.8491, -0.595994, -0.253107, -0.414529, 1.70693, -0.292399, -0.240201, -1.30604, 2.54625];
//[2.19679, -1.02491, -0.171881, -0.460098, 1.60701, -0.146908, -0.143431, -0.739075, 1.88251];
//[1.46483, -0.329073, -0.135755, -0.217658, 1.39304, -0.175387, -0.0286993, -0.304848, 1.33355];
//[2.00881, -0.824107, -0.1847, -0.31166, 1.69705, -0.385394, -0.0847783, -0.465219, 1.55];
let ccm1 = [
        [18491, 10000], [-5959, 10000], [-2531, 10000],	
        [-4145, 10000], [17069, 10000], [-2923, 10000],
        [-2402, 10000], [ -13060, 10000], [ 25462, 10000]];
for(let i=0;i<9;i++)
{
    ccm1[i][0] = (ccm_d65[i]*10000)>>0;
}
let ccm2 = [
    [18491, 10000], [-5959, 10000], [-2531, 10000],	
    [-4145, 10000], [17069, 10000], [-2923, 10000],
    [-2402, 10000], [ -13060, 10000], [ 25462, 10000]];
for(let i=0;i<9;i++)
{
    ccm2[i][0] = (ccm_a[i]*10000)>>0;
}
console.log(ccm1);
        
let tags = {};
tags.ImageWidth = (new Tag(TagType.ImageWidth, width));
tags.ImageLength = (new Tag(TagType.ImageLength, height));
tags.TileWidth = (new Tag(TagType.TileWidth, width));
tags.TileLength = (new Tag(TagType.TileLength, height));
tags.Orientation = (new Tag(TagType.Orientation, 1));
tags.PhotometricInterpretation = (new Tag(TagType.PhotometricInterpretation, 32803));
tags.SamplesPerPixel = (new Tag(TagType.SamplesPerPixel, 1));
tags.BitsPerSample = (new Tag(TagType.BitsPerSample, bpp));
tags.CFARepeatPatternDim = (new Tag(TagType.CFARepeatPatternDim, [2,2]));
tags.CFAPattern = (new Tag(TagType.CFAPattern, [2, 1, 1, 0]));
tags.BlackLevel = (new Tag(TagType.BlackLevel, 0));
tags.WhiteLevel = (new Tag(TagType.WhiteLevel, ((1 << cmos_bits) -1) ));
tags.ColorMatrix1 = (new Tag(TagType.ColorMatrix1, ccm1));
tags.ColorMatrix2 = (new Tag(TagType.ColorMatrix2, ccm2));
tags.CalibrationIlluminant1 = (new Tag(TagType.CalibrationIlluminant1, 21));
tags.AsShotNeutral = (new Tag(TagType.AsShotNeutral, [[1,1],[1,1],[1,1]]));
tags.DNGVersion = (new Tag(TagType.DNGVersion, [1, 4, 0, 0]));
tags.DNGBackwardVersion = (new Tag(TagType.DNGBackwardVersion, [1, 2, 0, 0]));
tags.Make = (new Tag(TagType.Make, "Camera Brand"));
tags.Model = (new Tag(TagType.Model, "Camera Model"));
tags.PreviewColorSpace = (new Tag(TagType.PreviewColorSpace, [2]));

let buffer = DNG.convert(rawData, tags, "custom", "")
fs.writeFileSync('out.dng',buffer);