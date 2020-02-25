#ifndef _MEUNS_H_
#define _MEUNS_H_

typedef struct arrayMenu{
    int number;
    int id;
    char * name;
    void (*function)();
}arrayMenu;


class list{
protected:
	char *name;
	int listCount;
    int Id;
    int List;
	list* father;
    int fatherList;
    int fatherId;
    int sonListNumber;
	int isFunc;
    arrayMenu sonList[10];
public:
	/*init*/
    list(char* _name, int _list, int _id, int _fatherlist, int _fatherid, list *_father, int _isFunc);
	char* getName();
    int   getId();
    int   getList();
	list *getFather();
    int   getFatherId();
    int   getFatherList();
	int   getsonListNumber();
	int   getIsFunc();
    void  addSonList(int id, char * str, void (*func)());
    arrayMenu getSonList(int id);
	/*handle*/
	void  listKeyInit();   //only mainlist call
	int   keyScan();
	void  show();

};

extern int recentId;
extern int recentList;
extern void listGpioInit();
extern void menuListInit();

#define listTotal   5
extern list menuList[listTotal];
#endif