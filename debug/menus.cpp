#include "menus.h"
#include "debug.h"
#include "lcdScreen.h"
#include "gpio.h"
#include "imgSth.h"
#include "ball.h"
/****************/
extern debug Debug;
lcd Lcd;
/****************/
int recentId;
int recentList;
int probeValue = 1;   
int keyValue = 0;

#define KEY_GPIO HW_GPIOB
#define KEY_ESC   22
#define KEY_UP    16
#define KEY_DOWN  10
#define KEY_OK    20

#define FUNCTION_ESC   0x01
#define FUNCTION_UP    0x02
#define FUNCTION_DOWN  0x04
#define FUNCTION_OK    0x08

/*init list*/
list menuList[listTotal] = {
	/*first layout */
	list("MAINLIST", 1, 1, 0, 0,    &menuList[0], 0),
	/*second layout */
	list("SEE VALUE",   2, 1, 1, 1, &menuList[0], 1),
	list("OLED IMAGE",  2, 2, 1, 1, &menuList[0], 1),
	list("SEND",        2, 3, 1, 1, &menuList[0], 2),
	list("GAME",        2, 4, 1, 1, &menuList[0], 1),
	
};

/*===================================================================================================*/
/*===================================================================================================*/
/****手动添加***********/
/*(1,1)MAINLIST*/
void gogogo(){
	Lcd.clear();
	return;
}
void gotoList_1(){
	Lcd.clear();
	menuList[1].show();
}
void gotoList_2(){
	Lcd.clear();
	menuList[2].show();
}
void gotoList_3(){
	Lcd.clear();
	menuList[3].show();
}
void gotoList_4(){
	Lcd.clear();
	menuList[4].show();
}

/*(2,1) SEE VALUE */
void seeValue(){
	static int seeValueClearFrequency;
	seeValueClearFrequency++;
	if(seeValueClearFrequency == 500){
		Lcd.clear();
		seeValueClearFrequency = 0;
	}
	Lcd.setPos(0,0);
	Lcd.wtireString(" ****");
	Lcd.wtireString("SEE VALUE");
	Lcd.wtireString("****");
	/***demo***/
	Lcd.setPos(2,1);
	Lcd.wtireString("pi:");
	Lcd.writeFloat(3.1415926, 7);
	/**********/
}

/*(2,2) OLED IMAGE*/
void oledSeeValue(){
	static int oledSeeValueFlag;
	if(!oledSeeValueFlag){
		oledSeeValueFlag = 1;
		Lcd.clear();
		camera_init(imgbuff);
	}
	
	camera_get_img();  
	Debug.extractImage(picturess, imgbuff, 1200);  
	imgHandle();
	Debug.showLine();
	Debug.reduceImage(picturess, imgbuff, 9600);
	Debug.ledSeeImage(); 
}

/*(2,3) SEND*/
void sendImage(){
	static int sendImageFlag;
	if(!sendImageFlag){
		sendImageFlag = 1;
		camera_init(imgbuff); 
	}
	
	camera_get_img(); 
	Debug.extractImage(picturess, imgbuff, 1200);  
	imgHandle();
	Debug.showLine();
	Debug.reduceImage(picturess, imgbuff, 9600);
	Debug.sendImage(imgbuff, CAMERA_SIZE);
}

void sendImageZhu(){
	static int sendImageZhuFlag;
	if(!sendImageZhuFlag){
		sendImageZhuFlag = 1;
		camera_init(imgbuff);
	}
	
	camera_get_img();
	Debug.extractImage(picturess, imgbuff, 1200);  
	imgHandle();
	Debug.showLine();
	Debug.reduceImage(picturess, imgbuff, 9600);
	Debug.sendImageZhu(imgbuff, CAMERA_SIZE);
}

void sendWave(){
	//Debug.sendWave(uint8_t *aim,uint8_t *get,uint32_t length);
}

/*(2,4) GAME*/
void game(){
	static int gameFlag;
	
	if(!gameFlag){
		gameFlag = 1;
		Lcd.clear();
	} 
	for(int i = 0; i < ballNumber; i++){
		ballList[i].play();
	}	
}
/***********************/
/*===================================================================================================*/
/*===================================================================================================*/

void menuListInit(){
	/*(1,1) MAINLIST*/
	menuList[0].addSonList(1, "GOGOGO!!!",  gogogo);
	menuList[0].addSonList(2, "SEE VALUE",  gotoList_1);
	menuList[0].addSonList(3, "OLED IMAGE", gotoList_2);
	menuList[0].addSonList(4, "SEND",       gotoList_3);
	menuList[0].addSonList(5, "GAME",       gotoList_4);
	
	/*(2,1) SEE VALUE*/
	menuList[1].addSonList(1, "", seeValue);
	
	/*(2,2) OLED IMAGE*/
	menuList[2].addSonList(1, "", oledSeeValue);
	
	/*(2,3) SEND*/
	menuList[3].addSonList(1, "SEND IMAGE"     , sendImage);
	menuList[3].addSonList(2, "SEND IMAGE ZHU" , sendImageZhu);
	menuList[3].addSonList(3, "SENDWAVE"       , sendWave);
	
	/*(2,4) GAME*/
	menuList[4].addSonList(1, "GAME"       , game);
}

void list::listKeyInit(){
	GPIO_QuickInit(KEY_GPIO,  KEY_ESC    ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KEY_GPIO,  KEY_UP     ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KEY_GPIO,  KEY_DOWN   ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KEY_GPIO,  KEY_OK     ,kGPIO_Mode_IPU);
}

int list::keyScan(){
	uint8_t key = 0;
	if (!GPIO_ReadBit(KEY_GPIO, KEY_ESC))	
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KEY_GPIO, KEY_ESC));
		key |= FUNCTION_ESC;
	}
	if (!GPIO_ReadBit(KEY_GPIO, KEY_UP))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KEY_GPIO, KEY_UP));
		key |= FUNCTION_UP;
	}
	if (!GPIO_ReadBit(KEY_GPIO, KEY_DOWN))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KEY_GPIO, KEY_DOWN));
		key |= FUNCTION_DOWN;
	}
	if (!GPIO_ReadBit(KEY_GPIO, KEY_OK))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KEY_GPIO, KEY_OK));
		key |= FUNCTION_OK;
	}
	return key;
}

list::list(char* _name, int _list, int _id, int _fatherlist, int _fatherid, list *_father, int _isFunc){
	listCount++;
	
	this->name       = _name;
	this->Id         = _id;
	this->List       = _list;
	this->father     = _father;
	this->fatherList = _fatherlist;
	this->fatherId   = _fatherid;
	this->isFunc     = _isFunc;
}

char* list::getName(){
	return name;
}

int list::getId(){
	return this->Id;
}

int list::getList(){
	return this->List;
}

list* list::getFather(){
	return this->father;
}

int list::getFatherList(){
	return this->fatherList;
}

int list::getFatherId(){
	return this->fatherId;
}

int list::getsonListNumber(){
	return this->sonListNumber;
}

int list::getIsFunc(){
	return this->isFunc;
}

void list::addSonList(int id, char * str, void (*func)()){
	this->sonList[this->sonListNumber].id = id;
	this->sonList[this->sonListNumber].name = str;
	this->sonList[this->sonListNumber].function = func;
	this->sonListNumber++;
	
}

arrayMenu list::getSonList(int id){
	int i;
	for(i = 0; i < this->sonListNumber; i++){
		if(this->sonList[i].id == id)
			return sonList[i];
	}
	
	if(i == this->sonListNumber)
		Debug.print("no list");
}

int multiTasking = 0;
void list::show(){
	int i;
	/*title*/
	Lcd.setPos(0,0);
	Lcd.wtireString(" ****");
	Lcd.wtireString(this->getName());
	Lcd.wtireString("****");
	/*content*/
	int listNumber = this->getsonListNumber();
	for(i = 0; i < listNumber; i++){
		Lcd.setPos(8, i+1);
		Lcd.writeInt(i+1);
		Lcd.wtireString(".");
		Lcd.wtireString(this->getSonList(i+1).name);
	}
	/*probe*/
	probeValue = 1;
	Lcd.setPos(0, 1);
	Lcd.wtireString(">");
	/*function*/
	recentId = this->getId();        
	recentList = this->getList();    
	
direction:
	keyValue = this->keyScan();
	switch(this->getIsFunc())
	{
	case 0: //跳转界面
		switch(keyValue){
		case FUNCTION_ESC:
			list* myFather = this->getFather();
			Lcd.clear();
			(*myFather).show();
			break;
		case FUNCTION_UP:
			if(probeValue == 1)
				probeValue = sonListNumber;
			else
				probeValue -= 1;
			for(i = 0; i < sonListNumber; i++){
				Lcd.setPos(0, i+1);
				if(i+1 == probeValue)
					Lcd.wtireString(">");
				else
					Lcd.wtireString(" ");
			}
			break;
		case FUNCTION_DOWN:
			if(probeValue == sonListNumber)
				probeValue = 1;
			else
				probeValue += 1;
			for(i = 0; i < sonListNumber; i++){
				Lcd.setPos(0, i+1);
				if(i+1 == probeValue)
					Lcd.wtireString(">");
				else
					Lcd.wtireString(" ");
			}
			break;
		case FUNCTION_OK:
			this->getSonList(probeValue).function();
			break;
		default:
			for(i = 0; i < this->getsonListNumber(); i++){
				Lcd.setPos(0, i+1);
				if(i+1 == probeValue)
					Lcd.wtireString(">");
				else
					Lcd.wtireString(" ");
			}
			break;
		}
		break;//break case:0
		
	case 1:  //单功能界面
		switch(keyValue){
		case FUNCTION_ESC:
			list* myFather = this->getFather();
			Lcd.clear();
			(*myFather).show();
			break;
		case FUNCTION_UP:
			break;
		case FUNCTION_DOWN:
			break;
		case FUNCTION_OK:
			break;
		default:
			this->getSonList(probeValue).function();
			break;
		}
		break;//break case:1
		
	case 2: //多任务界面
		switch(keyValue){
		case FUNCTION_ESC:
			list* myFather = this->getFather();
			Lcd.clear();
			(*myFather).show();
			break;
		case FUNCTION_UP:
			if(multiTasking == 0)
			{
				if(probeValue == 1)
					probeValue = sonListNumber;
				else
					probeValue -= 1;
				for(i = 0; i < sonListNumber; i++){
					Lcd.setPos(0, i+1);
					if(i+1 == probeValue)
						Lcd.wtireString(">");
					else
						Lcd.wtireString(" ");
				}
			}
			break;
		case FUNCTION_DOWN:
			if(multiTasking == 0)
			{
				if(probeValue == sonListNumber)
					probeValue = 1;
				else
					probeValue += 1;
				for(i = 0; i < sonListNumber; i++){
					Lcd.setPos(0, i+1);
					if(i+1 == probeValue)
						Lcd.wtireString(">");
					else
						Lcd.wtireString(" ");
				}
			}
			break;
		case FUNCTION_OK:
			if(multiTasking == 0)
				multiTasking = 1;
			else
				multiTasking = 0;
			break;
		default:
			if(multiTasking == 1)
				this->getSonList(probeValue).function();
			for(i = 0; i < this->getsonListNumber(); i++){
				Lcd.setPos(0, i+1);
				if(i+1 == probeValue){
					if(multiTasking == 1)
						Lcd.wtireString("*");
					else
						Lcd.wtireString(">");
				}
				else
					Lcd.wtireString(" ");
			}
			break;
		}
		
		break;//break case:2
		
	default:
		break;
		
		
	}
	if(recentId == 1 && recentList == 1 && keyValue == FUNCTION_OK && probeValue == 1)           //跳出菜单
		goto Bye;
	else
		goto direction;
	
Bye:
	
}

