/*
Simple programme d'affichage de points et de segments en opengl
utilise GL et glut
*/



#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "base_opengl.h"

int cpt=0;
char presse,calc;
Point ville;
int anglex,angley,x,y,xold,yold;


/* affiche la chaine fmt a partir des coordonnées x,y*/
void draw_text(float x, float y, const char *fmt, ...)
{
	char		buf[1024];			//Holds Our String
	char		*text = buf;
	va_list		ap;				// Pointer To List Of Arguments

	if (fmt == NULL)				// If There's No Text
		return;					// Do Nothing

	va_start(ap, fmt);				// Parses The String For Variables
	    vsprintf(text, fmt, ap);			// And Converts Symbols To Actual Numbers
	va_end(ap);					// Results Are Stored In Text

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glRasterPos2f(x, y);
	while (*text)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text++);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

/* *************************************************** */
/* main */
int main(int argc,char **argv) {

 
/* initilisation de glut et creation de la fenetre */
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
glutInitWindowPosition(200,200);
glutInitWindowSize(530,530);
glutCreateWindow("Kohonen");

/* initialisation OpenGL */
glClearColor(0,0,0,0);
glColor3f(1,1,1);
glPointSize(4);
glEnable(GL_DEPTH_TEST);

/* enregistrement des fonctions de rappel */
glutDisplayFunc(affichage);
glutKeyboardFunc(clavier);
glutReshapeFunc(reshape);
glutIdleFunc(idle);
glutMouseFunc(mouse);
glutMotionFunc(mousemotion);



/* Entree dans la boucle principale */
glutMainLoop();
return 0;
}


/* *************************************************** */

/* fonction d'affichage appelée a chaque refresh*/

void affichage() {
 int i,j;
 printf("affichage\n");
 /* effacement de l'image avec la couleur de fond */
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glLoadIdentity();
 glRotatef(-angley,1,0,0); 
 glRotatef(-anglex,0,1,0);
 
 glColor3f(1.0,1.0,1.0);
 draw_text(-0.9,0.9,"nb iter: %d",cpt);


 glBegin(GL_POINTS);
 glColor3f(1.0,0.0,0.0);
 glVertex2f(0.4,0.1);
 glEnd();


 glBegin(GL_LINE_LOOP); 
  glVertex2f(0.50,0.50);
  glVertex2f(0.4,0.1);
 glEnd();

 glFlush();
  

 /* On echange les buffers */ 
 glutSwapBuffers();
} 


void idle(){
	int i;
	if(calc){
	  cpt++;
	  printf("mettre les calculs ici\n");
   glutPostRedisplay();
	}
}


void clavier(unsigned char touche,int x,int y) {
  int i;
 switch(touche) {
  case 'p': 
  	calc=!calc;
	  //if(calc) calc=0; else calc=1;
   break;

  case 'q': /* la touche 'q' permet de quitter le programme */
   exit(0);
 } /* switch */

} /* clavier */




void reshape(int x,int y) {
 if (x<y)
  glViewport(0,(y-x)/2,x,x);
 else
  glViewport((x-y)/2,0,y,y);  
} /* reshape */



/* getion des boutons de  la souris*/
void mouse(int bouton,int etat,int x,int y) {
 /* si on appuie sur la bouton de gauche */
 if (bouton==GLUT_LEFT_BUTTON && etat ==GLUT_DOWN) {
  presse=1; // vrai 
  xold=x;   // sauvegarde de la position de la souris
  yold=y;
 }
 
 /* si on relache la souris */
 if (bouton==GLUT_LEFT_BUTTON && etat ==GLUT_UP) {
  presse=0; // faux
 }
} /* mouse */



/*gestion des mouvements de la souris */
void mousemotion(int x,int y) {
 if (presse) { /* si le bouton gauche est presse */
  /* on mofifie les angles de rotation de l'objet en fonction de la position actuelle de la souris et de la derniere position sauvegard?e */ 
  anglex=anglex+(x-xold);
  angley=angley+(y-yold);
  glutPostRedisplay();
 }

 xold=x; /* sauvegarde des valeurs courante des positions de la souris */
 yold=y;
} /* mousemotion */
