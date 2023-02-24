# Projet : Jeu d'infiltration
#### *Auteur : Hakim AOUDIA, Ilhan ACHERCHOUR*
#### *Langage : C*
#### *Date : 2022*
#### *Niveau : L3 S5*
#### *Note obtenue : 20/20*

<p align="center">
  <img src="https://user-images.githubusercontent.com/106891439/220529804-d204556a-42af-4b18-beca-f0ddbec23dae.png" width="40%" height="40%">
  <img src="https://user-images.githubusercontent.com/106891439/220529977-a78f1284-92df-453b-89ac-92b2dfbb57be.png" width="40%" height="40%">
</p>

## Présentation
Ce projet de fin de semestre est la synthèse de tout ce que nous avions appris en C. On a du réalisé un jeu d'infiltration ou le joueur doit récupérer 3 reliques sur le terrain puis revenir à la base pour valider la partie. S'il récupère toutes les reliques et qu'il ne se fait pas toucher par un gardien, il peut écrire son pseudo dans le tableau des scores.

## Fonctionalités
- Génération de terrain aléatoire (méthode de division récursive)
- Déplacment du joueur toute directions 60 fps
- Détéction collision des murs sur les angles du joueur
- Collection mana (liste chainée)
- Pouvoir du joueur
- Tableau de classement
- Vision des garidens (2D raycast)
- Sons et musique


## Début de partie
- Jouer
- Paramètres
- Quitter

Voici les variables modifiable dans l'écran de paramètre :
- Le nombre de gardiens (entre 0 - 15)
- Le nombre de reliques (entre 1 - 20)
- La taille du plus petit mur (entre 4 - 15)
- La durée du mode panique (entre 3 - 60)
- Activer ou non le son
- permettre au joueur ou non d’avoir des pouvoirs


## Joueur
Pour bouger le personnage :
- Z pour monter
- S pour descendre
- Q pour aller à gauche
- D pour aller à droite

Pour utiliser les pouvoirs :
- Touche ESPACE pour rendre le personnage invisible
- Touche SHIFT pour déclencher le pouvoir d'accélération

Pour faire pause :
- touche ESPACE

## Informations de la partie
- la barre de mana
- le nombre de reliques volé / le nombre total de relique sur le terrain
- le nombre de mana utilisé par le joueur
- le temps écoulé de la partie
- la durée du mode panique des gardiens
