#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
//#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>
#include <intelfpgaup/accel.h>

int screen_x, screen_y;

int char_x, char_y;

void gen_line (int *, int *, int *, int *, unsigned *);

void print_text (int, int, int, int, unsigned);

//volatile sig_atomic_t stop;

//void catchSIGINT(int signum){
//    stop = 1;
//}

void printa_gaiola(unsigned int color){
	//linha horizontal em cima
	video_line(0, 0, 319, 0, color);
	//fazer linha lateral esqu
	video_line(0,0, 0, 239, color);
	// fazer linha dir
	video_line(319,0,319,239, color);
	//
	video_show();
}

/* This program draws randomly-generated lines on the VGA display. The code uses
* the character device drivers IntelFPGAUP/video and IntelFPGAUP/KEY. It draws a
* random line each time a pushbutton KEY is pressed. The coordinates of each new
* line are displayed on the bottom right of the screen. Exit by typing ^C. */
int main(int argc, char *argv[]) {
    unsigned int video_color[] = {video_WHITE, video_YELLOW, video_RED,
    video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
    video_PINK, video_ORANGE};

   // int KEY_data;
    int x1, y1, x2, y2;
    unsigned color;
    char msg_buffer[80];
    time_t t; // used to seed the rand() function
    // catch SIGINT from ^C, instead of having it abruptly close this program
    //signal(SIGINT, catchSIGINT);
    srand ((unsigned) time(&t)); // seed the rand function
    // Open the character device drivers
   // if (!KEY_open ( ) || !video_open ( ))
   //     return -1;
   video_open();
    video_read (&screen_x, &screen_y, &char_x, &char_y); // get screen & text size
   printf("Tamanho tela em x=%i y=%i\n Tamanho tela para texto em x=%i e y=%i\n", screen_x, screen_y, char_x, char_y); 
video_erase ( ); // erase any text on the screen
    // set random initial line coordinates
    //gen_line (&x1, &y1, &x2, &y2, &color);
    // Faz linha vertical
    x1 = 50;//rand()%(screen_x - 1); // random x position
    y1 = 100;//rand()%(screen_y - 1); // random y position
    x2 = 50;//rand()%(screen_x - 1); // random x position
    y2 = 140;//rand()%(screen_y - 1); // random y position
    color = video_color[1]; // random out of 10 video colors


    /* There are two VGA buffers. The one we are drawing on is called the Back
    * buffer, and the one being displayed is called the Front buffer. The function
    * video_show swaps the two buffers, allowing us to display what has been drawn
    * on the Back buffer */
    video_clear ( ); // clear current VGA Back buffer
    video_show ( ); // swap Front/Back to display the cleared buffer
    video_clear ( ); // clear the VGA Back buffer, where we will draw lines
  
    
  // x1=5; x2=5; y1=100; y2=150;color=0; 
   //video_line (x1, y1, x2, y2, color);
  //video_show();	
	int i = 0;
	int aux_x1 = 0;
	int aux_y1 = 0;
	int aux_x2 = 0;
	int aux_y2 = 0;
	
	printa_gaiola(color);

	while(1){
		video_show();

		aux_x1=40+i;
		aux_y1=20+i;
		
		aux_x2=41+i;
		aux_y2=21+i;
		
		
		video_box(aux_x1-1,aux_y1-1,aux_x2-1,aux_y2-1,0);
	
		video_box(40+i, 20+i,41+i ,21+i , color);
		video_show();
		i = i+1;
		if (i==10000){
			printf("SAindo do loop\n\n");
			break;
		}

	}
  


	//A barra que move
	video_line(144, 235, 174, 235, color);

video_text (0, 59, "MSG final aqui");

    // while (!stop) {
    //     sprintf (msg_buffer, "(%03d,%03d) (%03d,%03d) color:%04X", x1, y1, x2, y2,
    //     color);
    //     video_text (char_x - strlen(msg_buffer), char_y - 1, msg_buffer);
    //     printf ("Press a pushbutton KEY (^C to exit)\n");
    //     KEY_read (&KEY_data);
    //     while (!KEY_data && !stop)
    //         KEY_read (&KEY_data);
    //     gen_line (&x1, &y1, &x2, &y2, &color);
    //     video_show ( ); // swap Front/Back
    //     video_line (x1, y1, x2, y2, color);
    //     video_show ( ); // swap Front/Back to display lines
    // }
    video_close ( );
   // KEY_close ( );
    printf ("\nExiting sample program\n");
    return 0;
}

/* Generate a new random line and change the values by referer*/
void gen_line (int *x1, int *y1, int *x2, int *y2, unsigned *color) {
    unsigned int video_color[] = {video_WHITE, video_YELLOW, video_RED,
    video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
    video_PINK, video_ORANGE};
    *x1 = 50;//rand()%(screen_x - 1); // random x position
    *y1 = 100;//rand()%(screen_y - 1); // random y position
    *x2 = 50;//rand()%(screen_x - 1); // random x position
    *y2 = 140;//rand()%(screen_y - 1); // random y position
    *color = video_color[0]; // random out of 10 video colors
}
