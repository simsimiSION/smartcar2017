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
	
	int  row = 80;    //����������
	
	for (row = 80; row >= left; row--)                                  //�ҵ�һ�е���߽��
	{
		if (picture[59][row] != picture[59][row + 1])                     //�ж��������Ƿ���ͬ�ķ�������߽��
		{
			if (picture[59][row + 1] == picture[59][row + 2])               //�˲�
			{
				lineinf[59].LeftPos = row;
				break;
			}
			else if (row == left)                                       //���û�����ж���߽��Ϊ��߽�
				lineinf[59].LeftPos = left;
			else {}
		}
		else if (row == left)                                            //���û�����ж���߽��Ϊ��߽�
			lineinf[59].LeftPos = left;
		else {}
	}
	for (row = 80; row <= right; row++)                                  //�ҵ�һ�е��ұ߽��
	{
		if (picture[59][row] != picture[59][row - 1])                      //�ж��������Ƿ���ͬ�ķ�������߽��
		{
			if (picture[59][row - 1] == picture[59][row - 2])                //�˲�
			{
				lineinf[59].RightPos = row;
				break;
			}
			else if (row == right)                                       //���û�����ж��ұ߽��Ϊ�ұ߽�
				lineinf[59].RightPos = right;
			else {}
		}
		else if (row == right)                                           //���û�����ж��ұ߽��Ϊ�ұ߽�
			lineinf[59].RightPos = right;
		else {}
	}
}

void base::otherLine(int effect){
	/*
	��ر���
	*/
	int column = 58;   //����������
	int row = 80;      //����������
	
	/*
	����㷨
	�㷨˵����������ص��Ϸ��ĵ����ɫ���жϱ߽硣���Ϊ��ɫ�����������Ѱ�ң���������Ա߽�Ѱ��
	����һ��������߽���˵���ܳ��ֺܶ���߽綼Ϊleftֵ����������Զ�����߽���˵��δ�ҵ��߽����(row == (left))��������ұ߽�ͬ��
	*/
	for (column = 58; column >= effect; column--)
	{
		if (picture[column][lineinf[column + 1].LeftPos] == 255)                     //�ж��Ϸ������ɫ����ɫ��
		{
			
			if (lineinf[column + 1].LeftPos == right)
				lineinf[column].LeftPos = right;
			else
			{
				for (row = lineinf[column + 1].LeftPos; row < right; row++)              //������߽���˵���Ϸ���Ϊ��ɫ��������Ѱ��
				{
					if (picture[column][row] != picture[column][row + 1])
					{
						lineinf[column].LeftPos = row;
						break;
					}
					else if (row == (right - 1) || (row == (right)))                      //���û���ҵ���ֵΪ�ұ߽�
						lineinf[column].LeftPos = right;
					else {}
				}
			}
			
		}
		else                                                                          //�ж��Ϸ������ɫ����ɫ��
		{
			for (row = lineinf[column + 1].LeftPos; row >= left; row--)               //������߽���˵���Ϸ���Ϊ��ɫ��������Ѱ��
			{
				if (picture[column][row] != picture[column][row + 1])
				{
					lineinf[column].LeftPos = row;
					break;
				}
				else if ((row == (left + 1)) || (row == (left)))                      //���û���ҵ���ֵΪ��߽�
					lineinf[column].LeftPos = left;
				else {}
				
			}
		}
		//�ұ߽�Ҳ��һ���ģ���ע���ˣ�̫���ˣ� ^0^
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
	
	if ( effect < down )        //�ж����ߴ������ƣ����߹�һ��
	{
		if (lineinf[down].MiddlePos < lineinf[probe].MiddlePos && probe != up)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, right);
		else if (lineinf[down].MiddlePos > lineinf[probe].MiddlePos && probe != up)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, left);
		else if (probe != 0)
			base::patchMiddle(probe, lineinf[probe].MiddlePos, up, (left + right) / 2);
		else {}
		
		if (structRamp != 0)   //�µ����м䲹��
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