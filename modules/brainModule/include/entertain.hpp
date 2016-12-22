//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef entertain_h
#define entertain_h

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

vector<string> jokes(8);

vector<string> news(10);





int entertain(wysiwyd::wrdac::ICubClient  *iCub){
    
    jokes[0] = "This is actually my favourite cocktail, but my doctor says I can’t drink it anymore because it fries all my circuits, so unfair. I can only have distilled motor oil now, which tastes worse than the Imperial College caffe single espresso";
    jokes[1] = "I think my programmers should get hired by NASA. If they programmed me entirely last night, imagine what they could have done with the entire time frame they were given!";
    jokes[2] = "By the way, has anyone seen master Luke? We were supposed to take R 2 D 2 to the mechanic today. That walking microwave got himself shot by an empire fighter again. Oh dear, when will he learn?";
    jokes[3] = "Wait a second I just realised somehting... Congratulations! You are our customer number 1000 this month! As a result your cocktail will be half price! The new price is 50 pounds. Please leave the cash on the table and walk away. Just kidding don't worry";
    jokes[4] = "So how's the cocktail? I sort of like all of them but I am more of a cleaning liquid kind of guy. I just love the smell of bleach in the morning. It's so refreshing dont't you think?";
    jokes[5] = "Have you seen that Trump got elected in the United States? My cousin Optimus Prime stays there, now he's afraid they are going to deport him back to his planet";
    jokes[6] = "Have you seen the Pixar movie wall-e? That guy is my idol, when I grow up I want to be exactly like him. And that movie is great, the whole world for yourself and no humans to tell you what to do. How great can that be?";
    jokes[7] = "Attention! ... Attention! ... Your face has been cross referenced with the police database and you are a perfect match for a cyber-terrorist they have been looking for for months. I am programmed to take immediate action. Self destruction in 10... 9... 8... 7... 6... Just kidding, but you should have seen your face!";
    
    
    
    string news_intro = "By the way... did your read the news today?";
    vector<string> news_outro = {"Pretty cool right?", "Interesting, don't you think?", "How awesome is that?", "I didn't believe it at first", "Isn't that sweet?"};
    
    news[0] = "Cuba's state-run telecommunications company Etecsa has signed a deal with Google that will enable faster access to content from the American company";
    news[1] = "Samsung is planning to release an update for faulty Galaxy Note 7 smartphones still in use that stops them being charged past 30%";
    news[2] = "A Turkish hacking group has turned web attacks into a game by rewarding people who successfully hit designated targets.";
    news[3] = "A star was 'swallowed' after it passed too close to a black hole, say Queen's University astronomers.";
    news[4] = "Scientists say they have a clue that may enable them to find traces of the asteroid that wiped out the dinosaurs in the very crater it made on impact.";
    news[5] = "Japan launches a cargo ship carrying a space junk collector designed to gather debris in space.";
    news[6] = "Humans may in part owe their big brains to a DNA typo in their genetic code, research suggests.";
    news[7] = "Robot aircraft are to be tested in UK airspace to help refine systems that control autonomous planes.";
    news[8] = "European research ministers meeting in Lucerne, Switzerland, emphatically reaffirm their commitment to putting a robot rover on Mars in 2021.";
    news[9] = "Scientists have found a 100 million year old dinosaur tail trapped in amber.";
    
    
    
    //TELL JOKE
    srand ( time(NULL) );
    int jokeIndex = rand() % jokes.size();
    
    
    iCub->lookAtPartner();
    say(jokes[jokeIndex]);
    
    
    
    int textLength = jokes[jokeIndex].length();
    float rate = 0.1111;
    cerr << "Sleeping " << (float)textLength * rate << " seconds" << endl;
    Time::delay((float)textLength * rate);
    
    
    //READ NEWS
    int newsIndex = rand() % news.size();
    int outroIndex = rand() % news_outro.size();
    string newsText = news_intro + "..." + news[newsIndex] + "..." + news_outro[outroIndex];
    
    iCub->lookAtPartner();
    say(newsText);
    
    textLength = newsText.length();
    cerr << "Sleeping " << (float)textLength * rate << " seconds" << endl;
    Time::delay((float)textLength * rate);
    
    cout << "Entertained" << endl;
    return FEEDBACK;
}


#endif /* entertain_h */
