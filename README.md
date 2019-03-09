!!! Shakespearian Monkeys !!!

Tous droits réservés :  Laurent GENTY ; Florian MORNET

Ce projet a été réalisé durant le premier semestre à l'ENSEIRB-Matmeca dans le cadre de notre formation en tant qu'ingénieur en informatique. Il vient s'intégrer durant notre formation au lanage C et en algorithmie.

Objectif de ce projet :
	 * réaliser une reproduction de texte déjà existant
	 * générer du texte aléatoire à partir de texte déjà existant
	 * gérer des structures en C
	 * gérer des listes chaînées

----------------------------------

# Too Long ; Didn't Read

	 * Version rendue : Achievement n°1 (version du master, version la plus avancée dans le projet fonctionnelle)
	 * Autres versions sur les autres branches du git
	 * Compiler avec ./project fichier.txt -s seed -t turns
	 * Output sous forme : texte aléatoire... puis statistiques dessus

----------------------------------

# Pré-requis

Il vous faut un compilateur C : GCC... normalement ça va.
Convention std=c99

----------------------------------

# Rendu

La version rendue est la version finale du dernier achievement fonctionnel sur master : à savoir l'achievement numéro 1. Cependant, afin de gestionner notre projet, nous avons choisi d'utiliser les branches de Git et non pas de créer plusieurs dossiers c'est donc pourquoi si jamais vous souhaitez aller voir le code correspondant à la version de base ou bien l'avancement des autres achievements (en l'occurrence, achievement 2), il vous suffit tout simplement de naviguer entre les branches du Git (dans lesquels il y aura les fichiers des différentes versions : Makefile, ...).

Fichiers :
	 * achiev1.c (ou autre version) : main
	 * code source : cell, queue...
	 * tests/ : dossier test
	 * Makefile
	 * input/ : jeu de test pour le programme
	 * rapport/ : rapport pdf + latex

----------------------------------

# Installation

Pour installer le programme rien de plus simple : il vous suffit de simplement clone le dossier sur votre machine locale et de compiler (cf. ci-après). Ou alors vous télécharger le version finale que nous avon rendu le 14 décembre.

----------------------------------

# Compilation

Afin de compiler le programme il vous faut au préalable un fichier rempli de chaînes de caractères (pas forcément un fichier .txt, tout marche!). Afin de pouvoir compiler le projet vous devrez utiliser les commandes suivantes :

     * make clean : clean le directory (si cela n'a pas déjà été fait par nos soins)
     * make : compiler l'ensemble des dépendances et créer les fichiers objets adéquats
     * ./project fichier.txt <-s:seed> <-t:number of turns>

Afin de bien comprendre la commande ci-dessus, il faut impérativement que vous mettiez en argument un fichier. En effet, si vous ne le faites pas, alors le jeu se terminera directement et vous affichera un message "Usage" afin de vous rappelez comment utiliser le programme.
Afin de diversifier un peu, nous avons rajouter deux options :
     - s : correspondant à la seed permettant de générer des nombres aléatoires
     - t : correspondant au nombre de tours que l'on souhaite
Ces différentes options et le fichier peuvent être placés dans n'importe quel ordre à condition que le chiffre que l'on souhaite pour l'option soit placée juste après l'option (par exemple : nous souhaitons la seed 42 et 300 tours, nous ne pouvons pas compiler comme ceci : ./project bla.txt -s -t 42 300).

----------------------------------

# Output

Dans la version rendue, donc achievement 1, notre rendu ressemble à cela :

the riper should ; from fairest . from fairest creatures we desire increase that thereby beauty's rose might never die but as the riper should by time decease his tender heir might never . should by time : fresh ornament and only herald to thine own bright eyes feed'st thy light's flame with self-substantial fuel making a famine where abundance . feed'st thy light's flame with self-substantial fuel making a famine where abundance lies thy light's flame ; a !
[...]
Max turns reached: end of the game
[ 1500 turns passed ]
Number of words read: 83
Number of different words: 65
Number of words printed: 645
Minimal occs : 1
from(1) | 
Maximal occs : 5
thy(5) | 

Dans un premier temps nous avons l'imprimeur qui écrit dans le terminal les phrases construites par son écrivain et un résumé de ce qu'il s'est passé :
     * si le nombre de tours maximum a été atteint
     * des statistiques sur les mots qui ont été lus, affichés, ...
     * des statistiques sur le minimum et le maximum d'occurrences

ATTENTION : lorsque l'on souhaite régler le nombre de tours avec l'option -t, veuillez à ne pas mettre des chiffres trop grands. En effet, soit il n'y aura pas assez de mémoire et selon la taille du fichier, le jeu se terminera après plus ou moins de temps. Mais aussi, il est possible que sur la forge, la limite de caractères soit atteinte et donc qu'il y ait des erreurs.

----------------------------------

# Todo

Nous ne sommes malheureusement pas arrivé bien loin dans l'achievement 2. En effet nous nous étions concentré sur l'élaboration d'un code propre et fonctionnel. Cependant dans l'achievement 2 il nous reste à faire en sorte que les singes se réveillent à la fin d'une phrase mais aussi qu'ils lisent chacun un texte bie propre et non pas une suite de mot depuis la même file.

----------------------------------

# Contributeurs

Coutributeurs courants :
	      * Laurent Genty
	      * Florian Mornet
