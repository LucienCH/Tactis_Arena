#ifndef state_h
#define state_h
/** \file state.h
 * \brief Defining Statelist Structures and all primitive functions
 * \author Robin BUTEL
 */
#include "substruct.h"

/** \struct listelem
 * \brief Defined as List_Elem, is the basic structure for an element of a Statelist
 */
typedef struct listelem{
  Status * value;             //!< A pointer to a Status, keeps mod info, needed for removing the buff at the end of it's duration
  struct entity_t * entity;   //!< A pointer to the entity affected by the mod, needed for removing the buff at the end of it's duration
  struct listelem * suiv;     //!< A pointer to the next element of the list
  struct listelem * prec;     //!< A pointer to the precedent element of the list
} List_Elem;
/** \typedef List_Elem
 * \brief An element of a Statelist, containing mod info, modded entity, and list pointers
 */

/** \struct StateList
 * \brief A list, containing a flag and a pointer to the current element.
 */
typedef struct {
  List_Elem * drapeau; //!< A pointer to the flag element, acts as the list's root, allowing it to loop, while always knowing where the beginning and the end of the list is
  List_Elem * ec;      //!< A pointer to the current element of the list, if the list if empty, it will point to the flag
} StateList;

/** \fn init_list(StateList ** list)
 * \brief Initialises a list
 * \details Allocates dyanmically space for the list, the flag and sets the current element onto it, as well as sets the flag's next and precedent pointers to itself
 * \param list pointer to the list to initialise
 * \return err_t POINTER_NULL if malloc fails, otherwise err_t OK
 */
err_t init_list(StateList ** list);

/** \fn out_of_list(StateList * list)
 * \brief Indicates if current element is out of the list
 * \details Current element is not in the list if it points to the flag
 * \param list The list to check
 * \return TRUE if the current element (ec) is out of list
 */
bool out_of_list(StateList * list);

/** \fn list_empty(StateList * list)
 * \brief Indicates if a list is empty
 * \details A list is empty if the flag's next element and precedent element point to itself
 * \param list The list to check
 * \return TRUE if the list is empty
 */
bool list_empty(StateList * list);

/** \fn start_list(StateList * list)
 * \brief Sets the current element to the beginning of the list
 * \param list The list to set
 * \return an err_t custom error type
 */
err_t start_list(StateList * list);

/** \fn end_list(StateList * list)
 * \brief Sets the current element to the end of the list
 * \param list The list to set
 * \return err_t OK
 */
err_t end_list(StateList * list);

/** \fn list_next(StateList * list)
 * \brief Sets the current element to the next element of the list
 * \param list The list to set
 * \return err_t OK
 */
err_t list_next(StateList * list);

/** \fn list_change(StateList * list, int d)
 * \brief Increases or decreases current mod's duration
 * \details Changes current element's Status' duration by d (increase if positive, decrease if negative), if the duration falls to or under zero, then a pointer to the element is returned so that the mod may be removed
 * \param list The list to change
 * \param d How much to change the duration by (negative value if a decrease is wanted)
 * \return A pointer to the current element if the duration falls to or under zero, otherwise NULL
 */
List_Elem * list_change(StateList * list, int d);

/** \fn list_remove(StateList * list)
 * \brief Removes the current element from the list
 * \details frees all dynamically allocated pointers and sets their values to NULL to avoid any double frees
 * \param list The list to remove from
 * \return err_t OK
 */
err_t list_remove(StateList * list);

/** \fn list_add(StateList * list, Status v, struct entity_t * entity)
 * \brief Adds a new element to the list
 * \details allocates dyanmically a new List_Elem as well as a Status 
 * \param list The list to add to
 * \param v The status to add
 * \param entity The targeted entity
 * \return err_t OK
 */
err_t list_add(StateList * list, Status v, struct entity_t * entity);

/** \fn list_search(StateList * list, struct entity_t * entity, statusId status)
 * \brief Search a list for a certain element
 * \details Searches a list starting from the current element and returns the first element that fits the search parametres (at least one parametre has to be specified)
 * \param list The list to search
 * \param entity A search parametre, the entity affected by the mod, can be NULL to specify no specific entity
 * \param status A search parametre, the status effect, can be -1 to specify no specific status effect
 * \exception standard_out:NO_SEARCH_PARAMETRES Neither of the search parametres were specified
 * \return a pointer to the current element if one is found, else NULL
 */
List_Elem * list_search(StateList * list, struct entity_t *entity, statusId status); //Set status to -1 for any status

/** \fn list_check(StateList * list)
 * \brief Checks if the current element is a buff or a debuff
 * \param list The list to check
 * \return TRUE if buff, FALSE if debuff
 */
bool list_check(StateList * list);

/** \fn list_destroy(StateList * list)
 * \brief Destroys a list
 * \details frees all dynamically allocated pointers and sets their values to NULL to avoid any double frees
 * \param list pointer to the list to destroy
 * \return err_t OK
 */
err_t list_destroy(StateList * list);


#endif