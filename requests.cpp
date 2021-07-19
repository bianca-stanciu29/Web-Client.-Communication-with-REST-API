#include <stdlib.h> /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

//functie pentru generarea unui mesaj pentru o cerere de tip Get
char *compute_get_request(const char *host,const char *url,const char *query_params,
                          char *cookies, char *token, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    //se scrie numele metodei, url-ul, parametrul de cerere si tipul protocolului
    if (query_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    //adaugarea host-ului
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    //adaugarea cookie-ului
    if (cookies != NULL)
    {
        if (strcmp(cookies, "") != 0)
        {
            sprintf(line, "Cookie: %s", cookies);
            compute_message(message, line);
        }
    }
    //verificarea accesului conform token-ului
    if (token != NULL)
    {
        if (strcmp(token, "") != 0)
        {
            sprintf(line, "Authorization: Bearer %s", token);
            compute_message(message, line);
        }
    }
    //adaugarea unei noi linii
    compute_message(message, "");
    //returnarea mesajului
    return message;
}
//functie pentru generarea unui mesaj pentru o cerere de tip Post
char *compute_post_request(const char *host,const char *url,const char *content_type, const char *body_data,
                           char *cookies, char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *)calloc(LINELEN, sizeof(char));

    //scrierea numelui metodei, url-ul si tipul protocolului
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    //adaugarea host-ului
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
   //adaugarea headerelor necesare(Content-Type si Content Length) 
    sprintf(line, "Content-Type: %s", content_type);
    //se compune mesajul
    compute_message(message, line);
    strcpy(body_data_buffer, body_data);
    int len = strlen(body_data_buffer);
    sprintf(line, "Content-Length: %d", len);
    compute_message(message, line);
    //se adauga cookie-ul
    if (cookies != NULL)
    {
       if(strcmp(cookies,"") != 0)
        {
            sprintf(line, "Cookie: %s", cookies);
            compute_message(message, line);
        }
    }
    //verificarea accesului conform token-ului
    if (token != NULL)
    {

        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    //adaugarea unei linii noi la finalul header-ului
    compute_message(message, "");
    memset(line, 0, LINELEN);
    //adaugarea payload-ului actual
    compute_message(message, body_data_buffer);

    free(line);
    return message;
}
//functie pentru generarea unui mesaj pentru o cerere de tip Delete
char *compute_delete_request(const char *host,const char *url,
                             char *cookies, int cookies_count, char *token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    //scrierea numelui metodei, URL-ul si a tipului de protocol
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    //adaugarea host-ului
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    //verificarea accesului conform token-ului
    if (token != NULL)
    {

        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    //adaugarea unei linii noi la header.
    compute_message(message, "");
    return message;
}