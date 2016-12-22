
#ifndef brainModule_h
#define brainModule_h

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


class brainModule : public yarp::os::RFModule {
private:

    //conf options
    double      period;

    std::string robot, arm, moduleName;

    yarp::os::Port   rpcPort;
    yarp::os::Port   portToSim;
    yarp::os::Port   portToISpeak;

    //call ARE to do an action
    yarp::os::Bottle myAction(std::string action);
    
    //call speech to speak
    yarp::os::Bottle mySpeechSynthesis(std::string text);

    //send a rpc command to the simulator to add an object
    yarp::os::Bottle myPopulate(std::string action);

    //Global variable for predetermined objects. Could be loaded by .ini file if you want

        //vector of the location of the objects created in myPopulate, iCub reference frame;
        yarp::sig::Vector x;

        //radius for cyl/sph;
        double radius;

        //length for cyl;
        double length;

        //dimension for the box (x/y/z in iCub_SIM frame!);
        double box_x, box_y, box_z;

    //ICubClient to use ARE
    wysiwyd::wrdac::ICubClient  *iCub;
    
    
public:
    


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


#endif /* brainModule_h */
