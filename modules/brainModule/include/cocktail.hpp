//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef cocktail_h
#define cocktail_h

#include <vector>
#include <string>

using namespace std;


struct ingredient{
    string name;
    int index;
    int quantity;
};



class Cocktail {
    
private:
    
    //    vector<string> cocktail_list = { "Mojito" , "Jungle" , "Sunrise" ,  "Shirley" , "Paradise"};
    

    
    
public:
    
    
    string m_cocktailName;
    vector<ingredient> m_ingredients;     //list of ingredients we put in
    string m_topping;
    int m_numIngredients;
    
    
//    string m_ingredientNames = {"orange juice", "pinapple juice", "apple juice", "pear juice", "ginger", "carrot juice"};
//    int m_toppingNames = {"orange" , "lime", "apple"};
    
    
    
    Cocktail(string name, vector<ingredient> &ingredients, string topping);
    
    
    int getNumberOfIngredients();
    vector<ingredient> getIngredients();
    //vector<int> getQuantities();
    //vector<string> getIngredientNames();
    string getName();
    
    //void adjustIngredient(int cocktailIndex, int ingredientIndex, int amount);

    
    
};

Cocktail::Cocktail(string name, vector<ingredient> &ingredients, string topping){
    
    m_cocktailName = name;
    m_ingredients = ingredients;
    m_topping = topping;
    m_numIngredients = m_ingredients.size();
}

int Cocktail::getNumberOfIngredients(){
    return m_numIngredients;
}

vector<ingredient> Cocktail::getIngredients(){
    return m_ingredients;
}


string Cocktail::getName(){
    return m_cocktailName;
}



#endif /* cocktail_h */
