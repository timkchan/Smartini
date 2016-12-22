//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef cocktailMenu_h
#define cocktailMenu_h

#include <vector>
#include <string>
#include "cocktail.hpp"
#include "fileUtils.hpp"


class CocktailMenu {
    
private:
    
public:
    
    vector<string> m_ingredientNames;	//= {"orange juice", "pinapple juice", "apple juice", "pear juice", "ginger", "carrot juice"};
    vector<string> m_toppingNames;		// = { "orange", "lime", "apple" };

    int m_numCocktails;
    vector<Cocktail> m_cocktailList;
    
    CocktailMenu();
    
//    Cocktail getCocktail(string name);
//    Cocktail getCocktail(int cocktailIndex);
    Cocktail addCocktail(string name, vector<int> &ingredientIndices, vector<int> &ingredientQuantities, string topping);
    Cocktail* getCocktail(int cocktailIndex);
    
    void adjustIngredient(int cocktailIndex, int ingredientIndex, int amount);

};

CocktailMenu::CocktailMenu(){
    
    m_numCocktails = 5;
    m_ingredientNames = { "orange juice", "pinapple juice", "apple juice", "pear juice", "ginger", "carrot juice" };
    m_toppingNames = { "orange", "lime", "apple" };
    
    vector<int> ingredients1 = {0, 2, 4, 5};    //orange, apple, ginger, carrot
    vector<int> ingredients2 = {2, 3};          //apple, pear
    vector<int> ingredients3 = {1, 2, 5};       //pinapple, apple, carrot
    vector<int> ingredients4 = {0, 1, 4};       //orange, pinapple, ginger
    vector<int> ingredients5 = {3, 4};          //pear, ginger
    
   /* vector<int> quantities1 = {70, 70, 20, 40};     //orange, apple, ginger, carrot
    vector<int> quantities2 = {100, 100};              //apple, pear
    vector<int> quantities3 = {60, 100, 40};         //pinapple, apple, carrot
    vector<int> quantities4 = {110, 70, 20};          //orange, pinapple, ginger
    vector<int> quantities5 = {160, 40};               //pear, ginger
	*/



	vector<int> quantities1 = readFromFile(0);// = { 70, 70, 20, 40 };     //orange, apple, ginger, carrot
	vector<int> quantities2 = readFromFile(1);// = {100, 100};              //apple, pear
	vector<int> quantities3 = readFromFile(2);// = {60, 100, 40};         //pinapple, apple, carrot
	vector<int> quantities4 = readFromFile(3);// = {110, 70, 20};          //orange, pinapple, ginger
	vector<int> quantities5 = readFromFile(4);// = {160, 40};               //pear, ginger

    
    
    Cocktail cocktail1 = addCocktail("sunrise", ingredients1, quantities1, m_toppingNames[0]);
    Cocktail cocktail2 = addCocktail("smartee knee", ingredients2, quantities2, m_toppingNames[2]);
    Cocktail cocktail3 = addCocktail("tropical", ingredients3, quantities3, m_toppingNames[2]);
    Cocktail cocktail4 = addCocktail("passion", ingredients4, quantities4, m_toppingNames[0]);
    Cocktail cocktail5 = addCocktail("classic", ingredients5, quantities5, m_toppingNames[2]);
    
    m_cocktailList.push_back(cocktail1);
    m_cocktailList.push_back(cocktail2);
    m_cocktailList.push_back(cocktail3);
    m_cocktailList.push_back(cocktail4);
    m_cocktailList.push_back(cocktail5);
    
    
}


Cocktail CocktailMenu::addCocktail(string name, vector<int> &ingredientIndices, vector<int> &ingredientQuantities, string topping){
    
    
    vector<ingredient> ingrediensForCocktail(ingredientIndices.size());
    ingredient tempIngredient;
    for (int i = 0; i < ingredientIndices.size(); i++) {
        
        tempIngredient.name = m_ingredientNames[ingredientIndices[i]];
        tempIngredient.index = ingredientIndices[i];
        tempIngredient.quantity = ingredientQuantities[i];
        
        ingrediensForCocktail[i] = tempIngredient;
        
    }
    
    Cocktail newCocktail(name, ingrediensForCocktail, topping);
    
    return newCocktail;
    
}

Cocktail* CocktailMenu::getCocktail(int cocktailIndex){
    
    
    if (cocktailIndex < 0 || cocktailIndex > 5) {
        cerr << "ERROR: Cocktail not found. Returning Default" << endl;
        cocktailIndex = 0;
    }
    
    return &m_cocktailList[cocktailIndex];
    
}

//increments/decrements an ingredient by a certain amount
//decrements all other ingredients (except ginger) by a proportional amount, so the sum is constant
//parameters:
//cocktailIndex : number of cocktail in the list
//ingredientIndex : position of desired ingredient in "cocktail[cocktailIndex].ingredients"
//      i.e. different from position of ingredient in the list containing all possible ingredients
//incDecStay: we must increase/decrease or keep the ingredient as it is, can be -1, 1, or 0
void CocktailMenu::adjustIngredient(int cocktailIndex, int ingredientIndex, int incDecStay){
    
    Cocktail *currCocktail = getCocktail(cocktailIndex);
    ingredient *toAdjust = &(currCocktail->m_ingredients[ingredientIndex]);
    
    cerr << "Adjusting cocktail: " << currCocktail->m_cocktailName << ", ingredient: " << toAdjust->name << ", " << incDecStay << endl;
    
    
    //increment/decrement by 5%
    //round to nearest int
    int amount = 0.05 * (float)toAdjust->quantity * (float)incDecStay;
    cerr << "Amount for change: " << amount << endl;
    
    toAdjust->quantity += amount; //adjust target ingredient
    
    
    //find out how much to decrease other ingredients
    //round to nearest int
    int proportion = 0;
    if (currCocktail->m_numIngredients > 1) {
        proportion = amount / (currCocktail->m_numIngredients - 1);
    }
    
    
    for (int i = 0; i < currCocktail->m_numIngredients; i++) {
        
        //do not change ginger (has id 4) and target ingredient
        if (currCocktail->m_ingredients[i].index != 4 && i != ingredientIndex) {
            currCocktail->m_ingredients[i].quantity -= proportion;
        }
    }
    
    //all this works for negative "amount" as well
    
    
    //DEBUG
    cerr << "Ingredients: " << endl;
    for (int i = 0; i < currCocktail->m_numIngredients; i++) {
        
        cerr << currCocktail->m_ingredients[i].name << ": " << currCocktail->m_ingredients[i].quantity << endl;
    }
    
    
}



#endif /* cocktailMenu_h */
