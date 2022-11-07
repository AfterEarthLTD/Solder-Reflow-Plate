#ifndef solderplate_h
#define solderplate_h

float getTemp();
void cancelledPB();
void cancelledTimer();
void coolDown();
void completed();
bool heat(byte maxTemp);
void main_menu();

#endif