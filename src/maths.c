#include <glib.h>
#include <math.h>
#include "maths.h"


enum { TABSIZE = 256 };

/* cubic interpolation coefficient table for first tap */
static float ct0[TABSIZE] = {
    0.000000, -0.001938, -0.003845, -0.005723,
    -0.007570, -0.009388, -0.011176, -0.012934,
    -0.014664, -0.016364, -0.018035, -0.019678,
    -0.021292, -0.022877, -0.024435, -0.025964,
    -0.027466, -0.028940, -0.030386, -0.031805,
    -0.033197, -0.034562, -0.035901, -0.037213,
    -0.038498, -0.039757, -0.040990, -0.042197,
    -0.043379, -0.044535, -0.045666, -0.046771,
    -0.047852, -0.048907, -0.049938, -0.050945,
    -0.051928, -0.052886, -0.053820, -0.054731,
    -0.055618, -0.056482, -0.057323, -0.058140,
    -0.058935, -0.059707, -0.060457, -0.061184,
    -0.061890, -0.062573, -0.063235, -0.063875,
    -0.064493, -0.065091, -0.065667, -0.066222,
    -0.066757, -0.067272, -0.067765, -0.068239,
    -0.068693, -0.069127, -0.069542, -0.069937,
    -0.070312, -0.070669, -0.071007, -0.071326,
    -0.071627, -0.071909, -0.072173, -0.072419,
    -0.072647, -0.072858, -0.073051, -0.073227,
    -0.073385, -0.073527, -0.073652, -0.073760,
    -0.073853, -0.073928, -0.073988, -0.074032,
    -0.074060, -0.074073, -0.074071, -0.074053,
    -0.074020, -0.073973, -0.073911, -0.073835,
    -0.073744, -0.073639, -0.073520, -0.073388,
    -0.073242, -0.073083, -0.072911, -0.072725,
    -0.072527, -0.072316, -0.072093, -0.071857,
    -0.071609, -0.071350, -0.071079, -0.070796,
    -0.070501, -0.070196, -0.069879, -0.069552,
    -0.069214, -0.068865, -0.068506, -0.068137,
    -0.067759, -0.067370, -0.066972, -0.066564,
    -0.066147, -0.065721, -0.065286, -0.064842,
    -0.064390, -0.063930, -0.063461, -0.062984,
    -0.062500, -0.062008, -0.061508, -0.061002,
    -0.060488, -0.059967, -0.059439, -0.058905,
    -0.058365, -0.057818, -0.057266, -0.056707,
    -0.056143, -0.055573, -0.054998, -0.054418,
    -0.053833, -0.053243, -0.052649, -0.052050,
    -0.051447, -0.050840, -0.050229, -0.049614,
    -0.048996, -0.048374, -0.047750, -0.047122,
    -0.046492, -0.045859, -0.045223, -0.044585,
    -0.043945, -0.043304, -0.042660, -0.042015,
    -0.041368, -0.040721, -0.040072, -0.039423,
    -0.038773, -0.038122, -0.037471, -0.036820,
    -0.036169, -0.035518, -0.034868, -0.034218,
    -0.033569, -0.032921, -0.032274, -0.031629,
    -0.030985, -0.030342, -0.029702, -0.029063,
    -0.028427, -0.027793, -0.027162, -0.026533,
    -0.025908, -0.025285, -0.024666, -0.024050,
    -0.023438, -0.022829, -0.022225, -0.021624,
    -0.021029, -0.020437, -0.019850, -0.019269,
    -0.018692, -0.018121, -0.017555, -0.016994,
    -0.016439, -0.015891, -0.015348, -0.014812,
    -0.014282, -0.013759, -0.013243, -0.012734,
    -0.012232, -0.011737, -0.011250, -0.010771,
    -0.010300, -0.009836, -0.009382, -0.008935,
    -0.008497, -0.008068, -0.007648, -0.007237,
    -0.006836, -0.006444, -0.006062, -0.005689,
    -0.005327, -0.004975, -0.004634, -0.004303,
    -0.003983, -0.003673, -0.003375, -0.003089,
    -0.002813, -0.002550, -0.002298, -0.002058,
    -0.001831, -0.001616, -0.001414, -0.001224,
    -0.001047, -0.000883, -0.000733, -0.000596,
    -0.000473, -0.000364, -0.000268, -0.000187,
    -0.000120, -0.000068, -0.000030, -0.000008
};


/* cubic interpolation coefficient table for second tap */
static float ct1[TABSIZE] = {
    1.000000, 0.999962, 0.999848, 0.999659,
    0.999395, 0.999058, 0.998646, 0.998161,
    0.997604, 0.996975, 0.996275, 0.995503,
    0.994661, 0.993750, 0.992769, 0.991719,
    0.990601, 0.989415, 0.988162, 0.986842,
    0.985456, 0.984005, 0.982489, 0.980908,
    0.979263, 0.977555, 0.975784, 0.973951,
    0.972055, 0.970099, 0.968082, 0.966004,
    0.963867, 0.961671, 0.959416, 0.957103,
    0.954733, 0.952306, 0.949822, 0.947282,
    0.944687, 0.942037, 0.939333, 0.936575,
    0.933764, 0.930900, 0.927984, 0.925016,
    0.921997, 0.918928, 0.915808, 0.912640,
    0.909422, 0.906156, 0.902842, 0.899480,
    0.896072, 0.892618, 0.889118, 0.885573,
    0.881983, 0.878349, 0.874671, 0.870951,
    0.867188, 0.863382, 0.859536, 0.855649,
    0.851721, 0.847753, 0.843746, 0.839701,
    0.835617, 0.831496, 0.827337, 0.823142,
    0.818911, 0.814644, 0.810342, 0.806006,
    0.801636, 0.797232, 0.792796, 0.788327,
    0.783827, 0.779295, 0.774733, 0.770140,
    0.765518, 0.760867, 0.756187, 0.751479,
    0.746744, 0.741982, 0.737193, 0.732379,
    0.727539, 0.722674, 0.717786, 0.712873,
    0.707937, 0.702979, 0.697998, 0.692996,
    0.687973, 0.682929, 0.677866, 0.672783,
    0.667681, 0.662560, 0.657422, 0.652267,
    0.647095, 0.641906, 0.636702, 0.631483,
    0.626249, 0.621001, 0.615740, 0.610466,
    0.605179, 0.599880, 0.594570, 0.589249,
    0.583918, 0.578577, 0.573226, 0.567867,
    0.562500, 0.557125, 0.551743, 0.546355,
    0.540960, 0.535560, 0.530155, 0.524746,
    0.519333, 0.513916, 0.508497, 0.503075,
    0.497652, 0.492228, 0.486802, 0.481377,
    0.475952, 0.470528, 0.465106, 0.459685,
    0.454268, 0.448853, 0.443442, 0.438035,
    0.432632, 0.427235, 0.421844, 0.416459,
    0.411081, 0.405711, 0.400348, 0.394994,
    0.389648, 0.384313, 0.378987, 0.373672,
    0.368368, 0.363076, 0.357796, 0.352529,
    0.347275, 0.342035, 0.336809, 0.331598,
    0.326403, 0.321223, 0.316060, 0.310914,
    0.305786, 0.300676, 0.295584, 0.290512,
    0.285460, 0.280427, 0.275416, 0.270425,
    0.265457, 0.260511, 0.255588, 0.250689,
    0.245813, 0.240962, 0.236137, 0.231337,
    0.226562, 0.221815, 0.217095, 0.212403,
    0.207739, 0.203104, 0.198498, 0.193922,
    0.189377, 0.184862, 0.180380, 0.175929,
    0.171511, 0.167126, 0.162774, 0.158457,
    0.154175, 0.149928, 0.145716, 0.141542,
    0.137403, 0.133303, 0.129240, 0.125216,
    0.121231, 0.117286, 0.113380, 0.109515,
    0.105692, 0.101910, 0.098171, 0.094474,
    0.090820, 0.087211, 0.083646, 0.080125,
    0.076651, 0.073222, 0.069840, 0.066505,
    0.063217, 0.059978, 0.056787, 0.053646,
    0.050554, 0.047513, 0.044523, 0.041583,
    0.038696, 0.035862, 0.033080, 0.030352,
    0.027678, 0.025058, 0.022494, 0.019985,
    0.017532, 0.015137, 0.012798, 0.010517,
    0.008295, 0.006132, 0.004028, 0.001984
};


/* cubic interpolation coefficient table for third tap */
static float ct2[TABSIZE] = {
    0.000000, 0.001984, 0.004028, 0.006132,
    0.008295, 0.010517, 0.012798, 0.015137,
    0.017532, 0.019985, 0.022494, 0.025058,
    0.027678, 0.030352, 0.033080, 0.035862,
    0.038696, 0.041583, 0.044523, 0.047513,
    0.050554, 0.053646, 0.056787, 0.059978,
    0.063217, 0.066505, 0.069840, 0.073222,
    0.076651, 0.080125, 0.083646, 0.087211,
    0.090820, 0.094474, 0.098171, 0.101910,
    0.105692, 0.109515, 0.113380, 0.117286,
    0.121231, 0.125216, 0.129240, 0.133303,
    0.137403, 0.141542, 0.145716, 0.149928,
    0.154175, 0.158457, 0.162774, 0.167126,
    0.171511, 0.175929, 0.180380, 0.184862,
    0.189377, 0.193922, 0.198498, 0.203104,
    0.207739, 0.212403, 0.217095, 0.221815,
    0.226562, 0.231337, 0.236137, 0.240962,
    0.245813, 0.250689, 0.255588, 0.260511,
    0.265457, 0.270425, 0.275416, 0.280427,
    0.285460, 0.290512, 0.295584, 0.300676,
    0.305786, 0.310914, 0.316060, 0.321223,
    0.326403, 0.331598, 0.336809, 0.342035,
    0.347275, 0.352529, 0.357796, 0.363076,
    0.368368, 0.373672, 0.378987, 0.384313,
    0.389648, 0.394994, 0.400348, 0.405711,
    0.411081, 0.416459, 0.421844, 0.427235,
    0.432632, 0.438035, 0.443442, 0.448853,
    0.454268, 0.459685, 0.465106, 0.470528,
    0.475952, 0.481377, 0.486802, 0.492228,
    0.497652, 0.503075, 0.508497, 0.513916,
    0.519333, 0.524746, 0.530155, 0.535560,
    0.540960, 0.546355, 0.551743, 0.557125,
    0.562500, 0.567867, 0.573226, 0.578577,
    0.583918, 0.589249, 0.594570, 0.599880,
    0.605179, 0.610466, 0.615740, 0.621001,
    0.626249, 0.631483, 0.636702, 0.641906,
    0.647095, 0.652267, 0.657422, 0.662560,
    0.667681, 0.672783, 0.677866, 0.682929,
    0.687973, 0.692996, 0.697998, 0.702979,
    0.707937, 0.712873, 0.717786, 0.722674,
    0.727539, 0.732379, 0.737193, 0.741982,
    0.746744, 0.751479, 0.756187, 0.760867,
    0.765518, 0.770140, 0.774733, 0.779295,
    0.783827, 0.788327, 0.792796, 0.797232,
    0.801636, 0.806006, 0.810342, 0.814644,
    0.818911, 0.823142, 0.827337, 0.831496,
    0.835617, 0.839701, 0.843746, 0.847753,
    0.851721, 0.855649, 0.859536, 0.863382,
    0.867188, 0.870951, 0.874671, 0.878349,
    0.881983, 0.885573, 0.889118, 0.892618,
    0.896072, 0.899480, 0.902842, 0.906156,
    0.909422, 0.912640, 0.915808, 0.918928,
    0.921997, 0.925016, 0.927984, 0.930900,
    0.933764, 0.936575, 0.939333, 0.942037,
    0.944687, 0.947282, 0.949822, 0.952306,
    0.954733, 0.957103, 0.959416, 0.961671,
    0.963867, 0.966004, 0.968082, 0.970099,
    0.972055, 0.973951, 0.975784, 0.977555,
    0.979263, 0.980908, 0.982489, 0.984005,
    0.985456, 0.986842, 0.988162, 0.989415,
    0.990601, 0.991719, 0.992769, 0.993750,
    0.994661, 0.995503, 0.996275, 0.996975,
    0.997604, 0.998161, 0.998646, 0.999058,
    0.999395, 0.999659, 0.999848, 0.999962
};


/* cubic interpolation coefficient table for fourth tap */
static float ct3[TABSIZE] = {
    0.000000, -0.000008, -0.000030, -0.000068,
    -0.000120, -0.000187, -0.000268, -0.000364,
    -0.000473, -0.000596, -0.000733, -0.000883,
    -0.001047, -0.001224, -0.001414, -0.001616,
    -0.001831, -0.002058, -0.002298, -0.002550,
    -0.002813, -0.003089, -0.003375, -0.003673,
    -0.003983, -0.004303, -0.004634, -0.004975,
    -0.005327, -0.005689, -0.006062, -0.006444,
    -0.006836, -0.007237, -0.007648, -0.008068,
    -0.008497, -0.008935, -0.009382, -0.009836,
    -0.010300, -0.010771, -0.011250, -0.011737,
    -0.012232, -0.012734, -0.013243, -0.013759,
    -0.014282, -0.014812, -0.015348, -0.015891,
    -0.016439, -0.016994, -0.017555, -0.018121,
    -0.018692, -0.019269, -0.019850, -0.020437,
    -0.021029, -0.021624, -0.022225, -0.022829,
    -0.023438, -0.024050, -0.024666, -0.025285,
    -0.025908, -0.026533, -0.027162, -0.027793,
    -0.028427, -0.029063, -0.029702, -0.030342,
    -0.030985, -0.031629, -0.032274, -0.032921,
    -0.033569, -0.034218, -0.034868, -0.035518,
    -0.036169, -0.036820, -0.037471, -0.038122,
    -0.038773, -0.039423, -0.040072, -0.040721,
    -0.041368, -0.042015, -0.042660, -0.043304,
    -0.043945, -0.044585, -0.045223, -0.045859,
    -0.046492, -0.047122, -0.047750, -0.048374,
    -0.048996, -0.049614, -0.050229, -0.050840,
    -0.051447, -0.052050, -0.052649, -0.053243,
    -0.053833, -0.054418, -0.054998, -0.055573,
    -0.056143, -0.056707, -0.057266, -0.057818,
    -0.058365, -0.058905, -0.059439, -0.059967,
    -0.060488, -0.061002, -0.061508, -0.062008,
    -0.062500, -0.062984, -0.063461, -0.063930,
    -0.064390, -0.064842, -0.065286, -0.065721,
    -0.066147, -0.066564, -0.066972, -0.067370,
    -0.067759, -0.068137, -0.068506, -0.068865,
    -0.069214, -0.069552, -0.069879, -0.070196,
    -0.070501, -0.070796, -0.071079, -0.071350,
    -0.071609, -0.071857, -0.072093, -0.072316,
    -0.072527, -0.072725, -0.072911, -0.073083,
    -0.073242, -0.073388, -0.073520, -0.073639,
    -0.073744, -0.073835, -0.073911, -0.073973,
    -0.074020, -0.074053, -0.074071, -0.074073,
    -0.074060, -0.074032, -0.073988, -0.073928,
    -0.073853, -0.073760, -0.073652, -0.073527,
    -0.073385, -0.073227, -0.073051, -0.072858,
    -0.072647, -0.072419, -0.072173, -0.071909,
    -0.071627, -0.071326, -0.071007, -0.070669,
    -0.070312, -0.069937, -0.069542, -0.069127,
    -0.068693, -0.068239, -0.067765, -0.067272,
    -0.066757, -0.066222, -0.065667, -0.065091,
    -0.064493, -0.063875, -0.063235, -0.062573,
    -0.061890, -0.061184, -0.060457, -0.059707,
    -0.058935, -0.058140, -0.057323, -0.056482,
    -0.055618, -0.054731, -0.053820, -0.052886,
    -0.051928, -0.050945, -0.049938, -0.048907,
    -0.047852, -0.046771, -0.045666, -0.044535,
    -0.043379, -0.042197, -0.040990, -0.039757,
    -0.038498, -0.037213, -0.035901, -0.034562,
    -0.033197, -0.031805, -0.030386, -0.028940,
    -0.027466, -0.025964, -0.024435, -0.022877,
    -0.021292, -0.019678, -0.018035, -0.016364,
    -0.014664, -0.012934, -0.011176, -0.009388,
    -0.007570, -0.005723, -0.003845, -0.001938
};

static float logvolct[TABSIZE] = {
    0.000000, 0.000000, 0.000000, 0.000000,
    0.000001, 0.000002, 0.000004, 0.000007,
    0.000010, 0.000015, 0.000021, 0.000029,
    0.000039, 0.000051, 0.000065, 0.000082,
    0.000101, 0.000124, 0.000150, 0.000179,
    0.000213, 0.000250, 0.000292, 0.000338,
    0.000390, 0.000446, 0.000508, 0.000576,
    0.000650, 0.000731, 0.000818, 0.000912,
    0.001013, 0.001122, 0.001239, 0.001364,
    0.001498, 0.001641, 0.001793, 0.001955,
    0.002126, 0.002308, 0.002500, 0.002703,
    0.002918, 0.003144, 0.003382, 0.003633,
    0.003896, 0.004172, 0.004462, 0.004765,
    0.005083, 0.005415, 0.005761, 0.006124,
    0.006501, 0.006895, 0.007305, 0.007732,
    0.008176, 0.008637, 0.009117, 0.009614,
    0.010131, 0.010666, 0.011221, 0.011796,
    0.012391, 0.013007, 0.013644, 0.014302,
    0.014982, 0.015685, 0.016410, 0.017158,
    0.017930, 0.018725, 0.019546, 0.020390,
    0.021261, 0.022156, 0.023078, 0.024026,
    0.025001, 0.026004, 0.027034, 0.028092,
    0.029180, 0.030296, 0.031441, 0.032617,
    0.033823, 0.035059, 0.036327, 0.037627,
    0.038959, 0.040324, 0.041721, 0.043152,
    0.044617, 0.046117, 0.047651, 0.049221,
    0.050826, 0.052468, 0.054146, 0.055862,
    0.057615, 0.059406, 0.061236, 0.063105,
    0.065013, 0.066962, 0.068950, 0.070980,
    0.073051, 0.075164, 0.077320, 0.079518,
    0.081759, 0.084045, 0.086374, 0.088749,
    0.091168, 0.093633, 0.096145, 0.098703,
    0.101309, 0.103962, 0.106663, 0.109413,
    0.112212, 0.115061, 0.117960, 0.120910,
    0.123911, 0.126963, 0.130068, 0.133226,
    0.136436, 0.139701, 0.143019, 0.146392,
    0.149821, 0.153305, 0.156845, 0.160442,
    0.164097, 0.167809, 0.171579, 0.175409,
    0.179297, 0.183246, 0.187255, 0.191325,
    0.195456, 0.199649, 0.203905, 0.208223,
    0.212606, 0.217052, 0.221563, 0.226139,
    0.230780, 0.235488, 0.240263, 0.245104,
    0.250014, 0.254992, 0.260038, 0.265155,
    0.270341, 0.275597, 0.280925, 0.286324,
    0.291795, 0.297339, 0.302956, 0.308647,
    0.314412, 0.320252, 0.326168, 0.332159,
    0.338227, 0.344372, 0.350595, 0.356895,
    0.363275, 0.369734, 0.376272, 0.382891,
    0.389591, 0.396373, 0.403236, 0.410182,
    0.417212, 0.424325, 0.431522, 0.438805,
    0.446172, 0.453626, 0.461167, 0.468794,
    0.476510, 0.484314, 0.492206, 0.500188,
    0.508260, 0.516423, 0.524677, 0.533023,
    0.541461, 0.549992, 0.558616, 0.567335,
    0.576148, 0.585056, 0.594061, 0.603162,
    0.612359, 0.621655, 0.631048, 0.640540,
    0.650132, 0.659823, 0.669616, 0.679509,
    0.689504, 0.699601, 0.709801, 0.720105,
    0.730512, 0.741025, 0.751642, 0.762366,
    0.773196, 0.784133, 0.795178, 0.806331,
    0.817593, 0.828964, 0.840446, 0.852038,
    0.863742, 0.875557, 0.887485, 0.899526,
    0.911681, 0.923950, 0.936334, 0.948834,
    0.961450, 0.974182, 0.987032, 1.000000,
};


float cerp(float y0, float y1, float y2, float y3, guint8 d)
{
    float s0;

    s0 = y0 * ct0[d];
    s0 += y1 * ct1[d];
    s0 += y2 * ct2[d];
    s0 += y3 * ct3[d];

    return s0;
}


float log_amplitude(float x)
{
    int i = x * (TABSIZE - 1);
    
    return logvolct[i];
}
