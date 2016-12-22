//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef feedback_h
#define feedback_h

using namespace std;

//#include "brain.hpp"
#include "speak.hpp"
#include "cocktailMenu.hpp"
#include <fstream>

enum FBState {FB_START, FB_MAKE_BETTER, FB_ASK_FEEDBACK, FB_FEEDBACK, FB_GOODBYE, FB_DONE};


string initial_greeting = "Do you like it? How would you rate your drink from 1 to 10?";
string make_better = "Could I have made it better?";
string ask_feedback = "Would you like to help me improve the cocktail? If you say yes, I will just ask you a few questions";
string could_not_understand = "Sorry something went wrong, and I could not quite understand what you said.";
string str_goodbye = "Thank you so much! I hope you had fun and you enjoyed your drink. See you again soon! Bye bye!";


bool listenAskIngredient(wysiwyd::wrdac::ICubClient  *iCub, int &adjust);


//used to understand a vote from 1 to 10 from the user
bool listenRateCoctail(wysiwyd::wrdac::ICubClient  *iCub, int &vote){
    
    
    
    int timeout = 3;
    
    int reply = UNDEFINED; //can be 0 for undefined, 1 to 10 for the vote
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenNumber(reply, iCub))
            return false;
        
        if (reply != 0) {  //-1: undefined
            vote = reply;
            return true;
        }
        
    }
    
    return false;    
}

int fb_start(wysiwyd::wrdac::ICubClient  *iCub,int cocktailIndex){
    
    cerr << "Starting feedback " << endl;

    
    bool listenOk;   //checks if listening action was carried over correctly
    
    say(initial_greeting);
    Time::delay(4);
    
    int vote = 1;
    listenOk = listenRateCoctail(iCub, vote);
    if (!listenOk) {
        //say(could_not_understand);
        return FB_GOODBYE;
    }
    
    
    //save vote somewhere
	writeVote(cocktailIndex, vote);
    
    if (vote > 0 && vote < 6) {
        return FB_MAKE_BETTER;
    }
    
    if (vote > 5 && vote < 11) {
        return FB_ASK_FEEDBACK;
    }
    
    say(could_not_understand);
    return FB_GOODBYE;
    
}

bool listenMakeBetter(wysiwyd::wrdac::ICubClient  *iCub, bool &canMakeBetter){
    
    int timeout = 3;
    
    int reply = UNDEFINED; //can be UNDEFINED, YES or NO
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenYesNo(reply, iCub))
            return false;
        
        if (reply == YES) {
            canMakeBetter = true;
            return true;
        }
        
        if (reply == NO) {
            canMakeBetter = false;
            return true;
        }
    }
    
    return false;
    
    
}

int fb_make_better(wysiwyd::wrdac::ICubClient  *iCub){
    
    cerr << "Asking if we can make better cocktail " << endl;
    
    bool listenOk;   //checks if listening action was carried over correctly
    bool canMakeBetter = false;
    
    say(make_better);
	Time::delay(1);
    
    listenOk = listenMakeBetter(iCub, canMakeBetter);
    if (!listenOk) {
        say(could_not_understand);
        return FB_GOODBYE;
    }

    if (canMakeBetter) {
        return FB_ASK_FEEDBACK;
    }
    
    return FB_GOODBYE;
}


bool listenAskFeedback(wysiwyd::wrdac::ICubClient  *iCub, bool &wantsToHelp){
    
    int timeout = 3;
    
    int reply = UNDEFINED; //can be UNDEFINED, YES or NO
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenYesNo(reply, iCub))
            return false;
        
        if (reply == YES) {
            wantsToHelp = true;
            return true;
        }
        
        if (reply == NO) {
            wantsToHelp = false;
            return true;
        }
    }
    
    return false;
    
}



int fb_ask_feedback(wysiwyd::wrdac::ICubClient  *iCub){
    
    cerr << "Asking if user wants to provide feedback " << endl;
    
    bool listenOk;   //checks if listening action was carried over correctly
    bool wantsToHelp = false;
    
    say(ask_feedback);
	Time::delay(6);
    
    listenOk = listenAskFeedback(iCub, wantsToHelp);
    if (!listenOk) {
        say(could_not_understand);
        return FB_GOODBYE;
    }
    
    
    if (wantsToHelp) {
        return FB_FEEDBACK;
    }
    else
        return FB_GOODBYE;
    
}


//called when user does not want to give feedback
int fb_goodbye(){
    
    
    string thank_anyway = "Thank you anyway!";
    
    say(thank_anyway);
    
    cerr << "Saying Goodbye " << endl;
    
    
    say(str_goodbye);
    
    return FB_DONE;
    
}

/*
bool listenAskIngredient(wysiwyd::wrdac::ICubClient  *iCub, int &adjust){
    
    
    
    //sets +1 if needs to increase
    //sets -1 if needs to decrease
    //sets 0 if the ingredient was ok
    adjust = 0;
    
    
    int timeout = 3;
    
    int reply = 2; //can be 2 for undefined, -1 to decrease, +1 to increase, 0 to leave
    
    for (int i = 0; i < timeout; i++) {
        
        if(!listenIngredient(reply, iCub))
            return false;
        
        if (reply != 2) {  //2: undefined
            adjust = reply;
            return true;
        }
        
    }
    
    return false;
    
}*/

int fb_feedback(wysiwyd::wrdac::ICubClient  *iCub, CocktailMenu *menu, int &cocktailIndex){
    
    
    bool listenOk;      //checks if listening action was carried over correctly
    
    //used to adjust the quantity of any ingredient
    //can be -1, 0 or 1 to decrease, keep or increase
    //an ingredient's quantity
    int adjust;
    
    
    cerr << "Starting to gather feedback for coctail " << cocktailIndex << endl;
    
    Cocktail *currCocktail;
    //CocktailMenu *menu = new CocktailMenu();
    currCocktail = menu->getCocktail(cocktailIndex);
    
    
    vector<ingredient> ingredients = currCocktail->m_ingredients;
    
    
    string ingredient_question_begin = "Ok, tell me about the ";
    string ingredient_question_end = ". Did I put too much? too little? or was it just fine?";
    
    for (int i = 0; i < ingredients.size(); i++) {
        
        string currIngredient = ingredients[i].name;
        string question = ingredient_question_begin + currIngredient + ingredient_question_end;
        
        say(question);
		Time::delay(5);
        
        listenOk = listenAskIngredient(iCub, adjust);
        if (!listenOk) {
            say(could_not_understand);
            return FB_GOODBYE;
        }
        
        //Adjust ingredient if user said it was not just fine
        if (adjust != 0) {
            menu->adjustIngredient(cocktailIndex, i, adjust);
        }
        
        
    }

    
    
    
    string thank_for_feedback = "I believe that was all the ingredients! Thank you very much for your feedback, it will surely help me make a better cocktail next time! ";
    
    say(thank_for_feedback);


	// saving new cocktail quantities

	writeQuantitiesOnFile(currCocktail, cocktailIndex);

    
    return FB_DONE;
    
    
    
}

bool listenAskIngredient(wysiwyd::wrdac::ICubClient  *iCub, int &adjust){


	//sets +1 if needs to increase
	//sets -1 if needs to decrease
	//sets 0 if the ingredient was ok
	adjust = 0;


	int timeout = 3;

	int reply = 2; //can be 2 for undefined, -1 to decrease, +1 to increase, 0 to leave

	for (int i = 0; i < timeout; i++) {

		if (!listenIngredient(reply, iCub))
			return false;

		if (reply != 2) {  //2: undefined
			adjust = reply;
			return true;
		}

	}

	return false;

}

int feedback(wysiwyd::wrdac::ICubClient  *iCub, CocktailMenu *menu, int cocktailIndex){
    
    int state = FB_START;
    bool done = false;
    
    while (!done) {
        
        switch (state) {
            case FB_START:
                state = fb_start(iCub,cocktailIndex);
                break;
                
            case FB_MAKE_BETTER:
                state = fb_make_better(iCub);
                break;
                
            case FB_ASK_FEEDBACK:
                state = fb_ask_feedback(iCub);
                break;
                
            case FB_FEEDBACK:
                state = fb_feedback(iCub, menu, cocktailIndex);
                break;
                
            case FB_GOODBYE:
                state = fb_goodbye();
                break;
                
            case FB_DONE:
                done = true;
                break;
                
            default:
                done = true;
                break;
        }
        
    }
    
    
    
    cout << "Feedback Gathered" << endl;
    return GOODBYE;
}

#endif /* feedback_h */
