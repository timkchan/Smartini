//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef fileUtils_h
#define fileUtils_h

#include <string>
#include <yarp/math/Math.h>
#include <yarp/math/SVD.h>
#include "wrdac/clients/icubClient.h"
#include "cocktail.hpp"
#include <fstream>
#include <sstream>

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace wysiwyd::wrdac;



void writeVote(int &cocktailIndexInMenu, int vote){

	string filename = "votes.txt";
	ofstream outfile;

	outfile.open(filename, ios::out | ios::ate | ios::app);
	
	outfile << cocktailIndexInMenu << ", " << vote << endl;

	outfile.close();
	cout << "Writing votes: DONE!\n";

}


void writeQuantitiesOnFile(Cocktail *cocktail,int &cocktailIndexInMenu){

	vector<ingredient> ingredients = cocktail->m_ingredients;
	
	stringstream filename;
	filename << "cocktail_";
	filename << cocktailIndexInMenu;
	filename << ".txt";

	ofstream outfile;
	outfile.open(filename.str().c_str(), ios::out | ios::ate | ios::app);

	for (int i = 0; i < ingredients.size(); i++){

		outfile << ingredients[i].quantity << ", ";
		
	}

	outfile << ingredients[ingredients.size() - 1].quantity << endl;
	outfile.close();
	yInfo() << "Writing done!";
}


vector<int> readFromFile(int cocktailIndex){

	//vector<int> quantities1;// = { 70, 70, 20, 40 };     //orange, apple, ginger, carrot
	//vector<int> quantities2;// = {100, 100};              //apple, pear
	//vector<int> quantities3;// = {60, 100, 40};         //pinapple, apple, carrot
	//vector<int> quantities4;// = {110, 70, 20};          //orange, pinapple, ginger
	//vector<int> quantities5;// = {160, 40};               //pear, ginger

	vector<int> quantities;

	vector<vector<int>> quantityMenu = { { 70, 70, 20, 40 }, { 100, 100 }, { 60, 100, 40 }, { 110, 70, 20 }, { 160, 40 } };

	stringstream filename;
	filename << "cocktail_";
	filename << cocktailIndex;
	filename << ".txt";

	cout << "Reading from file: " << filename.str() << endl;
	ifstream infile(filename.str().c_str(),ios::in);

	if (!infile.good()){

		cout << filename.str() << " does not exist: creating..."<< endl;

		quantities = quantityMenu[cocktailIndex];

		ofstream outfile;
		outfile.open(filename.str().c_str(), ios::out | ios::ate | ios::app);
		for (int i = 0; i < quantities.size()-1; i++){
			outfile << quantities[i] << ", ";
		}
		outfile << quantities[quantities.size()-1];
		outfile.close();
		cout << filename.str() << " correctly created!";

	}
	else{

		//infile.open(filename.str().c_str(), ios::in);
		string line, lastline;
		while (infile)
		{
			lastline = line;
			getline(infile, line);
		}
		cout << lastline << endl;
		stringstream lastline_stream(lastline);

		while (std::getline(lastline_stream, line, ','))
		{
			quantities.push_back(atoi(line.c_str()));
		}

		infile.close();
		cout << filename.str() << " correctly loaded"<<endl;

	}

	return quantities;

}


#endif /* fileUtils_h */
