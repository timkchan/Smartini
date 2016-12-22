//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef brain_h
#define brain_h

#include "cocktailMenu.hpp"

enum State {IDLE, GREET, MAKE_COCKTAIL, ENTERTAIN, FEEDBACK, GOODBYE};

CocktailMenu *m_CocktailMenu;

int state = IDLE;  //start in IDLE
int chosenCocktail = 0; //DEFAULT COCKTAIL
bool running = true;

bool arduinoConnected = false;

#endif /* brain_h */
