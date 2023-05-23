---------------
ATTENTION, l'utilisation de cette bibliothèque n'est pas obligatoire.
Le principal c'est de fournir un projet qui tourne meme si il n'y a pas des appuis touches dynamiques (scanf suffisant).

Si vous tentez d'utiliser des biliothèques (celle ci ou une autre) mais que vous galérez à l'utiliser et qu'a cause de ca votre projet n'est pas fonctionnel, vous serez pénalisé pour quelque chose qui n'est pas explicitemen demandé : donc réfléchissez bien au but qui motive l'utilisation d'une bibliothèque externe.

---------------
Il existe un makefile deja présent pour compiler la lib et pour lancer le programme de test
mais il vous faudra surement le modifier pour compiler vos propres fichiers. Prenez ça en compte aussi dans votre choix.

---------------
Je fournis un fichier test.c qui est le fichier principal
Il y a dedans des TODO, qui indiquent là ou vous devez mettre votre code.

---------------
Dans le fichier test.c il y a des entetes qui indiquent ce que font chacune des fonctions.
Lisez les bien : 
- il y a une fonction qui initialise vos données de jeu (init)
- une fonction qui récupère les appuis touches dynamiquement (event)
- une fonction qui met à jour vos données de jeu en fonction du temps si besoin (update)
- une fonction qui affiche les données à l'écran (draw)
- une fonction qui termine le jeu (finish) utilisée basiquement à la fin du programme

Ces fonctions sont appelées automatiquement par le moteur de jeu.
La fonction main configure le jeu et le lance justement.
Dans l'exemple vous avez un petit affichage de quelques caracteres à l'écran pour vous montrer comment cela fonctionne

---------------
Si vous voulez quitter le programme en indiquant qu'il y a eu un souci, 
vous pouvez utiliser l'instruction suivante 
RAGE_QUIT(99, "Mon message comme avec printf %d %c %f \n", 10, 'A', 5.237);
Elle affichera tout plein d'informations utiles comme le nom du fichier et la ligne qui a créé le probleme.
Le premier paramètre est le code d'erreur que vous voulez retourner, ensuite
vous mettez l'affichage que vous voulez comme avec printf.

---------------
Pour compiler, vous tapez make
ensuite pour lancer vous pouvez taper make run mais vous ne verrez pas les messages d'erreur.
Je vous conseille de taper une fois par session de terminal la commande suivante
export LD_LIBRARY_PATH=lib
ensuite quand vous aurez compilé avec make vous pouvez lancer votre executable (par defaut j'ai mis ./test)

---------------
Si vous voulez afficher des informations de debug, vous ne pourrez pas le faire directement à l'écran mais ces infos peuvent être récupérées dans un fichier de sortie
vous pouvez utiliser la fonction debug(...) qui s'utilise exactement comme printf
Pour récupérer après exécution de votre programme le résultat du debug, vouspouvez lancer le jeu comme suit : 
./test 2> error.log
vous aurez dans le fichier error.log l'ensemble de vos printf de debug ;)
