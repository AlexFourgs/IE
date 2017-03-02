# Projet "Gumstix" réalisé par Alexandre Fourgs & Arya Jemo

## Introduction

<img src="https://github.com/AlexFourgs/IE/raw/master/RapportSceance/gumstix.jpg" width="300">

Le projet "Gumstix" a été réalisé au cours de notre première année de Master d'Informatique et Ingénierie des Systèmes Complexes pour le cours d'Informatique Embarqué encadré par M. Pierre Andry.
Le but de ce projet était d'aborder les notions de temps réél en informatique embarqué sur un Gumstix. Le sujet était de mettre en place une communication UDP entre le Gumstix et un serveur (ordinateur) servant à envoyer les données récoltées par les accéléromètres du Gumstix.

## Étude de la connexion UDP

*Afin d'éviter de surcharger le document avec des informations "inutiles", on ne précisera pas les étapes préliminaires comme la configuration du Gumstix et du serveur*

Le Gumstix est configuré pour envoyé les données à une certaine cadence tout en ayant une contrainte de temps (deadline) à ne pas dépasser. Si cette contrainte de temps est dépassée, on parle d'une échéance manquée, et la donnée n'est donc pas exploitable puisqu'envoyer "en retard".

### Rappel temps réél mou et temps réél dur

#### Le temps réel dur
"Ne tolère aucun dépassement de ces contraintes, ce qui est souvent le cas lorsque de tels dépassements peuvent conduire à des situations critiques, voire catastrophiques : pilote automatique d'avion, système de surveillance de centrale nucléaire, etc. On peut considérer qu'un système temps réel strict doit respecter des limites temporelles données même dans la pire des situations d'exécution possibles."

#### Le temps réel souple 
"S'accommode de dépassements des contraintes temporelles dans certaines limites au-delà desquelles le système devient inutilisable ou pénible : visioconférence, jeux en réseau, etc. Un système temps réel souple doit respecter ses limites pour une moyenne de ses exécutions. On tolère un dépassement exceptionnel, qui pourra être compensé à court terme."

### Déportation des calculs

Afin de respecter le plus possible les contraintes de temps réel, nous avons déportés l'intégralité des calculs possibles sur le serveur, l'ordinateur hôte. Tel que le calcul de l'intervalle des mesures reçues, l'écriture des résultats dans un fichier et la génération de courbes au format image.

### Protocole

Nous avons étudié la "qualité" de la connexion afin de déterminer les limites du Gumstix en matière de cadences et échéances.
Les échéances sont proportionnelles à la cadence.

échéance = cadence/10

Pour tester le Gumstix nous avons donc augmenté la cadence d'envois des données palier par palier tout en baissant la deadline. Côté serveur, les derniers paquets reçus permettent de récolter les informations sur la configuration mise en place sur le Gumstix :
* cadence
* valeur de l'échéance
* échéance(s) manquée(s)
* nombre de paquets envoyés

Le serveur compte le nombre de paquet reçu aussi.

Voici les résultats obtenus :

<img src="https://github.com/AlexFourgs/IE/raw/master/RapportSceance/courbeie.png">

Commentaires sur la courbe

## Récupération de la position du Gumstix

### Principe

Le Gumstix est équipé de trois accéléromètres pour chaque axes (X, Y, Z).

