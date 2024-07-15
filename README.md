# Simulation de gaz parfaits

## Description

Comme son nom l'indique, l'objectif est de modéliser le comportement des gaz parfaits pour vérifier par simulation plusieurs lois (loi des gaz parfaits, loi de Dalton, etc) et visualiser des transformations (cycle de Stirling, cycle de Carnot, etc). 

### Avertissements

Ce fichier n'est pas une documentation, mais explique comment utiliser le programme.

Aucune documentation n'est fournie avec le code.

Ce projet est à la base un devoir pour un cours de programmation. De nombreuses fonctionnalités ont été ajoutées pour approfondir l'énoncé. Ainsi, certains commentaires ou nom de fichiers sont adaptés pour la correction (exemple exerciceP9.cc).

## 

## Sommaire

[Fonctionnalités](#fonctionnalités)

[Installation](#installation)

[Créer sa propre simulation](#simulation)

[Performances de la simulation](#performances)

## 

## Fonctionnalités

<a name="fonctionnalités"></a> Ce classement des fonctionnalités est par ordre logique. Les ajouts les plus conséquents sont à la fin.

###### Simulation

Les simulations peuvent être visuelles ou textuelles (dans un fichier ou dans le terminal). Il est également possible d'écrire les données du système dans un fichier .csv pour ensuite tracer des graphiques avec un tableur.

![Basic](https://github.com/NilsCt/gazparfaits/blob/main/images/basic1.png)
![Basic](https://github.com/NilsCt/gazparfaits/blob/main/images/basic2.png)

###### Différentes particules

Il est possible d'ajouter dans l'enceinte plusieurs types de particules déjà existantes (Argon, Hélium, Néon) mais aussi d'en créer de nouvelles (sous-classe de Particule pour un nouveau type ou instance de particule pour une particule unique). 

Chaque particule a une masse propre influant sur sa vitesse pour une température donnée.

La couleur d'une particule dépend de son type (sauf si l'option de visualisation de la chaleur est activée).

On peut initialiser un nombre choisi de particules à une température donnée en suivant la loi de probabilité de Boltzmann.

###### Trajectoire

Il est possible de suivre la trajectoire d'une ou plusieurs particules sélectionnées à la création en activant l'option est_suivi.

Les trajectoires sont représentées par des segments dans la visualisation graphique.

![Suivi](https://github.com/NilsCt/gazparfaits/blob/main/images/suivi.png)

###### Enceinte

La taille de l'enceinte est configurable (hauteur, largeur, profondeur) mais il s'agit toujours d'un pavé droit. Sa taille peut varier au cours du temps.

###### Contrôle de l'interface graphique

La touche espace permet de mettre en pause la simulation.

La souris et les flèches et les touches q e permettent de gérer l'orientation de la caméra.

Les touches w s a d r f permettent de se déplacer dans l'espace.

La touche t permet d'afficher les grandeurs d'état du système (pression, volume, etc).

![Touche t](https://github.com/NilsCt/gazparfaits/blob/main/images/touche-t.png)

###### Calcul des grandeurs d'état du système simulé

Le programme calcule la température, la pression, les pressions partielles, la quantité de mouvement et le centre de masse du système.

Les calculs sont dynamiques : les grandeurs sont actualisées à chaque évolution d'une particule pour diminuer la complexité.

Le calcul de la pression se fait en calculant l'impulsion donnée à chaque particule qui rebondit sur une paroi. Un filtre à moyenne mobile est utilisé pour lisser les variations brutales de pression liées à l'aléatoire des rebonds.

###### Transformations

Le système gère une file d'attente de transformations appliquées chacune leur tour pendant une durée propre.

Les transformations sont des modifications de la température (évolution géométrique), détente ou compression isotherme (évolution linéaire des dimensions de l'enceinte), détente ou compression adiabatique et pauses.

Il est de plus possible d'initier des cycles de Carnot et Stirling avec des paramètres configurables.

Enfin, il est possible d'activer une option pour que les couleurs des particules dépendent de leur énergie cinétique afin de visualiser les modifications de température dans l'enceinte.

![Cycle de Stirling froid](https://github.com/NilsCt/gazparfaits/blob/main/images/stirling_froid.png)

![Cycle de Stirling chaud](https://github.com/NilsCt/gazparfaits/blob/main/images/stirling_chaud.png)



###### Comportement

Le comportement est un attribut du système décrivant le protocole pour détecter et gérer les chocs entre particules.

Deux protocoles existent : un protocole par défaut plutôt naif qui parcourent toutes les particules et compare les positions et un protocole dynamique, plus optimisé, qui gére une liste de cellules (petits morceaux de l'enceinte).

Il est possible de créer ses propres comportements.

## 

## Installation

<a name="installation"></a>Ce projet utilise OpenGL, Qt, make et g++.

##### Pour Linux

OpenGL est sûrement déjà installé. Il suffit de faire les commandes :

```
sudo apt install qtbase5-dev
sudo apt install make
sudo apt install g++
```

##### Lancement

Une fois le code téléchargé, il faut se rendre dans le répertoire du projet et entrer les commandes : `qmake` et `make`.

Puis, il faut entrer dans le dossier correspondant au type de simulation souhaité (text pour simulation textuelle ou Qt_GL pour une simulation graphique) et lancer le ficher exécutable "simulation".

Exemple :

```
qmake
make
cd Qt_GL
./simulation
```

Une fenêtre s'ouvre et la simulation commence.

##### Choisir son test

Plusieurs tests ont déjà été créés.

Pour les sélectionner, il faut choisir le répertoire text ou Qt_GL, puis ouvrir le fichier .pro pour commenter la ligne correspondant test actuel, décommenter le test souhaité et sauvegarder le fichier.

Exemple  dans Qt_GL.pro pour sélectionner testEtat :

```
...
testEtat.cc \
#testTransformations.cc \
#testAutre.cc \
...
```

Il faut ensuite entrer les commandes `qmake` `make` et `./simulation`.

De plus, certains fichiers comme testTransformations.cc contiennent plusieurs tests commentables / décommentables. Ce fichier est particulièrement intéressant pour visualiser les cycles de Carnot et Stirling.

## 

## Créer sa propre simulation

##### Création du fichier

<a name="simulation"></a>Il vaut mieux partir d'un fichier déjà existant comme testEtat.cc pour la partie graphique ou testCompression.cc pour la partie textuelle.

Puis, il faut ajouter le nom du fichier .cc dans le fichier .pro correspondant et sauvegarder.

Exemple dans Qt_GL.pro :

```
...
testPersonnalise.cc \
...
```

Lire les anciens tests permet de mieux connaître les différentes manières d'initialiser les particules du système et les transformations souhaitées.

##### Création du système

Pour une simulation graphique, il faut créer le système à travers le GLWidget.
```
GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(100,80,60),std::make_unique<ComportementDynamique>()));
Systeme& systeme(w.get_systeme());
```

Pour une simulation textuelle, on peut créer le système directement.
```
Systeme systeme1;
```

La construction peut prendre en paramètre une enceinte et/ou un comportement.
On peut aussi créé un système en donnant les dimensions de l'enceinte.
```
Systeme systeme2(20,30,40);
```
Par défaut, l'enceinte est de taille 20x20x20 et le comportement est naïf.

##### Initialisation des particules

On peut ajouter de manière aléatoire un certain nombre de particules à une température donnée.

```
systeme.initialiser_particules(300, 2);
systeme.initialiser_particules_precises<Helium>(5, 10);
```

Le premier paramètre correspond au nombre de particules et le second à la température.

On peut aussi ajouter une particule avec des paramètres précis.

```cpp
systeme.ajouter_particule(make_unique<Argon>(Vecteur3D(3, 3, 3), Vecteur3D(1, 1, 1)));
systeme.ajouter_particule(std::make_unique<Neon>(Vecteur3D(2, 7, 3), Vecteur3D(4, -3, -5), true));
```

Le premier vecteur est la position et le second la vitesse. Ajouter true permet d'afficher la trajectoire de la particule.

##### Gérer les transformations

Une transformation ne commence qu'une fois que toutes les transformations ajoutées antérieurement soient terminées.

Une seule transformation est réalisée à la fois.

Pour ajouter une pause :

```
systeme.ajoute_transformation(std::make_unique<Pause>(5));
```

Cette transformation ne fait rien pendant la durée en seconde donnée en paramètre.

Elle est utile pour retarder les prochaines transformations ou pour ajouter un délai entre deux transformations.

On peut ajouter une modification de la température : 

```
systeme.ajoute_transformation(std::make_unique<ModificationTemperature>(25, 10));
```

Le premier paramètre est la température finale et le second la durée de transformation en secondes. La température suit une évolution géométrique.

On peut aussi ajouter un travail isotherme :

```
systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(40,30,50, 10));
```

Cette ligne permet de faire des compressions ou détentes à température constante. Les 3 premiers paramètres correspondent aux dimensions finales de l'enceinte et le dernier à la durée en seconde de la transformation.

Enfin, on peut ajouter un travail adiabatique (la température change à cause du travail fourni) : 

```
systeme.ajoute_transformation(std::make_unique<TravailAdiabatique>(20,20,20, 10));
```

Les 3 premiers paramètres sont la dimension finale de l'enceinte et le dernier la durée en secondes.

##### Initialiser un cycle

On peut également lancer un cycle moteur.

```
systeme.initialiser_cycle_stirling(10, 5, 64000, 300, 8000);
OU
systeme.initialiser_cycle_carnot(10, 100, 64000, 300, 8000);
```

Le premier paramètre est la durée en seconde de chaque transformation. Puis on donne la température et le volume du point en bas à droite du diagramme (p,V) et enfin la température et le volume du point en haut à gauche du diagramme (p,V).

Une pause de 2 secondes et ajoutée entre chaque transformation.

La température est ajustée au tout début du cycle pour correspondre aux attentes.

Pour un cycle de Carnot, la température est corrigée à chaque boucle pour éviter une dérive petit à petit de la température liée à l'incertitude des calculs des adiabatiques.

On donne en paramètre des volumes, l'enceinte est donc toujours cubique et de côté la racine cubique des volumes donnés.

En réalité, initialiser un cycle ajoute les transformations nécéssaires pour 100 répétitions. Le cycle ne commencera qu'une fois que toutes les transformations sont terminées et finira par s'arrêter au bout d'une longue durée. 

Ajouter des transformations n'a pas d'effet, car elles ne seront réalisées qu'une fois que le cycle est terminé.

##### Visualiser la chaleur

Pour que les couleurs des particules dépendent de leur énergie cinétique, il faut utiliser la ligne : 

```
w.actualiser_energie_reference(); # w est le GLWidget
```

Une fois activée, cette option ne peut plus être désactivée. Les couleurs sont déterminées par comparaison à l'énergie cinétique moyenne d'une particule au moment de l'exécution de la ligne. 

Il faut donc que des particules soit déjà présentes dans l'enceinte et à la température que l'on juge être la température moyenne.

```
Systeme& systeme(w.get_systeme());
systeme.initialiser_particules(500, 25);
w.actualiser_energie_reference();
systeme.initialiser_cycle_stirling(10,5,64000,300,8000);
```

Il est donc utile d'initialiser les particules à la température moyenne, d'actualiser l'énergie cinétique de référence puis de lancer le cycle pour observer de manière adaptée les couleurs.

##### Simulation textuelle dans le terminal

Il est conseillé de regarder le fichier testEvolution.cc dans le repertoire text pour un exemple complet.

```
ostream& flot(cout);
Systeme systeme1; // choc de 2 particules
systeme1.ajouter_particule(make_unique<Argon>(Vecteur3D(3, 3, 3), Vecteur3D(1, 1, 1)));
systeme1.ajouter_particule(make_unique<Argon>(Vecteur3D(5, 5, 5), Vecteur3D(0, 0, 0)));
systeme1.simulation(flot);
```

La méthode simulation gére automatiquement l'évolution et le TextViewer.

Il est toutefois possible de faire l'équivalent soi-même (voir exerciceP9.cc).

```
ostream& flot(cout);
TextViewer textViewer(flot);
systeme.dessine_sur(textViewer);
for(int i(0); i < 10; ++i) {
        flot << "==========----------" << endl;
        systeme.evolue(1e-10, textViewer);
        systeme.dessine_sur(textViewer);
}
```

##### Simulation textuelle dans un fichier

Le fonctionnement est très ressemblant. Voir le fichier testFileWriter.cc.

```
FileWriter fileWriter("evolution.txt");

systeme.simulation(fileWriter.get_flot());
OU
systeme.dessine_sur(fileWriter);
for(int i(0); i < 10; ++i) {
        flot << "==========----------" << endl;
        systeme.evolue(1e-10, fileWriter);
        systeme.dessine_sur(fileWriter);
}
```

Il faut donner en paramètre le nom du fichier. Celui-ci doit se trouver dans le même répertoire que le fichier .cc.

##### Exporter des données dans un fichier .csv

Le fichier testDataWriter.cc dans le repertoire text constitue un bon exemple.

```
DataWriter dataWriter("donnees.csv");
systeme.initialiser_cycle_carnot(17, 260, 64000, 350, 16000 );
for(int i(0); i < 400; ++i) {
    systeme.evolue(0.2*Systeme::time_ratio, dataWriter);
    systeme.dessine_sur(dataWriter);
}
```

Le fichier doit se trouver ou va être créé automatiquement dans le même répertoire que le fichier .cc.

## Performances de la simulation

<a name="performances"></a>Ce projet permet de simuler de manière fluide aux alentours de 1000 particules à basse température. Il n'est donc pas fait pour de plus grandes simulations.

##### Loi des gaz parfaits

À basse température, la pression est, en effet, proportionnelle à la température.  

![Graphe basse température](https://github.com/NilsCt/gazparfaits/blob/main/images/graphe_basse.png)

On retrouve à 5% près le coefficient de proportionnalité prédit par la loi des gaz parfaits (voir Google Sheet).  
(La courbe en rouge n'est pas la courbe théorique, mais la courbe de tendance).

![Graphe hautes températures](https://github.com/NilsCt/gazparfaits/blob/main/images/graphe_haute.png)

À plus haute température, la simulation ressemble moins à une droite.  
De plus, la précision du coefficient de proportionnalité chute à 27%.  
Cela est peut-être dû au fait que les particules vont trop vite pour la simulation, le pas de temps n'est plus adapté.

##### Cycle de Stirling

Le cycle de Stirling a un diagramme plutôt ressemblant au diagramme (p,V) théorique.

![Cycle de Stirling](https://github.com/NilsCt/gazparfaits/blob/main/images/graphe_stirling.png)

##### Cycle de Carnot

Mais le cycle de Carnot est moins proche de la théorie.  
Cela est dû au fait que les adiabatiques sont plus difficiles à modéliser et sont moins précises.

![Cycle de Carnot](https://github.com/NilsCt/gazparfaits/blob/main/images/graphe_carnot.png)

##### Conclusion

[Lien GoogleSheet](https://docs.google.com/spreadsheets/d/1b06dz_DghfzNdqJPd8c1oSLjqB7fxvw1w95UKtQ5kDM/edit?usp=sharing)

Pour conclure, ces tests sont plutôt satisfaisants, mais ils ont été réalisés avec une quantité de matière très faible (300 particules) dans une enceinte très petite (8e-27 m3).  
La simulation n'est pas adaptée pour un nombre de particules trop important.
