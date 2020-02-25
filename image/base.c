#include "base.h"
#include "imgSth.h"

LineType lineinf[60];
int effect = 0;

base::base(){
	
	effect = 0;  
	for(int i = up; i <= down; i++){
		lineinf[i].LeftPos   = left;
		lineinf[i].MiddlePos = (left + right) / 2;
		lineinf[i].RightPos  = right;
	}
	
	firstLine();
	otherLine(0);
	effect = getEffect();
	otherLine(effect);
	setTrueRL();
}
/*==============================*/
base Base;
/*==============================*/
void base::Do(){
	effect = 0;
	for(int i = up; i <= down; i++){
		lineinf[i].LeftPos   = left;
		lineinf[i].MiddlePos = (left + right) / 2;
		lineinf[i].RightPos  = right;
	}
	
	firstLine();
	otherLine(0);
	effect = getEffect();
	otherLine(effect);
	setTrueRL();
}

LineType base::getLineinf(){
	return lineinf[60];
}

void base::setLineinf(LineType line[]){
	for(int i = 0; i < down; i++){
		lineinf[i].LeftPos   = line[i].LeftPos;
		lineinf[i].MiddlePos = line[i].MiddlePos;
		lineinf[i].RightPos  = line[i].RightPos;
	}
}

void base::firstLine(){
	
	int  row = 80;    //定义列向量
	
	for (row = 80; row >= left; row--)                                  //找第一行的左边界点
	{
		if (picture[59][row] != picture[59][row + 1])                     //判断两个点是否相同的方法找左边界点
		{
			if (picture[59][row + 1] == picture[59][row + 2])               //滤波
			{
				lineinf[59].LeftPos = row;
				break;
			}
			else if (row == left)                                       //如果没有则判断左边界点为左边界
				lineinf[59].LeftPos = left;
			else {}
		}
		else if (row == left)                                            //如果没有则判断左边界点为左边界
			lineinf[59].LeftPos = left;
		else {}
	}
	for (row = 80; row <= right; row++)                                  //找第一行的右边界点
	{
		if (picture[59][row] != picture[59][row - 1])                      //判断两个点是否相同的方法找左边界点
		{
			if (picture[59][row - 1] == picture[59][row - 2])                //滤波
			{
				lineinf[59].RightPos = row;
				break;
			}
			else if (row == right)                                       //如果没有则判断右边界点为右边界
				lineinf[59].RightPos = right;
			else {}
		}
		else if (row == right)                                           //如果没有则判断右边界点为右边界
			lineinf[59].RightPos = right;
		else {}
	}
}

void base::otherLine(int effect){
	/*
	相关变量
	*/
	int column = 58;   //定义行向量
	int row = 80;      //定义列向量
	
	/*
	相关算法
	算法说明：根据相关点上方的点的颜色来判断边界。如果为黑色则向相对中心寻找，否则向相对边界寻找
	解释一：对于左边界来说可能出现很多左边界都为left值的情况，所以对于左边界来说在未找到边界出现(row == (left))的情况，右边界同理
	*/
	for (column = 58; column >= effect; column--)
	{
		if (picture[column][lineinf[column + 1].LeftPos] == 255)                     //判断上方点的颜色（黑色）
		{
			
			if (lineinf[column + 1].LeftPos == right)
				lineinf[column].LeftPos = right;
			else
			{
				for (row = lineinf[column + 1].LeftPos; row < right; row++)              //对于左边界来说若上方点为黑色，则向右寻找
				{
					if (picture[column][row] != picture[column][row + 1])
					{
						lineinf[column].LeftPos = row;
						break;
					}
					else if (row == (right - 1) || (row == (right)))                      //如果没有找到则赋值为右边界
						lineinf[column].LeftPos = right;
					else {}
				}
			}
			
		}
		else                                                                          //判断上方点的颜色（白色）
		{
			for (row = lineinf[column + 1].LeftPos; row >= left; row--)               //对于左边界来说若上方点为白色，则向右寻找
			{
				if (picture[column][row] != picture[column][row + 1])
				{
					lineinf[column].LeftPos = row;
					break;
				}
				else if ((row == (left + 1)) || (row == (left)))                      //如果没有找到则赋值为左边界
					lineinf[column].LeftPos = left;
				else {}
				
			}
		}
		//右边界也是一样的，不注释了，太懒了， ^0^
		if (picture[column][lineinf[column + 1].RightPos] == 255)
		{
			if (lineinf[column + 1].RightPos == left)
				lineinf[column].RightPos = left;
			else
			{
				for (row = lineinf[column + 1].RightPos; row > left; row--)
				{
					if (picture[column][row] != picture[column][row - 1])
					{
						lineinf[column].RightPos = row;
						break;
					}
					else if (row == (left + 1) || (row == left))
						lineinf[column].RightPos = left;
					else {}
				}
			}
			
			
		}
		else
		{
			for (row = lineinf[column + 1].RightPos; row <= right; row++)
			{
				if (picture[column][row] != picture[column][row - 1])
				{
					lineinf[column].RightPos = row;
					break;
				}
				else if ((row == (right - 1)) || (row == (right)))
					lineinf[column].RightPos = right;
				else {}
			}
		}
	}
}

int base::getEffect(){
	
	for (int i = down; i > up; i--)
	{
		if ((lineinf[i].RightPos - lineinf[i].LeftPos) <= 5 || lineinf[i].LeftPos > 155 || lineinf[i].RightPos < 5)
			return i;
		if (i == 1)
			return 1;
	}
	
	return 0;
}

void base::setTrueRL(){
	for(int i = down; i >= effect; i--){
		lineinf[i].TrueLeft  = lineinf[i].LeftPos;
		lineinf[i].TrueRight = lineinf[i].RightPos;
	}
}


void base::setMiddleLine(){
	
	int i = 0;
	int probe = effect + 1;
	
	for(i = down; i >= effect; i --){
		lineinf[i].MiddlePos = ( lineinf[i].LeftPos + lineinf[i].RightPos ) / 2;
	}
	
	if ( effect < down )        //判断中线大致趋势，中线归一化
	{
		if (lineinf[down].MiddlePos < lineinf[probe].MiddlePos && probe != up)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, right);
		else if (lineinf[down].MiddlePos > lineinf[probe].MiddlePos && probe != up)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, left);
		else if (probe != 0)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, (left + right) / 2);
		else {}
		
		if (structRamp != 0)   //坡道向中间补线
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, (left + right) / 2);
	}
}

void base::patchLeft(int Down, int DownPos, int Up, int UpPos){
	
	if (up == down)
		return ;
	else
	{
		float DValue = 1.0 * (UpPos - DownPos) / (Down - Up);
		
		for (int i = Down; i >= Up; i--)
		{
			lineinf[i].LeftPos = (int)(DownPos + (Down - i) * DValue);
			LimitPr(lineinf[i].LeftPos);
		}
	}
}

void base::patchMiddle(int Down, int DownPos, int Up, int UpPos){
	
	if (up == down)
		return ;
	else
	{
		float DValue = 1.0 * (UpPos - DownPos) / (Down - Up);
		
		for (int i = Down; i >= Up; i--)
		{
			lineinf[i].MiddlePos = (int)(DownPos + (Down - i) * DValue);
			LimitPr(lineinf[i].MiddlePos);
		}
	}
}

void base::patchRight(int Down, int DownPos, int Up, int UpPos){
	
	if (up == down)
		return ;
	else
	{
		float DValue = 1.0 * (UpPos - DownPos) / (Down - Up);
		
		for (int i = Down; i >= Up; i--)
		{
			lineinf[i].RightPos = (int)(DownPos + (Down - i) * DValue);
			LimitPr(lineinf[i].RightPos);
		}
	}
}