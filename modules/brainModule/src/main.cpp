#include "brainModule.hpp"

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

    brainModule mod;
    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("brainModule");
    rf.setDefaultConfigFile("brainModule.ini");
    rf.configure(argc, argv);
    return mod.runModule(rf);
}
