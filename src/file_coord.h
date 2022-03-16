#ifndef file_coord_h
#define file_coord_h
#include "struct.h"

/** \file file_coord.h
 * \brief Countains all fonctions needed to manage a file
 */

/** \struct Element
 * \brief Structure representing an element of a file, used to navigate between elements.
 */
typedef struct Element Element;
struct Element
{
    Coord nombre; //!< nombre : value of the coordinate
    Element *suivant; //!< suivant : the next element
};

/** \struct File
 * \brief Structure representing the first element of a file. 
 */
typedef struct File File;
struct File
{
    Element *premier; //!< premier : the first element in the file
};

/** \fn initialiser()
 * \brief Initializes the empty file
 * \return the file 
 */
File *initialiser();


/** \fn file_vide(File *f)
 * \brief Deliver true if the file is empty, false otherwise.
 * \param f The file
 * \return True or False
 */
bool file_vide(File *f);


/** \fn enfiler(File *file, Coord nvNombre)
 * \brief Adds the queue value
 * \param file The file
 * \param nvNombre Value that we put in file
 */
void enfiler(File *file, Coord nvNombre);


/** \fn defiler(File *file)
 * \brief Removes the file head value
 * \param file The file
 * \return Element's value
 */
Coord defiler(File *file);


/** \fn afficherFile(File *file)
 * \brief Display the file
 * \param file The file
 */
void afficherFile(File *file);


/** \fn detruire_file(File *file)
 * \brief Destruct the file
 * \param file The file
 */
void detruire_file(File *file);

#endif