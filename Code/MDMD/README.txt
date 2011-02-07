En pièce jointe, je t'envoie :

 

· Le .cxx à faire évoluer en filtre. Il reste encore à choisir un
          filtre duquel hériter, et cela dépend:

o De l’input du filtre mdmd qui doit être compatible avec l’output du
filtre VCA ; à mon avis, le mieux est de partir sur un filtre
composite qui initialise par défaut avec VCA, et peut optionnellement
initialiser différemment si d’autre algorithmes de démixage sont
implémentés pas la suite ;

o De l’output : en effet, deux informations sont pertinentes : la
matrices des spectres et la matrice des abondances. D’après moi, le
plus intuitif est probablement de leur associer un vector list et un
image list, respectivement. Reste à trancher sur ce que doit retourner
la méthode GetOuput()…

· Des éléments sur les tests de validation. En effet, j’ai observé que
          les sorties de mdmd_matlab et mdmd_otb ne sont pas
          identiques. Dans les deux fichiers .txt (en pj) j’ai comparé
          l’évolution de certains paramètres intéressants sur les 100
          premières itérations, avec un pas de 10. Note que
          l’exécution de mdmd requiert plusieurs milliers voir
          dizaines de milliers d’itérations. En gros, il n’y a pas de
          bug apparent, ni à la compilation ni à l’exécution, et les
          résultats concordent sur les premières itérations et
          s’écartent progressivement. Il est fortement possible que
          cela soit lié :

o A l’optimisation algorithmique de certains calculs matriciels que
j’ai remplacé par des boucles dans le souci de réduire la complexité
algorithmique ; donc approximations numériques en chaîne ;

o A la non-convexité de la fonctionnelle minimisée : une mauvaise
initialisation peut nous amener à un point ou « zone » stationnaire.

o A une erreur de code. J’en doute, mais cette possibilité n’est pas à
exclure… Il faudra vérifier la performance de démixage lorsque VCA OTB
sera fonctionnel.

En ce qui concerne l’obtention des fichiers .txt en pj :

o Pour la version OTB, ces informations peuvent être observées en
jouant avec les valeurs des constantes de pré compilation «
divisorParam » et « maxIter ». Lancer avec la ligne de commande :

./Mdmd cupriteSubHsi.tif mdmdTest.tif 5

o Pour la version matlab, il faut modifier la fonction enc_mdmd (ligne
1.17) : commenter l’appel de nmf et décommenter celui de
nmf_validationOtb. Dans la fonction nmf_validationOtb, tu as aussi la
possibilité de jouer avec les paramètres maxIter et divisorParam. Il
faut ensuite lancer le script buildToolsForMdmdTest.m. Je t’enverrai
l’ensemble des codes matlab dans un prochain mail.
