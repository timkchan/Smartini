//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef greet_h
#define greet_h

//#include "brain.hpp"
#include "speak.hpp"
#include "cocktailMenu.hpp"

using namespace std;


//--------- FOR GREET -----------
bool listenCocktailYesNo(wysiwyd::wrdac::ICubClient  *iCub, bool &wantsCocktail){
    //implement waiting for cocktail decision
    
    int timeout = 3;
    
    int reply = UNDEFINED; //can be UNDEFINED, YES or NO
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenYesNo(reply, iCub))
            return false;
        
        if (reply == YES) {
            wantsCocktail = true;
            return true;
        }
        
        if (reply == NO) {
            wantsCocktail = false;
            return true;
        }
    }
    
    return false;
    
}

bool listenCocktailChoice(wysiwyd::wrdac::ICubClient  *iCub, int &choice){
    //implement waiting for picked cocktail amond list
    
    
    int timeout = 3;
    
    int reply = -1; //can be -1 for undefined, or 0 to 4 for the cocktail
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenCocktail(reply, iCub))
            return false;
        
        if (reply != -1) {  //-1: undefined
            choice = reply;
            return true;
        }

    }
    
    return false;
    
}

bool listenCocktailConfirmation(wysiwyd::wrdac::ICubClient  *iCub, bool &confirmed){
    //implement waiting for user to confirm we understood the right cocktail
    
    int timeout = 3;
    
    int reply = UNDEFINED; //can be UNDEFINED, YES or NO
    
    for (int i = 0; i < timeout; i++) {
        
        
        if(!listenYesNo(reply, iCub)){
            cerr << "Aborting" << endl;
            return false;
        }
        
        
        
        if (reply == YES) {
            cerr << "Replied YES" << endl;
            confirmed = true;
            return true;
        }
        
        if (reply == NO) {
            cerr << "Replied NO" << endl;
            confirmed = false;
            return true;
        }
    }
    
    return false;
    
}

int greet(wysiwyd::wrdac::ICubClient  *iCub, CocktailMenu *menu, int &cocktail){
    
    //bool connected = configureISpeak("greet");
    
    string initial_greeting = "Hello there! I am a bartender robot, would you like me to make you a cocktail?";
    string doesnt_want_cocktail = "That's ok, come back some other time, my cocktails are very good";
    string which_cocktail_question = "Ok, which cocktail would you like? You can choose the one you prefer from the Cocktail Menu on your right";
    
    bool wantsCocktail;
    bool listenOk;   //checks if listening action was carried over correctly
    
    //used to be here, now passed as argument
    //CocktailMenu *menu = new CocktailMenu();
    
    iCub->lookAtPartner();
    say(initial_greeting);
    Time::delay(5);
    
    listenOk = listenCocktailYesNo(iCub, wantsCocktail);
    if (!listenOk) {
        return IDLE; //if something went wrong, go back to IDLE
    }
    
    if (wantsCocktail) {
        
        bool confirmed = false;
        
        //find out which cocktail and confirm you understood correctly
        while (!confirmed){
            
            //ask which cocktail
            iCub->lookAtPartner();
            say(which_cocktail_question);
            //Time::delay(0.5);
			iCub->point("start_pos");
			Time::delay(0.5);
			iCub->home();

            //wait for answer, understand which cocktail
            listenOk = listenCocktailChoice(iCub, cocktail);
            if (!listenOk) {
                return IDLE; //if something went wrong, go back to IDLE
            }
            
            string confirm_question = "Did you say you wanted a " + menu->m_cocktailList[cocktail].m_cocktailName + "?";
            iCub->lookAtPartner();
            say(confirm_question);
            Time::delay(2);
            
            listenOk = listenCocktailConfirmation(iCub, confirmed);
            if (!listenOk) {
                return IDLE; //if something went wrong, go back to IDLE
            }
            
        }
        
        return MAKE_COCKTAIL; //start making cocktail
    }
    else{ //doesnt want a cocktail, say bye and return to idle
        iCub->lookAtPartner();
        say(doesnt_want_cocktail);
        Time::delay(15);
        return IDLE;
    }
    
    cout << "I greeted" << endl;
    return MAKE_COCKTAIL;
    
}



#endif /* greet_h */
