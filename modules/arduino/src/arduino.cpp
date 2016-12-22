/*
 * Copyright (C) 2015 WYSIWYD Consortium, European Commission FP7 Project ICT-612139
 * Authors:  Maxime Petit
 * email:  m.petit@imperial.ac.uk
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * wysiwyd/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#include "arduino.h"


using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace wysiwyd::wrdac;


bool arduino::configure(yarp::os::ResourceFinder &rf)
{


    // open a client interface to connect to the cartesian server of the simulator
    // we suppose that:
    //
    // 1 - the iCub simulator is running
    //     (launch: iCub_SIM)
    //
    // 2 - the cartesian server is running
    //     (launch: yarprobotinterface --context simCartesianControl)
    //
    // 3 - the cartesian solver for the left arm is running too
    //     (launch: iKinCartesianSolver --context simCartesianControl --part left_arm)
    //
    // 4 - the cartesian solver for the left arm is running too
    //     (launch: iKinCartesianSolver --context simCartesianControl --part right_arm)
    //
    // 5 - the gazecontrol
    //     (launch: iKinGazeCtrl --from configSim.ini)
    //
    // 5 - wholeBodyDynamics (dependency of ARE)
    //     (launch: wholeBodyDynamics --robot icubSim --autoconnect --dummy_ft --no_legs)
    //
    // 5 - ActionsRenderingEngine
    //     (launch: actionsRenderingEngine --from sim/config.ini)

    moduleName = rf.check("name", Value("arduino")).asString().c_str();
    setName(moduleName.c_str());

    yInfo() << "findFileByName " << rf.findFileByName("arduino.ini") ;
    cout << moduleName << ": finding configuration files..." << endl;
	period = 2.0;
	comPort = rf.check("comport", Value("\\\\.\\COM12")).asString().c_str();

	printf("COM_PORT setted: %s.\n", comPort);



	// connecting to the SerialPort
	unsigned int counter = 1;
	bool isArduinoConnected = 0;
	sp = new Serial(comPort.c_str());

	while (!isArduinoConnected && counter <= 3){
		yInfo() << "Trying connecting to Arduino.... \n";
		isArduinoConnected = sp->IsConnected();

	}
	if (isArduinoConnected){
		yInfo() << "Serial Port STATUS: CONNECTED to " << comPort << ". \n";
	}
	else{
		yError()  << "Serial Port STATUS: NOT CONNECTED. ";
		return false;
	}

	//rpc port
	rpcPort.open(("/" + moduleName + "/rpc").c_str());
	if (attach(rpcPort))
		yInfo() << "Arduino port correctly attached \n";


	yInfo() << "\n \n" << "----------------------------------------------" << "\n \n" << moduleName << " ready and streaming!!!!! \n \n ";

    return true;
}

bool arduino::respond(const Bottle& command, Bottle& reply) {
    string helpMessage = string(getName().c_str()) +
        " commands are: \n" +
        " cocktail pump_number quantity_in_ml \n"+
        "quit \n" ;

    reply.clear();

	string cmd = command.get(0).asString();
	string target = "";

	// COCKTAIL 
	if (cmd == "cocktail"){
		
		yInfo() << "We are doing cocktail";
		char *act = &target[0u];
		if (command.size() == 2){

			if (sp->IsConnected()){

				reply.addString("ACK");
				target = command.get(1).asString();
				yInfo() << "Sending: " << target<<"\n";
				
				if(sp->WriteData(act, sizeof(target)))
					cout << "Sent to Arduino: " << act << " \n";

			}
			else{
				yError() << "Serial port STATUS: DISCONNECTED." << " \n";
			}

		}
		else{

			reply.addString("NACK");
		}
	}
	// QUIT
	else if (command.get(0).asString() == "quit") {
		reply.addString("quitting");
		rpcPort.reply(reply);
		return false;
	}
	// DEFAULT
	else {
		cout << helpMessage;
		reply.addString("ok");
	}

    rpcPort.reply(reply);

    return true;
}


/* Called periodically every getPeriod() seconds */
bool arduino::updateModule() {
	/*
	char ansFromArduino[80];
	if (sp->IsConnected()){

		int nByte = sp->ReadData(ansFromArduino, 80);
		if (nByte > 0)
		{ }
			//yInfo() << "Sent from Arduino: " << ansFromArduino << " \n";
		else
			yInfo() << "Nothing to read";
	}*/

    return true;
}

/* interrupt ports when interrupting the module */
bool arduino::interruptModule() {
    rpcPort.interrupt();


    return true;
}

/* close ports when closing the module */
bool arduino::close() {

    rpcPort.interrupt();
    rpcPort.close();



    return true;
}
