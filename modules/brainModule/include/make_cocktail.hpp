//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef make_cocktail_h
#define make_cocktail_h

#include "speak.hpp"
#include "brainModule.hpp"
#include "cocktailMenu.hpp"
#include "arduinoUtils.hpp"

double timeout = 8.0;
enum MCSTATE {MC_START ,MC_POURING,MC_DONE};

// Text strings
string start_cocktail = "Great, let's start preparing your cocktail!";
string thank_you = "Thank you!";
// Phase: MC_START
string take_glass = "First of all, could you take the glass here on your right?...";
string put_glass = "and put it under the Smartee knee Cocktail Machine?...";
vector<string> help_glass = {"Oh sorry, maybe i wasn't clear! Please, could you help me and put the glass under the Smartee knee Cocktail Machine?" ,
						 	 " Umh...i'm not sure you really want this cocktail...but i'm a good boy, so i will give you one more chance! Come on!...Take the glass and put it under my amazing Smartee knee machine!",
							 "Ok, it seems you don't want to help me!" };

// Phase: MC_POURING



// Objects string (TO INITIALIZE INTO THE OPC TABLE!!!)
string initial_glass_position = "initial_glass_position";
string final_glass_position = "final_glass_position";
// Reference Final Position of the glass
double f_pos_x = -0.63;
double f_pos_y =  0.23;
double f_pos_z = -0.15;
// Tollerance
double delta = 0.1;

// Counter for checking the position

int MAX_ITERATION = 2;
int counter = 0;



// if all objects are in correct position , start making cocktail

std::string bottle = "bottle";
wysiwyd::wrdac::ICubClient  *iCub;

int prepareCocktail(Cocktail chosenCocktail);
bool isTargetInPosition(string target);
bool isPresent(string target);
void populateTable(wysiwyd::wrdac::ICubClient  *iCub);
void agentPopTest(wysiwyd::wrdac::ICubClient  *iCub);
void subPopulateObjects(wysiwyd::wrdac::ICubClient  *iCub, Bottle* objectList, bool addOrRetrieve, Bottle *x, Bottle *y, Bottle *z, Bottle* present);
void configureOPC(yarp::os::ResourceFinder &rf, wysiwyd::wrdac::ICubClient  *iCub);
bool isPartnerPresent(wysiwyd::wrdac::ICubClient  *iCub_in);


int prepareCocktail(Cocktail currCocktail){
    
    // To Arduino
    Bottle bBottle;
	Bottle bReply;
    
    // Cocktail
    int nIngredients = currCocktail.getNumberOfIngredients();
    vector<ingredient> ingredients = currCocktail.getIngredients();
    
    string cmdToArduino = "cocktail";
    
    iCub->lookAtPartner();
    
    for (int i = 0; i < nIngredients; i++){
        
		bBottle.clear();
		bReply.clear();

        string ingredientName = ingredients[i].name;
        string ingredientQuantityAsString = to_string(ingredients[i].quantity);
        
        say("Ok, now we add " + ingredientQuantityAsString + " milliliter of " + ingredientName + ".");
        cout << "Now, we add " + ingredientQuantityAsString + " milliliter of " + ingredientName + "." << endl;
        

		iCub->look("final_pos");

        string ss = to_string(ingredients[i].index) + " " + to_string(ingredients[i].quantity);
        bBottle.addString(cmdToArduino);
		bBottle.addString(ss);

		//Write to Arduino
		portToArduino.write(bBottle,bReply);
		yInfo() << "Arduino says: " + bReply.get(0).asString();
		Time::delay(cocktail_ml[ingredients[i].quantity]/1000.0);

		cout << ss << endl;
		Time::delay(2);

		iCub->lookAtPartner();
        Time::delay(2);
        
    }
    
	iCub->home();
    // Toppings
    string toppingName = currCocktail.m_topping;
    say("Ok, your cocktail is almost ready! You only need to add " + toppingName + " in your drink!");
	Time::delay(3);
    cout << "Ok, your cocktail is almost ready! You only need to add " + toppingName + " in your drink!";
    say("You can find it there!");
	iCub->point(toppingName);
	Time::delay(2);
	iCub->home();
	Time::delay(2);
    
    
    return MC_DONE;
}


bool isTargetInPosition(string target){
    
    
    iCub->home();
    	
	//iCub->lookAtPartner();
	Time::delay(3);		// delay

	say(take_glass);
    iCub->point("start_pos");
	//Time::delay(2);           // TO REMOVEE!!	
	iCub->home();
	say(put_glass);
    iCub->point("final_pos");
	Time::delay(1);
	iCub->home();
    
	while (!isPresent(target) && counter <= MAX_ITERATION){
		yInfo() << "the object is not present!\n";
		
		
		iCub->lookAtPartner();
		Time::delay(1);			// delay
		say(help_glass[counter]);
		Time::delay(1);
		iCub->home();
		Time::delay(1);
	
        if (counter == MAX_ITERATION){
			counter = 0;
            return false;
        }
		counter++;
    }

	yInfo() << "Object FOUND!!! \n";
	counter = 0;
    return true;
}



bool isPresent(string target) {
    
    bool isInPosition = false;
	wysiwyd::wrdac::Object* o;
	// retrieve the object
	
    
    double start = yarp::os::Time::now();  //timer for going out the the loop if nothing happens  after X second. you can use it to  have another loop that will say something to the humanand going back in
    
	while (!isInPosition && start + timeout > yarp::os::Time::now()){ //while loop, go out as soon as you detect the object not here or after 8 sec
        //ask to retrieve the current state of the opc
        iCub->opc->checkout();		
        
		o = iCub->opc->addOrRetrieveEntity<Object>(target);
		if (o->m_ego_position[0] < (f_pos_x + delta) && o->m_ego_position[0] > (f_pos_x - delta) &&
			o->m_ego_position[1] < (f_pos_y + delta) && o->m_ego_position[1] > (f_pos_y - delta) &&
			o->m_ego_position[2] < (f_pos_z + delta) && o->m_ego_position[2] > (f_pos_z - delta) &&
			                          o->m_present == 1.0){  // check the position of the object, not only if it is present!!!! TO DOO!!!!!!
			isInPosition = true;
           // cout << target<< " object is present" << endl;
			return isInPosition;
        }


    }
    
    //cout << target << " object is not present" << endl;
	return isInPosition;
    
}

bool isPartnerPresent(wysiwyd::wrdac::ICubClient  *iCub_in){

	unsigned int cnt = 0;
	int MAX_ITERATION = 3;
	iCub->opc->checkout();
	Agent* ag = dynamic_cast<Agent*>(iCub->opc->getEntity("partner"));

	//yInfo() << "Checkout done!";

	while (ag->m_present == 0.0 && cnt <= MAX_ITERATION){

		iCub->opc->checkout();
		ag = dynamic_cast<Agent*>(iCub->opc->getEntity("partner"));
		Time::delay(2.0);
		yInfo() << "Waiting for an agent........";
		if (counter == MAX_ITERATION){
			say("Are you still here? i can't see you...");
			return false;
		}

	}
	return true;
}


int make_cocktail(wysiwyd::wrdac::ICubClient  *iCub_in, CocktailMenu *menu, int chosenCocktailIndex){
    
    Cocktail chosenCocktail = menu->m_cocktailList[chosenCocktailIndex];
    
    iCub = iCub_in;
    
    bool cocktailDone = false;
    int MC_state = MC_START;
    
    say(start_cocktail);
    cout << "Start preparing cocktail" << endl;
    while (!cocktailDone){
        // target must be the same string in the OPC table
        string target = "glass"; ///IMPORTANT!!! change to final_glass
        
        switch (MC_state){
            case MC_START:
				
                if (!isTargetInPosition(target)){  // wait for glass to be in position 
                   // iCub->home();
                    return GOODBYE;
                }
                else
                    say(thank_you);
					
                MC_state = MC_POURING;
                break;
            case MC_POURING:
                
                MC_state = prepareCocktail(chosenCocktail);
                
                break;
                
            case MC_DONE:
				if (isPartnerPresent(iCub)){

					say("Your cocktail is done! Let's taste it!");
					//iCub->lookAtPartner();
				}

                cocktailDone = true;
                break;
                
            default:
                cocktailDone = true;
                break;
        }
    }
    cout << "Made Cocktail" << endl;
    return ENTERTAIN;
}

void configureOPC(yarp::os::ResourceFinder &rf, wysiwyd::wrdac::ICubClient  *iCub)
{
	//Populate the OPC if required
	yDebug() << "Populating OPC...";

	//1. Populate AddOrRetrieve part
	Bottle grpOPC_AOR = rf.findGroup("OPC_AddOrRetrieve");


	bool shouldPopulate_AOR = grpOPC_AOR.find("populateOPC").asInt() == 1;


	if (shouldPopulate_AOR)
	{
		Bottle *objectList = grpOPC_AOR.find("objectName").asList();
		Bottle *X_pos = grpOPC_AOR.find("X").asList();
		Bottle *Y_pos = grpOPC_AOR.find("Y").asList();
		Bottle *Z_pos = grpOPC_AOR.find("Z").asList();
		Bottle *present = grpOPC_AOR.find("isPresent").asList();
		subPopulateObjects(iCub, objectList, true, X_pos, Y_pos, Z_pos, present);


	}


	yDebug() << "configureOPC done";
}

void subPopulateObjects(wysiwyd::wrdac::ICubClient  *iCub, Bottle* objectList, bool addOrRetrieve, Bottle *x, Bottle *y, Bottle *z, Bottle* present) {
	iCub->opc->clear();
	if (objectList)
	{
		for (int d = 0; d < objectList->size(); d++)
		{
			std::string name = objectList->get(d).asString().c_str();
			double ispresent = present->get(d).asDouble();
			double x_new = x->get(d).asDouble();
			double y_new = y->get(d).asDouble();
			double z_new = z->get(d).asDouble();
			wysiwyd::wrdac::Object* o;
			if (addOrRetrieve) {
				o = iCub->opc->addOrRetrieveEntity<Object>(name);
				o->m_ego_position[0] = x_new;
				o->m_ego_position[1] = y_new;
				o->m_ego_position[2] = z_new;
				o->m_present = ispresent;
				o->m_color[0] = Random::uniform(100, 180);
				o->m_color[1] = Random::uniform(0, 80);
				o->m_color[2] = Random::uniform(180, 250);

			}
			else {
				o = iCub->opc->addEntity<Object>(name);
			}
			yInfo() << " [configureOPC] object " << o->name() << "added";


			iCub->opc->commit(o);

		}
	}
}
/*
void populateTable(wysiwyd::wrdac::ICubClient  *iCub){

	iCub->opc->clear();

	Object* obj1 = iCub->opc->addOrRetrieveEntity<Object>("start_glass");
	obj1->m_ego_position[0] = -0.5;
	obj1->m_ego_position[1] = 1;
	obj1->m_ego_position[2] = 0.2;
	obj1->m_present = 1.0;
	obj1->m_color[0] = Random::uniform(0, 80);
	obj1->m_color[1] = Random::uniform(80, 180);
	obj1->m_color[2] = Random::uniform(180, 250);
	iCub->opc->commit(obj1);

	Object* obj2 = iCub->opc->addOrRetrieveEntity<Object>("orange");
	obj2->m_ego_position[0] = -0.5;
	obj2->m_ego_position[1] = 0.5;
	obj2->m_ego_position[2] = 0.2;
	obj2->m_present = 1.0;
	obj2->m_color[0] = Random::uniform(0, 180);
	obj2->m_color[1] = Random::uniform(0, 80);
	obj2->m_color[2] = Random::uniform(180, 250);
	iCub->opc->commit(obj2);

	Object* obj3 = iCub->opc->addOrRetrieveEntity<Object>("lime");
	obj3->m_ego_position[0] = -0.5;
	obj3->m_ego_position[1] = 0.0;
	obj3->m_ego_position[2] = 0.2;
	obj3->m_present = 1.0;
	obj3->m_color[0] = Random::uniform(100, 180);
	obj3->m_color[1] = Random::uniform(80, 180);
	obj3->m_color[2] = Random::uniform(0, 80);
	iCub->opc->commit(obj3);


	Object* obj4 = iCub->opc->addOrRetrieveEntity<Object>("strawberry");
	obj4->m_ego_position[0] = -0.5;
	obj4->m_ego_position[1] = -0.5;
	obj4->m_ego_position[2] = 0.2;
	obj4->m_present = 1.0;
	obj4->m_color[0] = Random::uniform(100, 180);
	obj4->m_color[1] = Random::uniform(0, 80);
	obj4->m_color[2] = Random::uniform(180, 250);
	iCub->opc->commit(obj4);

	Object* obj5 = iCub->opc->addOrRetrieveEntity<Object>("final_glass");
	obj5->m_ego_position[0] = -0.5;
	obj5->m_ego_position[1] = -1;
	obj5->m_ego_position[2] = 0.2;
	obj5->m_present = 0.0;
	obj5->m_color[0] = Random::uniform(100, 180);
	obj5->m_color[1] = Random::uniform(0, 80);
	obj5->m_color[2] = Random::uniform(180, 250);
	iCub->opc->commit(obj5);

	cout << "Population done!" << endl;

}*/

void agentPopTest(wysiwyd::wrdac::ICubClient  *iCub){


	Agent* agent = iCub->opc->addOrRetrieveEntity<Agent>("partner");
	agent->m_ego_position[0] = (-1.5) * (Random::uniform()) - 0.5;
	agent->m_ego_position[1] = (2) * (Random::uniform()) - 1;
	agent->m_ego_position[2] = 0.60;
	agent->m_present = 0.0; //rimetti a zero!!!!!!
	agent->m_color[0] = Random::uniform(0, 80);
	agent->m_color[1] = Random::uniform(180, 250);
	agent->m_color[2] = Random::uniform(80, 180);
	iCub->opc->commit(agent);
}




#endif /* make_cocktail_h */
