/**
 * \file servFcnt.c
 * \brief All the function used by the server or the client
 * \details Contains all the function for server and client 
 * \author Lucien Chauvin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "servFcnt.h"
#include "common.h"

/*
* If program run on Windows
*/
#ifdef _WIN32
#include <winsock2.h>
/*
  * Needed  non-existent type with winsock2
*/
typedef int socklen_t;
/*
* Else if program run on Linux
*/
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
/*
  * Avoid differences to close a socket
  * between socket.h and winsock2.h
  * Adding missing definitions with socket.h
*/
#define closesocket(param) close(param)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
/*
  * Adding missing types with socket.h
*/
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

char monStr[80];
char monIP[85];

int nbPlayer = 0;
int socketConnected = 0;

void getLocalIP()
{
  char s[81];
  FILE *fic;
  system("ifconfig | grep \"inet 1[97]2.*\" | sed \"s/netmask.*//g\" | sed \"s/inet//g\" > .test.txt");

  fic = fopen(".test.txt", "r");
  if (fic == NULL)
  {
    printf("The file was not opened\n");
  }
  else
  {
    /*
      * Get ip from the file named fic
      */
#ifdef _WIN32
    fgets(s, sizeof(s), fic);
#else
    fscanf(fic, "%s", s); // C4996
#endif
    int cpt = 0;
    for (int i = 0; s[i]; i++)
    {
      if (((int)s[i] >= 46) && ((int)s[i] <= 57))
      {
        monIP[cpt] = s[i];
        cpt++;
      }
    }
    if (verbose >= 1)
      printf("L'ip du serveur est : %s", monIP);
  }
}

err_t sendStruct(void *structure, int size, int socket,  err_t (*print)(void * s, char tab[STR_SHORT]))
{
  int error;
  if(verbose>=2)printf("Envoi en Cours\n");

  if(print!=NULL && verbose>= 2)
  {
    printf("\n");
    print(structure,"SENDING : ");
  }

  if((error=send(socket, structure, size, 0))!=-1)
  {
    bool received;

    if(verbose>=2)printf("Awaiting Confirmation!\n");
    recv(socket, &received, sizeof(bool), MSG_WAITALL);

    return OK;
  }
  else
  {
    strerror(error);
    return SEND_ERROR;
  }
}

void *recep(void *container, int size, int socket, err_t (*print)(void * s, char tab[STR_SHORT]))
{
  int error;
  if(verbose>= 2)printf("En Reception\n");
  bool received;

  if((error = recv(socket, container, size, MSG_WAITALL))!=-1)
  {
    received = TRUE;

    if(print!=NULL && verbose>= 2)
    {
      printf("\n");
      print(container,"RECIEVED : ");
    }

  }
  else
  {
    strerror(error);
    received = FALSE;
  }

  if(verbose>=2)printf("Sending Confirmation\n");
  send(socket, &received, sizeof(bool), MSG_WAITALL);

  return container;
}

int setupMultiMap(MultiTile * mapMulti, Tile * mapLocal){
  for(int i = 0; i < _X_SIZE_ * _Y_SIZE_; i++){
    mapMulti[i].tile_id = mapLocal[i].tile_id;
  }
  return 1;
}

int displayMapMulti(MultiTile  *mapMulti){
  for(int i = 0; i <_X_SIZE_ * _Y_SIZE_; i++){
    printf("tile id = %d \n", mapMulti[i].tile_id);
  }
  return 1;
}