#include <stdlib.h>
#include "state.h"
#include "print.h"

/** \file state.c
 * \brief Primitive functions for the Statelist system
 * \author Robin BUTEL
 * \details All functions used to navigate the buff/debuff list system made using pointers
 */

err_t init_list(StateList ** list)
{
    *list = malloc(sizeof(StateList));
    (*list)->drapeau = malloc(sizeof(List_Elem));
    if((*list)->drapeau == NULL) return POINTER_NULL;
    (*list)->drapeau->value = NULL;
    (*list)->drapeau->prec = (*list)->drapeau;
    (*list)->drapeau->suiv = (*list)->drapeau;
    (*list)->ec = (*list)->drapeau;

    return OK;
}

bool out_of_list(StateList * list)
{
    return list->ec == list->drapeau;
}

bool list_empty(StateList * list)
{
    return list->drapeau->suiv == list->drapeau;
}

err_t start_list(StateList * list)
{
    list->ec=list->drapeau->suiv;
    return OK;
}

err_t end_list(StateList * list)
{
    list->ec=list->drapeau->prec;
    return OK;
}

err_t list_next(StateList * list)
{
    list->ec=list->ec->suiv;
    return OK;
}

List_Elem * list_change(StateList * list, int d)
{
    list->ec->value->duration += d;

    if(list->ec->value->duration<=0)
        return list->ec;

    return NULL;
}

err_t list_remove(StateList * list)
{
    if(!(out_of_list(list)))
    {
        List_Elem * temp = list->ec;
        temp->prec->suiv=temp->suiv;
        temp->suiv->prec=temp->prec;
        list->ec=temp->prec;

        if(temp->value!=NULL)
        {
            free(temp->value);
            temp->value = NULL;
        }

        if(temp!=NULL)
        {
            free(temp);
            temp = NULL;
        }
    }

    return OK;
}

err_t list_add(StateList * list, Status v, struct entity_t * entity)
{
    end_list(list);
    
    Status * newstat = malloc(sizeof(Status));
    if(newstat == NULL) return POINTER_NULL;
    *newstat = v;


    List_Elem * newelem = malloc(sizeof(List_Elem));
    if(newelem == NULL) return POINTER_NULL;
    newelem->value = newstat;
    newelem->entity = entity;
    newelem->prec = list->ec;
    newelem->suiv = list->drapeau;

    list->ec->suiv = newelem;
    list->drapeau->prec = newelem;

    list->ec = newelem;
    return OK;
}

List_Elem * list_search(StateList * list, struct entity_t * entity, statusId status)
{
    if(entity == NULL)
    {
        if(status!=-1)
        {
            while(!out_of_list(list))
            {
                if(list->ec->value->value == 0 && list->ec->value->stat == status)
                {
                    return list->ec;
                }
                list_next(list);
            }
        }
        else
        {
            printf("NO_SEARCH_PARAMETRES\n");
        }

    }
    else if(status == -1)
    {
        while(!out_of_list(list))
        {
            if(list->ec->entity == entity)
            {
                return list->ec;
            }
            list_next(list);
        }
    }
    else
    {
        while(!out_of_list(list))
        {
            if(list->ec->entity == entity && list->ec->value->value == 0 && list->ec->value->stat == status)
            {
                return list->ec;
            }
            list_next(list);
        }
    }


    return NULL;
}

bool list_check(StateList * list) //Returns true if Buff, false if debuff
{
    if(list->ec->value->value==0)
    {
        if(list->ec->value->stat<Blessed)
            return FALSE;
        else
            return TRUE;
    }
    else
    {
        if(list->ec->value->value<0)
            return FALSE;
        else
            return TRUE;
    }
}

err_t list_destroy(StateList * list)
{
    if(list!=NULL)
    {
        if(!(list_empty(list)))
        {
            start_list(list);
            while(!(out_of_list(list)))
            {
                list_remove(list);
                list_next(list);
            }
        }

        if(list->drapeau!=NULL)
        {
            free(list->drapeau);
            list->drapeau = NULL;
        }

        free(list);
        list = NULL;
    }

    return OK;
}