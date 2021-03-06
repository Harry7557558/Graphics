template<typename vec3, typename Float>
class ColorFunctions {
	static Float clp(double x) {
		return (Float)(x<0.?0.:x>1.?1.:x);
	}
public:
	static vec3 AlpineColors(double t) {
		double r = ((-0.460410*t+1.299402)*t-0.124433)*t+0.291151;
		double g = ((0.701259*t-0.551196)*t+0.511800)*t+0.358617;
		double b = ((((9.850110*t-27.276721)*t+26.325992)*t-9.114766)*t+0.677029)*t+0.469208;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 LakeColors(double t) {
		double r = ((0.170498*t-0.462347)*t+0.951617)*t+0.281310;
		double g = (((((((-222.687136*t+878.739929)*t-1399.827368)*t+1152.423332)*t-520.594887)*t+125.492391)*t-14.824370)*t+2.136362)*t+0.039965;
		double b = (((((((-245.209626*t+1019.919939)*t-1724.576278)*t+1512.943368)*t-725.047546)*t+181.940901)*t-21.839607)*t+2.182335)*t+0.510837;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 ArmyColors(double t) {
		double r = ((((-4.153678*t+11.919164)*t-12.031831)*t+5.158674)*t-0.594547)*t+0.470007;
		double g = ((((-8.314768*t+20.561526)*t-16.932380)*t+5.473920)*t-0.637862)*t+0.606450;
		double b = ((((-4.902141*t+12.744644)*t-11.564813)*t+4.898135)*t-1.038423)*t+0.517779;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 MintColors(double t) {
		double r = ((-0.040723*t-0.515012)*t+1.007432)*t+0.455659;
		double g = ((-0.039363*t-0.306733)*t-0.015336)*t+0.972412;
		double b = ((-0.023873*t-0.512441)*t+0.687374)*t+0.629698;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 AtlanticColors(double t) {
		double r = ((((((63.609352*t-220.185769)*t+289.485976)*t-178.105925)*t+49.481473)*t-4.767794)*t+0.823218)*t+0.126111;
		double g = ((((((59.343485*t-208.324695)*t+279.939083)*t-176.698973)*t+49.065066)*t-3.945080)*t+0.952423)*t+0.154633;
		double b = ((((((62.682564*t-219.824467)*t+296.028823)*t-188.740339)*t+53.994074)*t-4.668004)*t+0.979985)*t+0.158019;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 NeonColors(double t) {
		double r = (((1.065625*t-2.612422)*t+1.766582)*t-0.151428)*t+0.730725;
		double g = ((((((-80.136254*t+302.361051)*t-442.814108)*t+312.937417)*t-107.749031)*t+17.242816)*t-2.580003)*t+0.944868;
		double b = (((((24.117901*t-76.320225)*t+88.498887)*t-44.826834)*t+9.871994)*t-0.873899)*t+0.311434;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 AuroraColors(double t) {
		double r = ((((-7.377312*t+25.041035)*t-29.026343)*t+13.700870)*t-1.741400)*t+0.274645;
		double g = ((((12.730719*t-29.061300)*t+18.415445)*t-1.647106)*t-0.424230)*t+0.259195;
		double b = (((-8.688463*t+17.349448)*t-10.064237)*t+2.102346)*t+0.239764;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 PearlColors(double t) {
		double r = (((-4.253988*t+10.499200)*t-7.122280)*t+0.939858)*t+0.889312;
		double g = (((-4.224414*t+10.402171)*t-7.617237)*t+1.433683)*t+0.815841;
		double b = (((-6.507132*t+14.065599)*t-9.006051)*t+1.658168)*t+0.755390;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 AvocadoColors(double t) {
		double r = ((((((((-882.288061*t+3959.450117)*t-7384.639366)*t+7391.598399)*t-4268.644801)*t+1413.932340)*t-248.304574)*t+20.523277)*t-0.638933)*t+0.004196;
		double g = ((((-2.760548*t+7.139322)*t-5.578178)*t+0.276656)*t+1.920774)*t-0.016355;
		double b = ((0.437256*t-0.629665)*t+0.436066)*t-0.008204;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 PlumColors(double t) {
		double r = ((3.177451*t-4.900908)*t+2.688923)*t-0.027889;
		double g = ((0.800061*t-0.403509)*t+0.513439)*t-0.005103;
		double b = ((-1.988911*t+2.225284)*t+0.172015)*t+0.003292;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 BeachColors(double t) {
		double r = (((((-8.641307*t+24.785284)*t-22.076044)*t+6.902319)*t-1.059187)*t+0.264122)*t+0.848538;
		double g = (((3.924883*t-5.381801)*t+1.191127)*t+0.799176)*t+0.497889;
		double b = (((-4.605454*t+7.497016)*t-2.589964)*t+0.422272)*t+0.252597;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 RoseColors(double t) {
		double r = (((((-18.663522*t+56.544713)*t-60.260427)*t+25.334043)*t-3.796088)*t+1.391436)*t+0.141690;
		double g = (((((-15.590809*t+47.581595)*t-51.133120)*t+21.411346)*t-2.967309)*t+0.519299)*t+0.302366;
		double b = (((2.754625*t-5.705759)*t+2.639859)*t+0.330081)*t+0.093967;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 CandyColors(double t) {
		double r = ((((-7.255706*t+20.806608)*t-19.527908)*t+5.347037)*t+0.882267)*t+0.408638;
		double g = (((((-21.219812*t+64.222029)*t-72.261162)*t+35.460804)*t-6.030600)*t+0.491628)*t+0.196064;
		double b = ((-2.014010*t+2.847061)*t-0.318266)*t+0.350469;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 SolarColors(double t) {
		double r = (((0.303970*t+0.272243)*t-1.890279)*t+1.871831)*t+0.443041;
		double g = ((-1.151525*t+1.865516)*t+0.082776)*t+0.011869;
		double b = ((-0.204874*t+0.465491)*t-0.153349)*t+0.018102;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 CMYKColors(double t) {
		double r = (((((9.270347*t-32.656921)*t+40.777609)*t-24.729841)*t+6.156415)*t+0.989395)*t+0.294212;
		double g = (((((((((1640.257186*t-8322.695132)*t+18127.694162)*t-22172.495563)*t+16682.549265)*t-7879.165823)*t+2218.719066)*t-299.065362)*t+2.068440)*t+1.562245)*t+0.666766;
		double b = ((((((79.354423*t-314.463440)*t+479.432615)*t-365.874541)*t+151.758129)*t-33.405535)*t+2.400130)*t+0.875719;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 SouthwestColors(double t) {
		double r = (((((-9.083745*t+41.756003)*t-68.877532)*t+50.735816)*t-18.127495)*t+3.584249)*t+0.357235;
		double g = (((5.850033*t-14.083156)*t+9.827158)*t-1.331198)*t+0.331948;
		double b = (((((((((3840.115969*t-19665.602526)*t+42837.492390)*t-51736.263329)*t+37791.772137)*t-17029.846446)*t+4607.012001)*t-696.490186)*t+54.792378)*t-2.328029)*t+0.219307;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 DeepSeaColors(double t) {
		double r = (((((((((1958.182097*t-8312.255630)*t+14184.797062)*t-12167.089992)*t+5114.868730)*t-560.749479)*t-321.958475)*t+117.807741)*t-14.136307)*t+1.175460)*t+0.156409;
		double g = ((((3.684232*t-9.358525)*t+7.089168)*t-0.624148)*t+0.135086)*t+0.002969;
		double b = ((-0.227953*t-0.399081)*t+1.324820)*t+0.289083;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 StarryNightColors(double t) {
		double r = ((((5.852741*t-15.179333)*t+12.191305)*t-2.907211)*t+0.923035)*t+0.080067;
		double g = ((((5.023371*t-12.102318)*t+8.869370)*t-2.423812)*t+1.307592)*t+0.135643;
		double b = (((2.047370*t-4.859669)*t+2.330914)*t+0.630959)*t+0.209040;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 FallColors(double t) {
		double r = (((-2.976110*t+4.746324)*t-1.868557)*t+0.788262)*t+0.250162;
		double g = (((-4.977781*t+9.892899)*t-5.179914)*t+0.665275)*t+0.378050;
		double b = ((-0.383254*t+1.010048)*t-0.768541)*t+0.396166;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 SunsetColors(double t) {
		double r = (((((((((822.509670*t-4095.552899)*t+8612.014446)*t-9874.423851)*t+6610.580667)*t-2532.769573)*t+487.472086)*t-29.236624)*t-2.046158)*t+2.467795)*t-0.015402;
		double g = (((-2.138495*t+3.191751)*t-0.902520)*t+0.831316)*t-0.001494;
		double b = (((((((((-1569.776983*t+6880.423618)*t-11963.738921)*t+9838.928574)*t-2674.501258)*t-1731.858135)*t+1729.728508)*t-583.709021)*t+75.810407)*t-0.323758)*t+0.015502;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 FruitPunchColors(double t) {
		double r = (((-3.096261*t+7.175056)*t-4.604476)*t+0.496418)*t+0.984911;
		double g = (((3.190810*t-4.430265)*t+0.343937)*t+0.781746)*t+0.492161;
		double b = (((((((139.925962*t-530.594608)*t+821.721789)*t-669.258992)*t+301.431905)*t-71.071033)*t+8.835515)*t-0.465453)*t+0.005826;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 ThermometerColors(double t) {
		double r = (((3.739407*t-8.975019)*t+4.840758)*t+0.775449)*t+0.152765;
		double g = (((((9.909064*t-31.460648)*t+43.829953)*t-32.400926)*t+8.511813)*t+1.579476)*t+0.114388;
		double b = (((1.310361*t-0.992970)*t-2.171245)*t+1.232916)*t+0.786308;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 IslandColors(double t) {
		double r = (((7.024084*t-18.003175)*t+14.419226)*t-3.583224)*t+0.799827;
		double g = ((((4.398976*t-11.548815)*t+11.526790)*t-6.726459)*t+2.792318)*t+0.341331;
		double b = (((((-3.084243*t+16.349326)*t-29.221930)*t+24.727652)*t-13.224244)*t+4.589624)*t+0.169477;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 WatermelonColors(double t) {
		double r = (((-2.985012*t+4.766153)*t-2.634289)*t+1.628917)*t+0.084590;
		double g = (((((((126.178667*t-488.891009)*t+775.126285)*t-647.409793)*t+303.092714)*t-78.325589)*t+8.939479)*t+1.542606)*t+0.092279;
		double b = ((((((((413.259086*t-1708.556567)*t+2927.083422)*t-2685.009511)*t+1420.851021)*t-438.975792)*t+77.628189)*t-7.110972)*t+1.093877)*t+0.090329;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 BrassTones(double t) {
		double r = (((((((((8222.131879*t-42072.418439)*t+91302.111724)*t-109228.981333)*t+78376.224558)*t-34384.944449)*t+9026.191684)*t-1342.451334)*t+103.752644)*t-1.598966)*t+0.158810;
		double g = (((((((((7437.969338*t-38058.156227)*t+82581.904153)*t-98772.720542)*t+70836.068554)*t-31042.144770)*t+8129.816639)*t-1203.946636)*t+92.783015)*t-1.576804)*t+0.168996;
		double b = (((((((((3819.612948*t-19590.473802)*t+42632.993065)*t-51180.855536)*t+36885.909562)*t-16274.607731)*t+4304.115135)*t-646.652369)*t+50.913823)*t-0.960686)*t+0.055909;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 GreenPinkTones(double t) {
		double r = (((((((((-2775.588884*t+12676.399187)*t-23880.307084)*t+23923.814296)*t-13768.519494)*t+4699.677742)*t-1037.724996)*t+179.226432)*t-17.427694)*t+0.676043)*t-0.005965;
		double g = (((((((((-4373.597077*t+23034.208751)*t-51236.496592)*t+62495.005100)*t-45407.286671)*t+19975.647479)*t-5154.137719)*t+707.694624)*t-45.570262)*t+4.360627)*t+0.218363;
		double b = (((((((((-4005.481726*t+18706.478399)*t-36271.006315)*t+37692.169630)*t-22634.323575)*t+7954.417604)*t-1638.740834)*t+210.928618)*t-15.040710)*t+0.796198)*t+0.015089;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 BrownCyanTones(double t) {
		double r = (((1.983431*t-3.000834)*t-0.903105)*t+1.939941)*t+0.329567;
		double g = ((-1.005122*t-0.319894)*t+1.775389)*t+0.193352;
		double b = ((-1.323533*t+0.255944)*t+1.758881)*t+0.067869;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 PigeonTones(double t) {
		double r = ((((-4.178191*t+5.785016)*t-0.474880)*t-1.198836)*t+0.855609)*t+0.185403;
		double g = ((((-4.402089*t+7.861543)*t-3.320230)*t-0.289471)*t+0.968648)*t+0.165149;
		double b = ((((-5.365734*t+9.473938)*t-3.687339)*t-0.652826)*t+1.004194)*t+0.207671;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 CherryTones(double t) {
		double r = (((-1.481560*t+4.957752)*t-5.906149)*t+3.246821)*t+0.183639;
		double g = ((-0.615352*t+2.064235)*t-0.666106)*t+0.218721;
		double b = ((-0.709857*t+2.181442)*t-0.691495)*t+0.219939;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 RedBlueTones(double t) {
		double r = ((((3.955175*t-4.623828)*t-0.868819)*t-0.211663)*t+1.453034)*t+0.444854;
		double g = (((((-13.707818*t+35.859381)*t-28.526327)*t+1.849032)*t+4.003657)*t+0.652329)*t+0.157344;
		double b = (((0.707358*t-3.691135)*t+2.895810)*t+0.395280)*t+0.223373;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 CoffeeTones(double t) {
		double r = (((-1.289765*t+3.050707)*t-2.674967)*t+1.498357)*t+0.386581;
		double g = (((-2.546274*t+5.843102)*t-4.125512)*t+1.518516)*t+0.305482;
		double b = (((((((((-1914.936599*t+9499.141819)*t-20034.286091)*t+23366.468120)*t-16386.903027)*t+7008.821735)*t-1755.174305)*t+230.215245)*t-13.379589)*t+0.752913)*t+0.273854;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 RustTones(double t) {
		double r = (((((((244.269662*t-979.045945)*t+1576.220506)*t-1297.982673)*t+577.973075)*t-137.378399)*t+16.157964)*t+0.793826)*t-0.001702;
		double g = (((1.705130*t-3.419240)*t+1.715006)*t+0.469944)*t+0.008180;
		double b = ((0.005312*t+0.156144)*t-0.317714)*t+0.195550;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 FuchsiaTones(double t) {
		double r = ((-0.466156*t+0.066281)*t+1.267901)*t+0.094449;
		double g = (((((13.373851*t-38.595778)*t+38.826420)*t-16.126419)*t+2.973218)*t+0.378597)*t+0.099165;
		double b = ((-0.544785*t+0.357985)*t+1.050557)*t+0.100979;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 SiennaTones(double t) {
		double r = (((0.248879*t+0.431829)*t-1.989025)*t+1.782505)*t+0.441441;
		double g = ((-0.800216*t+0.935688)*t+0.556558)*t+0.174783;
		double b = ((((3.581880*t-9.085284)*t+6.880188)*t-0.658441)*t+0.023900)*t+0.071818;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 GrayTones(double t) {
		double r = (((-2.360204*t+4.329507)*t-2.064468)*t+0.912430)*t+0.087710;
		double g = (((-1.864168*t+3.533443)*t-1.844189)*t+0.999569)*t+0.087621;
		double b = (((-1.647894*t+3.293501)*t-2.007490)*t+1.151440)*t+0.085328;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 ValentineTones(double t) {
		double r = ((-0.464731*t+0.625526)*t+0.276213)*t+0.527680;
		double g = ((-0.714282*t+1.520089)*t-0.086158)*t+0.123112;
		double b = ((-0.753774*t+1.432981)*t-0.012373)*t+0.212358;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 GrayYellowTones(double t) {
		double r = ((-1.565155*t+1.783088)*t+0.518247)*t+0.179629;
		double g = ((-2.070609*t+2.152388)*t+0.419566)*t+0.217944;
		double b = (((-1.213197*t-0.734655)*t+1.225069)*t+0.640990)*t+0.292821;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 DarkTerrain(double t) {
		double r = (((((-23.071561*t+67.536225)*t-72.561438)*t+38.458323)*t-12.531675)*t+3.217179)*t-0.031514;
		double g = ((((((-48.744094*t+140.286723)*t-150.930492)*t+77.209875)*t-15.713642)*t-4.050191)*t+2.900230)*t+0.044884;
		double b = ((((((-60.224561*t+179.766670)*t-206.202795)*t+119.822186)*t-36.936027)*t+4.294622)*t+0.023800)*t+0.466161;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 LightTerrain(double t) {
		double r = (((1.326056*t-3.993269)*t+3.672734)*t-0.664319)*t+0.556015;
		double g = ((-1.840647*t+3.196736)*t-1.249471)*t+0.772278;
		double b = ((-1.855286*t+4.108936)*t-2.220142)*t+0.859387;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 GreenBrownTerrain(double t) {
		double r = (((5.641544*t-9.755103)*t+4.004644)*t+1.121284)*t-0.000966;
		double g = (((6.008189*t-7.749246)*t+0.538799)*t+2.259241)*t-0.021667;
		double b = (((2.044244*t+1.274378)*t-5.001379)*t+2.722898)*t-0.017723;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 SandyTerrain(double t) {
		double r = (((((((-242.327925*t+913.565766)*t-1376.547892)*t+1061.684575)*t-445.607055)*t+98.147291)*t-10.602964)*t+1.312638)*t+0.647332;
		double g = (((((((-278.413383*t+1111.689645)*t-1796.897480)*t+1501.336429)*t-680.106053)*t+155.260825)*t-14.178758)*t+1.345264)*t+0.305753;
		double b = (((1.754341*t-3.237118)*t+1.361760)*t+0.092905)*t+0.213042;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 BrightBands(double t) {
		double r = (((((((((8963.706553*t-48956.611185)*t+112727.696622)*t-142206.523119)*t+106861.360056)*t-48653.157750)*t+12996.975352)*t-1829.474132)*t+95.915482)*t+0.206853)*t+0.874957;
		double g = (((((((((-6518.768726*t+27333.714619)*t-46148.676474)*t+39474.344448)*t-17175.152693)*t+2832.088320)*t+331.696838)*t-126.146747)*t-7.407318)*t+4.969774)*t+0.055010;
		double b = (((((((((-17479.136796*t+75874.291784)*t-134705.060785)*t+124846.355983)*t-63097.980829)*t+15579.519161)*t-518.903329)*t-616.429648)*t+120.547951)*t-3.087074)*t+0.254035;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 DarkBands(double t) {
		double r = (((((((((20440.180509*t-96929.628112)*t+194476.481726)*t-215541.492146)*t+144707.565443)*t-60803.903183)*t+15996.118575)*t-2581.192748)*t+249.872288)*t-13.887387)*t+0.810998;
		double g = (((((((((13257.425373*t-56666.606122)*t+97501.197998)*t-84794.286062)*t+37108.063564)*t-5592.047010)*t-1312.103231)*t+547.214528)*t-47.428082)*t-1.443520)*t+0.889197;
		double b = (((((((((13499.308392*t-76823.156651)*t+185045.281117)*t-245732.802123)*t+196253.034189)*t-96412.351529)*t+28582.892142)*t-4802.400047)*t+405.087918)*t-15.660995)*t+1.073891;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 Aquamarine(double t) {
		double r = ((2.648153*t-3.758469)*t+1.094755)*t+0.663675;
		double g = ((1.617527*t-2.414953)*t+0.817216)*t+0.722749;
		double b = ((1.382887*t-1.774002)*t+0.407752)*t+0.841571;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 Pastel(double t) {
		double r = (((((20.472478*t-56.236842)*t+56.655630)*t-26.877218)*t+5.501692)*t+0.149051)*t+0.763604;
		double g = ((((11.305244*t-28.614866)*t+24.188804)*t-8.579304)*t+1.959939)*t+0.451801;
		double b = ((((((((-501.512857*t+2468.364935)*t-5013.611819)*t+5384.680173)*t-3239.544387)*t+1041.921451)*t-135.430799)*t-6.084756)*t+1.208750)*t+0.921963;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 BlueGreenYellow(double t) {
		double r = ((0.437406*t+0.739996)*t-0.370063)*t+0.125395;
		double g = ((0.401903*t-1.419056)*t+1.923438)*t-0.011473;
		double b = ((0.778786*t-1.798718)*t+0.984957)*t+0.383446;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 Rainbow(double t) {
		double r = (((((33.038589*t-100.425221)*t+116.136811)*t-67.842553)*t+23.470346)*t-4.018730)*t+0.498427;
		double g = (((((39.767595*t-128.776104)*t+160.884144)*t-98.285228)*t+27.859936)*t-1.455343)*t+0.123248;
		double b = (((((31.953017*t-102.708635)*t+118.024799)*t-53.054309)*t+3.232700)*t+2.168052)*t+0.513964;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 DarkRainbow(double t) {
		double r = ((((((-142.210521*t+502.789576)*t-671.245947)*t+415.716559)*t-119.389602)*t+15.448203)*t-0.623802)*t+0.244188;
		double g = (((((((((-3275.134771*t+16029.375036)*t-33642.901094)*t+39518.705484)*t-28343.282455)*t+12658.429894)*t-3434.015649)*t+524.912604)*t-37.208371)*t+1.017535)*t+0.333310;
		double b = (((((-8.166417*t+46.807367)*t-85.946244)*t+66.844005)*t-21.270276)*t+1.417417)*t+0.552708;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 TemperatureMap(double t) {
		double r = ((((((73.052363*t-259.906642)*t+358.531339)*t-234.028643)*t+67.623722)*t-6.328365)*t+1.714365)*t+0.168980;
		double g = (((((((((3471.595719*t-17952.888856)*t+39302.582249)*t-47373.095399)*t+34235.830038)*t-15139.742341)*t+4009.865560)*t-600.251717)*t+45.723531)*t+0.178452)*t+0.307254;
		double b = (((((((((-4121.176737*t+21492.414743)*t-47120.126696)*t+56425.210886)*t-40202.875444)*t+17471.890317)*t-4590.744303)*t+698.277358)*t-55.593666)*t+1.978274)*t+0.914874;
		return vec3(clp(r),clp(g),clp(b));
	}
	static vec3 LightTemperatureMap(double t) {
		double r = (((((((143.024654*t-633.249489)*t+1128.500575)*t-1031.757986)*t+512.777002)*t-137.341435)*t+17.901059)*t+0.816187)*t+0.166621;
		double g = ((0.139935*t-2.973745)*t+2.918569)*t+0.266553;
		double b = (((((((((-1920.760455*t+8574.065847)*t-15680.209301)*t+14984.604404)*t-7835.286028)*t+2072.790092)*t-169.764873)*t-33.390234)*t+7.471559)*t-0.289807)*t+0.937674;
		return vec3(clp(r),clp(g),clp(b));
	}
};
