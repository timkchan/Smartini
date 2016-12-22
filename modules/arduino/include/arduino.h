#include <string>
#include <yarp/math/Math.h>
#include <yarp/math/SVD.h>
#include "wrdac/clients/icubClient.h"
#include "SerialClass.h"

class arduino : public yarp::os::RFModule {
private:

    //conf options
    double      period;

	std::string robot, arm, moduleName, comPort;

    yarp::os::Port   rpcPort;
    yarp::os::Port   portToSim;

    //Global variable for predetermined objects. Could be loaded by .ini file if you want

		// Serial Port to Arduino;
		Serial *sp;

    //ICubClient to use ARE
    wysiwyd::wrdac::ICubClient  *iCub;


public:
    bool configure(yarp::os::ResourceFinder &rf);

    bool interruptModule();

    bool close();

    double getPeriod()
    {
        return period;
    }

    bool updateModule();

    //RPC & scenarios
    bool respond(const yarp::os::Bottle& cmd, yarp::os::Bottle& reply);
};
