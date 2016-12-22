#include "brainModule.hpp"


#include "wrdac/subsystems/subSystem_recog.h"


#include "speak.hpp"
#include "brain.hpp"
#include "idle.hpp"
#include "greet.hpp"
#include "make_cocktail.hpp"
#include "entertain.hpp"
#include "feedback.hpp"
#include "goodbye.hpp"
#include "arduinoUtils.hpp"


bool brainModule::configure(yarp::os::ResourceFinder &rf)
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
    // 8 - OPC
    //     (launch: objectsPropertiesCollector --name OPC --no-load-db --no-save-db)


    moduleName = rf.check("name", Value("brainModule")).asString().c_str();
    setName(moduleName.c_str());

    yInfo() << "findFileByName " << rf.findFileByName("brainModule.ini") ;
    cout << moduleName << ": finding configuration files..." << endl;
    period = rf.check("period", Value(0.1)).asDouble();
    robot  = rf.check("robot", Value("icubSim")).asString().c_str();
    arm    = rf.check("arm", Value("both")).asString().c_str();


    //rpc port
    rpcPort.open(("/" + moduleName + "/rpc").c_str());
    attach(rpcPort);

    //port to iCub_SIM (rpc)
    string portToSimName = "/" + moduleName + "/toSim";
    portToSim.open(portToSimName.c_str());


    //try to connect to iCub_SIM world port 3 times and quit otherwise
    unsigned int counter = 0;
    bool isConnected = true;


  /*  while(!isConnected && counter < 3){
        yInfo() << "Trying to connect to the iCub_SIM" ;
        isConnected = Network::connect(portToSimName, "/icubSim/world");
        counter++;
        Time::delay(1.0);
    }
    if(!isConnected){
               yError() << "The port /icubSim/world from iCub_SIM is needed!" ;
               return false;
    }*/
    
    
    bool connected = configureISpeak("brainModule");
    connected = configureSpeechRecognition("brainModule", rf);


	bool arduinoIsConnected = connectToArduino(moduleName);
    
    
    //Create the ICubclient, check client.ini to know which subsystem to load (should be ARE)
    iCub = new ICubClient(moduleName, "brainModule", "client.ini");

    // we connect just to ARE (skip connecting to OPC)
    if (!iCub->connectSubSystems())
    {
        cout<<"ARE seems to be unavailable!"<<endl;
        return false;
    }
    
    // Connect to OPC system
    iCub->connectOPC();
    if (iCub->opc->isConnected()){
        cout << "OPC is correctly connected" << endl;
        
    }

    configureOPC(rf,iCub);
    agentPopTest(iCub);
    m_CocktailMenu = new CocktailMenu();
    
    

    yInfo() << "\n \n" << "----------------------------------------------" << "\n \n" << moduleName << " ready and streaming!!!!! \n \n ";

    return true;
}

bool brainModule::respond(const Bottle& command, Bottle& reply) {
    
    
    string helpMessage = string(getName().c_str()) +
        " commands are: \n" +
        "order action \n" +
        "populate object \n" +
        "help \n" +
        "quit \n" ;

    reply.clear();


    reply.addString("ok");
    rpcPort.reply(reply);

    return true;
}

/* Called periodically every getPeriod() seconds */
bool brainModule::updateModule() {
    

    //chosenCocktail is of type int, is the index to the cocktail
    
    //state = MAKE_COCKTAIL;
    
    if (running) {
        switch (state) {
            case IDLE:
                state = idle(iCub);
                break;
                
            case GREET:
                state = greet(iCub, m_CocktailMenu, chosenCocktail);
                break;
 
            case MAKE_COCKTAIL:
                state = make_cocktail(iCub, m_CocktailMenu, chosenCocktail);
                break;
                
            case ENTERTAIN:
                state = entertain(iCub);
                break;
                
            case FEEDBACK:
                state = feedback(iCub, m_CocktailMenu, chosenCocktail);
                break;
                
                
            case GOODBYE:
                state = goodbye();
                break;
                
            default:
                break;
        }

    }
    
    
    return true;
}


/* interrupt ports when interrupting the module */
bool brainModule::interruptModule() {
    rpcPort.interrupt();
    

    return true;
}

/* close ports when closing the module */
bool brainModule::close() {

    rpcPort.interrupt();
    rpcPort.close();

    return true;
}


