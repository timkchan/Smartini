#include "arduino.h"

using namespace std;
using namespace yarp::os;

int main(int argc, char * argv[])
{
    yarp::os::Network yarp;
    if (!yarp.checkNetwork())
    {
        yError()<<"YARP network seems unavailable!";
        return 1;
    }

	arduino mod;
    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("arduino");
    rf.setDefaultConfigFile("arduino.ini");
    rf.configure(argc, argv);
    return mod.runModule(rf);
}
