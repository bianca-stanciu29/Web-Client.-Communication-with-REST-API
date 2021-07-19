STANCIU BIANCA-ANDREEA, 325CC
                                    Tema 3: Protocoale de comunicatie
                                 - Client Web. Comunicatie cu REST API. -

        In cadrul acestei teme am plecat de la laboratorul 10, folosind unele resurse puse la
dispozitie in scheletul laboratorului. Pentru a parsa mesajele care vor urma sa fie trimise catre server am folosit
biblioteca pusa la dispozitie, si anume nlohmann pentru o usurinta de a transfera datele intr-un json.

Requests:
    -acest fisier este destinat generarii mesajelor de tip GET, POST sau DELETE. In cadrul celor 3 functii
s-au urmat urmatorii pasi: 
			 -se scriu numele metodei, url-ul, parametrul de cerere si tipul protocolului.
                         -se adauga hostul
                         -se adauga cookie-ul (daca acesta nu este null si nici gol)
                         -se verifica accesul cu ajutorul token-ului (daca token-ul nu este NULL si nici gol)
                         -se va afisa "Authorization: Bearer" + token-ul care va avea rol de acces
                         -se va adauga de fiecare data o noua linie
                         -se trimite mesajul

Client:
    -acest fisier este destinat interpretarii comenzilor de la tastatura pentru a interactiona cu serverul
    Variabile ajutatore: -pentru a trimite mesajul am un buffer de mesaje(message)
                         -pentru a identifica comanda primita am un buffer in care se va citi comanda de la tastatura
                         -pentru raspunsul primit de la server am un buffer de response(response)
                         -un cookie folosit pentru demonstrarea autentificarii
                         -un token pentru a demonstra accesul la biblioteca

Comenzi Client:
    Observatii: -dupa citirea comenzii se va verifica de fiecare data ce comanda s-a introdus
    		-dupa fiecare comanda se inchide conexiunea.
    1. "register" :-pentru a crea payload-ul dorit cu username si password am folosit un json.
                   -am oferit promptul specific pentru citirea de la tastatura, urmand ca dupa citire sa adaug datele
                   in json.
                   -pentru a trimite mesajul catre server am deschis conexiunea catre acesta, am creat mesajul pentru
                   o cerere de tip POST folosindu-ma de functia compute_post_request unde i-am dat datele de conectare:
                   HOST, PORT, query_params si payload-ul, ceilalti parametrii fiind "null".
		   -mesajul creat se trimite la server si se afiseaza raspunsul primit.
                   

    2."login" :-pentru a crea payload-ul am folosit de asemenea un json pentru a retine username-ul si parola.
               -am creat promptul specific citirii datelor de intrare.
               -la fel ca la register, am creat mesajul specific pentru o cerere de tip post si l-am trimis la server
               -pentru a intoarce cookie-ul am cautat in raspunsul dat de server linia unde se gaseste cookie-ul :
               "Set-Cookie: " si am parcurs caracter cu caracter pentru a extrage cookie-ul dorit.

    3."enter-library" :-pentru demonstrarea autentificarii se trimite prin GET si cookie-ul returnat la login.
                       -pentru a intoarce token-ul folosit mai departe pentru autorizare, am testat daca exista cookie
                       si am cautat token-ul folosindu-ma de parsarea jsonului.

    4."get_books" :-pentru a se compune mesajul de tip get a trebuit sa trimit token-ul care autorizeaza accesul la
                    biblioteca.
                    -dupa crearea mesajului se trimite la server informatia dorita.

    5. "get_book" :-dupa deschiderea conexiunii la server se citeste de la tastatura id-ul.
		   -am verificat daca id-ul introdus este un numar si se va citi id-ul pana se va introduce un numar
                   -pentru a se crea mesajul de tip GET va trebui sa fie trimis in loc de url id-ul in tipar specific al
                    cartii, de aceea am concatenat id-ul citit la restul structurii "/api/v1/tema/library/books/".
                    -dupa crearea mesajului se va trimite informatia la server.

    6. "add_book" :-pentru citirea informatiilor specifice unei carti am creat promptul pentru date specific.
                   -titlul, autorul, genul si publicatie sunt stringuri care pot contine spatii, numere, caractere
                   speciale. In privinta numarului de pagini am verificat daca stringul respectiv este un numar,
                   introducand de la tastatura siruri de caractere pana acesta va fi un numar. Pentru a testa ca este
                   numar, am parcurs stringul si am verificat daca este cifra carcaterul, avand o variabila len care va
                   contoriza lungimea, aceasta fiind incrementata la fiecare cifra. Daca len-ul este egal cu lungimea
                   stringului atunci se va opri solicitarea introducerii de la tastatura a altei valori.
                   -pentru a trimite informatia catre server am folosit de asemenea un json care va retine toate datele
                   introduse.
                   -dupa crearea mesajului de tip GET pentru care am trimis json-ul si token-ul de acces, se trimite
                   informatia catre server.

    7. "delete_book" :-pentru a sterge o carte va fi nevoie de id, care se va citi de la tastatura.
                      -pentru a compune mesajul va trebui adaugat id-ul la structura specifica 
		      "/api/v1/tema/library/books/id".
		      -am verificat daca id-ul introdus este un numar si se va citi id-ul pana se va introduce un numar
                      -dupa ce s-a compus mesajul de tip DELETE, se trimite informatia catre server.

    8. "logout" :- pentru delogare este necesar cookie-ul obtinut la logare.
                 - dupa ce s-a compus mesajul de tip GET si se trimite informatia la server, atat cookie-ul, cat si
                 token-ul se golesc.

    9. "exit" :- la introducerea acestei comenzi se va inchide programul.