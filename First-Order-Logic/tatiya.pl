/* Facts */

ship(serenity).

/* Relationships */

captain(mal,serenity).

crewmember(zoe,serenity).
crewmember(wash,serenity).
crewmember(kaylee,serenity).

passenger(simon,serenity).
passenger(river,serenity).

married(zoe,wash).

friend(mal,zoe).

/* Rules */

/* serves -> All crewmembers of a ship serve its captain */
/* Who serves whom */
serves(X,Y) :- crewmember(X,Z),captain(Y,Z),ship(Z).

/* protects -> The captain of a ship protects its passengers */
/* Who protects whom */
protects(X,Y) :- captain(X,Z), passenger(Y,Z),ship(Z).

/* Kaylee is friends with all passengers on Serenity */
friend(Z,X) :- Z = 'kaylee',passenger(X,serenity).

/*=======================*/
/* Output of the queries */

/* Who serves whom */
/* 2 ?- serves(X,Y). */
/* X = zoe, */
/* Y = mal ; */
/* X = wash, */
/* Y = mal ; */
/* X = kaylee, */
/* Y = mal. */

/* Who is friends with whom */
/* 3 ?- friend(X,Y). */
/* X = mal, */
/* Y = zoe ; */
/* X = kaylee, */
/* Y = simon ; */
/* X = kaylee, */
/* Y = river. */

/* Who is married to whom  */
/* 4 ?- married(X,Y). */
/* X = zoe, */
/* Y = wash. */