

Canvas OpenGl pour les TP de l'université de Cergy-Pontoise.

Pierre Andry


Affichage :
-----------

Affiche un point et un segment en rouge dans une fenetre openGL.
L'affichage est basé sur la projection du plan 2D de la fenetre.

l'affichage se fait dans la fonction affiche(). 

Pour un affichage 3D, utiliser les primitives 
 
glVertex3f(x,y,z); en conservant les modes GL_POINTS et GL_LINE_LOOP.


Un texte informatif (décompte du nombre de pas de calcul) est affiché
en haut à gauche.


un printf de la chaine "affichage" est effectué en sortie standard
pour que vous compreniez quand se fait l'affichage.



Gestion de la souris :
----------------------

Seul le clic gauche est pris en compte.
clic gauche + mouvement de la souris = rotation du point de vue par
rapport au centre de la fenêtre



Calculs :
---------
Les calculs de simulations sont à mettre ou a appeler dans la fonction idle();


Gestion du clavier :
---------------------


'p' commence/interomp la simulation

'q' pour quitter
 


