Pour compiler la version originale : make OPT=NOOPT
Pour compiler la première version optimisée : make OPT=OPT1
Pour compiler la seconde version optimisée : make OPT=OPT2

Pour vérifier la sortie avec une taille 300 et l'enregistrer dans out.txt :
 ./check 300 out.txt

Pour calibrer avec une taille 300 le bon nombre de répétitions (max 100) de warmup à utiliser:
 ./calibrate 300 100

Pour mesurer avec une taille 300, 100 répétitions de warmup (lors de la première méta) et 30 répétitions de mesure :
 ./measure 300 100 30

Pour exécuter avec MAQAO :
maqao oneview -R1 -- ./measure 300 100 30
