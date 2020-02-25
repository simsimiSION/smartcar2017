#include "food.h"
#include "lcdScreen.h"
#include "common.h"
/*****************/
extern lcd Lcd;
/*****************/

food::food(){
	int i;
	for(int i = 0; i < 64; i++){
		this->Food[i] = 8;
	}

}

void food::print(){
	for(int j = 0; j < 64; j++){
		Lcd.printVerLine(2*j  , 0, this->Food[j]);
		Lcd.printVerLine(2*j+1, 0, this->Food[j]);
	}
	
}

void food::eaten(int _x){
	this->Food[_x] -= 1;
}