/**
 * \file Socket_Server.c
 * \brief Server programm for Tactics Arena
 * \details Contains TCP server function
 * \author Lucien Chauvin
 * \version 0.1.0
 * \date 18/03/2020
*/

/* =============== DEPENDENCES =============== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "servFcnt.h"
#include "struct.h"
#include "common.h"
#include "grid.h"
#include "map_editor.h"

// For Windows user

#ifdef _WIN32
#include <winsock2.h>

// Needed non-existent type with winsock2

typedef int socklen_t;

// Other OS
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(param) close(param)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

/* =============== VARIABLES =============== */
unsigned int logFlag = 0;

/* =============== FONCTIONS =============== */

/**
 * \fn err_t stopTCPSocketServ(int socket)
 * \return err_t SERV_OK
 * \brief Function to stop the server (close sockets)
*/

err_t stopTcpSocketServ(int socketConnected)
{
  if (verbose >= 1)
    printf("Shutdown socketConnected ...\n");
  shutdown(socketConnected, 2);
  if (verbose >= 1)
    printf("Close socket : socketConnected...\n");
  closesocket(socketConnected);
  nbPlayer -= 1;

// Command to close Windows firewall 
#ifdef _WIN32
  system("netsh advfirewall firewall delete rule name=\"Tactics\"");
#endif

  return OK;
}

/**
 * \fn err_t startTCPSocketServ(void)
 * \return err_t SERV_OK or SERV_ERROR
 * \brief Function to create the server and wait incoming connection
*/

err_t startTCPSocketServ()
{

#ifdef _WIN32
  // Cange codepage CMD
  system("chcp 65001");
  // Creating rules for Firewall
  system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=in localport=3555 action=allow");
  system("netsh advfirewall firewall add rule name=\"Tactics\" protocol=TCP dir=out localport=3555 action=allow");
  // Get the local IP Address of the server
  system("ipconfig | findstr /r \"IPv4.*192\" > .test.txt");
  system("cls");
  WSADATA WSAData;

  int windWSAError;
  /*
    * WSAStratup Initialising winsock2 library
    * return 0 if there is no problems
    */

  // return 0 if OK
  windWSAError = WSAStartup(MAKEWORD(2, 2), &WSAData);

#else
  int windWSAError = 0;
#endif

  logFlag = 1;
  nbPlayer += 1;

  SOCKADDR_IN serveurAddr = {0};
  SOCKET sock;
  SOCKET sockServ;
  SOCKADDR_IN clientAddr;
  socklen_t sizeofSocketConnected;

  t_user infoClient;
  sprintf(infoClient.pseudo, "PasDeCli");

  ServerStatus_t startGame;
  startGame.isServerStartGame = 0;
  sprintf(startGame.mapNameGame, "0");
  
  Tile gridTemp[_X_SIZE_][_Y_SIZE_];

  if (verbose >= 1)
    printf("\nLancement de la créatoin du serveur...\n");

  if (!windWSAError)
  {

    // Init structure
    // Can change s_addr with given ip inet_addr("192.168.0.0") or INADDR_ANY

    serveurAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveurAddr.sin_family = AF_INET;
    serveurAddr.sin_port = htons(PORT);

    /*
      * Creating socket :
      * param 1 : Use TCP/IP
      * param 2 : Use with TCP
      * param 3 : Protocole parameter (useless) -> 0
    */

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET)
    {

      if (verbose >= 1)
        printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", sock);
      sleep(1);
      logFlag = 2;

      //Bind info to the socket
      sockServ = bind(sock, (SOCKADDR *)&serveurAddr, sizeof(serveurAddr));

      if (sockServ != SOCKET_ERROR)
      {
        if (verbose >= 1)
          printf("\nDémarrage du serveur... \n");
        getLocalIP();
        if (verbose >= 1)
          printf("LIP DU SERV EST %s", monIP);

        // Start to listen incomming connexion
        sockServ = listen(sock, 5);

        if (sockServ != SOCKET_ERROR)
        {
          if (verbose >= 1)
            printf("\nEn attente de la connexion d'un client...\n");
          sleep(1);
          logFlag = 3;

          sizeofSocketConnected = sizeof(clientAddr);
          // Save the Client socket to an extern int
          socketConnected = accept(sock, (struct sockaddr *)&clientAddr, &sizeofSocketConnected);

          if (socketConnected != SOCKET_ERROR)
          {

            if (verbose >= 1)
              printf("\nConnexion établie avec le client !\n");
            sleep(1);
            logFlag = 4;

            if (verbose >= 1)
              printf("socketConnectedCli = %d\n", socketConnected);

            if (recep((void *)&infoClient, sizeof(infoClient), socketConnected, NULL) != NULL)
            {
              if (verbose >= 1)
                printf("\nPseudo client = %s\n", infoClient.pseudo);
              sprintf(pseudoClient, "%s s'est connecté !", infoClient.pseudo);
              if (verbose >= 1)
                printf("SocketServer pseudoCli : %s\n", pseudoClient);
              logFlag = 5;
            }

            while (serverStatus != 2)
            {
              if (verbose >= 2)
                printf("Waiting to start ... \n");
              sleep(2);
            }
            startGame.isServerStartGame = 2;
            sprintf(startGame.mapNameGame, "%s", mapMultiSelected);

            loadMap(gridTemp[0],mapMultiSelected);

            setupMultiMap(startGame.multiMap[0], gridTemp[0]);
            if(verbose>=2)displayMapMulti(startGame.multiMap[0]);

            if (sendStruct((void *)&startGame, sizeof(startGame), socketConnected, NULL) != OK)
            {
              printf("Erreur d'envoie \n");
            }
            else
            {
              if (verbose >= 1)
                printf("Structure envoyée .... \n");
              if (verbose >= 1)
                printf("Struct envoyé : isServerStartGame : %d \n", startGame.isServerStartGame);
              if (verbose >= 1)
                printf("Struct envoyé : isServerStartGame : %s \n", startGame.mapNameGame);
            }
            
            if (recep((void *)&startGame, sizeof(startGame), socketConnected, NULL) != NULL)
            {
              serverStatus = startGame.isServerStartGame;
              printf("Status server : %d", startGame.isServerStartGame);
            }
            else
            {
              if (verbose >= 2)
                printf("Pas de recep de status MAP \n");
            }
            if (verbose >= 1)
              printf("\nChargement de la partie... \n Fermeture de la fonction ... \n");
          }
          else
          {
            printf("\nUn problème est survenu lors de la connexion du client :( \n");
            return SERV_ERROR;
          }
        }
        else
        {
          printf("\nUn problème est survenu lors de la liaison avec le client :( \n");
          return SERV_ERROR;
        }
      }
      else
      {
        printf("\nUn problème est survenu lors de la création de la socket :( \n");
        return SERV_ERROR;
      }
    }
    else
    {
      printf("Un problème est survenu avec Windows :( \n");
      return SERV_ERROR;
    }
  }
  loadMap(matrix, startGame.mapNameGame);
  //loadMap(matrix, "map_ice_island");
  return OK;
}