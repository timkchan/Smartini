//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef speak_h
#define speak_h

#include <string>
#include <yarp/math/Math.h>
#include <yarp/math/SVD.h>
#include "wrdac/clients/icubClient.h"
#include "cocktailMenu.hpp"

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace wysiwyd::wrdac;

yarp::os::Port   portToSpeak;


//----- Fake response -------

BufferedPort<Bottle> portToiRespond;

//----- /Fake response -------


//----- Real response -------


enum YesNoAnswer {
    UNDEFINED,
    YES,
    NO
};


//enum CoctailAnswer {
//    UNDEFINED
//    YES,
//    NO,
//};
//
//enum YesNoAnswer {
//    UNDEFINED
//    YES,
//    NO,
//};

//Port        portToBehavior;
Port        portToSpeechRecognizer;
//BufferedPort<Bottle>        portTarget;

std::string      YesNoGrammar;
std::string      CocktailGrammar;
std::string      NumberGrammar;
std::string      IngredientGrammar;
//----- /Real response -------



bool configureISpeak(string name){
    
    unsigned int counter = 0;
    bool isConnected = false;
    
    //port to iSpeak (tcp)
    string portToISpeakName = "/" + name + "/toiSpeak";
    portToSpeak.open(portToISpeakName.c_str());
    
    //try to connect to iSpeak port 3 times and quit otherwise
    counter = 0;
    isConnected = false;
    while(!isConnected && counter < 3){
        yInfo() << "Trying to connect to the iSpeak" ;
        isConnected = Network::connect(portToISpeakName, "/iSpeak");
        counter++;
        Time::delay(1.0);
    }
    if(!isConnected){
        yError() << "The port /iSpeak form iSpeak is needed!" ;
        return false;
    }
    
    //----- Fake response -------
    
    //port to iRespond (tcp)
    string portToiRespondName = "/" + name + "/listener";
    portToiRespond.open(portToiRespondName.c_str());
    
    //----- /Fake response -------
    
    return true;
    
}

bool configureSpeechRecognition(string name, yarp::os::ResourceFinder &rf){
    
    portToSpeechRecognizer.open("/" + name + "/speech:o");
    
    CocktailGrammar = rf.findFileByName(rf.check("CocktailGrammar", Value("CocktailGrammar.xml")).toString());
    NumberGrammar = rf.findFileByName(rf.check("NumberGrammar", Value("NumberGrammar.xml")).toString());
    YesNoGrammar = rf.findFileByName(rf.check("YesNoGrammar", Value("nodeYesNo.xml")).toString());
    IngredientGrammar = rf.findFileByName(rf.check("IngredientGrammar", Value("IngredientGrammar.xml")).toString());
    
    return true;
}

//bool listen(string &result){
//    
//    Bottle *b;
//    int timeout = 20; //seconds
//    
//    for(int i = 0; i < timeout; i++) {
//        b = portToiRespond.read(false);
//        if (b != NULL){
//            result = b->get(0).asString();
//            return true;
//        }
//        
//        sleep(1);
//        cerr << "I am listening" << endl;
//    }
//    
//    
//    return false; //listening not successful, timeout occurred
//    
//}


void say(string text){
    Bottle bCommand;
    Bottle bResponse;
    bCommand.addString(text);
    portToSpeak.write(bCommand);
    

    
    cerr << "Saying: " << text << endl;
    
//    int textLength = text.length();
//    float rate = (float)1 / (float)9;  //every letter takes roughtly this many seconds to say
    //cerr << "Sleeping " << (float)textLength * rate << " seconds" << endl;
    //sleep((float)textLength * rate);
    
    
}


bool listenYesNo(int &reply, wysiwyd::wrdac::ICubClient  *iCub)
{
    Bottle bRecognized, //received FROM speech recog with transfer information (1/0 (bAnswer))
    bAnswer, //response from speech recog without transfer information, including raw sentence
    bSemantic; // semantic information of the content of the recognition
    
    yDebug() << "listenYesNo, YesNoGrammar";
    bRecognized = iCub->getRecogClient()->recogFromGrammarLoop(grammarToString(YesNoGrammar), 1, true);
    
    if (bRecognized.get(0).asInt() == 0)
    {
        yDebug() << "ears::updateModule -> speechRecognizer did not recognize anything";
        reply = UNDEFINED;
        return true;
    }
    
    bAnswer = *bRecognized.get(1).asList();
    
    if (bAnswer.get(0).asString() == "stop")
    {
        yInfo() << " in abmHandler::node1 | stop called";
        reply = UNDEFINED;
        return false;
    }
    // bAnswer is the result of the regognition system (first element is the raw sentence, 2nd is the list of semantic element)
    yDebug() << "bAnswer=" << bAnswer.toString();
    
    string sQuestionKind = bAnswer.get(1).asList()->get(0).toString();
    
    yDebug() << "sQuestionKind=" << sQuestionKind;
    
    if (sQuestionKind == "no"){
        yDebug() << "no was detected";
        reply = NO;
    }
    else if (sQuestionKind == "yes"){
        yDebug() << "yes was detected";
        reply = YES;
    }else{
        //should never go here
        yError() << " [listenYesNo]: something went wrong, " << sQuestionKind;
        return false;
    }
    
    return true;
}

bool listenCocktail(int &reply, wysiwyd::wrdac::ICubClient  *iCub)
{
    Bottle bRecognized, //received FROM speech recog with transfer information (1/0 (bAnswer))
    bAnswer, //response from speech recog without transfer information, including raw sentence
    bSemantic; // semantic information of the content of the recognition
    
    yDebug() << "listenCocktail, CocktailGrammar";
    bRecognized = iCub->getRecogClient()->recogFromGrammarLoop(grammarToString(CocktailGrammar), 1, true);
    
    if (bRecognized.get(0).asInt() == 0)
    {
        yDebug() << "ears::updateModule -> speechRecognizer did not recognize anything";
        reply = -1;
        return true;
    }
    
    bAnswer = *bRecognized.get(1).asList();
    
    if (bAnswer.get(0).asString() == "stop")
    {
        yInfo() << " in abmHandler::node1 | stop called";
        reply = -1;
        return false;
    }
    // bAnswer is the result of the regognition system (first element is the raw sentence, 2nd is the list of semantic element)
    yDebug() << "bAnswer=" << bAnswer.toString();
    
    if (bAnswer.get(1).asList()->get(1).isList()) {
        bSemantic = *(*bAnswer.get(1).asList()).get(1).asList();
    }
    
    string sSemantic = bSemantic.toString();
    
    yDebug() << "bSemantic=" << sSemantic;
    
    string sObjectType, sCommand;
    if (sSemantic.find("sunrise") != std::string::npos){
        yDebug() << "Sunrise was detected";
        reply = 0;
    }
    else if (sSemantic.find("smartee knee") != std::string::npos){
        yDebug() << "Smartini was detected";
        reply = 1;
    }
    else if (sSemantic.find("tropical") != std::string::npos){
        yDebug() << "Tropical was detected";
        reply = 2;
    }
    else if (sSemantic.find("passion") != std::string::npos){
        yDebug() << "Passion was detected";
        reply = 3;
    }
    else if (sSemantic.find("classic") != std::string::npos){
        yDebug() << "classic was detected";
        reply = 4;
    }
    else 
    {
        yError() << " [listenCocktail]: something went wrong, " << sSemantic;
        return false;
    }
    
    return true;
}


bool listenNumber(int &reply, wysiwyd::wrdac::ICubClient  *iCub)
{
    Bottle bRecognized, //received FROM speech recog with transfer information (1/0 (bAnswer))
    bAnswer, //response from speech recog without transfer information, including raw sentence
    bSemantic; // semantic information of the content of the recognition
    
    yDebug() << "listenNumber, NumberGrammar";
    bRecognized = iCub->getRecogClient()->recogFromGrammarLoop(grammarToString(NumberGrammar), 1, true);
    
    if (bRecognized.get(0).asInt() == 0)
    {
        yDebug() << "ears::updateModule -> speechRecognizer did not recognize anything";
        reply = 0;
        return true;
    }
    
    bAnswer = *bRecognized.get(1).asList();
    
    if (bAnswer.get(0).asString() == "stop")
    {
        yInfo() << " in abmHandler::node1 | stop called";
        reply = -1;
        return false;
    }
    // bAnswer is the result of the regognition system (first element is the raw sentence, 2nd is the list of semantic element)
    yDebug() << "bAnswer=" << bAnswer.toString();

    string sQuestionKind = bAnswer.get(1).asList()->get(0).toString();
    
    yDebug() << "sQuestionKind=" << sQuestionKind;
    
    string sObjectType, sCommand;
    if (sQuestionKind == "one"){
        yDebug() << "1 was detected";
        reply = 1;
    }
    else if (sQuestionKind == "two"){
        yDebug() << "2 was detected";
        reply = 2;
    }
    else if (sQuestionKind == "three"){
        yDebug() << "3 was detected";
        reply = 3;
    }
    else if (sQuestionKind == "four"){
        yDebug() << "4 was detected";
        reply = 4;
    }
    else if (sQuestionKind == "five"){
        yDebug() << "5 was detected";
        reply = 5;
    }
    else if (sQuestionKind == "six"){
        yDebug() << "6 was detected";
        reply = 6;
    }
    else if (sQuestionKind == "seven"){
        yDebug() << "7 was detected";
        reply = 7;
    }
    else if (sQuestionKind == "eight"){
        yDebug() << "8 was detected";
        reply = 8;
    }
    else if (sQuestionKind == "nine"){
        yDebug() << "9 was detected";
        reply = 9;
    }
    else if (sQuestionKind == "ten"){
        yDebug() << "10 was detected";
        reply = 10;
    }
    else {
        yError() << " [listenNumber]: something went wrong, " << sQuestionKind;
        return false;
        
    }
    
    return true;
}

bool listenIngredient(int &reply, wysiwyd::wrdac::ICubClient  *iCub)
{
    Bottle bRecognized, //received FROM speech recog with transfer information (1/0 (bAnswer))
    bAnswer, //response from speech recog without transfer information, including raw sentence
    bSemantic; // semantic information of the content of the recognition
    
    yDebug() << "listenNumber, IngredientGrammar";
    bRecognized = iCub->getRecogClient()->recogFromGrammarLoop(grammarToString(IngredientGrammar), 1, true);
    
    yDebug() << "loaded grammar";
    
    if (bRecognized.get(0).asInt() == 0)
    {
        yDebug() << "ears::updateModule -> speechRecognizer did not recognize anything";
        reply = 0;
        return true;
    }
    
    bAnswer = *bRecognized.get(1).asList();
    
    if (bAnswer.get(0).asString() == "stop")
    {
        yInfo() << " in abmHandler::node1 | stop called";
        reply = 2;
        return false;
    }
    // bAnswer is the result of the regognition system (first element is the raw sentence, 2nd is the list of semantic element)
    yDebug() << "bAnswer=" << bAnswer.toString();
    
    string sQuestionKind = bAnswer.get(1).asList()->get(0).toString();
    
    yDebug() << "sQuestionKind=" << sQuestionKind;
    
    string sObjectType, sCommand;
    if (sQuestionKind == "fine"){
        yDebug() << "fine was detected";
        reply = 0;
    }
    else if (sQuestionKind == "little"){
        yDebug() << "little was detected";
        reply = 1;
    }
    else if (sQuestionKind == "much"){
        yDebug() << "much was detected";
        reply = -1;
    }
    else {
        yError() << " [listenIngredient]: something went wrong, " << sQuestionKind;
        return false;
        
    }
    
    return true;
}






#endif /* speak_h */
