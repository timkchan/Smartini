//
//  brain.hpp
//  brain
//
//  Created by Filippo Baldini on 02/12/2016.
//  Copyright © 2016 Filippo Baldini. All rights reserved.
//

#ifndef goodbye_h
#define goodbye_h

using namespace std;

string final_greeting = "That was all for today! I hope you enjoyed the experience and hopefully I will see you again soon! Have a nice day!";


int goodbye(){
    
    iCub->lookAtPartner();
    say(final_greeting);
    Time::delay(30);
    
    
    cout << "Said Goodbye" << endl;
    return IDLE;
}


#endif /* goodbye_h */
