#ifndef textures_h
#define textures_h

/** \file textures.h
 * \brief Textures method functions
 * \author Thibault DOUCET
 */

/* =============== DEPENDENCES =============== */


#include "struct.h"


/* =============== CONSTANTES =============== */


#define _NB_MAX_TEXTURES_ 50
#define _NB_CLASSES_ 6


/* =============== VARIABLES =============== */


extern int pxBase;      //!< Resolution the textures should be displayed (64 or 128)


/* =============== STRUCTURES =============== */

/**
 * \enum block_id
 * \brief Blocks ids
 */
typedef enum {BLANK, BLOCK, BLUE, RED, WATER, SAND, ICE, SNOW} block_id;   


/* =============== DICTIONNAIRES DE TEXTURES =============== */


extern TabTexture cSprites[_NB_MAX_TEXTURES_];      //!< Table where characters info textures are loaded
extern TabTexture textures[_NB_MAX_TEXTURES_];      //!< Table where general and map textures are loaded
extern CharTexture charTextures[_NB_CLASSES_];      //!< Table where characters textures are loaded 
extern AnimTexture animTextures[NB_AB];             //!< Table where animations textures are loaded


/* =============== VARIABLES =============== */


bool textures_loaded;           //!< True if map related textures are already loaded  
bool cSprites_loaded;           //!< True if characters info related textures are already loaded
bool charTextures_loaded;       //!< True if characters related textures are already loaded
bool animTextures_loaded;       //!< True if animations related textures are already loaded 


/* =============== FONCTIONS =============== */

/** \fn optimize(SDL_Surface *surf)
 * \brief Modify surface attributes to make it lighter to display
 * \param surf SDL_Surface * to optimize
 * \return a pointer to an optimized SDL_Surface
 */
SDL_Surface *optimize(SDL_Surface *surf);

/** \fn freeTextures(TabTexture * textures)
 * \brief Destroy a TabTexture table
 * \param textures TabTexture table to destroy
 * \return void
 */
void freeTextures(TabTexture * textures);

/** \fn freeAllTextures()
 * \brief Destroy all the loaded textures tables (TabTexture, CharTexture, AnimTexture)
 * \return an error code (err_t)
 */
err_t freeAllTextures();

/** \fn addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, char * texture_name)
 * \brief Add textures to a TabTexture dictionnary and put a name on it
 * \param texturesTable TabTexture table where to load textures
 * \param texture Pointer to the SDL_Surface (64x46) to load
 * \param big_texture Pointer to the SDL_Surface (128x128) to load
 * \param texture_name Name of the textures in the dictionnary
 * \return int
 */
int addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, char * texture_name);

/** \fn getTexture(TabTexture * textures, const char * texture_name)
 * \brief Get a texture (64x64) by its name in the given table
 * \param textures Table where to look for the texture
 * \param texture_name Name of the texture
 * \return a pointer to the SDL_Texture (64x64)
 */
SDL_Texture * getTexture(TabTexture * textures, const char * texture_name);

/** \fn getTexture(TabTexture * textures, const char * texture_name)
 * \brief Get a texture (128x128) by its name in the given table
 * \param textures Table where to look for the texture
 * \param texture_name Name of the texture
 * \return a pointer to the SDL_Texture (128x128)
 */
SDL_Texture * getBigTexture(TabTexture * textures, const char * texture_name);

/** \fn loadOptImage(char * img)
 * \brief Turn a png image to an optimized SDL_Surface
 * \param img Path to the image
 * \return a pointer to the SDL_Surface created
 */
SDL_Surface * loadOptImage(char * img);

/** \fn loadImage(char * img)
 * \brief Turn a png image to a SDL_Surface
 * \param img Path to the image
 * \return a pointer to the SDL_Surface created
 */
SDL_Surface * loadImage(char * img);

/** \fn loadTexture(SDL_Renderer * renderer, SDL_Surface * surface)
 * \brief Turn a SDL_Surface to a SDL_Texture
 * \param renderer SDL_Renderer where the image should be loaded
 * \param surface Pointer to the SDL_Surface to turn into a SDL_Texture
 * \return a pointer to the SDL_Texture generated
 */
SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface);

/** \fn loadSprites(SDL_Renderer * renderer, TabTexture * cSprites)
 * \brief Load all the textures needed for characters information and sprites
 * \param renderer SDL_Renderer where the textures should be loaded
 * \param cSprites TabTexture where the textures will be loaded
 * \return int
 */
int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites);

/** \fn loadMapTextures(SDL_Renderer * renderer)
 * \brief Load all the textures needed for map display
 * \param renderer SDL_Renderer where the textures should be loaded
 * \return int
 */
int loadMapTextures(SDL_Renderer * renderer);

#endif