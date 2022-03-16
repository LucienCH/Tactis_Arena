#ifndef common_h
#define common_h
/** \file common.h
 * \brief Defining macros and other global variables common to the whole project
 * \author Robin BUTEL, Lucien CHAUVIN, Thibault DOUCET
 */

#define STR_SHORT 50        //!< Defines a length to use for short string arrays
#define STR_LONG 256        //!< Defines a length to use for long string arrays
#define NUM_ERROR 10        //!< Indicates the number of custom error messages created
#define MAXRANGE 256        //!< Indicates the Max Range of an ability, usefull when creating an array to store individual coordinates encompassed by the range
#define _NB_MAX_ANIM_ 13    //!< Indicates the max number of animation frames an ability can have
#define _X_SIZE_ 30         //!< Indicates the max horizontal size of the gameboard
#define _Y_SIZE_ 30         //!< Indicates the max vertical size of the gameboard
#define NUMBER_OF_MUS 2     //!< Indicates the number of music tracks available in game

/** \enum err_t
 * \brief Labeling custom error types.
 * 
 * To be used with string database error_message which contains custom error message for all custom error types
 */
typedef enum 
{
    OK/**<No error message*/, POINTER_NULL/**<Pointer is Null*/, SDL_ERROR/**<SDL error*/, 
    COORD_OUT_OF_BOUND/**<Coordinates out of limits*/, STR_TOO_LONG/**<String too long*/, 
    INIT_COORD_ERR/**<Ability coordinates were not initialised correctly*/, 
    CLI_ERROR/**<Client not initialised  correctly*/, SERV_ERROR/**<Server not initialised correctly*/,
    SEND_ERROR/**<Structure not sent correctly*/, ALREADY_IN_CACHE/**<Text already in cache, cannot add it twice*/
} err_t;

/** \enum bool
 * \brief Boolean Enumeration
 */
typedef enum {FALSE/**<0*/, TRUE/**<1*/} bool;

extern int verbose; //!< Global variable used to print messages depending on the level put in program parametres. 0 is saved only for debugging.

extern int serverStatus;    //!< Status of the server used in multi_menu 
extern int nbPlayer;        //!< Number of players in game 

extern char error_message[NUM_ERROR][STR_LONG]; //!< String database containing custom error messages to be used with err_t enumeration

extern char pseudoUser[50];         //!< Name of the user used in multi_menu, socket_Client and socket_server
extern char mapMultiSelected[50];   //!< Name of the map selected by the host used in multi_menu, socket_client and socket_server

/** \fn strToUpper(char *chaine)
 * \brief Transforms a string to all upper characters
 * \param chaine The string to be transformed
 * \return the transformed string
 */
char * strToUpper(char *chaine);

#endif
