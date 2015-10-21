# calculatrice-c

Memory Verification: Valgrind

- Redo print numbers not recursively.
- malloc errors
- 999 algorithm pour addition de nombres négatifs?
- Trimer les zéros inutiles au début d'un nombre
- Vérifier que les nombres ne soient pas vide avant opérations
- variables only lowercase letter?
- initNumberStack make sure to free old stack's memory allocation
- initNumberStack write method to free instead of harcoding it



Algorithme d'addition:

L'algorithme d'addition s'inspire fortement de la méthode classique sur papier apprise à l'école primaire, avec quelques cas à traiter dans le cas d'addition de nombres négatifs. En effet, l'algorithme calcule le résultat caractère par caractère, en additionnant les deux chaînes, de la figure la moins significative vers la plus significative, sans oublier les retenus s'il y a lieu.

Par exemple, prenons les chaînes A= "a_n, a_n-1, ..., a_k, ..., a_1" de longueur n et B= "b_m, b_m-1, ..., b_k, b_1" de longueur m.
Dans le cas où A et B sont les deux positifs, pour additionner les deux chaînes, il faudra itérativement effectuer c_k-1 + a_k + b_k = c_k r_k pour créer une nouvelle chaîne R= "r_o, r_o-1, ..., r_k, ..., r_1" de longueur o = max{m,n} ou max{m,n}+1 dépandant de la dernière retenue. IL est facile de traiter le cas où A et B ne sont pas de même tailles, simplement en "ajoutant" des zéros au début du chiffre. Simplement, on peut dire que si k > n, alors a_n = 0, et le même principe pour B.

Dans le cas où les deux chaînes sont négatives, on peut simplement additionner celles-ci comme si elles étaients positives et assumer que la réponse sera la négation du résultat. Ceci est du à la distributivité de l'addition tel que (-a + -b) = -(a+b).

Ne reste maintenant que le cas où les chaînes sont de signes opposés


Algorithme de multiplication:

mult(Number n1, n2)

	où cellule -> {int valeur; cellule prochain}
	cellule c1_curr = n1.debut,
			c2_curr = n2.debut,
			cr_curr;

	où Number -> {booléen estPositif; cellule début}
	Number résultat;
	résultat.estPositif = vrai si n1 et n2 sont de même signe, faux sinon;

	int temp;

	int i = 0,
		j;
	tant que (c1_curr existe){
		j = 0;
		tant que (c2_curr existe){
			cr_curr = (i+j)ème cellule de "résultat" en créant les cellules qui n'existe pas au passage
			cr_curr += c1_curr.valeur * c2_curr.valeur;
			tant que (cr_curr.valeur > 9){
				si (cr_curr.prochain n'existe pas) 
					créer la cellule cr_curr.prochain avec valeur 0
				cr_curr.prochain.valeur += cr_curr.valeur / 10 (division entière)
				cr_curr.valeur = cr_curr.valeur % 10;
				cr_curr = cr_curr.prochain
			}
			j++
		}
		i++
	}

	retourne number;

	TYPES D'ERREURS:

	- Syntaxe
	- Allocation de memoire
	- Utilisation de variable non-alloue