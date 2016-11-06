// Mindmelt //
#include <fcntl.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graph.h>
#include <i86.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void disp_debug(char *mess, int debug);
void read_map(char *map_nam);
void disp_map(void);
void display_pos(int x,int y);
void get_mouse_and_keys(void);
void do_keys(void);
void read_obj(char *obj_file);
void move_ob(int ob,int x,int y, int mode);
void init_disp(void);
void disp_pos(int x, int y);
void clrs(void);
char *strip(char *b);
void disp_inv(int you);
void load_init();
void set_palette();
void inst_mouse();
void ex(char *mess);
void display();
void load_game(char save_game[]);
void load_map(char map_nam[]);
void set_top_objs();
void read_line(char *buff, FILE *fp);
void read_control(char control_name[]);
char *strip(char line[]);
int enter_map();
void read_map(char map_nam[]);
void read_font(char ft_name[] );
void read_sprites(char sp_name[] );
void read_obj(char obj_name[] );
void init_disp(void);
void disp_screen();
void get_input();
void disp_inv(int who);
void do_keys();
void move_key(int mov);
void mouse_zone();
void do_mouse();
int vec_conv(int x, int y);
void do_adjacent(int zx,int zy);
int get_drop_ob(int x,int y);
int can_drop(int x, int y);
int can_get(int ob);
void get_put_backpack(int get);
void attack_monst(int mon);
void talk_to(int person);
int move_fwd(int x,int y);
void change_sq(char sq, int x, int y);
void from_to(int ob, int x, int y, int xx, int yy, int mode);
void move_all(int x,int y, int xx, int yy, int mode);
void add_code(char *cod,int x,int y);
void do_code();
int exec_code(int cp,int tx, int ty);
void code_get4p(char *p, int *p1, int *p2, int *p3, int *p4);
void code_message(char *p);
void code_change(int x,int y,int sq);
void code_toggle(int x, int y, int sq1, int sq2);
void code_moveall(int x, int y, int x1, int y1, int mode);
int code_in(int x, int y, int ob);
void print6(int x, int y, int back, int fore, char line[]);
void outch6(int x, int y, int back, int fore, char c);
void copy_screen();
void _copy_screen(void);
void gotoxy(int x, int y);
void clrs();
void sprite(int sp, int x, int y);
void rem_mouse();
void inst_mouse();
char pix_colour(int x, int y);
char input6(int x, int y, int b, int f, char buff[], int len);
void cast_mindmelt();
void cast_circle_tele();
void cast_direction();
void cast_xy();
void cast_light();
void cast_xray();
void cast_water();
void cast_stun();
void cast_jump();
void cast_zap();
void cast_heal();
void cast_field();
void cast_back();
void cast_overview();
void cast_spell_handler(int subcode, int state);
void control_handler(int subcode, int state);
void throw_handler(int x, int y, int subcode);
void adjacent_handler(int x, int y, int subcode);
void inventory_handler(int x, int y, int subcode);

#define SPELL_OFF 1
#define SPELL_RESET 2
#define SPELL_SET 3

/*************** Windows stuff *****************/
typedef struct Zone;
typedef struct Button;
typedef struct Area;

typedef struct Window
{
	int x,y; // Coords 
	int h,w; // Height and width
	struct Window *next; // pointer to next window
	struct Zone *zone; // pointer to zone
	struct Button *button;
	struct Area *area;
};

struct System 
{
	struct Window *head; // pointer to 1st window
	int screen_width, screen_height;
} winsystem;

#define BUTTON_OFF 1
#define BUTTON_UP  2
#define BUTTON_DOWN 3
typedef struct Button
{
	int x,y; // Relative to parent window
	int active_icon;
	int inactive_icon;
	int state; // off / on  /down
	int subcode;
	void (*handler)(); // pointer to handler function
	struct Window *parent;
	struct Button *next;
};

typedef struct Zone
{
	int x,y;
	int h,w;
	int subcode;
	void (*handler)(); // pointer to handler function
	struct Window *parent;
	struct Zone *next;
};

typedef struct Area
{
	int x,y;
	int h,w;
	struct Window *parent;
	struct Area *next;
};

void WindowInit()
{
	winsystem.head = NULL;
}

void AddWindow(struct Window *win, int x, int y, int w, int h)
{
	struct Window *next;

	next = winsystem.head;
	win->x = x;
	win->y = y;
	win->w = w;
	win->h = h;
	win->next = next;
	winsystem.head = win;
}

void DispButton(struct Button *but)
{
	int x,y;
	struct Window *parent;

	if(but==NULL) return;
	x = but->x;
	y = but->y;
	parent = but->parent;
	x += parent->x;
	y += parent->y;
	switch(but->state)
	{
		case BUTTON_OFF:
			sprite(but->inactive_icon,x,y);
			sprite(150,x,y);
			break;
		case BUTTON_UP:
			sprite(but->active_icon,x,y);
			sprite(151,x,y);
			break;
		case BUTTON_DOWN:
			sprite_down(but->active_icon,x,y);
			sprite(152,x,y);
			break;
	}
}

void AddButton(struct Window *win, struct Button *but, int x, int y, 
			   int inact_icon, int act_icon, int state, 
			   int subcode, void (*handler)() )
{
	struct Button *next;

	next = win->button;
	win->button = but;
	but->x = x;
	but->y = y;
	but->inactive_icon = inact_icon;
	but->active_icon = act_icon;
	but->next = next;
	but->parent = win;
	but->state = state;
	but->subcode = subcode;
	but->handler = handler;
	DispButton(but);
}

void AddButtonHandler(struct Button *but, int subcode, void (*handler)())
{
	but->subcode = subcode;
	but->handler = handler;
}

void SetButton(struct Button *but, int state)
{
	if(but==NULL) return;
	but->state = state;
	DispButton(but);
}

int GetButton(struct Button *but)
{
	if(but==NULL) return NULL;
	return but->state;
}

void AddZone(struct Window *win, struct Zone *zone, 
			 int x, int y, int w, int h, int subcode, void(*handler)())
{
	struct Zone *next;

	next = win->zone;
	win->zone = zone;
	zone->x = x;
	zone->y = y;
	zone->w = w;
	zone->h = h;
	zone->subcode = subcode;
	zone->handler = handler;
	zone->next = next;
}

void DoMouseEvent(int mous_x, int mous_y)
{
	int x,y;
	struct Window *win;
	struct Button *but;
	struct Zone *zon;
	int but_state;

	//find window
	win = winsystem.head;
	while(win!=NULL)
	{
		if( mous_x < win->x || mous_x > win->x + win->w ||
				mous_y < win->y || mous_y > win->y + win->h )
			win = win->next;
		else
			break;
	}
	if(win==NULL) return; // Not in a window

	//go through buttons
	but = win->button;
	x = mous_x - win->x ;
	y = mous_y - win->y ;
	while(but!=NULL)
	{
		if( x < but->x || x > but->x + 16 ||
			y < but->y || y > but->y + 16 )
				but = but->next;
		else
		{
			but_state = GetButton(but);
			if(but_state==BUTTON_OFF)
				return;
			if(but_state==BUTTON_UP)
			{
				if(but->handler) 
				{
					SetButton(but,BUTTON_DOWN);
					(but->handler)(but->subcode,but_state);
				}
				return;
			}
			if(but_state==BUTTON_DOWN)
			{
				if(but->handler) 
					(but->handler)(but->subcode,but_state);
				return;
			}
			return;
		}
	}
	
	//go through zones
	zon = win->zone;
	while(zon!=NULL)
	{
		if( x < zon->x || x > zon->x + zon->w ||
			y < zon->y || y > zon->y + zon->h )
				zon = zon->next;
		else
		{
				if(zon->handler)
					(zon->handler)(x-zon->x,y-zon->y,zon->subcode);
				return;
		}
	}
}

int mouse_event = 0 ;
int mouse_code = 0 ;
int mouse_x = 0 ;
int mouse_y = 0 ;
int mouse_l = 0 ;
int mouse_r = 0 ;

struct SREGS sregs;
union REGS inregs, outregs;

#pragma off (check_stack)
void _loadds far click_handler( int max, int mcx, int mdx )
{
#pragma aux click_handler parm [EAX] [ECX] [EDX]
	mouse_event = 1;
	mouse_code = max;
	mouse_x = mcx>>1 ;
	mouse_y = mdx ;
}
#pragma on (check_stack)

void inst_mouse()
{
	/* Install mouse handler */
	inregs.w.ax = 0xC;
	inregs.w.cx = 0x1 | 0x2 | 0x4 | 0x8 | 0x10;
	inregs.x.edx = FP_OFF(click_handler);
	sregs.es = FP_SEG(click_handler);
	int386x( 0x33, &inregs, &outregs, &sregs );
}

#define MOVE_NORMAL 1
#define MOVE_THROW 2
#define MOVE_DROP 3

#define SCREEN_SEC 0xa000
#define SCREEN_LIN ((SCREEN_SEC)<<4)
#define FONT6SIZE (6*6*96)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define MESS_HEIGHT 37
#define MESS_LINE 193

#define P_SKIN 0x001b203fL
#define P_GOLD 0x00002d3fL
#define P_DGREY 0x00181818L
#define P_GREEN 0x00003f00L
#define P_BLACK 0
#define P_YELLOW 0x00003f3fL
#define P_RED 0x0000003fL
#define P_MAGENTA 0x003f003fL
#define P_BLUE 0x003f0000L
#define P_CYAN 0x003f3f00L
#define P_BROWN 0x00001828L
#define P_LGREY 0x00303030L
#define P_DGREEN 0x00002800L
#define P_GREY 0x00202020L
#define P_WHITE 0x003f3f3fL

#define TRANS 0
#define GREEN 1
#define BLACK 2
#define YELLOW 3
#define RED 4
#define MAGENTA 5
#define BLUE 6
#define CYAN 7
#define GREY 8
#define BROWN 9
#define SKIN 10
#define LGREY 11
#define GOLD 12
#define DGREEN 13
#define DGREY 14
#define WHITE 15

#define MAXOVERLAY 512
#define SZ 64
#define wid 9
#define MAXMAPS 20
#define MAXOBJS 256
#define OBJS 61
#define MOUS_SPR 183
#define DEAD_SPR 184-OBJS
#define BACK_SPR 153
#define REGEN_MONST 20
#define SMELL 5
#define CIRCLE_X 22
#define CIRCLE_Y 22
#define CIRCLE_DESC "World                          "
#define monsters 180
#define monend 255
#define people 1
#define peoend 99
#define object 100
#define objend 179
#define spells 14
#define melts 6
#define talkmax 300
#define trigmax 70
#define codemax 300
#define addcodemax 100
#define sgn(x) ((x)>0?1:((x)<0?-1:0))
#define top_ob(x,y) top_obj[y][x]
#define map(x,y) map_sq[y][x]

/*constants*/
char tiles[]={".$ o.t)(UzX#\\/mSbB[]_GZ@T%+'*|YMn~^:?!=-WxOFHV<>œhApDIdlLfEPrQ"};

char thru[]={   1,1,0,0,0,0,0,0,0,0,
				0,1,1,1,1,1,1,1,1,1,
				0,0,0,0,0,0,0,0,0,1,
				1,1,0,0,0,0,0,0,0,0,
				0,0,1,0,0,0,0,0,0,0,
				0,0,0,0,0,0,0,1,1,1,
				0,0 };

char mov[]={    1,1,0,0,0,0,0,0,0,1,
				0,1,1,1,1,0,1,1,1,1,
				0,1,1,0,0,0,1,0,0,0,
				0,1,0,0,1,0,1,0,0,0,
				0,0,1,1,0,0,0,0,0,0,
				1,1,1,1,1,1,0,1,1,1,
				0,1};
int a[4]={      1,0,-1,0    };
int b[4]={      0,-1,0,1    };
int xi[8]={ 0,1,1,1,0,-1,-1,-1 };
int yi[8]={ -1,-1,0,1,1,1,0,-1 };

			   //0  1  2  3  4  5  6  7  8  9
char pix[]={     0, 0,10,10,10,10,10,10,10,10,
				14,11,11,11,11,11,11,11, 9, 9, //10
				 9, 8, 4, 8, 7, 8,15, 1,13,13, //20
				 9,15, 8, 6, 6, 7,15, 3, 9, 9, //30
				 6,15,14, 9, 5, 5,10,10, 0,10, //40
				10,10,10,10,10,10,10,11,11,11, //50
				10,14};                        //60
			   
/* global variables */
char sprite_back[256]={0};
char trig[trigmax+1][9];
int trig_x[trigmax+1];
int trig_y[trigmax+1];
int  code_trig[trigmax+1];
char code[codemax+1][40];
char addcode[addcodemax+1][9];
int addcode_x[addcodemax+1];
int addcode_y[addcodemax+1];
int talk[peoend-people+1+1];
char talk_topic[talkmax+1][5];
char say[talkmax+1][80];
char obj_names[objend-object][40];

char blank80[81]="                                                                             ";

char *spell_name[spells+1]={"","Mindmelt","Circle","Direction",
						"Position","Light","Xray","Water","Stun","Jump",
						"Zap","Heal","Forcefield","Back","Overview"
						};
char spell_valid[spells+2] = " mcdplxwsjzhfbo";
char map_description[80];

long seed=412294L;
int x_you=22;
int y_you=22;
char inkey,inkey1;
int zone_x,zone_y;
int thru_all=0;
int you=1;
int vxy[2][99];
int vmsk[2][wid+1][wid+1];
int level=1;
int dark_sw=0;
int disp_sw=0;
int inv_sw=1;
int dark=0;
int xray=0;
int water=0;
int feeld=0;
int stun=0;
int light=0;
int see_all=0;
int lev=0;
int back_x=0;
int back_y=0;
char back_map[20];
int r,dat;
int maxmaps;
int mx=160,my=96;
int mous_ptr = MOUS_SPR;
int ob_got=0;
char map_name[20];
unsigned char drr=0;
int spell[spells+1];
int melt[melts+1];
int cheat=0;
int codetop=1,codebot=1,trigend=0;

char dead_map[10];
int dead_x, dead_y;

struct {
	char name[8];
	int xin;
	int yin;
	int xout;
	int yout;
	char desc[80];
} map_entry[MAXMAPS+1];

struct ovr_struct 
{
	char x;
	char y;
	char level;
	char sq;
} overlay[MAXOVERLAY], overlay_temp[MAXOVERLAY];

struct {
char x[256];
char y[256];
char up[256];
char dn[256];
char spr[256];
char str[256];
char z[256];
} obj;

char top_obj[SZ+4][SZ];
char map_sq[SZ][SZ];
char map_tmp[SZ][SZ];
char spr[64000];
long back_screen[16000];
char font6[FONT6SIZE];
char *SCREEN; 

#define MOVE_SIZE 11
#define MOVE_HALF_SIZE (MOVE_SIZE/2)
int move_table[MOVE_SIZE][MOVE_SIZE];
#define MOVE_STACK_SIZE (MOVE_SIZE*MOVE_SIZE)
#define MOVE_X 0
#define MOVE_Y 1
#define MOVE_C 2
int move_list[MOVE_STACK_SIZE][3];

/***********************************************/
/*** Windows routines test code ***/

struct Button spell_but[14];
struct Window spell_win;

struct Button control_but[10];
struct Window control_win;

struct Zone throw_zon[4];
struct Zone adjacent_zon;
struct Window map_win;

struct Zone inventory_zon;
struct Window inventory_win;

void set_spells()
{
	int s,sp;

	for(sp=1;sp<=spells;sp++)
		{
		  if(spell[sp]==1) 
		  {
			switch(sp)
			{
				case 5:
					if(light>0)
						s=SPELL_SET;
					else
						s=SPELL_RESET;
				break;
				case 6:
					if(xray>0)
						s=SPELL_SET;
					else
						s=SPELL_RESET;
				break;
				case 7:
					if(water>0)
						s=SPELL_SET;
					else
						s=SPELL_RESET;
				break;
				case 8:
					if(stun>0)
						s=SPELL_SET;
					else
						s=SPELL_RESET;
				break;
				case 12:
					if(feeld>0)
						s=SPELL_SET;
					else
						s=SPELL_RESET;
				break;
				default:
					s=SPELL_RESET;
					break;
			}
			set_spell(sp,s);
		  }
		  else
			set_spell(sp,SPELL_OFF);
		}
}

void create_spell_buttons()
{
	int i,j,s,sp;

	sp=BUTTON_OFF;
	AddWindow(&spell_win,160,112,7*16,2*16);
	for(j=0;j<2;j++)
		for(i=0;i<7;i++)
		{
			s=i+j*7;
			AddButton(&spell_win,&spell_but[s],
						i*16,j*16,150,136+s,sp,s+1,cast_spell_handler);
		}
}

void create_control_buttons()
{
	int i,j,s,sp;

	sp=BUTTON_UP;
	AddWindow(&control_win,17*16,6*16,3*16,3*16);
	for(j=0,s=0;j<3;j++)
		for(i=0;i<3;i++,s++)
		{
			AddButton(&control_win,&control_but[s],
				i*16,j*16,150,127+s,sp,s,control_handler);
		}
}

void create_map_zones()
{
	AddWindow(&map_win,0,0,9*16,9*16);
	AddZone(&map_win,&throw_zon[0],2*16,0*16,5*16,2*16,0,throw_handler);
	AddZone(&map_win,&throw_zon[1],7*16,2*16,2*16,5*16,2,throw_handler);
	AddZone(&map_win,&throw_zon[2],2*16,7*16,5*16,2*16,4,throw_handler);
	AddZone(&map_win,&throw_zon[3],0*16,2*16,2*16,5*16,6,throw_handler);
	
	AddZone(&map_win,&adjacent_zon,3*16,3*16,3*16,3*16,0,adjacent_handler);
}

void create_inventory_zone()
{
	AddWindow(&inventory_win,10*16,0*16,10*16,3*16);
	
	AddZone(&inventory_win,&inventory_zon,0,0,10*16,3*16,0,inventory_handler);
}

/***********************************************/
void main(void)
{
	clock_t time;

	_setvideomode(_MRES256COLOR);
	SCREEN = (char *) back_screen;
	
	WindowInit();
	
	load_init();
	init_disp();
	inst_mouse();
	set_start_palette();
	disp_start_screen();
	disp_border();
	
	create_control_buttons();
	create_spell_buttons();
	create_map_zones();
	create_inventory_zone();
	set_spells();
	
	display();
	set_palette();
	x_you = obj.x[you];
	y_you = obj.y[you];
	do{
		do_auto();
		do_peo();
		do_mon();
		do_dead();
		scroll_mess();
		display();
		//time = clock() - time;
		//disp_debug("Time",time);
		get_input();
		//time = clock();
		do_keys();
		do_code();
	}while (confirm_exit());
	rem_mouse();
	ex("Goodbye From Mindmelt");
}

disp_start_screen()
{
	int screen_h;
	int time;

	SCREEN = (char *) SCREEN_LIN;
	screen_h = open("data\\start.pic",O_BINARY);
	if (screen_h==-1) ex("\nStart screen open error\n");
	read(screen_h,(char*)SCREEN,64000);
	close(screen_h);
	time = clock();
	do {} while(!kbhit() && clock()-time<CLOCKS_PER_SEC*5);
	SCREEN = (char *) back_screen;
}

void ex(char *mess)
{    
	_setvideomode(_DEFAULTMODE);
	printf("%s",mess);
	exit(0);
}

void load_init()
{
	set_dead_map("world",CIRCLE_X,CIRCLE_Y);
	read_obj("data\\mminit.obj");
	read_sprites("data\\mmsp.pc");
	read_font("data\\font6.pc");
	read_obj_names("data\\obj.nam");
	load_game("data\\saveinit.gam");
}

void load_the_game()
{
	if(confirm("OK to load?> "))
		return;
	read_obj("data\\mindmelt.obj");
	load_game("data\\save.gam");
	load_overlay("data\\map.ovr");
	reset_map_overlay();
	inv_sw = 1;
	set_spells();
}

void load_game(char save_game[])
{
	FILE *fp;
	char buff[80];
	int i;

	fp = fopen(save_game,"r");
	
	read_line( map_name, fp );
	read_line( buff, fp ); drr=(unsigned char)atoi(buff) ;    
	read_line( buff, fp ); feeld=atoi(buff) ;    
	read_line( buff, fp ); water=atoi(buff) ;    
	read_line( buff, fp ); ob_got=atoi(buff);
	read_line( buff, fp ); //ste sound ;    
	read_line( buff, fp ); stun=atoi(buff) ;    
	read_line( buff, fp ); xray=atoi(buff) ;    
	read_line( buff, fp ); light=atoi(buff) ;    
	read_line( buff, fp ); lev=atoi(buff) ;    

	for(i=1;i<=spells;i++)
	{
		read_line( buff, fp ); spell[i]=atoi(buff) ;    
	}

	for(i=1;i<=melts;i++)
	{
		read_line( buff, fp ); melt[i]=atoi(buff) ;    
	}
	read_line( buff,fp); strcpy( dead_map, buff);
	read_line( buff,fp); dead_x = atoi(buff);
	read_line( buff,fp); dead_y = atoi(buff);

	fclose(fp);
	map_description[0]=0;
	load_map(map_name);
}

void load_map(char map_nam[])
{
	char buff[30];    
	
	strcpy(map_name,map_nam);
	sprintf(buff,"data\\%s.map",map_nam);
	read_map(buff);
	sprintf(buff,"data\\%s.ctl",map_nam);
	read_control(buff);
	set_top_objs();
}

void set_top_objs()
{
	int i,j;

	/* Clear top objects from map part */
	for(i=0;i<SZ;i++)
		for(j=0;j<SZ;j++)
			top_ob(j,i) = 0;
	/* Re-init top objects for new level */
	for(i=1;i<MAXOBJS;i++)
		if( obj.z[i] == level && obj.up[i] == 0 )
			top_ob(obj.x[i],obj.y[i]) = i;
}

void read_line(char *buff, FILE *fp)
{
	fgets( buff, 80, fp);
	while(buff[0]==';') fgets( buff, 80, fp);
	strip(buff);
}

void save_the_game()
{
	if(confirm("OK to save?> "))
		return;
	save_overlay("data\\map.ovr");
	savegame("data\\save.gam");
	write_obj("data\\mindmelt.obj");
}

void savegame(char save_game[])
{
	FILE *fp;
	char buff[80];
	int i;

	fp = fopen(save_game,"w");
	
	fprintf(fp,"%s\n", map_name);
	fprintf(fp,"%d\n",drr );
	fprintf(fp,"%d\n",feeld );
	fprintf(fp,"%d\n",water );
	fprintf(fp,"%d\n",ob_got );
	fprintf(fp,"0\n" );
	fprintf(fp,"%d\n",stun );
	fprintf(fp,"%d\n",xray );
	fprintf(fp,"%d\n",light );
	fprintf(fp,"%d\n",lev );

	for(i=1;i<=spells;i++)
	{
		fprintf(fp,"%d\n",spell[i] );
	}
	
	for(i=1;i<=melts;i++)
	{
		fprintf(fp,"%d\n",melt[i] );
	}
	fprintf(fp,"%s\n",dead_map );
	fprintf(fp,"%d\n",dead_x );
	fprintf(fp,"%d\n",dead_y );

	fclose(fp);
}

void read_control(char control_name[])
{
	FILE *fp;
	char buff[80];
	char *p;
	int i,j,map,pers;

	fp = fopen(control_name,"r");
	
	read_line( buff, fp ); level = atoi(buff);
	read_line( buff, fp ); dark_sw = atoi(buff);

	read_line( buff, fp );
	for( map=1; buff[0] != '@';map++)
	{
		strcpy(map_entry[map].name,buff); 
		read_line( buff, fp ); map_entry[map].xin = atoi(buff); 
		read_line( buff, fp ); map_entry[map].yin = atoi(buff); 
		read_line( buff, fp ); map_entry[map].xout = atoi(buff); 
		read_line( buff, fp ); map_entry[map].yout = atoi(buff); 
		read_line( buff, fp ); strcpy(map_entry[map].desc,buff); 
/*        printf("%s %d %d %d %d %s", map_entry[map].name,
									map_entry[map].xin,
									map_entry[map].yin,
									map_entry[map].xout,
									map_entry[map].yout,
									map_entry[map].desc);
*/        
		read_line( buff, fp ); 
	}
	maxmaps=map-1;
	
	// Read in code
	read_line( buff, fp ); 
	for(i=1,j=1;buff[0]!='@';j++)
	{
		p=strtok(buff," \t\r\n");
		strcpy(trig[j],p);
		p=strtok(NULL," \t\r\n");
		trig_x[j]=atoi(p);
		p=strtok(NULL," \t\r\n");
		trig_y[j]=atoi(p);
		
		code_trig[j]=i;
		for(;buff[0]!='.';i++)
		{
			read_line( buff, fp ); 
			strcpy(code[i],buff);
		}
		read_line( buff, fp );
	}
	trigend=j-1;
	if(trigend>trigmax) ex("Too much code to load");
	
	// Read Talk
	for(i=1;;)
	{
		read_line( buff, fp );
		pers = atoi(buff);
		if(pers==0) break;
		talk[pers]=i;
		for(;;i++)
		{
			if(i>=talkmax) ex("Too much talk");
			read_line( buff, fp );
			if(buff[0]=='@') break;
			strncpy(talk_topic[i],buff,4);
			strcpy(say[i],buff+5);
		}
	}
			
	fclose(fp);
}

char *strip(char line[])
{
	line[strlen(line)-1]='\0';
	return (char*)line; 
}

int enter_map()
{
	int map=1;
	char message[80];

	for(map=1 ; map<=maxmaps; map++)
	{
		if(x_you == map_entry[map].xin && y_you == map_entry[map].yin)
		{
			sprintf(map_description,"%-20s",map_entry[map].desc);
			sprintf(message,"Entering %s",map_description);
			disp_mess(BLACK,DGREEN,message);
			set_dead_map(map_entry[map].name, map_entry[map].xout, map_entry[map].yout);
			goto_map(map_entry[map].name, map_entry[map].xout, map_entry[map].yout);
			return 1;
		}
	}
	return 0;
}

void goto_map(char *map, int x, int y)            
{
	move_ob(you,0,0,MOVE_NORMAL);
	set_map_overlay();
	load_map(map);
	reset_map_overlay();
	map_description[0] = 0;
	move_ob(you,x,y,MOVE_NORMAL);
}

int prepare_map_overlay()
{
	int i,j;

	for(i=0,j=0;i<MAXOVERLAY && overlay[i].level>0 ;i++)
		if(overlay[i].level != level)
			overlay_temp[j++] = overlay[i];
	return j;
}

void set_map_overlay()
{
	int x,y;
	int j;

	j = prepare_map_overlay();
	read_map_temp(map_name);
	for(y=1;y<SZ;y++)
		for(x=1;x<SZ;x++)
			if( map(x,y) != map_tmp[y][x])
			{
				overlay_temp[j].x = x;
				overlay_temp[j].y = y;
				overlay_temp[j].level = level;
				overlay_temp[j].sq = map(x,y);
				j++;
			}
	overlay_temp[j].level = 0;
}

void reset_map_overlay()
{
	int x,y;
	int j;

	for(j=0; j<MAXOVERLAY; j++)
	{
		overlay[j] = overlay_temp[j];
		if(overlay[j].level == level)
		{
			x = overlay[j].x;
			y = overlay[j].y;
			map(x,y) = overlay[j].sq;
		}
	}
}

void read_map(char map_nam[])
{
	int map_h;

	map_h=open(map_nam,O_BINARY);
	if (map_h==-1) ex("\nMap open error\n");
	read(map_h,(char *)map_sq,SZ*SZ);
	close(map_h);
}

void load_overlay(char ovr_nam[])
{
	int ovr_h;

	ovr_h=open(ovr_nam,O_BINARY);
	if (ovr_h==-1) ex("\nMap Overlay open error\n");
	read(ovr_h,overlay_temp,sizeof(overlay));
	close(ovr_h);
}

void save_overlay(char ovr_nam[])
{
	int ovr_h;

	set_map_overlay();
	ovr_h=open(ovr_nam,O_BINARY|O_WRONLY|O_TRUNC|O_CREAT);
	if (ovr_h==-1) ex("\nMap Overlay open error\n");
	write(ovr_h,overlay_temp,sizeof(overlay));
	close(ovr_h);
}

void read_map_temp(char map_nam[])
{
	int map_h;
	char buff[20];

	sprintf(buff,"data\\%s.map",map_nam);
	map_h=open(buff,O_BINARY);
	if (map_h==-1) ex("\nMap open error\n");
	read(map_h,(char *)map_tmp,SZ*SZ);
	close(map_h);
}

void read_font(char ft_name[] )
{
	int ft_h;

	ft_h = open(ft_name,O_BINARY);
	if (ft_h==-1) ex("\nFont open error\n");
	read(ft_h,(char *)font6,FONT6SIZE);
	close(ft_h);
}

void read_obj_names(char names[])
{
	FILE *fp;
	char buff[80];
	int i;

	fp = fopen(names,"r");
	
	read_line( buff, fp ); 
	for( i=0; buff[0] != '@';i++)
	{
		strcpy(obj_names[i],buff); 
		read_line( buff, fp ); 
	}
			
	fclose(fp);
}

void read_sprites(char sp_name[] )
{
	int sp_h;

	sp_h = open(sp_name,O_BINARY);
	if (sp_h==-1) ex("\nSprite open error\n");
	read(sp_h,(char *)spr,64000);
	close(sp_h);
}

void read_obj(char obj_name[] )
{
	int  obj_h;

	obj_h=open(obj_name,O_BINARY);
	if (obj_h==-1) ex("\nObject open error\n");
	read(obj_h,(char *)top_obj,SZ*SZ+256);
	read(obj_h,&obj,7*256);
	close(obj_h);

}

void write_obj(char obj_name[] )
{
	int  obj_h;

	obj_h=open(obj_name,O_BINARY|O_WRONLY);
	if (obj_h==-1) ex("\nObject open for write error\n");
	write(obj_h,(char *)top_obj,SZ*SZ+256);
	write(obj_h,&obj,7*256);
	close(obj_h);
}

void set_palette()
{             /*  B G R */
	long colors[256]={
		P_BLACK,   //0 
		P_GREEN,   //1  
		P_BLACK,   //2
		P_YELLOW,  //3
		P_RED,     //4
		P_MAGENTA, //5
		P_BLUE,    //6
		P_CYAN,    //7
		P_GREY,    //8
		P_BROWN,   //9
		P_SKIN,    //10
		P_LGREY,   //11
		P_GOLD,    //12
		P_DGREEN,  //13
		P_DGREY,   //14
		P_WHITE,   //15
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		 };
	_remapallpalette(colors);
}

void set_start_palette()
{             /*  B G R */
	long colors[256]={
		P_BLACK,   //0 
		P_GREY,   //1  
		P_BLACK,   //2
		P_RED,  //3
		P_BROWN,     //4
		P_CYAN, //5
		P_BLUE,    //6
		P_MAGENTA,    //7
		P_DGREEN,    //8
		P_YELLOW,   //9
		P_WHITE,    //10
		P_LGREY,   //11
		P_GOLD,    //12
		P_GREEN,  //13
		P_DGREY,   //14
		P_SKIN,   //15
		 };
	_remapallpalette(colors);
}

void disp_border()
{
	int x,y;

	rectang(144,48,319,48+16,GREY,WHITE,DGREY);
	rectang(144,96,319-48,96+15,GREY,WHITE,DGREY);
	rectang(144,0,159,159,GREY,WHITE,DGREY);
	rectang(0,144,319,159,GREY,WHITE,DGREY);
	line_up(159,49,49+14,GREY);
	line_up(159,96,96+14,GREY);
	line_across(145,144,145+14,GREY);
	print6shad(216,50,GREY,BROWN,"Backpack");
	print6shad(60,146,GREY,GREEN,"View");
	print6shad(200,146,GREY,YELLOW,"Spells");
	print6shad(280,146,GREY,MAGENTA,"Icons");
	print6shad(168,98,GREY,WHITE,"Status");
}

void print6shad(int x, int y, int bg, int fg, char line[])
{
	print6(x+1,y,bg,BLACK,line);
	print6(x,y,TRANS,fg,line);
}

void rectang(int x1, int y1, int x2, int y2, int col1, int col2, int col3)
{
	rectangle(x1,y1,x2,y2,col1);
	line_across(x1,y1,x2,col2);
	line_up(x1,y1,y2,col2);
	line_across(x1,y2,x2,col3);
	line_up(x2,y1,y2,col3);
}

void rectangle(int x1, int y1, int x2, int y2, int col)
{
	int x,y;

	for(x=x1;x<=x2;x++)
		for(y=y1;y<=y2;y++)
			plot(x,y,col);
}

void line_up(int x1, int y1, int y2, int col)
{
	int y;

	for(y=y1;y<=y2;y++)
		plot(x1,y,col);
}

void line_across(int x1, int y1, int x2, int col)
{
	int x;

	for(x=x1;x<=x2;x++)
		plot(x,y1,col);
}

void disp_icons(int icon)
{
	int i,j,c;

	for(c=0,j=0;j<=32;j+=16)
		for(i=0;i<=32;i+=16,c++)
		{
			sprite(127+c,i+272,j+96);
			if(c+1==icon)
			{
				sprite_down(127+c,i+272,j+96);
				sprite(126,i+272,j+96);
			}
			else
				sprite(125,i+272,j+96);
		}
}

void init_disp(void)
{
	int cnt,i,j,k,xx,yy,ix,iy,nx,ny;

	r = wid/2+1;
	dat = wid*wid-9;
	cnt=1;
	for(i=2;i<=(wid-1);i++){
		xx=r-i;
		yy=r;
		ix=1;
		iy=-1;
		for(k=1;k<=4;k++){
			for(j=1;j<=i;j++){
				if(xx>0&&xx<(wid+1)&&yy>0&&yy<(wid+1)){
					if(xx>(r-2)&&xx<(r+2)&&yy>(r-2)&&yy<(r+2)){
					}else{
						vxy[0][cnt]=xx;
						vxy[1][cnt]=yy;
						cnt++;
						nx=sgn(r-xx);
						ny=sgn(r-yy);
						vmsk[0][xx][yy]=xx+nx;
						vmsk[1][xx][yy]=yy+ny;
					}
				}
				xx+=ix;
				yy+=iy;
			}
			switch(k){
				case(1): ix=1;iy=1;break;
				case(2): ix=-1;iy=1;break;
				case(3): ix=-1;iy=-1;break;
			}
		}
	}
}

void disp_pos(int x,int y)
{
	static int i,j,x1,y1,x2,y2,tp,ob,spr;
	static int vdisp[wid+1][wid+1]={0};
	static int vmssk[wid+1][wid+1]={0};
	static int vobsp[wid+1][wid+1]={0};

	dark = (dark_sw==1 && light==0);
	for(i=1;i<=wid;i++){
		for(j=1;j<=wid;j++){
			x1=x+(j-r)*a[drr]+(i-r)*b[drr];
			y1=y+(i-r)*a[drr]-(j-r)*b[drr];
			if((x1<1)||(x1>SZ-1)||(y1<1)||(y1>SZ-1)){
				vdisp[j][i]=1;
				vmssk[j][i]=2;
				vobsp[j][i]=0;
			}else{
				vdisp[j][i]=map(x1,y1);
				tp=top_ob(x1,y1);
				vobsp[j][i]=tp;
				vmssk[j][i]=0;
				if(thru[vdisp[j][i]]==1)
					vmssk[j][i]=1;
			}
		}
	}

	if(xray>0) vmssk[r][r-1] = 0;
		
	if(!see_all)
		for(i=1;i<=dat;i++)
		{
			x2=vxy[0][i];
			y2=vxy[1][i];
			x1=vmsk[0][x2][y2];
			y1=vmsk[1][x2][y2];
			if( dark==1 || vmssk[x1][y1]!=0 ) 
				vmssk[x2][y2]=2;
		}
	
	for(j=1;j<=wid;j++)
	{
		for(i=1;i<=wid;i++)
		{
			if(vmssk[i][j]==2)
			{
				sprite(1,(i-1)*16,(j-1)*16);
			}
			else
			{
				spr=vdisp[i][j];
				if(cheat==0)
					if((spr>=4 && spr<=9) || spr==41) spr=2;
				sprite(spr,(i-1)*16,(j-1)*16);
				if((ob=vobsp[i][j])!=0)
				{
					for(;obj.dn[ob];ob=obj.dn[ob]);
					for(;ob;ob=obj.up[ob])    
						sprite(61+obj.spr[ob],(i-1)*16,(j-1)*16);
				}
			}
		}
	}
}

void display()
{
	SCREEN = (char *) back_screen;
	disp_position();
	//disp_3d(x_you,y_you);
	disp_inv(you);
	disp_status();
	disp_screen();
}
	
void disp_position() 
{
	disp_pos(x_you,y_you);
}

void disp_screen()
{
	spr_back1(mx,my);
	sprite(mous_ptr,mx,my);
	_copy_screen();
	spr_back2(mx,my);
}

void disp_status()
{
	int strength;
	int str_col;
	char str[40];
	char your_level[40];
	char map_desc[40];
	char current_obj[42];

	#define stat_line 69
	#define stat_col 168

	strength = obj.str[you];
	str_col = GREEN;
	if(strength<20) str_col = YELLOW;
	if(strength<10) str_col = RED;
	sprintf(str,"STRENGTH %d   ",strength);
	sprintf(your_level,"LEVEL %d  ",lev);
	if(map_description[0]==0)
	{
		sprintf(map_desc,"%-20s",map_name);
		map_desc[0] = toupper(map_desc[0]);
	}
	else
		sprintf(map_desc,"%s",map_description);
	
	if(ob_got<object)
		sprintf(current_obj,"%-40s"," ");
	else if(ob_got>=object && ob_got<=objend)
		strcpy(current_obj,obj_names[ob_got-object]);
	else if(ob_got>=monsters && ob_got<=monend)
		strcpy(current_obj,"PUTRID CORPSE");

	print6(stat_col,stat_line,BLACK,str_col,str);
	print6(stat_col,stat_line+6,BLACK,CYAN,your_level);
	print6(stat_col,stat_line+12,BLACK,MAGENTA,map_desc);
	print6(stat_col,stat_line+18,BLACK,DGREEN,current_obj);
}

void disp_3d(int x,int y)
{
	double direction,pi2,ang;
	int x0,y0;
	int x01,y01;
	int x02,y02;
	int x03,y03;
	double s,jj;
	int i,j;
	int c,sp;

	s = 1024;
	pi2 = atan(1.0)*2;
	direction = (double)(2-drr)*pi2;
	x0 = (x<<4)+8;
	y0 = (y<<4)+8;
	
	for(i=0;i<144;i++)
	{
		ang=-atan((i-72)/72.0);
		for(j=1;j<144;j++)
		{
			jj = s/(j*cos(ang));
			x01 = x0 + (int)(sin(ang+direction)*jj);
			y01 = y0 + (int)(cos(ang+direction)*jj);
			x02 = (int)x01 >> 4;
			y02 = (int)y01 >> 4;
			if(x02<1 || x02>=SZ || y02<1 || y02>=SZ) continue;
			x03 = (int)x01 & 0xF;
			y03 = (int)y01 & 0xF;
			sp = map(x02,y02);
			c = *(spr + (sp-1)*256 + x03 + y03*16);
			plot(i,j,c);
		}
	}
}

void get_input()
{
	spr_back1(mx,my);
	SCREEN = (char *) SCREEN_LIN;
	sprite(mous_ptr,mx,my);
	get_mouse_and_keys();
	SCREEN = (char *) back_screen;
}

/* return 0 if 'y' else return 1 */
int confirm(char line[])
{
	char confirm[9];

	strcpy(confirm,"");
	print6(0,193,BLACK,WHITE,line);
	if(input6(strlen(line)*6,193,2,7,confirm,1)==27)
		return 1;
	if(confirm[0]=='y')
		return 0;
	else
		return 1;
}

int confirm_exit()
{
	if(inkey!=27) return 1;
	if(cheat) return 0;
	return confirm("Are you sure you want to quit?> ");
}

void get_mouse_and_keys()
{
	do{
		//flushall();
		inkey=0;
		inkey1=0;
		if(kbhit())
		{
			inkey=getch();
			if (inkey==0) inkey1=getch();
			return;
		}
		else if(mouse_event)
		{
			spr_back2(mx,my);
			mx = mouse_x; my = mouse_y ;
			mx = mx>303 ? 303 : mx ;
			my = my>183 ? 183 : my ;
			spr_back1(mx,my);
			sprite(mous_ptr,mx,my);
			if(mouse_code&0xA) 
			{ 
				mouse_event=0 ; 
				return;
			}
			mouse_event=0;
		}
	}while(1);
}

void disp_inv(int who)
{
	int i,j,ob;
	
	if(inv_sw==0) return;
	ob=top_ob(who,SZ);
	
	// If you have no objects then make the mouse pointer an arrow
	if(ob==0)
	{
		mous_ptr = MOUS_SPR;
		ob_got = 0;
	}
	//If the top object is no longer the mouse object ...
	if(ob!=ob_got && ob_got!=0 && ob!=0)
	{
	  //If its in your backpack then make sure mouse object is first in backpack
	  if(obj.x[ob_got]==you && obj.y[ob_got]==SZ)
	  {
		move_ob( ob_got, you, SZ, MOVE_NORMAL );
		ob=ob_got;
		mous_ptr = OBJS+obj.spr[ob];
	  }
	  //If its not in your backpack then make the mouse pointer an arrow
	  else
	  {
		mous_ptr = MOUS_SPR;
		ob_got = 0;
	  }
	}
	//Dont show the mouse object in the backpack
	if(ob==ob_got && ob!=0) 
	{    
		if(mous_ptr != OBJS+obj.spr[ob])
			mous_ptr = OBJS+obj.spr[ob];
		ob=obj.dn[ob];
	}
	inv_sw = 0;

	for(i=0;i<=32;i+=16)
		for(j=160;j<=304;j+=16)
		{    
			sprite(BACK_SPR,j,i);
			if(ob!=0 && ob!=ob_got)
			{   
				sprite(OBJS+obj.spr[ob],j,i);
				ob=obj.dn[ob];
			}
		}
}       

void do_keys()
{
	char buff[9];

	if(mouse_code&0xA) 
	{
		DoMouseEvent(mx+7,my+6);
		return;
	}
	switch(tolower(inkey))
	{
		case(0):
		  switch(inkey1)
		  {
			case(72): move_you(0); break;
			case(77): move_you(2); break;
			case(80): move_you(4); break;
			case(75): move_you(6); break;
			case(71): turn_you(-1); break;
			case(73): turn_you(1); break;
			case(76): disp_sw = 1-disp_sw; break;
			case(59): help(); break;
			case(60): if(cheat) see_all=1-see_all; break;
			case(61): if(cheat) cast_light(); break;
			case(62): if(cheat) all_spells(); break;
			case(63): if(cheat) thru_all=1-thru_all; break;
			case(64): if(cheat) lev = 7; obj.str[you] = 80 ; break;
			case(65): if(cheat) make_object() ; break;
			case(66): if(cheat) go_map() ; break;
			case(67): if(cheat) edit_sprite() ; break;
			//case(67): if(cheat) obj_to_dat() ; break;
			//case(68): if(cheat) dat_to_obj() ; break;
			default: 
				if(cheat)
				{
					sprintf(buff,"%d   ",inkey1);
					print6(88,300,2,5,buff);
				}
				break;
		  } ; break ; 
		case('*') : cheat=!cheat; break;
		case('e') : enter_map(); break;
		case('c') : cast(); break;
		case('u') : use_object(); break;
		case('t') : talk_adjacent(); break;
		case('s') : save_the_game(); break;
		case('l') : load_the_game(); break;
		case(27) : break ;
		case('a') : attack(); break;
		case(13) : do_adjacent(0,-1); break;
		case(' '): do_anything(); break;
		case('1') : do_adjacent(-1,1); break;
		case('2') : do_adjacent(0,1); break;
		case('3') : do_adjacent(1,1); break;
		case('4') : do_adjacent(-1,0); break;
		case('5') : do_adjacent(0,0); break;
		case('6') : do_adjacent(1,0); break;
		case('7') : do_adjacent(-1,-1); break;
		case('8') : do_adjacent(0,-1); break;
		case('9') : do_adjacent(1,-1); break;
		case('0') : get_put_backpack(0); break;
		case(8) : get_put_backpack(0); break;
	}
}          

void make_object()
{
	char obj_no[9];
	int ob;

	obj_no[0] = 0;
	input6(0,193,2,7,obj_no,5);
	ob = atoi(obj_no);
	move_ob(ob,you,SZ,MOVE_NORMAL);
}

void go_map()
{
	char map[9];
	char xs[9],ys[9];
	int x,y;

	strcpy(map,"");
	strcpy(xs,"");
	strcpy(ys,"");
	input6(0,193,2,7,map,6);
	input6(0,193,2,7,xs,5);
	input6(0,193,2,7,ys,5);
	x=atoi(xs);
	y=atoi(ys);
	goto_map(map,x,y);
}

void obj_to_dat()
{
	int i;
	FILE *fp;

	disp_mess(BLACK,YELLOW,"Writing OBJ to DAT");
	fp = fopen("data\\obj.dat","w");
	for(i=0;i<MAXOBJS;i++)
		fprintf(fp,"%d, %d, %d, %d, %d, %d, %d, %d\n",
			i,obj.x[i],obj.y[i],obj.up[i],obj.dn[i],obj.z[i],obj.spr[i],obj.str[i]);
	fclose(fp);
}

void dat_to_obj()
{
	disp_mess(BLACK,YELLOW,"Writing DAT to OBJ");
}

void help()
{
	SCREEN = (char *)SCREEN_LIN;
	disp_clear(DGREY);
	
	print6shad(110,10,DGREY,BLUE,"Help");
	print6shad(40,30,DGREY,YELLOW, "C      Cast Spell");
	print6shad(40,40,DGREY,YELLOW, "L      Load Game");
	print6shad(40,50,DGREY,YELLOW, "S      Save Game");
	print6shad(40,60,DGREY,YELLOW, "U      Use Object");
	print6shad(40,70,DGREY,YELLOW, "T      Talk");
	print6shad(40,80,DGREY,YELLOW, "A      Attack Monster");
	print6shad(40,90,DGREY,YELLOW, "E      Enter New Location");
	print6shad(40,100,DGREY,YELLOW, "SPACE  Get/Drop/Talk/Attack");
	print6shad(40,110,DGREY,YELLOW,"RETURN Get/Drop/Talk/Attack in front");
	print6shad(40,120,DGREY,YELLOW,"ESC    Exit");
	print6shad(40,130,DGREY,YELLOW,"CURSOR Move/Turn");
	print6shad(40,140,DGREY,YELLOW,"BACKSP Backpack");
	
	print6shad(40,30,DGREY,WHITE, "C");
	print6shad(40,40,DGREY,WHITE, "L");
	print6shad(40,50,DGREY,WHITE, "S");
	print6shad(40,60,DGREY,WHITE, "U");
	print6shad(40,70,DGREY,WHITE, "T");
	print6shad(40,80,DGREY,WHITE, "A");
	print6shad(40,90,DGREY,WHITE, "E");
	print6shad(40,100,DGREY,WHITE, "SPACE");
	print6shad(40,110,DGREY,WHITE,"RETURN");
	print6shad(40,120,DGREY,WHITE,"ESC");
	print6shad(40,130,DGREY,WHITE,"CURSOR");
	print6shad(40,140,DGREY,WHITE,"BACKSP");
	
	SCREEN = (char *)back_screen;
	get_mouse_and_keys();
}

void move_you(int mov)
{
	int x_next,y_next;
	int x_mov,y_mov;
	int vec;

	vec=(drr*2+mov)&7;
	x_mov=xi[vec];        
	y_mov=yi[vec];

	x_next=x_you+x_mov;
	y_next=y_you+y_mov;
	if (move_fwd(x_next,y_next))
	{
		move_ob(you,x_next,y_next,MOVE_NORMAL);
	}
}

void throw_handler(int x, int y, int subcode)
{
	throw_object(subcode);
}

void throw_object(int dir)
{
#define THROW_DIST 5
	int x_next,y_next;
	int x_mov,y_mov;
	int vec;
	int ob,i;

	if(ob_got==0) return;
	vec=(drr*2+dir)&7;
	x_mov=xi[vec];        
	y_mov=yi[vec];
	ob = ob_got;

	x_next=x_you;
	y_next=y_you;
	
	for(i=0; i<THROW_DIST; i++)
        {
		if (move_throw(x_next+x_mov, y_next+y_mov))
		{
		        x_next += x_mov;
		        y_next += y_mov;
			move_ob(ob,x_next,y_next,MOVE_THROW);
			display();
			wait_interval(1);
		}
		else
			break;
        }
 	move_ob(ob,x_next,y_next,MOVE_NORMAL);      
}

void dead_tele()
{
	goto_map(dead_map,dead_x,dead_y);
	obj.str[you] = lev*10+10;
}

void set_dead_map(char *map, int x, int y)
{
	strcpy(dead_map,map);
	if(strncmp(dead_map,"world",5)==0)
	{
		dead_x = CIRCLE_X;
		dead_y = CIRCLE_Y;
	}
	else
	{
		dead_x = x;
		dead_y = y;
	}
}

void do_dead()
{
	if(obj.str[you]==0)
	{
		disp_mess(2,4,"DEAD!");
		dead_tele();
	}
	if(obj.str[255]==0)
		disp_mess(BLACK,RED,"                   YOU HAVE WON!!!");
}

void all_spells()
{
	int i;

	for(i=1; i<=spells; i++)
		spell[i] = (spell[i]==0) ? 1 : spell[i] ;
	set_spells();
}

void turn_you(int d)
{
	drr = (drr+d)&3;
}

void mouse_zone()
{
	zone_x = (mx+7)/16;
	zone_y = (my+6)/16;
}

void do_mouse()
{
	char buff[20];
	
	sprintf(buff,"%d %d ",zone_x,zone_y);
	//if(cheat) print6(248,80,4,7,buff);
	//if(zone_x>=10 && zone_x<=16 && zone_y>=7 && zone_y<=8) do_spells(zone_x-10,zone_y-7);
	if(zone_x>=17 && zone_x<=19 && zone_y>=6 && zone_y<=8) do_control(zone_x-17,zone_y-6);
	if(abs(zone_x-4)<2 && abs(zone_y-4)<2) do_adjacent(zone_x-4,zone_y-4);
	if(zone_x>=10 && zone_x<=19 && zone_y>=0 && zone_y<=2) get_put_backpack(0);
	
	if(zone_x>1 && zone_x<7 && zone_y<2) throw_object(0);
	if(zone_x>6 && zone_x<9 && zone_y>1 && zone_y<7) throw_object(2);
	if(zone_x>1 && zone_x<7 && zone_y>6 && zone_y<9) throw_object(4);
	if(zone_x<2 && zone_y>1 && zone_y<7) throw_object(6);
}

void adjacent_handler(int x, int y, int subcode)
{
	do_adjacent(x/16-1,y/16-1);
}

void wait_mouse()
{    
	static int interval = CLOCKS_PER_SEC/10;
	clock_t start;

	start = clock();
	do {} while( //!(mouse_event &&(mouse_code&0x1E)));
				((clock()-start) < interval) );
}

void wait_interval(interval)
{    
	clock_t start;

	start = clock();
	do {} while( clock()-start < interval );
}

void set_spell(int spl, int state)
{
	switch(state)
	{
		case SPELL_RESET:
			SetButton(&spell_but[spl-1],BUTTON_UP);
			break;
		case SPELL_SET:
			SetButton(&spell_but[spl-1],BUTTON_DOWN);
			break;
		case SPELL_OFF:
			SetButton(&spell_but[spl-1],BUTTON_OFF);
			break;
	}
}

void reset_spell(int spl)
{
	set_spell(spl,SPELL_RESET);
}

void cast()
{
	char *s;
	int spl;

	disp_mess(2,12,"Cast ");
	get_input();
	if(s=strchr(spell_valid,inkey))
	{
		spl = (int)s - (int)spell_valid;
		if(spell[spl]==1) 
		{
			set_spell(spl,SPELL_SET);
			cast_spell_handler(spl,0);
			return;
		}
		else
			print6(6*5,MESS_LINE,BLACK,YELLOW,"Spell Not Known Yet");
	}
	else
		print6(6*5,MESS_LINE,BLACK,YELLOW,"Unknown Spell");
}

void cast_spell(int spl)
{
	char sp[20];

	print6(6*5,MESS_LINE,BLACK,YELLOW,spell_name[spl]);
	scroll_mess();
	switch(spl)
	{
		case 1: // Mindmelt
			cast_mindmelt();
			break;
		case 2: // Circle teleport
			cast_circle_tele();
			break;
		case 3: // Direction
			cast_direction();
			break;
		case 4: // X,Y
			cast_xy();
			break;
		case 5: // Light
			cast_light();
			break;
		case 6: // Xray
			cast_xray();
			break;
		case 7: // Water
			cast_water();
			break;
		case 8: // Stun
			cast_stun();
			break;
		case 9: // Jump
			cast_jump();
			break;
		case 10: // Zap
			cast_zap();
			break;
		case 11: // Heal
			cast_heal();
			break;
		case 12: // Water
			cast_field();
			break;
		case 13: // Back
			cast_back();
			break;
		case 14: // Overview
			cast_overview();
			break;
		default:
			disp_debug("Spell",spl);
	}
	disp_screen();
	wait_mouse();
}

void cast_spell_handler(int spl,int state)
{
	char sp[20];
	int reset;

	print6(0,MESS_LINE,BLACK,GOLD,"Cast ");
	print6(6*5,MESS_LINE,BLACK,YELLOW,spell_name[spl]);
	scroll_mess();
	reset=0;
	switch(spl)
	{
		case 1: // Mindmelt
			cast_mindmelt();
			reset=1;
			break;
		case 2: // Circle teleport
			cast_circle_tele();
			reset=1;
			break;
		case 3: // Direction
			cast_direction();
			reset=1;
			break;
		case 4: // X,Y
			cast_xy();
			reset=1;
			break;
		case 5: // Light
			cast_light();
			break;
		case 6: // Xray
			cast_xray();
			break;
		case 7: // Water
			cast_water();
			break;
		case 8: // Stun
			cast_stun();
			break;
		case 9: // Jump
			cast_jump();
			reset=1;
			break;
		case 10: // Zap
			cast_zap();
			reset=1;
			break;
		case 11: // Heal
			cast_heal();
			reset=1;
			break;
		case 12: // Water
			cast_field();
			break;
		case 13: // Back
			cast_back();
			break;
		case 14: // Overview
			cast_overview();
			reset=1;
			break;
		default:
			disp_debug("Spell",spl);
	}
	disp_screen();
	wait_mouse();
	if(reset)
		reset_spell(spl);
}

void cast_mindmelt()
{
	int i;
	int x,y;
	int pers;

	for(i=0;i<=melts;i++)
	{
		if(i==0)
			pers = 255; // OMGRA
		else
			pers = abs(melt[i]);
		if (obj.z[pers]!=level) continue;
		x = obj.x[pers];
		y = obj.y[pers];
		if(abs(x-x_you)>1 || abs(y-y_you)>1) continue;
	
		if(pers==255) 
		{
			omgra_mindmelt();
			return;
		}
		pers = melt[i];
		if(pers<0) 
		{
			disp_mess(BLACK,YELLOW,"Already Mindmelted with");
			return;
		}
		melt[i] = -pers;
		lev++;
		disp_mess(BLACK,YELLOW,"MINDMELTED!");
		activate_spells();
		return;
	}
	disp_mess(BLACK,YELLOW,"No-one to Mindmelt with");
	cast_circle_tele();
}

void new_spell(int spl)
{
    spell[spl] = 1;
    set_spell(spl,SPELL_RESET);
}

void activate_spells()
{
	switch(lev)
	{        
		case(2):
			new_spell(3);
			break;
		case(3):
			new_spell(4);
			break;
		case(4):
			new_spell(5);
			break;
		case(5):
			new_spell(7);
			break;
		case(6):
			new_spell(9);
			break;
		case(7):
			new_spell(12);
			break;
	}
}

void cast_circle_tele()
{
	goto_map("world",CIRCLE_X,CIRCLE_Y);
	obj.str[you] = lev*10+10;
	set_dead_map("world",CIRCLE_X,CIRCLE_Y);
}

void cast_direction()
{
	switch(drr)
	{
		case 0: disp_mess(BLACK,YELLOW,"You are facing North"); break;
		case 1: disp_mess(BLACK,YELLOW,"You are facing East"); break;
		case 2: disp_mess(BLACK,YELLOW,"You are facing South"); break;
		case 3: disp_mess(BLACK,YELLOW,"You are facing West"); break;
		default: disp_mess(BLACK,RED,"You are in Void"); break;
	}
}
	
void cast_xy()
{
	char text[30];

	sprintf(text,"You are at position %d,%d",x_you,y_you);
	disp_mess(BLACK,YELLOW,text);
}

void cast_light()
{
	if(light>0) return;
	light = 1000;
}

void cast_xray()
{
	if(xray>0) return;
	xray = 3;
}

void cast_water()
{
	if(water>0) return;
	water = 10;
}

void cast_field()
{
	if(feeld>0) return;
	feeld = 3;
}

void cast_stun()
{
	if(stun>0) return;
	stun = lev+5;
}

int monst_close(int dist)
{
	int i,j;
	int x,y;
	int ob;

	for(i=monsters;i<=monend;i++)
	{
		if(obj.z[i]!=level) continue;
		if(abs(obj.x[i]-x_you)<=dist && abs(obj.y[i]-y_you)<=dist && (obj.str[i]>0) )
			return 1;
	}
	return 0;
}

void cast_heal()
{    
	if(monst_close(3))
		disp_mess(BLACK,YELLOW,"Can't - Monster too close");
	else
		obj.str[you] = lev*10+10;
}

void cast_jump()
{
#define JUMP_DIST 4
	int x_next, y_next;
	int x_mov, y_mov;
	int vec, i, sq;

	vec=(drr*2)&7;
	x_mov=xi[vec];        
	y_mov=yi[vec];
	x_next = x_you;
	y_next = y_you;
	
	for(i=0; i<JUMP_DIST; i++)
	{    
		x_next += x_mov;
		y_next += y_mov;
		if(x_next<1 || x_next>SZ-1 || y_next<1 || y_next>SZ-1) 
			break;
		sq = map( x_next, y_next);
		if(sq==27) // grass
		{
			move_ob(you, x_next, y_next, MOVE_NORMAL);
			return;
		}
	}
	disp_mess(BLACK,YELLOW,"Can't Jump here - no grass");
}

void cast_zap()
{
#define ZAP_DIST 4
	int x_next, y_next;
	int x_mov, y_mov;
	int vec, i,j, ob;
	int hits;
	int x,y;

	vec=(drr*2)&7;
	x_mov=xi[vec];        
	y_mov=yi[vec];
	x_next = x_you;
	y_next = y_you;
	
	for(i=0; i<ZAP_DIST; i++)
	{    
		x_next += x_mov;
		y_next += y_mov;
		if(!move_throw(x_next,y_next)) break;
	}
	if(i<1) return;
	ob = top_ob(x_next,y_next);
	if(ob<monsters || ob>monend) return;
	if(obj.str[ob]==0) return;
	hits = random(10);
	hits_on_mon(ob,hits);
	disp_mess(BLACK,CYAN,"ZAP!!");
	for(j=0;j<5;j++)
	{
		draw_zap(i);
		display();
	}
}

void cast_back()
{
	if(back_x==0 && back_y==0)    
	{
		back_x = x_you;
		back_y = y_you;
		strcpy(back_map,map_name);
		set_spell(13,SPELL_SET);
	}
	else
	{
		goto_map(back_map,back_x,back_y);
		back_x = 0;
		back_y = 0;
		set_spell(13,SPELL_RESET);
	}
}

void cast_overview()
{
	int x,y;
	int end_flag;
	char pixel;

	for(y=1;y<=8*9;y++)
	{
		end_flag = 1;
		for(x=1;x<=8*9;x++)
		{
			if(map(x,y)<=1) end_flag=0;
			if(x<SZ && y<SZ && end_flag)
			{
				plot(x*2-2,y*2-2,pix[map(x,y)]);
				plot(x*2-1,y*2-2,pix[map(x,y)]);
				plot(x*2-2,y*2-1,pix[map(x,y)]);
				plot(x*2-1,y*2-1,pix[map(x,y)]);
			}
			else
			{
				plot(x*2-2,y*2-2,2);
				plot(x*2-2,y*2-1,2);
				plot(x*2-1,y*2-2,2);
				plot(x*2-1,y*2-1,2);
			}
		}
	}
	pixel=pix_colour(x_you*2-2,y_you*2-2);    
	if(pixel!=0 && pixel!=2) //Transparent or Black
	{
		for(x=-2;x<1;x++)
			for(y=-2;y<1;y++)
				plot(x_you*2+x,y_you*2+y,2);
		plot(x_you*2-1,y_you*2-1,15);
	}
	disp_screen();
	get_input();
	display();
}

void draw_zap(int dist)
{
	int x,y,j;

	x=4*16+8;
	y=4*16;
	_setcolor(CYAN);
	_moveto(x,y);
	for(j=0;j<dist*2;j++)
	{
		y -= 8;
		x = 4*16+random(16);
		_lineto(x,y);
	}
	y -= 8;
	x = 4*16+8;
	_lineto(x,y);
	wait_interval(CLOCKS_PER_SEC/20);
}

void control_handler(int subcode, int state)
{
	display();
	wait_mouse();
	switch(subcode+1)
	{
		case 1:
			// USE
			use_object();
			break;
		case 2:
			// Load
			load_the_game();
			break;
		case 3:
			// Save
			save_the_game();
			break;
		case 4:
			//Turn Left
			turn_you(-1);break;
		case 5:
			//Forward
			move_you(0);break;
		case 6:
			//Turn Right
			turn_you(1);break;
		case 7:
			//Left
			move_you(6);break;
		case 8:
			//Back
			move_you(4);break;
		case 9:
			//Right
			move_you(2);break;
	}
	SetButton(&control_but[subcode],BUTTON_UP);
}

void do_control(int zx, int zy)
{
	int cntl;

	cntl = zx+zy*3+1;
	
	disp_screen();
	
	wait_mouse();

	switch(cntl)
	{
		case 1:
			// USE
			use_object();
			break;
		case 2:
			// Load
			load_the_game();
			break;
		case 3:
			// Save
			save_the_game();
			break;
		case 4:
			//Turn Left
			turn_you(-1);break;
		case 5:
			//Forward
			move_you(0);break;
		case 6:
			//Turn Right
			turn_you(1);break;
		case 7:
			//Left
			move_you(6);break;
		case 8:
			//Back
			move_you(4);break;
		case 9:
			//Right
			move_you(2);break;
	}
}

int vec_conv(int x, int y)
{
	static int vec_con[9]={7,6,5,0,8,4,1,2,3};

	return vec_con[x*3+y+4];
}

int talk_adjacent()
{
	int i;

	for(i=people;i<=peoend;i++)
		if(obj.z[i]==level)
			if(i!=you && abs(obj.x[i]-x_you)<2 && abs(obj.y[i]-y_you)<2)
			{
				talk_to(i);
				return 1;
			}
	return 0;
}

void do_anything()
{
	if(talk_adjacent()) return;
	if(attack()) return;
	do_adjacent(0,-1);
}

void do_adjacent(int zx,int zy)
{
	int vec,v;
	int x,y;
	int sq,tp;

	vec=vec_conv(zx,zy);
	if(vec<8)
	{
		v=(drr*2+vec)&7;
		x=x_you+xi[v];
		y=y_you+yi[v];
	}
	else
	{
		v=8;
		x=x_you;
		y=y_you;
	}

	sq=map(x,y);
	tp=top_ob(x,y);
	
	// Talking to Person
	if(tp>=people && tp<=peoend && tp!=you)
	{
		talk_to(tp);
		return;
	}

	// Attack Monster
	if(tp>=monsters && tp<=monend)
	{       
		if(obj.str[tp]>0)
		{
			attack_monst(tp);
			return;
		}
	}
	
	// Get
	if(ob_got==0)
	{
		if(get_drop_ob(x,y))
			return;
		if(v==8) 
			if(enter_map()) return;
	}

	// Open / Close Doors & Levers
	if(v!=8) switch(sq)
	{
		//Doors
		case(18):change_sq(20,x,y);return;
		case(20):change_sq(18,x,y);return;
		//Levers
		case(12):change_sq(13,x,y);return;
		case(13):change_sq(12,x,y);return;
	}

	// Drop or Enter
	if(ob_got!=0 && !mov[sq] && sq!=47 && sq!=47) 
	{
		if(v==8) 
			if(enter_map()) return;
		if(get_drop_ob(x,y))
			return;
	}
	
	//Click on Square
	add_code("GP",x,y);
}

int get_drop_ob(int x,int y)
{
	if(ob_got==0)
	{// Get
		ob_got = top_ob(x,y);
		if(ob_got==you) ob_got=obj.dn[you];
		if(ob_got!=0)
		{
			if(can_get(ob_got))
			{
				if(mouse_code&8) // Right click
				{
					move_all( x, y, you, SZ, MOVE_NORMAL );
				}
				else
				{
					move_ob( ob_got, you, SZ, MOVE_NORMAL );
					mous_ptr = OBJS + obj.spr[ob_got];
				}
				return 1;
			}
			else
				ob_got = 0;
		}
	}
	else
	{// Drop
		if(can_drop(x,y))
		{
			move_ob( ob_got, x, y, MOVE_NORMAL );
			if(x==x_you && y==y_you) move_ob(you,x,y,MOVE_NORMAL);
			mous_ptr = MOUS_SPR;
			ob_got = 0 ;
			return 1;
		}
	}
	return 0;
}

int can_drop(int x, int y)
{
	int ob;

	ob=top_ob(x,y);
	if(ob>=people && ob<=peoend && ob!=you) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	return 1;
}

int can_get(int ob)
{
	if(ob>=people && ob<=peoend) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	return 1;
}

void inventory_handler(int x, int y, int subcode)
{
	int get;

	get = (x/16)+(y/16)*10+1;
	get_put_backpack(get);
}

void get_put_backpack(int get)
{
	int got;

	if(ob_got==0)
	{
	// Get
		ob_got=top_ob(you,SZ);
		if(ob_got!=0)
		{    
			for(got=1;obj.dn[ob_got]&&got<get;ob_got=obj.dn[ob_got],got++);
			move_ob( ob_got, you, SZ, MOVE_NORMAL );
			mous_ptr = OBJS + obj.spr[ob_got];
			return;
		}
	}
	else
	{
	// Put
		if(ob_got<monsters || ob_got>monend)
		{
			mous_ptr = MOUS_SPR;
			ob_got = 0 ;
			inv_sw=1;
		}
		else
			disp_mess(BLACK,YELLOW,"Can't keep a dead corpse");
	}
}

void use_object()
{
	int lv, spl;
	if(ob_got==0) return;

	// Scrolls
	if(ob_got>=100 && ob_got<=105)
	{
		switch(ob_got)
		{
			case 100: lv=4; spl=6; break; //Xray
			case 101: lv=5; spl=8; break; //Stun
			case 102: lv=6; spl=10; break;//Zap
			case 103: lv=6; spl=11; break;//Heal
			case 104: lv=7; spl=13; break;//Back
			case 105: lv=7; spl=14; break;//Overview
		}
		if(lv>lev)
		{            
			disp_mess(BLACK,YELLOW,"Not high enough level yet");
			return;
		}
		if(spell[spl]>0)
		{
			disp_mess(BLACK,YELLOW,"Already know spell");
			return;
		}
		spell[spl] = 1;
		disp_mess(BLACK,YELLOW,"Spell learned");
	}
	else
	{
		disp_debug("Object",ob_got);
		disp_mess(BLACK,YELLOW,"Has not any use");
	}
}

int attack()
{
	int i;

	for(i=monsters;i<=monend;i++)
		if(obj.z[i]==level)
			if(abs(obj.x[i]-x_you)<2 && abs(obj.y[i]-y_you)<2)
			{    
				attack_monst(i);
				return 1;
			}
	return 0;
}

void attack_monst(int mon)
{
	int mon_str,hit,str;
	
	mon_str = obj.str[mon];
	if(mon_str == 0) return;
	str = obj.str[you];
	hit = random(str/5)+10;
	hits_on_mon(mon,hit);
}
	
hits_on_mon(int mon, int hits)
{
	int mon_str;
	int x,y;

	mon_str = obj.str[mon];
	if(mon!=255) // OMGRA
		mon_str -= hits;
	if(mon_str<=0) 
	{
		mon_str = 0;
		obj.spr[mon] = DEAD_SPR;
		move_all(mon,SZ,obj.x[mon],obj.y[mon],MOVE_NORMAL);
	}

	x = 5 + (obj.x[mon] - x_you)*a[drr] + (-obj.y[mon] + y_you)*b[drr];
	y = 5 + (obj.y[mon] - y_you)*a[drr] + (obj.x[mon] - x_you)*b[drr];

	disp_hits(x,y,hits,mon_str);
	obj.str[mon] = mon_str;
}

void talk_to(int person)
{
	#define maxtopic 11
	int pers,lin,i;
	char topic[maxtopic+2+4];

	if(person<people || person>peoend) return;
	pers = person-people+1;
	lin=talk[pers];
	if(lin==0)
	{
		disp_mess(BLACK,YELLOW,"Funny, no response");
		return;
	}
	if(talk_topic[lin][0]==' ') 
		disp_mess(BLACK,YELLOW,say[lin]);
	for(;;)
	{
	next_topic:
		strcpy(topic,"");
		scroll_mess();
		print6(0,193,BLACK,WHITE,"You Say> ");
		input6(9*6,193,2,7,topic,maxtopic);
		strcat(topic,"    ");
		topic[4]='\0';
		if(!strcmp(topic,"    ")) strcpy(topic,"bye ");
		print6(9*6,193,2,7,topic);
		scroll_mess();
		for(i=lin;;)
		{
			if(!strcmp(talk_topic[i],topic)) break;
			i++;
			if(!strcmp(talk_topic[i],"bye ") && strcmp(topic,"bye ")!=0) 
			{
				switch(random(4))
				{
					case 0: case 1: disp_mess(BLACK,YELLOW,"I can't help you with that"); break ;
					case 2: disp_mess(BLACK,YELLOW,"You'll have to ask someone else"); break ;
					case 3: disp_mess(BLACK,YELLOW,"Ask me something more specific"); break ;
					case 4: disp_mess(BLACK,YELLOW,"Try a different subject"); break ;
				}
				goto next_topic;
			}
		}
		if(say[i][0]=='*' || say[i][0]=='%')
		{
			add_code("PE",person,atoi(say[i]+1));
			disp_mess(BLACK,YELLOW,say[i]+3);
			if(say[i][0]=='%') break;
		}
		else
			disp_mess(BLACK,YELLOW,say[i]);
		if(!strcmp(topic,"bye ")) break;
	}
	scroll_mess();
}

int move_fwd(int x,int y)
{
	int ob,sq;

	if(x<1 || x>=SZ || y<1 || y>=SZ) return 0;
	if(thru_all) return 1;
	ob=top_ob(x,y);
	sq=map(x,y);
	if(ob>=people && ob<=peoend) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	switch (sq)
	{
		case(61) : return 1; // Stones
		case(33) : return (water>0); // WATER
		case(9)  : case(22) : return (feeld>0); // FORCE FIELD
		case(28) : case(32) : case(35) : if(random(2)==0) return 0; break;//MARSH,HILL
		//Doors
		case(18):change_sq(20,x,y);return 0;
	}
	return !mov[map(x,y)];
}

int move_throw(int x, int y)
{
	int sq,ob;

	sq=map(x,y);
	ob=top_ob(x,y);
	if(ob>=people && ob<=peoend) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	return !mov[map(x,y)];
}

int move_fwd_peo(int x,int y)
{
	int ob,sq;

	ob=top_ob(x,y);
	sq=map(x,y);
	if(ob>=people && ob<=peoend) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	switch (sq)
	{
		case(33) : return 0; break; // WATER
		case(9)  : case(22) : return 0; break; // FORCE FIELD
		case(28) : case(32) : case(35) : if(random(2)==0) return 0; break; //MARSH,HILL
		case(18):change_sq(20,x,y);return 0;
	}
	return !mov[map(x,y)];
}

int move_fwd_monst(int x,int y)
{
	int ob,sq;

	ob=top_ob(x,y);
	sq=map(x,y);
	if(ob>=people && ob<=peoend) return 0;
	if(ob>=monsters && ob<=monend) 
		if(obj.str[ob]>0) return 0;
	switch (sq)
	{
		case(33) : return 0; break; // WATER
		case(9)  : case(22) : return 0; break; // FORCE FIELD
		case(28) : case(32) : case(35) : if(random(2)==0) return 0; break; //MARSH,HILL
		case(18) : if(level>1) {change_sq(20,x,y);return 0;}
	}
	return !mov[map(x,y)];
}

void do_auto()
{
	x_you = obj.x[you];
	y_you = obj.y[you];
	if(obj.str[you]<lev*10+10 && random(3)==0 )
		obj.str[you]++;
	if(light>0) 
		if(!--light)
			reset_spell(5);
	if(xray>0) 
		if(!--xray)
			reset_spell(6);
	if(water>0) 
		if(!--water)
			reset_spell(7);
	if(stun>0) 
		if(!--stun)
			reset_spell(8);
	if(feeld>0) 
		if(!--feeld)
			reset_spell(12);
}

void do_peo()
{
	int person;

	for(person=people; person<=peoend; person++)
	{
		if(obj.z[person] == level)
			do_person(person);
	}
}

void do_person(int person)
{
	int dir;
	int x,y;
	int nx,ny;
	int i;

	if(person==you) return;
	x = obj.x[person];
	y = obj.y[person];
	// If YOU are adjacent then think about staying still
	if( abs(x_you-x)<2 && abs(y_you-y)<2 && random(3)>0) return;
	dir = random(3);
	nx = x+a[dir];
	ny = y+b[dir];
	if(move_fwd_peo(nx,ny))
		move_ob(person,nx,ny,MOVE_NORMAL);
}

void make_move_table()
{
	int i,j;
	int x,y;
	int c;
	int sp;

	for(i=0;i<MOVE_SIZE;i++)
		for(j=0;j<MOVE_SIZE;j++)
			move_table[i][j] = 32767;

	c = 0;
	sp = 0;

	x = MOVE_HALF_SIZE;
	y = MOVE_HALF_SIZE;

	sp = move_push(x,y,c,sp);

	for(;;)
	{
		sp = move_pop(&x,&y,&c,sp);
		if(sp==-1) break;
		c++;
		sp = move_push(x+1,y,c,sp);
		sp = move_push(x,y+1,c,sp);
		sp = move_push(x-1,y,c,sp);
		sp = move_push(x,y-1,c,sp);
	}
	//disp_move_table();
}

void disp_move_table()
{
	int x,y;

	for(y=0;y<MOVE_SIZE;y++)
		for(x=0;x<MOVE_SIZE;x++)
			if(move_table[x][y]<30000) 
				disp_at(x,y,move_table[x][y]);
	disp_screen();
}

int move_push(int x, int y, int c, int sp)
{
	int map_x,map_y;
	int sq;
	
	if(x<0 || x>MOVE_SIZE-1 || y<0 || y>MOVE_SIZE-1) return sp;
	if(move_table[x][y]<=c) return sp;
	
	map_x = x_you-MOVE_HALF_SIZE+x;
	map_y = y_you-MOVE_HALF_SIZE+y;
	if(map_x<1 || map_x>SZ-1 || map_y<1 || map_y>SZ-1) return sp;
	sq = map(map_x,map_y);
	if(mov[sq]) return sp;

	move_table[x][y] = c;
	move_list[sp][0] = x;
	move_list[sp][1] = y;
	move_list[sp][2] = c;
	sp++;
	return sp;
}

int move_pop(int *x, int *y, int *c, int sp)
{
	sp--;
	if(sp<0) return -1;
	*x = move_list[sp][0];
	*y = move_list[sp][1];
	*c = move_list[sp][2];
	return sp;
}

void do_mon()
{
	int monst;
	int mon_to_regen;
	char txt[80];

	mon_to_regen = 0;
	make_move_table();
	for(monst=monsters; monst<=monend; monst++)
	{
		if(obj.z[monst] == level && obj.str[monst]>0 && obj.x[monst]>0)
			do_monst(monst);
		else if( level==1 && monst>=monsters && monst<monsters+REGEN_MONST &&
				map(x_you,y_you)==61 ) 
					mon_to_regen = monst;
	}
	if(mon_to_regen!=0)        
		regen_mon(mon_to_regen);
}

void do_monst(int monst)
{
	int dir;
	int x,y,str;
	int nx,ny;
	int i;

	if(monst==you) return;
	x = obj.x[monst];
	y = obj.y[monst];
	str = obj.str[monst];
	if(abs(x_you-x)<2 && abs(y_you-y)<2 && random(3)!=0 && stun==0)
		mon_attack( monst, x, y);
	else
		move_monst( monst, x, y);
}

void move_monst(int monst, int x, int y)
{
	int dir,i;
	int nx,ny;
	int str;
	int x1,y1;
	int c;

	// Move towards YOU if can SMELL you
	if( obj.str[monst]>10 &&
		abs(x_you-x)<=MOVE_HALF_SIZE && abs(y_you-y)<=MOVE_HALF_SIZE && random(3)>0 )
	{
		if(level==1)
		{
			nx = sgn(x_you-x);
			ny = sgn(y_you-y);
			if(stun>0 && abs(x_you-x)<2 && abs(y_you-y)<2 ) // Move away if stuned
			{
				nx=-nx;
				ny=-ny;
			}
			if(nx!=0 && ny!=0)
				if(random(1)==0)
					nx=0;
				else
					ny=0;
			nx += x;
			ny += y;
		}
		else
		{
			c = 30000;
			dir = random(3);
			nx = x+a[dir];
			ny = y+b[dir];
			for(i=0;i<4;i++)
			{
				x1 = (x-x_you+MOVE_HALF_SIZE+a[i]);
				y1 = (y-y_you+MOVE_HALF_SIZE+b[i]);
				if(x1<0 || x1>MOVE_SIZE-1 || y1<0 || y1>MOVE_SIZE-1)
					continue;
				if(move_table[x1][y1]<c)
				{
					c = move_table[x1][y1];
					nx = x+a[i];
					ny = y+b[i];
				}
			}
		}
	}
	else
	{
		if(level==1 || abs(x_you-x)>MOVE_HALF_SIZE || abs(y_you-y)>MOVE_HALF_SIZE)
		{
			dir = random(3);
			nx = x+a[dir];
			ny = y+b[dir];
		}
		else
		{
			c = -1;
			dir = random(3);
			nx = x+a[dir];
			ny = y+b[dir];
			for(i=0;i<4;i++)
			{
				x1 = (x-x_you+MOVE_HALF_SIZE+a[i]);
				y1 = (y-y_you+MOVE_HALF_SIZE+b[i]);
				if(x1<0 || x1>MOVE_SIZE-1 || y1<0 || y1>MOVE_SIZE-1)
					continue;
				if(move_table[x1][y1]>c && move_table[x1][y1]<32000)
				{
					c = move_table[x1][y1];
					nx = x+a[i];
					ny = y+b[i];
				}
			}
		}
	}
	if(move_fwd_monst(nx,ny))
		move_ob(monst,nx,ny,MOVE_NORMAL);
}

void mon_attack( int monst, int x, int y )
{
	int str,hits;

	str = obj.str[monst];
	if(str==0) return;
	hits = random(str/5)+str/10;
        do_damage(hits);
}

void do_damage(int hits)
{
	int you_str;

	you_str = obj.str[you];
	you_str -= hits;
	disp_hits(5,5,hits,you_str);
	if(you_str<0) you_str=0;
	obj.str[you] = you_str;
}

void regen_mon(int monst)
{
	int x,y,i;

	//Regenerate only on grass (27)
	for(i=0;i<99;i++)
	{
		x = random(SZ);
		y = random(SZ);
		if(map(x,y)==27) 
		{
			obj.str[monst] = random(20)+10; // Set strength
			obj.spr[monst] = random(16)+2;  // Set sprite
			move_ob(monst,x,y,MOVE_NORMAL);
			return;
		}
	}
}

void change_sq(char sq, int x, int y)
{
	int squ;

	squ=map(x,y);
	if(sq==squ) return;
	map(x,y)=sq;
	if(sq==12) add_code("L0",x,y);
	if(sq==13) add_code("L1",x,y);
}

void move_ob(int ob,int x,int y, int mode)
{
	int xx,yy;

	if(ob==0) return;
	xx=obj.x[ob];
	yy=obj.y[ob];
	if (obj.up[ob]!=0) obj.dn[obj.up[ob]]=obj.dn[ob];
	if (obj.dn[ob]!=0) obj.up[obj.dn[ob]]=obj.up[ob];
	if (obj.up[ob]==0) top_ob(xx,yy)=obj.dn[ob];
	obj.dn[ob]=top_ob(x,y);
	obj.up[ob]=0;
	if (obj.dn[ob]!=0) obj.up[obj.dn[ob]]=ob;
	top_ob(x,y)=ob;
	obj.x[ob]=x;
	obj.y[ob]=y;
	obj.z[ob] = (y==SZ) ? 0 : level;
	from_to(ob,x,y,xx,yy,mode);
}

void from_to(int ob, int x, int y, int xx, int yy, int mode)
{
	int sq,o;

	if(ob==you) 
	{
		x_you = x;
		y_you = y;
	}
	
	// from //
	
	if(yy==SZ && xx==you) inv_sw=1;
	if(xx>0 && xx<SZ && yy>0 && yy<SZ)     
	{
		sq=map(xx,yy);
		o=top_ob(xx,yy);
		if((sq==3 || sq==4) && o==0 && mode!=MOVE_THROW)
			add_code("P0",xx,yy);
	}

	// to //
	
	if(y==SZ && x==you) inv_sw=1;
	if(x>0 && x<SZ && y>0 && y<SZ)     
	{
		sq=map(x,y);
		if((sq==3 || sq==4)  && mode!=MOVE_THROW) // On Pit
			if(ob!=0)
				if(obj.dn[ob]==0)
					add_code("P1",x,y);
		if(sq==5 || sq==24) // Teleport
			add_code("TP",x,y);
		if((sq==10 || sq==41)  && mode!=MOVE_THROW) // Pit
			add_code("PT",x,y);
		if(sq>=6 && sq<=8  && mode!=MOVE_THROW) // Turn it!!
			if(ob==you)
			{
				if(sq==6) drr=(drr+1)&3;
				if(sq==7) drr=(drr-1)&3;
				if(sq==8) drr=(drr+2)&3;
			}
	}
        if(mode==MOVE_DROP)
        {
                  if(ob==you)
                  {
                      do_damage(10);
                      disp_mess(2,4,"PIT!");
                  }
        }
}

void move_all(int x,int y, int xx, int yy, int mode)
{
	int ob,o1,o2,tp;

	tp = top_ob(xx,yy);
	ob = top_ob(x,y);
	if( ob==0 ) return;
	o1 = o2 = ob;
	for(; o1; o1=obj.dn[o1])
	{
		obj.x[o1] = xx;
		obj.y[o1] = yy;
		obj.z[o1] = (yy==SZ) ? 0 : level ;
		o2 = o1;
	}
	obj.dn[o2] = tp;
	if(tp!=0) obj.up[tp] = o2;
	top_ob(xx,yy) = ob;
	top_ob(x,y) = 0;

	from_to(o2,x,y,xx,yy,mode);
}

void add_code(char *cod,int x,int y)
{
	int c;

	c = (codebot==addcodemax) ? 1 : codebot+1;
	if(c==codetop) ex("Can't add code");
	strcpy(addcode[codebot],cod);
	addcode_x[codebot] = x;
	addcode_y[codebot] = y;
	codebot = c ;
}

void do_code()
{
	int x,y,i;
	char tr[3];

	for(;codebot!=codetop;codetop=(codetop==addcodemax) ? 1 : codetop+1)
	{
		strcpy(tr,addcode[codetop]);
		x=addcode_x[codetop];
		y=addcode_y[codetop];
		for(i=1;i<=trigend;i++)
			if(!strcmp(trig[i],tr) && trig_x[i]==x && trig_y[i]==y)
				if(exec_code(code_trig[i],x,y)) break;
	}
}

int exec_code(int cp,int tx, int ty)
{
	char *p;
	char buff[80];
	char com[3];
	int p1,p2,p3,p4;

	for(;;cp++)
	{
		strcpy(buff,code[cp]);
		p=strtok(buff," \t\n\r");
		strcpy(com,p);
		if(!strcmp(com,"..")) break;
		if(!strcmp(com,"MS"))
		{
			code_message(p);
			continue;
		}
		code_get4p(p,&p1,&p2,&p3,&p4);
		if(!strcmp(com,"CH")) 
		{
			code_change(p1,p2,p3);
			continue;
		}
		if(!strcmp(com,"TG"))
		{
			code_toggle(p1,p2,p3,p4);
			continue;
		}
		if(!strcmp(com,"TO"))
		{
			code_toggle(tx,ty,p1,p2);
			continue;
		}
		if(!strcmp(com,"MA"))
		{
			code_moveall(p1,p2,p3,p4,MOVE_NORMAL);
			continue;
		}
		if(!strcmp(com,"MV"))
		{
			code_move(p1,p2,p3,MOVE_NORMAL);
			continue;
		}
		if(!strcmp(com,"GE"))
		{
			code_get(p1);
			continue;
		}
		if(!strcmp(com,"TE"))
		{
			code_moveall(tx,ty,p1,p2,MOVE_NORMAL);
			continue;
		}
		if(!strcmp(com,"DR")) /* drop */
		{
			code_moveall(tx,ty,p1,p2,MOVE_DROP);
			continue;
		}
		if(!strcmp(com,"EX"))
		{
			code_ex(p1,p2);
			continue;
		}
		if(!strcmp(com,"DS"))
		{
			code_destroy();
			continue;
		}
		if(!strcmp(com,"IN"))
		{
			if(!code_in(p1,p2,p3)) return 0;
			continue;
		}
		if(!strcmp(com,"GT"))
		{
			if(!code_got(p1)) return 0;
			continue;
		}
		if(!strcmp(com,"HA"))
		{
			if(!code_have(p1)) return 0;
			continue;
		}
		if(!strcmp(com,"SP"))
		{
			if(!code_spr(p1)) return 0;
			continue;
		}
		if(!strcmp(com,"IF"))
		{
			if(!code_if(p1,p2,p3)) return 0;
			continue;
		}
		if(!strcmp(com,"TL"))
		{
			code_talk(p1);
			continue;
		}
	}
	return 1;
}       

void code_get4p(char *p, int *p1, int *p2, int *p3, int *p4)
{
	*p1 = *p2 = *p3 = *p4 = 0;
	p=strtok(NULL," \t\n\r");
	if(p==NULL) return;
	*p1=atoi(p);
	p=strtok(NULL," \t\n\r");
	if(p==NULL) return;
	*p2=atoi(p);
	p=strtok(NULL," \t\n\r");
	if(p==NULL) return;
	*p3=atoi(p);
	p=strtok(NULL," \t\n\r");
	if(p==NULL) return;
	*p4=atoi(p);
}

void code_message(char *p)
{
	p=strtok(NULL,"");
	disp_mess(2,4,p);
}

void code_change(int x, int y, int sq)
{
	change_sq(sq,x,y);
}

void code_toggle(int x, int y, int sq1, int sq2)
{
	int sq;

	sq = (map(x,y)==sq1) ? sq2 : sq1 ;
	change_sq(sq,x,y);
}

void code_moveall(int x, int y, int x1, int y1, int mode)
{
	move_all(x,y,x1,y1,mode);
}

void code_move(int ob, int x, int y, int mode)
{
	move_ob(ob,x,y,mode);
}

void code_get(int ob)
{
	if(ob_got!=0) return;
	move_ob(ob,you,SZ,MOVE_NORMAL);
	ob_got=ob;
}

int code_in(int x, int y, int ob)
{
	if(obj.x[ob]==x && obj.y[ob]==y) return 1;
	return 0;
}

int code_spr(int spr)
{
	if(ob_got==0) return 0;
	if(obj.spr[ob_got]==spr) return 1;
	return 0;
}

int code_got(int ob)
{
	if(ob==ob_got && ob_got!=0) return 1;
	return 0;
}

int code_have(int ob)
{
	if(obj.x[ob]==you && obj.y[ob]==SZ) return 1;
	return 0;
}

int code_if(int x, int y, int sq)
{
	if(map(x,y)==sq) return 1;
	return 0;
}

void code_ex(int x, int y)
{
	add_code("XX",x,y);
}

void code_destroy()
{
	move_ob(ob_got,0,0,MOVE_NORMAL);
}

void code_talk(int pers)
{
	talk_to(pers);
}

////////////////////////////////////////////

void disp_map()
{
	int i,j;

	for(j=0;j<24;j++){
		for(i=0;i<40;i++){
			putch(tiles[map(i,j)]);
		}
		getch();
	}
	getch();
}

void print6(int x, int y, int back, int fore, char line[])
{
	register int l;
	register int i;
	register char *scr_addr;
	register char *chr_addr;
	register int j;
	register char c,k;

	for(l=0;c=line[l];l++)
	{
	  if(!(x<0||x>320-6||y<0||y>200-6||c<32||c>127))
	  {
		scr_addr = (char *) SCREEN + x + y*320;
		chr_addr = (c-32)*6*6 + font6 ;
		if(back)
		{
			for(i=0;i<6;i++)
			{
				for(j=0;j<6;j++,scr_addr++,chr_addr++)
					*(scr_addr) = (*(chr_addr)) ? fore : back ;
				scr_addr += 320-6;
			}
		}
		else
		{
			for(i=0;i<6;i++)
			{
				for(j=0;j<6;j++,scr_addr++,chr_addr++)
					if(k=*(chr_addr)) *(scr_addr) = k ? fore : back ;
				scr_addr += 320-6;
			}
		}
		x += 6;
	  }
	}
}

void outch6(int x, int y, int back, int fore, char c)
{
	register char *scr_addr;
	register char *chr_addr;
	register int bak,fr;
	register int i,j;
	register char k;

	if(x<0||x>320-6||y<0||y>200-6||c<32||c>127) return;
	
	bak=back; fr = fore;
	scr_addr = (char *) SCREEN + x + y*320;
	chr_addr = (c-32)*6*6 + font6 ;
	if(bak)
	{
	  for(i=0;i<6;i++)
	  {
		for(j=0;j<6;j++,scr_addr++,chr_addr++)
			*(scr_addr) = (*(chr_addr)) ? fr : bak ;
		scr_addr += 320-6;
	  }
	}
	else
	{
	  for(i=0;i<6;i++)
	  {
		for(j=0;j<6;j++,scr_addr++,chr_addr++)
			if(k=*(chr_addr)) *(scr_addr) = k ? fr : bak ;
		scr_addr += 320-6;
	  }
	}
}

char input6(int x, int y, int b, int f, char buff[], int len)
{
	int cur;
	char key,key1;
	char blank[81] = "                                                                             ";
	
	cur = strlen(buff);
	blank[len+1]='\0';
	buff[cur]='*';buff[cur+1]='\0';
	for(;;)
	{
		print6(x,y,b,f,blank);
		print6(x,y,b,f,buff);
		disp_screen();
		
		get_input();
		key=tolower(inkey);
		key1=inkey1;
		
		switch(key) 
		{
		case 13:
			goto ret;
		case 27:
			goto ret;
		case 8:
			if(cur>0)
				buff[cur--]='\0';
				buff[cur]='*';
			break;
		default:
			if(cur<len && key>=' ' && key<=126)
			{
				buff[cur++]=key;
				buff[cur]='*';
				buff[cur+1]='\0';
				break;
			}
		}
	}
	ret:
	buff[cur]=' ';
	print6(x,y,b,f,buff);
	buff[cur]='\0';
	return key;
}

clear_messages()
{
	int i;

	for(i=150;i<200;i+=6)
		print6(0,i,2,2,blank80);
}

void plot(int x, int y, char c)
{
	*((char *)SCREEN+x+y*320) = c;
}

char pix_colour(int x, int y)
{
	return *((char *)SCREEN+x+y*320);
}

void copy_screen()
{
	register long *s,*b;
	register int i;

	s = (long *) SCREEN_LIN;
	b = (long *) back_screen;
	for(i=0;i<16000;i++) *s++ = *b++;
}

void _copy_screen(void);
#pragma aux _copy_screen =\
		"mov edi,0xA0000"\
		"lea esi,dword ptr back_screen"\
		"mov ecx,16000"\
		"rep movsd"\
		 modify [ecx esi edi];

void gotoxy(int x, int y)
{
	union REGS r;

	r.w.cx = 0 ;
	r.h.dh = y ;
	r.h.dl = x ;
	r.h.bh = 0 ;
	r.w.ax = 0x0200 ;
	int386( 0x10, &r, &r ) ;

}

void clrs()
{
	union REGS r;

	r.w.cx = 0 ;
	r.w.dx = 0x1850 ;
	r.h.bh = 7 ;
	r.w.ax = 0x0600 ;
	int386( 0x10, &r, &r ) ;
	gotoxy(0,0);
}

void sprite(int sp, int x, int y)
{
	register char *vid;
	register char *s;
	register int i,k;

	vid = (char *) SCREEN + x + y*320;
	s = spr + (sp-1)*256 ;
	for( k=0 ; k<16 ; k++ )
	{
		for( i=0 ; i<16 ; i++,s++,vid++ ) if(*s) *vid = *s ; 
		vid += 320-16 ;
	}
}

void sprite_down(int sp, int x, int y)
{
	register char *vid;
	register char *s;
	register int i,k;

	vid = (char *) SCREEN + (x+1) + (y+1)*320;
	s = spr + (sp-1)*256 + 1;
	for( k=0 ; k<14 ; k++ )
	{
		for( i=0 ; i<14 ; i++,s++,vid++ ) if(*s) *vid = *s ; 
		s+=2;
		vid += 320-14 ;
	}
}

void spr_back(int mode, int x, int y)
{
	register char *vid;
	static char back[256]={0};
	register char *s;
	register int i,k;

	vid = (char *) SCREEN + x + y*320;
	s = back;
	if(mode==1){
		for( k=0 ; k<16 ; k++ )
		{
			for( i=0 ; i<16 ; i++,s++,vid++ )  *s = *vid ; 
			vid = vid-16+320 ;
		}
	}
	else if(mode==2)
	{
		for( k=0 ; k<16 ; k++ )
		{
			for( i=0 ; i<16 ; i++,s++,vid++ )  *vid = *s; 
			vid = vid-16+320 ;
		}
	}
}

void spr_back1(int x, int y)
{
	register char *vid;
	register char *s;
	register int i,k;

	vid = (char *) SCREEN + x + y*320;
	s = sprite_back;
	for( k=0 ; k<16 ; k++ )
	{
		for( i=0 ; i<16 ; i++,s++,vid++ )  *s = *vid ; 
		vid = vid-16+320 ;
	}
}

void spr_back2(int x, int y)
{
	register char *vid;
	register char *s;
	register int i,k;

	vid = (char *) SCREEN + x + y*320;
	s = sprite_back;
	for( k=0 ; k<16 ; k++ )
	{
		for( i=0 ; i<16 ; i++,s++,vid++ )  *vid = *s; 
		vid = vid-16+320 ;
	}
}

void rem_mouse()
{
	/* Remove mouse handler */
	inregs.w.ax=0;
	int386( 0x33, &inregs, &outregs );
}

int random(int n)
{
	seed = (seed+1)*75 % 65537L - 1;
	return seed % (n+1);
}

void scroll_mess()
{
	register char *from, *to;
	register int i,j;
	#define lines 6

	to = (char *)( SCREEN + SCREEN_WIDTH * (SCREEN_HEIGHT-MESS_HEIGHT) );
	from = (char *)( SCREEN + SCREEN_WIDTH * (SCREEN_HEIGHT-MESS_HEIGHT + lines ) );
	// Scroll up
	for( j=0; j < MESS_HEIGHT - lines ; j++)
		for( i=0; i<SCREEN_WIDTH; i++ )
			*to++ = *from++;
	// Blank to end of screen
	for( j=0; j < lines ; j++)
		for( i=0; i<SCREEN_WIDTH; i++ )
			*to++ = 0;
}

void disp_mess(int back, int fore, char *mess)
{
	//scroll_mess();
	print6(0,MESS_LINE,back,fore,mess);
	disp_screen();
}

void disp_debug(char *mess, int debug)
{
	char text[80];

	sprintf(text,"%s = %d",mess,debug);
	disp_mess(BLACK,YELLOW,text);
}

void disp_hits(int x, int y, int hits, int str)
{
	char text[80];
	int col;

	col = GREEN;
	if(str<20) col = YELLOW;
	if(str<10) col = RED;

	sprintf(text,"%d",hits);
	print6(x*16-12,y*16-12,BLACK,col,text);
	disp_screen();
	wait_interval(CLOCKS_PER_SEC/5);
	disp_position();
}

void disp_at(int x, int y, int num)
{
	char text[80];

	sprintf(text,"%d",num);
	print6(x*16+8,y*16+8,BLACK,YELLOW,text);
}

void omgra_mindmelt()
{
	int i;

	obj.str[255] = 0;
	obj.spr[255] = DEAD_SPR;
	SCREEN = (char *)SCREEN_LIN;
	disp_clear(LGREY);
	print6shad(50,40,LGREY,RED,"Mr Flibble says , 'Game over boys!'");
	print6shad(40,70,LGREY,YELLOW,"Realising that Omgra is in fact the last");
	print6shad(40,80,LGREY,YELLOW," part of your personality, Mind-melting ");
	print6shad(40,90,LGREY,YELLOW,"with him took away all his powers and ");
	print6shad(40,100,LGREY,YELLOW,"    saved the world. Well done!!! ");
	SCREEN = (char *)back_screen;
	get_mouse_and_keys();
}

void disp_clear(char col)
{
	int i;
	char *screen;

	screen = SCREEN;
	for(i=0;i<64000;i++)
		*screen++ = col;
}

void edit_sprite()
{
	char line[] = "Sprite number> ";
	char num[8]="";
	char c;
	int x,y,k,i,n;
	char *s;
	
	print6(0,193,BLACK,WHITE,line);
	input6(strlen(line)*6,193,2,7,num,3);
	scroll_mess();
	print6(0,193,BLACK,YELLOW,num);
	n=atoi(num);
	SCREEN = (char *) SCREEN_LIN;
	s = spr + (n-1)*256 ;
	for( k=0 ; k<16 ; k++ )
		for( i=0 ; i<16 ; i++,s++ ) 
			for(y=0;y<8;y++)
				for(x=0;x<8;x++)
					plot(i*8+x,k*8+y,*s);
	get_input();
	SCREEN = (char *) back_screen;
}
