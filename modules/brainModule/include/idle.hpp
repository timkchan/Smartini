//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef idle_h
#define idle_h

#include "brain.hpp"
#include "brainModule.hpp"

using namespace std;

int idle(wysiwyd::wrdac::ICubClient  *iCub){
    
    //iCub->home();
    iCub->opc->checkout();
    Agent* ag = dynamic_cast<Agent*>(iCub->opc->getEntity("partner"));
    
    yInfo() << "Checkout done!";
    
    while (ag->m_present == 0.0){
        
        iCub->opc->checkout();
        ag = dynamic_cast<Agent*>(iCub->opc->getEntity("partner"));
        Time::delay(1.0);
        yInfo() << "Waiting for an agent........";
        
    }
    
    //iCub->lookAtPartner();
    cout << "Somebody Arrived" << endl;
    return GREET;
}


#endif /* idle_h */
