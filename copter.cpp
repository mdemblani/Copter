/**
 * Program implementing the copter game
 * Created and Developed by - Manish M. Demblani
 * Email - mdemblani@gmail.com
 * Github Repo - https://github.com/mdemblani/Copter
 */

#include<graphics.h>
#include<conio.h>
#include<iostream.h>
#include<dos.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

union REGS in,out;
void show_mouse()
{	in.x.ax=1;
	int86(0x33,&in,&out);
}

void hide_mouse()
{	in.x.ax=2;
	int86(0x33,&in,&out);
}
int detect_click()
{	in.x.ax=3;
	int86(0x33,&in,&out);
	return (out.x.bx);
}

void plot_heli()
{	//Helicopter Window
	setfillstyle(1,9);
	sector(190,212,0,90,12,7);

	//Helicopter Body Outline
	line(100,200,150,200);
	line(100,200,150,210);
	ellipse(170,217, 10, 120,40, 20);
	arc(200,217,270,30,10);
	ellipse(190,207, 189,285,40, 20);

	setfillstyle(1,4);
	floodfill(110,201,15);


	//Wings
	ellipse(175,186,0,360,50,10);

	setfillstyle(1,7);
	floodfill(176,187,15);

	//Tail Fan
	putpixel(175,186,WHITE);
	circle(100,200,10);

	setfillstyle(1,7);
	floodfill(101,201,15);

	//Base
	line(160,235,210,235);
	line(180,229,170,235);
	line(190,229,200,235);
}

void start_screen()
{
	plot_heli();
	setfillstyle(1,LIGHTGREEN);
	bar(0,0,getmaxx(),50);
	bar(0,getmaxy()-50,getmaxx(),getmaxy());
	setcolor(11);	//11 - LIGHTCYAN
	settextstyle( TRIPLEX_FONT, HORIZ_DIR, 6 );
	settextjustify( CENTER_TEXT, CENTER_TEXT );
	outtextxy(300,100,"COPTER");
	settextstyle( TRIPLEX_FONT, HORIZ_DIR, 4 );
	settextjustify( RIGHT_TEXT, CENTER_TEXT );
	outtextxy(550,200,"CLICK TO PLAY");
	settextstyle( TRIPLEX_FONT, HORIZ_DIR,1 );
	settextjustify( LEFT_TEXT, CENTER_TEXT );
	outtextxy(200,300," - Click and Hold the LEFT mouse button");
	outtextxy(280,320,"to move the copter up");
	outtextxy(200,340," - Release to go down");
	outtextxy(200,360," - Press any key to quit");
}

int plot_bar(int left)
{       int top;	//top - coordinate
	randomize();
	do
	{	top=random(280);
	}while(top<50);
	setfillstyle(1,LIGHTGREEN);
	bar(left,top,left+25,top+100);
	return top;
}

void move_bar(int left,int top)
{	setfillstyle(1,BLACK);
	bar(left+1,top,left+1+25,top+100);
	setfillstyle(1,LIGHTGREEN);
	bar(left,top,left+25,top+100);
};

int check_hit(int wleft,int wtop,int top,int bottom)
{
	/*left=90;
	  top=176;
	  right=225;
	  bottom=235;
	*/
	top=top-2;
	bottom=bottom+2;
	//Front and Back Collision
	if(((top)<=(wtop+100))&&((bottom)>=wtop))
	{	if(wleft==225||(wleft+25)==90)
			return 1;

	}
	//Base Collision

	if((wleft<=225)&&(wleft>=90))
	{	if(wtop==(bottom))
			return 1;
	}

	if(((wleft+25)>=90)&&((wleft+25)<225))
	{	if(wtop==(bottom))
			return 1;
	}

	//top collision

	if((wleft<=225)&&(wleft>=90))
	{	if((wtop+100)==(top))
			return 1;
	}
	if(((wleft+25)>=90)&&((wleft+25)<225))
	{       if((wtop+100)==(top))
			return 1;
	}


	/*//Base Collision
	//line(160,235,210,235);
	if(wtop==bottom)
	{	if((wleft<=210)&&(wleft>=160))
		{
			return 1;
		}

		if(((wleft+25)>=160)&&((wleft+25)<210))
		{
			return 1;
		}
	}
	//Tail Span Collision
	if(wtop==(bottom-25))
	{	if((wleft<=140)&&(wleft>=110))
		{
			return 1;
		}

		if(((wleft+25)>=110)&&((wleft+25)<160))
		{
			return 1;
		}
	}
	//Wing Collision

	/*{       //Main Wing
		if((wleft<=225)&&(wleft>=175))
		{      if(wtop+100==top)
			return 1;
		}

		if(((wleft+25)>=175)&&((wleft+25)<225))
		{      if(wtop+100==top)
			return 1;
		}
	}*/
	/*if((wleft>=90)&&(wleft<225))
		if((wtop+100)==top)
			return 1;
	if(((wleft+25)>=90)&&((wleft+25)<=225))
		if((wtop+100)==top)
			return 1;

	//Tail Wing Collision
	if(wtop+100==top)
	{       //Main Wing
		if((wleft<=175)&&(wleft>=90))
		{
			return 1;
		}

		if(((wleft+25)>=90)&&((wleft+25)<175))
		{
			return 1;
		}
	}*/
	return 0;
}

int start_game(int left,int top,int right,int bottom)
{
	unsigned int size,score=0;
	char *str;
	int i,click,flag=0;
	int wall[2][2];	/*bar[][] - { {left,top},
				      {left,top}
				    } - coordinates
			*/
	void *buffer;

	setcolor(15);	//15-WHITE
	plot_heli();
	setfillstyle(2,LIGHTGREEN);
	bar(0,0,getmaxx(),50);
	bar(0,getmaxy()-50,getmaxx(),getmaxy());

	size=imagesize(left,top,right,bottom);
	//rectangle(left,top,right,bottom);
	buffer=malloc(size);
	getimage(left,top,right,bottom,buffer);
	//To store the image
	size=imagesize(left,top,right,bottom);
	buffer=malloc(size);
	getimage(left,top,right,bottom,buffer);

	//Initial plotting of obstacle
	wall[0][0]=590;wall[1][0]=1090;
	wall[0][1]=plot_bar(wall[0][0]);
	wall[1][1]=plot_bar(wall[1][0]);
	while(flag==0)
	{       click=detect_click();
		putimage(left,top,buffer,XOR_PUT);
		if(click==1)
		{	top=top-2;bottom=bottom-2;
			putimage(left,top,buffer,XOR_PUT);
		}
		else
		{	top=top+2;bottom=bottom+2;
			putimage(left,top,buffer,XOR_PUT);
		}
		//Plotting the obstacles
		if(wall[0][0]<=(-50))
		{       //wall[0][0]=590;
			wall[0][0]=wall[1][0]+400;
			wall[0][1]=plot_bar(wall[0][0]);
		}
		if(wall[1][0]<=(-50))
		{       //wall[1][0]=1090;
			wall[1][0]=wall[0][0]+400;
			wall[1][1]=plot_bar(wall[1][0]);
		}
		wall[0][0]--;wall[1][0]--;
		for(i=0;i<=1;i++)
		{	if((wall[i][0]<=right)&&((wall[i][0]+25)>=left))
				flag=check_hit(wall[i][0],wall[i][1],top,bottom);
		}
		/*reduce left coordinate of the bar to create the illusion
		that helicopter is moving*/
		move_bar(wall[0][0],wall[0][1]);
		move_bar(wall[1][0],wall[1][1]);
		if(top<=50||bottom>=(getmaxy()-50))
			flag=1;
		score++;
		delay(5);
	}
	return score;
}

void main()
{       int click=0,s;
	int top,left,bottom,right;
	int gdriver=DETECT,gmode;
	initgraph(&gdriver,&gmode,"C:\\TURBOC3\\BGI");
	cleardevice();
	in.x.ax=0;
	int86(0x33,&in,&out);
	if(out.x.ax==0)
		printf("\nMouse Failed to Initialize");
	else
	{       show_mouse();
		start_screen();
		detect_click();
		do
		{
			click=detect_click();
			delay(100);
		}while(click!=1);
		delay(2000);
		//cleardevice();
		clearviewport();
		hide_mouse();
		//Set co-ordinates
		left=90;
		top=176;
		right=225;
		bottom=235;
		s=start_game(left,top,right,bottom);
		cleardevice();
		settextstyle( TRIPLEX_FONT, HORIZ_DIR, 4 );
		//settextjustify( CENTER_TEXT, CENTER_TEXT );
		outtextxy(150,50,"GAME OVER!!!");
		outtextxy(150,100,"Score = ");
		s=s/10;
		char *str;
		itoa(s,str,10);
		outtextxy(280,100,str);
	}
	getch();
	//while(out.x.bx!=1)
	closegraph();
}
