#ifndef servFcnt_h
#define servFcnt_h
#define MAX_BUFF_SIZE 1024
#define PORT 3555
#include "struct.h"
#include "common.h"

/**
 * \file servFcnt.h
 * \brief All the function used by the server or the client
 * \details Contains all the function for server and client 
 * \author Lucien Chauvin
*/

extern unsigned int logFlag;
extern int isPseudoValid;
extern int isInfoJoinSet;
extern char monIP[85];
extern char ipSrv[85];
extern char MapMultiSelected[50];
extern char pseudoClient[128];
extern int socketConnected;


err_t startTCPSocketServ();
err_t startTCPSocketCli();
err_t stopTCPSocketServ(int socketConnected);
err_t stopTCPSocketCli(int socketConnected);

/**
 * \fn err_t sendStruct(void * structure, int size, int socket, err_t (*print)(void * s, char tab[STR_SHORT]))
 * \brief Generic function to send a structur to a client or a server
 * \param structure The sturcture that we want to send
 * \param size Size of the structure to send
 * \param socket the socket that the user is connected to
 * \param print Pointer to print the structure to send (NULL if ther is no function to print)
 * \return err_t SEND_OK or SEND_ERROR
*/

err_t sendStruct(void * structure, int size,int socket, err_t (*print)(void * s, char tab[STR_SHORT]));

/**
 * \fn void getLocalIP(void)
 * \brief Function to get the local ip of the server
 * \return void
*/

void getLocalIP();

/**
 * \fn recep(void * structure, int size, int socket, err_t (*print)(void * s, char tab[STR_SHORT]))
 * \brief Generic function to receiv a structur of a client or a server
 * \param container The sturcture that we want to receive
 * \param size Size of the structure to receive
 * \param socket the socket that the user is connected to
 * \param print Pointer to print the structure to receiv (NULL if ther is no function to print)
 * \return err_t RECEP_OK
*/

void * recep(void * container, int size, int socket, err_t (*print)(void * s, char tab[STR_SHORT]));

/**
 * \fn setupMultiMap(MultiTile  * mapMulti, Tile * mapLocal)
 * \brief Function to create the multitile map that we want to send
 * \param mapMulti MultiTile structure that we want to send to the client
 * \param mapLocal Local map used to create the Multitile structure
 * \return 1 
*/

int setupMultiMap(MultiTile  * mapMulti, Tile * mapLocal);

/**
 * \fn displayMapMulti(MultiTile * mapMulti)
 * \brief Function to display the Multitile map that we send or receive
 * \param mapMulti MultiTile structure to display
 * \return 1 
*/

int displayMapMulti(MultiTile * mapMulti);

#endif
