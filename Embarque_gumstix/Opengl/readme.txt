

Canvas OpenGl pour les TP de l'universit� de Cergy-Pontoise.

Pierre Andry


Affichage :
-----------

Affiche un point et un segment en rouge dans une fenetre openGL.
L'affichage est bas� sur la projection du plan 2D de la fenetre.

l'affichage se fait dans la fonction affiche(). 

Pour un affichage 3D, utiliser les primitives 
 
glVertex3f(x,y,z); en conservant les modes GL_POINTS et GL_LINE_LOOP.


Un texte informatif (d�compte du nombre de pas de calcul) est affich�
en haut � gauche.


un printf de la chaine "affichage" est effectu� en sortie standard
pour que vous compreniez quand se fait l'affichage.



Gestion de la souris :
----------------------

Seul le clic gauche est pris en compte.
clic gauche + mouvement de la souris = rotation du point de vue par
rapport au centre de la fen�tre



Calculs :
---------
Les calculs de simulations sont � mettre ou a appeler dans la fonction idle();


Gestion du clavier :
---------------------


'p' commence/interomp la simulation

'q' pour quitter
 


