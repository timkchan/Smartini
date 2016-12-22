//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef arduinoUtils_h
#define arduinoUtils_h

#include <string>
#include <yarp/math/Math.h>
#include <yarp/math/SVD.h>
#include "wrdac/clients/icubClient.h"


using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace wysiwyd::wrdac;

yarp::os::Port   portToArduino;
bool isArduinoConnected = false;
int cocktail_ml[] = { 0, 84, 168, 252, 336, 420, 504, 588, 672, 756, 840, 924, 1008, 1092, 1176, 1260, 1344, 1428, 1512, 1596, 1680, 1764, 1848, 1932, 2016, 2100, 2156, 2212, 2268, 2324, 2380, 2436, 2492, 2548, 2604, 2660, 2716, 2772, 2828, 2884, 2940, 2996, 3052, 3108, 3164, 3220, 3276, 3332, 3388, 3448, 3500, 3552, 3604, 3656, 3708, 3760, 3812, 3864, 3916, 3968, 4020, 4072, 4124, 4176, 4228, 4280, 4332, 4384, 4436, 4488, 4540, 4592, 4644, 4696, 4748, 4800, 4868, 4936, 5004, 5072, 5140, 5208, 5276, 5344, 5412, 5480, 5548, 5616, 5684, 5752, 5820, 5888, 5956, 6024, 6092, 6160, 6228, 6296, 6364, 6432, 6500, 6568, 6636, 6704, 6772, 6840, 6908, 6976, 7044, 7112, 7180, 7248, 7316, 7384, 7452, 7520, 7588, 7656, 7724, 7792, 7860, 7928, 7996, 8064, 8132, 8200, 8268, 8336, 8404, 8472, 8540, 8608, 8676, 8744, 8812, 8880, 8948, 9016, 9084, 9152, 9220, 9288, 9356, 9424, 9492, 9560, 9628, 9696, 9764, 9832, 9900, 9968, 10036, 10104, 10172, 10240, 10308, 10376, 10444, 10512, 10580, 10648, 10716, 10784, 10852, 10920, 10988, 11056, 11124, 11192, 11260, 11328, 11396, 11464, 11532, 11600, 11668, 11736, 11804, 11872, 11940, 12008, 12076, 12144, 12212, 12280, 12348, 12416, 12484, 12552, 12620, 12688, 12756, 12824, 12892, 12960, 13028, 13096, 13164, 13232, 13300
};


bool connectToArduino(string moduleName){

	unsigned int counter = 0;
	

	//port to iCub_SIM (rpc)
	string portToArduinoName = "/" + moduleName + "/toArduino/rpc";
	portToArduino.open(portToArduinoName.c_str());


	//try to connect to arduinoModule port 3 times and quit otherwise

	while (!isArduinoConnected && counter < 3){
		yInfo() << "Trying to connect to the Arduino RPC Module... \n";
		isArduinoConnected = Network::connect(portToArduinoName, "/arduino/rpc");
		counter++;
		Time::delay(1.0);
	}
	if (!isArduinoConnected){
		yError() << "The port /arduino/rpc form Arduino RPC Module is needed! \n";
		return false;
	}

	return true;

}





#endif /* arduinoUtils_h */
