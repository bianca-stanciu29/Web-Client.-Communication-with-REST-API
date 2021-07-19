#include "json.hpp"
#include <iostream>
#include <string.h>
#include "helpers.h"
#include "requests.h"
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

using json = nlohmann::json;
using namespace std;
int main()
{
    //buffer pentru mesaj
    char *message;
    //buffer pentru raspuns-
    char *response;
    //variabila pentru a identifica comanda primita
    char *command = new char[256];
    //socket folosit pentru conectare
    int sockfd;
    //cookie folosit pentru demonstrarea autenticitatii
    char *cookie = new char[1000];
    //token folosit pentru a demonstra accesul la biblioteca
    char *token = new char[1000];
    while (1)
    {
        //citire comanda
        cin >> command;
        if (strcmp(command, "register") == 0)
        {
            //json folosit pentru a creea payload-ul
            json js;
            //se citesc date pentru user si parola
            string username;
            string password;
            cin.ignore();
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password);
            //se adauga in json datele citite
            js["username"] = username;
            js["password"] = password;
            //se deschide conexiunea catre server
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se creeaza mesajul pentru o cerere de tip post
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/register", "application/json",
                                           js.dump().c_str(), NULL, NULL);
            //se adauga mesajul
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspunsul de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "login") == 0)
        {
            //json folosit pentru a creea payload-ul
            json js;
            //se citesc date pentru user si parola
            string username;
            string password;
            cin.ignore();
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password);
            //se adauga in json datele introduse
            js["username"] = username;
            js["password"] = password;
            //se deschide conexiunea catre server
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se creeaza mesajul pentru o cerere de tip post
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/login", "application/json",
                                           js.dump().c_str(), NULL, NULL);
            //se adauga mesajul
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspuns de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se extrage cookie-ul de sesiune
            char *p;
            //se gaseste linie unde se afla
            p = strstr(response, "Set-Cookie:");
            //daca exista
            if (p != NULL)
            {
                p += 12;
                //se parcurge caracter cu caracter pentru a extrage intreg cookie-ul
                for (int i = 0; p[i] != ' '; i++)
                {
                    cookie[i] = p[i];
                }
            }
            //token-ul se initializeaza cu null
            strcpy(token, "");
            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "enter_library") == 0)
        {
            //se deschide conexiunea
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se compune mesajul, trimitand si cookie-ul pentru a demonstra autentificarea
            message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access", NULL, cookie, NULL, 1);
            //se adauga mesajul
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspunsul de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se extrage token-ul
            if (strcmp(cookie, "") != 0)
            {
                char *p = strstr(response, "{\"token");
                json jt = json::parse(p);
                string aux_token = jt["token"];
                strcpy(token, aux_token.c_str());
            }

            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "get_books") == 0)
        {
            //se deschide conexiunea
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se compune mesajul pentru o cerere Get, unde se trimite token-ul pentru acces
            message = compute_get_request("34.118.48.238", "/api/v1/tema/library/books", NULL, NULL, token, 1);
            //se adauga mesajul
            puts(message);
            //se trimite catre server mesajul
            send_to_server(sockfd, message);
            //se primeste raspuns
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "get_book") == 0)
        {
            //se deschide conexiunea
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se citeste id-ul corespunzator cartii
            string id;
            long unsigned int len = 0;
            cin.ignore();
            //verificare daca id-ul este numar
            while (1)
            {
                //variabila pentru a tine cont de lungime
                len = 0;
                cout << "id=";
                getline(cin, id);

                for (long unsigned int i = 0; i < id.length(); i++)
                {   //testare daca e cifra
                    if (isdigit(id[i]) != 0)
                    {
                        len++;
                    }
                }

                if (len == id.length())
                    break;
            }
            //se creaza id-ul cartii
            char *bookId = new char[1200];
            char *aux = new char[2000];
            strcpy(bookId, "/api/v1/tema/library/books/");
            strcpy(aux, id.c_str());
            strcat(bookId, aux);
            //se creeaza mesajul pentru o cerere de tip get, trimitand id-ul cartii si token-ul
            message = compute_get_request("34.118.48.238", bookId, NULL, NULL, token, 1);
            //se adauga mesajul
            puts(message);
            //se trimite mesajul catre server
            send_to_server(sockfd, message);
            //se primeste raspuns de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se opreste conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "add_book") == 0)
        {
            //se deschide conexiunea
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);

            cin.ignore();
            //se citesc datele de intrare: titlu, autor, genul, editura si numarul paginii
            string title;
            cout << "title=";
            getline(cin, title);

            string autor;
            cout << "author=";
            getline(cin, autor);

            string genre;
            cout << "genre=";
            getline(cin, genre);

            string publisher;
            cout << "publisher=";
            getline(cin, publisher);

            string page_count;
            long unsigned int len;
            //cin.ignore();
            //verificare daca numarul paginii este un numar 
            //daca nu este numar se va citi pana se va introduce un numar
            while (1)
            {
                //variabila pentru a tine cont de lungime
                len = 0;
                cout << "page_count=";
                getline(cin, page_count);

                for (long unsigned int i = 0; i < page_count.length(); i++)
                {   //testare daca e cifra
                    if (isdigit(page_count[i]) != 0)
                    {
                        len++;
                    }
                }

                if (len == page_count.length())
                    break;
            }
            //crearea obiectului de tip json
            //se adauga datele citite 
            json jsBook;
            jsBook["title"] = title;
            jsBook["author"] = autor;
            jsBook["genre"] = genre;
            jsBook["publisher"] = publisher;
            jsBook["page_count"] = stoi(page_count);
            //se creeaza mesajul pentru o cerere de tip post, trimitand jason-ul si token-ul de autentificare
            message = compute_post_request("34.118.48.238", "/api/v1/tema/library/books", "application/json",
                                           jsBook.dump().c_str(), NULL, token);
            //se adauga mesajul                               
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspuns de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "delete_book") == 0)
        {
            //citirea id carte
            string id;
            long unsigned int len = 0;
            cin.ignore();
            while (1)
            {
                //variabila pentru a tine cont de lungime
                len = 0;
                cout << "id=";
                getline(cin, id);

                for (long unsigned int i = 0; i < id.length(); i++)
                {   //testare daca e cifra
                    if (isdigit(id[i]) != 0)
                    {
                        len++;
                    }
                }

                if (len == id.length())
                    break;
            }
            //crearea id-ului corespunzator 
            char *bookId = new char[1200];
            char *aux = new char[2000];
            strcpy(bookId, "/api/v1/tema/library/books/");
            strcpy(aux, id.c_str());
            strcat(bookId, aux);
            //se deschide conexiunea catre server
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se creeaza mesajul pentru o cerere de tip delete, trimitand bookId si token-ul
            message = compute_delete_request("34.118.48.238", bookId, NULL, 1, token);
            //se adauga mesajul
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspuns de la server
            response = receive_from_server(sockfd);
            //se adauga raspunsul
            puts(response);
            //se inchide conexiunea
            close_connection(sockfd);
        }

        if (strcmp(command, "logout") == 0)
        {
            //se deschide conexiunea
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            //se creeaza o cerere de tip Get
            message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout", NULL, cookie, NULL, 0);
            //se adauga mesajul
            puts(message);
            //se trimite catre server
            send_to_server(sockfd, message);
            //se primeste raspuns de la server
            response = receive_from_server(sockfd);
            //se goleste cookie-ul
            strcpy(cookie, "");
            //se goleste token-ul
            strcpy(token, "");
            //se adauga raspunsul de la server
            puts(response);
            //se inchide conexiunea
            close_connection(sockfd);
        }
        //inchiderea programului la exit
        if (strcmp(command, "exit") == 0)
        {
            return 0;
        }
    }
}