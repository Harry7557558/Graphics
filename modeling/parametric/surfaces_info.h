// included by "surfaces.h"

struct shapeInfo {
	char name[32];
	int Trig_N; bool isSolid;
	vec3 AABB_min, AABB_max;
	double SA_or_V; vec3 CoM; mat3 InertiaTensor_u;
	vec3 minGravPotential_vec; double minGravPotential_u;
};

const std::vector<shapeInfo> info({
	shapeInfo{
		"land snail", 9560, false,
		vec3(-1.6666667,-1.5,-1), vec3(2,1.8333333,1.2),
		39.692873, vec3(0.037553219,0.23235076,0.29980444),
		mat3(1.0287555,-0.088597071,0.025349699,
		     -0.088597071,1.1020865,0.065133478,
		     0.025349699,0.065133478,1.4264523 ),
		vec3(-0.19460876,-0.08358241,-0.97731336), 1.1208473
	},
	shapeInfo{
		"river snail", 9560, false,
		vec3(-1.7778359,-1.6666667,-1), vec3(2,1.8775276,2.2),
		59.358073, vec3(0.016591398,0.15591507,0.55208584),
		mat3(1.1894389,-0.062251414,0.021198646,
		     -0.062251414,1.2226908,0.083662692,
		     0.021198646,0.083662692,1.448832 ),
		vec3(-0.18418774,-0.07894415,-0.97971562), 1.3685309
	},
	shapeInfo{
		"field snail", 9560, false,
		vec3(-1.1312687,-0.9447958,-1), vec3(1.5,1.3177415,1.1),
		19.571683, vec3(0.050634132,0.20522757,0.26598447),
		mat3(0.6040561,-0.061228026,0.029545697,
		     -0.061228026,0.66658911,0.046495186,
		     0.029545697,0.046495186,0.69159457 ),
		vec3(-0.53932494,-0.13459103,-0.83127244), 0.99589391
	},
	shapeInfo{
		"pond snail", 9560, false,
		vec3(-0.95243011,-0.82836218,-1), vec3(1.2,1.0778689,1.8),
		21.595118, vec3(0.014562024,0.072745182,0.44322145),
		mat3(0.55162665,-0.034758419,0.014910785,
		     -0.034758419,0.56625479,0.029091457,
		     0.014910785,0.029091457,0.43402925 ),
		vec3(0.00000004,0.76920434,0.63900288), 0.8110295
	},
	shapeInfo{
		"cone snail", 9560, false,
		vec3(-1.1053068,-1.0840289,-1), vec3(1.2,1.1536454,3.5),
		68.623182, vec3(0.0013615369,0.022699925,0.88156534),
		mat3(0.90045896,-0.010893479,0.0027725777,
		     -0.010893479,0.90185592,0.019841066,
		     0.0027725777,0.019841066,0.43999488 ),
		vec3(-0.63100393,0.70080086,0.33273441), 0.85619433
	},
	shapeInfo{
		"pot snail", 9600, false,
		vec3(-1.662179,-1.8629999,-4.2344429), vec3(2.0384466,1.4951598,2.5632856),
		79.861321, vec3(0.065485634,-0.11523596,-0.91703732),
		mat3(2.5661016,0.046356903,0.14856173,
		     0.046356903,2.6788441,-0.17025022,
		     0.14856173,-0.17025022,1.0903471 ),
		vec3(-0.43739295,-0.81547600,0.37905977), 1.3111324
	},
	shapeInfo{
		"moon snail", 12800, false,
		vec3(-3.5629059,-1.372238,-2.9690883), vec3(1.8973374,2.6295159,1.0515233),
		52.277753, vec3(-0.48009016,0.77314599,-0.38985667),
		mat3(2.1902156,0.30670538,-0.43308957,
		     0.30670538,3.1192721,0.24417158,
		     -0.43308957,0.24417158,3.0099454 ),
		vec3(0.44676946,0.28568986,-0.84780797), 1.6558312
	},
	shapeInfo{
		"snail (in)", 12800, false,
		vec3(-4.1281931,-1.4804126,-2.0137527), vec3(2.1005266,2.9767201,2.0137527),
		61.234629, vec3(-0.63220545,0.94547874,0),
		mat3(2.3101332,0.38023897,0,
		     0.38023897,3.6768245,0,
		     0,0,3.967997 ),
		vec3(0.23526333,0.08045930,-0.96859561), 1.5766597
	},
	shapeInfo{
		"snail (out)", 12800, false,
		vec3(-1.219709,-0.3292207,-0.40656966), vec3(0.51632083,0.80975225,0.40656966),
		3.7048148, vec3(-0.25953309,0.3019076,0),
		mat3(0.12122862,0.019919115,0,
		     0.019919115,0.25770262,0,
		     0,0,0.29721989 ),
		vec3(0.16399489,0.03265845,-0.98592043), 0.30569234
	},
	shapeInfo{
		"snail (outer)", 12800, false,
		vec3(-22.383696,-1.7575401,-13.309224), vec3(4.3228511,10.632498,7.2532537),
		618.22501, vec3(-8.6915097,5.0546784,-2.5474861),
		mat3(30.266427,-4.1376903,-22.887012,
		     -4.1376903,71.540167,-2.4136303,
		     -22.887012,-2.4136303,59.629856 ),
		vec3(0.48555068,0.32925230,-0.80983545), 4.1397065
	},
	shapeInfo{
		"textured snail 1", 160000, false,
		vec3(-8.0806289,-5.0157769,-2.8207294), vec3(5.9388696,7.0455383,2.8207294),
		469.64135, vec3(-0.37017445,1.1705004,0),
		mat3(10.082335,1.6075109,0,
		     1.6075109,12.164165,0,
		     0,0,18.565768 ),
		vec3(0.07688087,-0.02621631,-0.99669556), 2.4319661
	},
	shapeInfo{
		"textured snail 2", 160000, false,
		vec3(-3.0701905,-4.6451943,-4.2285278), vec3(4.1055545,3.5711464,0.94472968),
		204.7071, vec3(0.52699835,-0.13476199,-0.89098478),
		mat3(4.7875769,0.4344653,0.58176322,
		     0.4344653,4.355626,-0.37289956,
		     0.58176322,-0.37289956,5.8504016 ),
		vec3(-0.11642941,0.36325856,-0.92438488), 2.5746118
	},
	shapeInfo{
		"textured snail 3", 320000, false,
		vec3(-5.5716404,-3.4920527,-5.4365637), vec3(4.0603396,4.890348,2.6699355),
		340.9607, vec3(-0.18666089,0.61457439,-0.91549199),
		mat3(6.3708059,0.58766952,-0.62237152,
		     0.58766952,7.0063763,0.88085518,
		     -0.62237152,0.88085518,6.770968 ),
		vec3(0.44608098,-0.04383210,-0.89391863), 3.3453836
	},
	shapeInfo{
		"textured snail 4", 160000, false,
		vec3(-3.123793,-4.8321449,-5.6026837), vec3(4.2477191,3.6279382,4.429774),
		215.7106, vec3(0.61081276,-0.1754761,-0.46818526),
		mat3(8.2444504,0.51689603,1.2483371,
		     0.51689603,7.6616845,-0.76729033,
		     1.2483371,-0.76729033,6.3877478 ),
		vec3(-0.50206806,0.46480024,-0.72930679), 3.4325405
	},
	shapeInfo{
		"textured snail #1", 160000, false,
		vec3(-4.3859646,-6.5430903,-9.3235893), vec3(5.3052096,4.8950662,4.9899349),
		459.92287, vec3(0.54094692,-0.14860146,-1.7182177),
		mat3(13.290524,0.44877891,1.90417,
		     0.44877891,12.768304,-1.165608,
		     1.90417,-1.165608,6.8972838 ),
		vec3(-0.35921525,0.56078370,-0.74597993), 4.7059935
	},
	shapeInfo{
		"textured snail #2", 200000, false,
		vec3(-7.7042086,-4.7821406,-3.7247465), vec3(5.6621825,6.7173334,3.6504362),
		544.07448, vec3(-0.48498205,1.2721209,0.26401248),
		mat3(9.917175,1.5239202,-0.65735964,
		     1.5239202,12.344777,0.91677235,
		     -0.65735964,0.91677235,17.140046 ),
		vec3(0.17187216,-0.11994485,-0.97778995), 3.1749074
	},
	shapeInfo{
		"melon shell", 36000, false,
		vec3(-5.4365637,-3.3129487,-10.107338), vec3(3.9166262,4.6235923,4.6707743),
		327.35463, vec3(-0.43427926,1.0589711,-1.2995341),
		mat3(14.706379,0.91287816,-1.0399261,
		     0.91287816,16.354358,1.132667,
		     -1.0399261,1.132667,10.048661 ),
		vec3(0.09974901,0.99273286,0.06731714), 3.5368871
	},
	shapeInfo{
		"bailer shell", 36000, false,
		vec3(-5.4365637,-3.3129487,-11.361932), vec3(3.9166262,4.6235923,4.0494429),
		345.00002, vec3(-0.41828375,0.97371492,-0.29279163),
		mat3(16.935723,0.8512899,-2.0135982,
		     0.8512899,18.399269,1.6770776,
		     -2.0135982,1.6770776,9.4807033 ),
		vec3(0.11459291,0.99088957,-0.07075533), 3.6216772
	},
	shapeInfo{
		"wonder shell", 36000, false,
		vec3(-1.3763861,-2.4883532,-3.4248701), vec3(2.0533268,1.6811218,2.7684358),
		48.496435, vec3(0.37073406,-0.14167347,0.30969758),
		mat3(2.5026768,0.14472366,0.26551982,
		     0.14472366,2.2847239,-0.25290001,
		     0.26551982,-0.25290001,1.4922434 ),
		vec3(0.71813963,0.59081729,-0.36770966), 1.4849488
	},
	shapeInfo{
		"pear shell", 36000, false,
		vec3(-1.563014,-2.4402711,-5.9025887), vec3(2.1098482,1.8159632,2.9399186),
		69.28653, vec3(0.28100048,-0.080537325,0.80649576),
		mat3(3.063867,0.1276434,0.092325948,
		     0.1276434,2.933928,-0.094175263,
		     0.092325948,-0.094175263,1.5456912 ),
		vec3(0.52686371,-0.14117271,0.83814372), 1.6790532
	},
	shapeInfo{
		"torus", 6400, true,
		vec3(-3,-3,-1), vec3(3,3,1),
		39.275861, vec3(0,0,0),
		mat3(2.6199962,0,0,
		     0,2.6199962,0,
		     0,0,4.7420444 ),
		vec3(0.00000000,0.00000000,-1.00000000), 1
	},
	shapeInfo{
		"uneven torus", 12000, true,
		vec3(-2.24,-2.579604,-1.1754749), vec3(2.88,2.579604,1.1754749),
		20.659001, vec3(1.0563705,0,0),
		mat3(2.3192575,0,0,
		     0,1.6391693,0,
		     0,0,3.4956713 ),
		vec3(-0.16991324,-0.00000000,-0.98545903), 0.92165933
	},
	shapeInfo{
		"deformed torus 1", 14400, true,
		vec3(-3.1016849,-3.2124356,-1), vec3(3.1016849,3.2124356,1),
		39.39158, vec3(0,0,0),
		mat3(2.6814025,0,0,
		     0,2.6814025,0,
		     0,0,4.8639902 ),
		vec3(0.00000000,0.00000000,-1.00000000), 1
	},
	shapeInfo{
		"deformed torus 2", 24000, true,
		vec3(-3.7912552,-3.268186,-0.98731273), vec3(3.7912552,3.9746255,0.98731273),
		21.506538, vec3(0,0,0),
		mat3(3.1701259,0,0,
		     0,3.1701259,0,
		     0,0,6.0052625 ),
		vec3(-0.00000000,0.00000000,-1.00000000), 0.98731273
	},
	shapeInfo{
		"textured torus (tire)", 129600, true,
		vec3(-2.7999023,-2.7999023,-0.79990234), vec3(2.7999023,2.7999023,0.79990234),
		23.36407, vec3(0,0,0),
		mat3(2.3728261,0,0,
		     0,2.3728261,0,
		     0,0,4.4472841 ),
		vec3(-0.00000000,0.00000000,-1.00000000), 0.79990234
	},
	shapeInfo{
		"textured torus (cells)", 288000, true,
		vec3(-2.8850574,-2.8850574,-0.9), vec3(2.8850574,2.8850574,0.9),
		26.883458, vec3(0,0,0),
		mat3(2.428435,0,0,
		     0,2.428435,0,
		     0,0,4.5140577 ),
		vec3(0.00000000,-0.00000000,-1.00000000), 0.9
	},
	shapeInfo{
		"textured torus (wavy)", 129600, true,
		vec3(-2.9,-2.9,-0.88033276), vec3(2.9,2.9,0.88033276),
		26.652683, vec3(0,0,0),
		mat3(2.4243383,0,0,
		     0,2.4243383,0,
		     0,0,4.5090652 ),
		vec3(0.00000000,-0.00000000,-1.00000000), 0.88033276
	},
	shapeInfo{
		"textured torus (hexa)", 129600, true,
		vec3(-2.9,-2.8974392,-0.9), vec3(2.8593016,2.8974392,0.9),
		27.511135, vec3(-1.0744402e-06,0,0),
		mat3(2.4387012,0,0,
		     0,2.4387578,0,
		     0,0,4.5263444 ),
		vec3(0.00000000,-0.00000000,-1.00000000), 0.9
	},
	shapeInfo{
		"bended torus", 12000, true,
		vec3(-3.150673,-3.150673,-0.90329351), vec3(3.150673,3.150673,1.7836366),
		31.635418, vec3(0,0,0.33774401),
		mat3(3.6711799,2.540946e-05,0,
		     2.540946e-05,3.6711729,0,
		     0,0,6.749967 ),
		vec3(-0.00174876,-0.00371630,-0.99999157), 1.2410271
	},
	shapeInfo{
		"twisted torus", 24000, true,
		vec3(-2.8,-2.6132026,-2.8), vec3(2.8,2.6132026,2.8),
		25.179939, vec3(0,0,0),
		mat3(3.474258,0,0,
		     0,3.401203,0,
		     0,0,2.3948355 ),
		vec3(-0.10982689,-0.59240902,-0.79811628), 2.5162986
	},
	shapeInfo{
		"toric heart", 32000, true,
		vec3(-2.2805657,-2.8144526,-0.11222629), vec3(2.2805657,1.656436,0.11222629),
		1.1368147, vec3(0,0.089270399,0),
		mat3(1.440308,0,0,
		     0,2.0375698,0,
		     0,0,3.470619 ),
		vec3(-0.00000000,-0.00000000,-1.00000000), 0.11222629
	},
	shapeInfo{
		"toric cercis", 32000, true,
		vec3(-3.6813461,-3.13154,-0.39300704), vec3(3.6813461,3.8707963,0.39300704),
		3.7553971, vec3(0,0,0),
		mat3(3.5518347,0,0,
		     0,3.5518347,0,
		     0,0,7.050396 ),
		vec3(-0.00000000,0.00000000,-1.00000000), 0.39300704
	},
	shapeInfo{
		"egg", 6240, true,
		vec3(-0.69280312,-0.69280312,-1), vec3(0.69280312,0.69280312,1),
		1.9822745, vec3(0,0,-0.099576686),
		mat3(0.29586915,0,0,
		     0,0.29586915,0,
		     0,0,0.1908329 ),
		vec3(-0.86552939,-0.48471966,0.12611788), 0.68165246
	},
	shapeInfo{
		"droplet", 6240, true,
		vec3(-0.60885,-0.62956058,-1), vec3(0.60885,0.59550379,1),
		1.3326058, vec3(0,-0.021879345,-0.22585183),
		mat3(0.23949067,0,0,
		     0,0.23897955,-0.0052391229,
		     0,-0.0052391229,0.14404593 ),
		vec3(0.03811141,-0.97000007,0.24009869), 0.56384255
	},
	shapeInfo{
		"bowling", 7140, true,
		vec3(-0.959727,-0.959727,-2.6), vec3(0.959727,0.959727,2),
		6.396697, vec3(0,0,-0.89147345),
		mat3(1.1744656,0,0,
		     0,1.1744656,0,
		     0,0,0.32569888 ),
		vec3(0.25404146,-0.94809566,0.19125257), 0.89925204
	},
	shapeInfo{
		"gourd", 23920, true,
		vec3(-0.95990916,-0.95990916,-1.4), vec3(0.95990916,0.95990916,1.4042203),
		3.5437147, vec3(0,-9.5789565e-06,-0.46368834),
		mat3(0.43121093,0,0,
		     0,0.43121073,1.3782172e-05,
		     0,1.3782172e-05,0.33474285 ),
		vec3(0.00102189,-0.01952291,-0.99980889), 0.93613253
	},
	shapeInfo{
		"pumpkin", 47880, true,
		vec3(-1.1740001,-1.1740001,-0.73276784), vec3(1.1740001,1.1740001,1.094775),
		4.2757527, vec3(0,0.0001967329,0.0076838409),
		mat3(0.3896399,0,0,
		     0,0.38963305,-0.00018804768,
		     0,-0.00018804768,0.50512186 ),
		vec3(0.00000000,0.00000000,-1.00000000), 0.74045168
	},
	shapeInfo{
		"pepper (without stem)", 7140, true,
		vec3(-1.1534712,-1.0028384,-3.3692967), vec3(0.95505771,1.0028384,2.0274682),
		10.1862, vec3(-0.1210771,0,0.041170118),
		mat3(1.6996205,0,-0.060433119,
		     0,1.7184858,0,
		     -0.060433119,0,0.40187612 ),
		vec3(-0.35615309,-0.92781047,-0.11100766), 0.88076157
	},
	shapeInfo{
		"ball cactus", 35800, true,
		vec3(-1.0826618,-1.0826618,-1.4), vec3(1.0826618,1.0826618,1),
		5.0389455, vec3(0,0,-0.039851916),
		mat3(0.50168827,0,0,
		     0,0.50168827,0,
		     0,0,0.40431472 ),
		vec3(0.26586853,0.94344450,-0.19805656), 0.99265666
	},
	shapeInfo{
		"ice cream", 14280, true,
		vec3(-1.2228037,-1.1760201,-1), vec3(1.129552,1.1466206,2),
		4.9929982, vec3(-0.0062409795,0.024129639,-0.042203657),
		mat3(0.5234456,-2.7956192e-05,-0.018023804,
		     -2.7956192e-05,0.52412898,-0.0027453183,
		     -0.018023804,-0.0027453183,0.43215007 ),
		vec3(0.00092844,-0.00036201,-0.99999950), 0.95774256
	},
});
