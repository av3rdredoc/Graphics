#include<iostream>
using namespace std;
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>

int BOXSIZE;          //constant to describe the diemnsion of the square of the game of life box
//global variables
int m,n,b1=0,b2=1;          //m is the hieght of box, n is the width of the box, b1 and b2 are buffer states that change value between 1 and 0
int win_width,win_height;
bool **state;              //to describe the state of the matrix buffer
void draw_box(int i, int j);         // draws a small box
void draw(void);            // draws the complete grid and makes changes to the grid as values of b1 and b2 are changed
void state_change();             //function to update the state of the cells
void idle(void);            //idle function
void my_mouse ( int btn, int mouse_state, int x, int y );       //initialize the board state with mouse buttons
void my_keyboard ( unsigned char key, int x, int y );           //updates the state of the cellular automaton
void state_reset ( int m, int n, int i, int j );
void init(void)
{
  glClearColor ( 1.0, 1.0, 1.0, 0.0 );
  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity ( );
  gluOrtho2D ( 0.0, ( double ) win_width, 0.0, (double ) win_height );
  glMatrixMode ( GL_MODELVIEW );
}

int main(int argc, char **argv)
{
    cout<<"\nEnter the height(no of vertical cells):";
    cin>>m;
    cout<<"\nEnter the width(no of horizontal cells):";
	cin>>n;
	//each cell is a square of dimension BOXSIZE

   if(m==n)
    {
        BOXSIZE=500/n;
        win_height=500;
        win_width=500;
    }
    else if(m>n)
    {
        BOXSIZE=500/m;
        win_height=BOXSIZE*m;
        win_width=BOXSIZE*n;
    }
    else if(m<n)
    {
        BOXSIZE=500/m;
        win_height=BOXSIZE*m;
        win_width=BOXSIZE*n;
    }

    win_height=BOXSIZE*m;
    win_width=BOXSIZE*n;
    glutInit(&argc, argv);
    state = new bool*[m*n];

    for(int i = 0; i < m*n; ++i)
    state[i] = new bool[2];
    int i,j;
    for(j=0;j<n;j++)
    {
        for(i=0;i<m;i++)
            state[i+j*m][b1]=state[i+j*m][b2]=false;
    }


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(0,0);

	glutCreateWindow("CONWAYS GAME OF LIFE");
	glutDisplayFunc(draw);
	init();
    glutMouseFunc ( my_mouse );
    glutKeyboardFunc ( my_keyboard );
	//glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void box_draw(int i,int j)
{
    int x,y,c;
    c=BOXSIZE;
    x=i*BOXSIZE;
    y=j*BOXSIZE;
    float grey[3] = { 0.8, 0.8, 0.8 };
    float yellow[3] = { 1.0, 1.0, 0.0 };

    if(state[i+j*m][b1])
        glColor3fv(yellow);
    else
        glColor3fv(grey);

    glBegin ( GL_POLYGON );
     glVertex2d(x,y);
     glVertex2d(x+c,y);
     glVertex2d(x+c,y+c);
     glVertex2d(x,y+c);
    glEnd();
    glColor3f(0,0,1.0);
    glBegin ( GL_LINE_LOOP );
     glVertex2d ( x,y );
     glVertex2d ( x+c,y );
     glVertex2d ( x+c,y+c );
     glVertex2d ( x,y+c );
    glEnd();

 glFlush ( );

 return;
}

void draw()
{
    int i,j;
    glClear ( GL_COLOR_BUFFER_BIT );
    for(j=0;j<n;j++)
    {
        for(i=0;i<m;i++)
            box_draw(i,j);
    }

  glFlush ( );
  glutSwapBuffers ( );

  return;
}

void state_change()
{
    int i,j,neighbors;
    for ( j = 0; j < n; j++ )
    {
      for ( i = 0; i < m; i++ )
      {
        neighbors = 0;
        if ( 0 < i && 0 < j )
        {
          neighbors = neighbors + ( int ) state[(i-1)+(j-1)*m][b1];
        }
        if ( 0 < i )
        {
          neighbors = neighbors + ( int ) state[(i-1)+(j)*m][b1];
        }
        if ( 0 < i && j < n - 1 )
        {
          neighbors = neighbors + ( int ) state[(i-1)+(j+1)*m][b1];
        }
        if ( 0 < j )
        {
          neighbors = neighbors + ( int ) state[(i)+(j-1)*m][b1];
        }
        if ( j < n - 1 )
        {
          neighbors = neighbors + ( int ) state[(i)+(j+1)*m][b1];
        }
        if ( i < m - 1 && 0 < j )
        {
          neighbors = neighbors + ( int ) state[(i+1)+(j-1)*m][b1];
        }
        if ( i < m - 1 )
        {
          neighbors = neighbors + ( int ) state[(i+1)+(j)*m][b1];
        }
        if ( i < m - 1 && j < n - 1 )
        {
          neighbors = neighbors + ( int ) state[(i+1)+(j+1)*m][b1];
        }

        if ( neighbors == 3 )
        {
          state[i+j*m][b2] = true;
        }
        else if ( neighbors == 2 )
        {
          state[i+j*m][b2] = state[i+j*m][b1];
        }
        else
        {
          state[i+j*m][b2] = false;
        }
      }
    }
    b1=1-b1;
    b2=1-b2;
   /* for ( j = 0; j < n; j++ )
  {
    for ( i = 0;  i < m; i++ )
    {
      state[i+j*m][b1] = state[i+j*m][b2];
    }
  }*/
 /* int temp;
  temp=b1;
  b1=b2;
  b2=temp;
*/
}

void my_mouse ( int btn, int mouse_state, int x, int y )
{
  int i;
  int j;
  int k;

  j = y / BOXSIZE;
  i = x / BOXSIZE;

  if ( btn == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN )
  {
    state_reset ( m, n, i, j );
  }
  else if ( btn == GLUT_MIDDLE_BUTTON && mouse_state == GLUT_DOWN )
  {
    state_reset ( m, n, i, j );
  }
  else if ( btn == GLUT_RIGHT_BUTTON && mouse_state == GLUT_DOWN )
  {
    state_reset ( m, n, i, j );
  }
//
//  Redisplay the screen.
//  Since this causes a jerky screen, it would be best to double buffer!
//
  draw ( );

  return;
}

void state_reset ( int m, int n, int i, int j )

{
  if ( i < 0 || m <= i )
  {
    cerr << "\n";
    cerr << "STATE_RESET - Fatal error!\n";
    cerr << "  Illegal row index I.\n";
    exit ( 1 );
  }

  if ( j < 0 || n <= j )
  {
    cerr << "\n";
    cerr << "STATE_RESET - Fatal error!\n";
    cerr << "  Illegal column index J.\n";
    exit ( 1 );
  }
  state[i+(m-1-j)*m][b1] = !state[i+(m-1-j)*m][b1];

  return;
}
int num=0;
int delay=10;
void idle(void)
{

    if(!(++num %delay))
    {
        delay = rand()%100 + 50;
    }
    glutPostRedisplay();
}

void my_keyboard ( unsigned char key, int x, int y )

{
  state_change ();

  draw ( );
  return;
}
