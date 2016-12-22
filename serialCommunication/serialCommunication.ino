int cocktail_ml [] = {0,84,168,252,336,420,504,588,672,756,840,924,1008,
                      1092,1176,1260,1344,1428,1512,1596,1680,1764,1848,
                      1932,2016,2100,2156,2212,2268,2324,2380,2436,2492,
                      2548,2604,2660,2716,2772,2828,2884,2940,2996,3052,
                      3108,3164,3220,3276,3332,3388,3448,3500,3552,3604,
                      3656,3708,3760,3812,3864,3916,3968,4020,4072,4124,
                      4176,4228,4280,4332,4384,4436,4488,4540,4592,4644,
                      4696,4748,4800,4868,4936,5004,5072,5140,5208,5276,
                      5344,5412,5480,5548,5616,5684,5752,5820,5888,5956,
                      6024,6092,6160,6228,6296,6364,6432,6500,6568,6636,
                      6704,6772,6840,6908,6976,7044,7112,7180,7248,7316,
                      7384,7452,7520,7588,7656,7724,7792,7860,7928,7996,
                      8064,8132,8200,8268,8336,8404,8472,8540,8608,8676,
                      8744,8812,8880,8948,9016,9084,9152,9220,9288,9356,
                      9424,9492,9560,9628,9696,9764,9832,9900,9968,10036,
                      10104,10172,10240,10308,10376,10444,10512,10580,10648,
                      10716,10784,10852,10920,10988,11056,11124,11192,11260,
                      11328,11396,11464,11532,11600,11668,11736,11804,11872,
                      11940,12008,12076,12144,12212,12280,12348,12416,12484,
                      12552,12620,12688,12756,12824,12892,12960,13028,13096,13164,13232,13300
};

const int blinkingTime = 100; // in ms
int ingredient_index, quantity_in_ml;

String input = "";   // for incoming serial data
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
}

void loop() {
 
  if (Serial.available() > 0) {
    
      // read the incoming byte:
      input = Serial.readString();
      // and blink for external feedback
      blinkWhenReceived(blinkingTime);
    // extract ingredient_index and quantity_in_ml substrings from input
    for (int i = 0; i < input.length(); i++) {
      if (input.substring(i, i+1) == " ") {
        // cast to int
        ingredient_index = input.substring(0, i).toInt();
        quantity_in_ml = input.substring(i+1).toInt();
        
        Serial.println("Received: "+ String(ingredient_index,10) + ":" + String(quantity_in_ml,10));
        // pouring the liquid
        pour(convertToCorrectPin(ingredient_index),quantity_in_ml);  

        break;
      }
    }   
  }

}
int convertToCorrectPin(int pump_num){
  // Mapping the ingredient index on the correct pump number
  // i.e. on correct output pin 
  if(pump_num == 0) return 3;
  if(pump_num == 1) return 5;
  if(pump_num == 2) return 6;
  if(pump_num == 3) return 9;
  if(pump_num == 4) return 10;
  if(pump_num == 5) return 11;
  
}


void pour(int pump, int quantity){

  float period = cocktail_ml[quantity];

  digitalWrite(pump,HIGH);  // turn on the pump
  delay(period);            // wait for period time
  digitalWrite(pump,LOW);   // turn off the pump

}

void blinkWhenReceived(int blinkingTime){

  for(int i=0;i<3;i++){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(blinkingTime);      // wait for a blinkingTime ms
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(blinkingTime); 
  }

}


