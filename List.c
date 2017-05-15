//
// Created by Gervaise ALINA on 5/10/2017.
//
/********************************************************************
 *
 * $Id: List.h 1989 2015-10-16 18:19:52Z phil $
 *
 ********************************************************************
 */
/**
 *
 * Creation of a generic (simply linked) List structure.
 *
 * To create a list, one must provide two functions (one function to
 * compare / order elements, one function to display them). Unlike arrays,
 * indices begins with 1.
 *
 ********************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "List.h"
#include "status.h"



/** Empty List creation by dynamic memory allocation (O(1)).
 * @param comp comparison function between elements (ala strcmp())
 * @param pr display function for list elements
 * @return a new (empty) list if memory allocation OK
 * @return 0 otherwise
 */
List*	newList	(compFun comp, prFun pr){
    // create the list
    List * result = (List*)malloc(sizeof(List));
    if(!result) {
        return 0;
    }
    result->head = 0;
    result->nelts = 0;
    result->comp = comp;
    result->pr = pr;
    return result;
}

/** destroy the list by deallocating used memory (O(N)).
 * @param l the list to destroy */
void 	delList	(List* L){
    while(L->head){
        Node * temp = L->head;
        L->head = temp->next;
        free(temp);
    }
    free(L);
}

/** get the Nth element of the list (O(N)).
 * @param l the list
 * @param n the index of the element in list
 * @param e (out) the searched element
 * @return OK if element found
 * @return ERRINDEX if index out of list bounds
 */
status 	nthInList	(List* L, int n, void** e){
    if( n == L->nelts){
        return ERRINDEX;
    }
    int i = 0;
    Node * temp = L->head;
    while(temp || i < n){
       temp = temp->next;
    }
    memcpy( temp->val, e, sizeof(L)/L->nelts);
    return OK;
}
/** Insert element at a given position in the list (O(N)).
 * @param l the list to store the element in
 * @param p the position of the insertion point
 * @param e the element to insert
 * @return ERRINDEX if position is out of list bounds
 * @return ERRALLOC if memory allocation failed
 * @return OK otherwise
 */
status 	addListAt	(List*L, int p, void* e){

}

/** remove the element located at a given position in list (O(N)).
 * @param l the list to remove the element from
 * @param p the position of the element to remove
 * @param e (out) the removed element
 * @return ERRINDEX if position is out of list bounds
 * @return OK otherwise
 */
status 	remFromListAt	(List* L, int p,void** e){
    if (!L->comp){
        return ERRUNABLE;
    }
    Node * temp = L->head;
    Node *prev;
    int count = 0;
    int isFound = 0;
    while(temp){
        prev = temp;
        temp = temp->next;
        count ++;
        if (count > L->nelts){
            return  ERRINDEX;
        }
        if ((count == p) || (L->comp(temp->val, e)==0)){
            isFound =1;
            prev->next = temp->next;
            free(temp);
        }
    }
    if (!isFound){
        return ERRABSENT;
    }
    return OK;

}

/** remove given element from given list (O(N)).
 * implies the user has given a comparison function.
 * @param l the list to remove the element from
 * @param e the element to remove
 * @return ERRABSENT if element is not found in list
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status 	remFromList	(List*L, void*e){
    if (!L->comp){
        return ERRUNABLE;
    }
    Node * temp = L->head;
    Node *prev;
    int isFound = 0;
    while(temp){
        prev = temp;
        temp = temp->next;
        if (L->comp(temp->val, e)==0){
            isFound = 1;
            prev->next = temp->next;
            free(temp);
        }
    }
    if (!isFound){
        return ERRABSENT;
    }
    return OK;
}

/** display list elements as "[ e1 ... eN ]" (O(N)).
 * The display of the element is delegated to the prFun function
 * @param l the list to display.
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
*/
status 	displayList	(List* L){
    if (!L->pr){
        return ERRUNABLE;
    }
    Node * temp = L->head;
    while(!temp->next){
        L->pr(temp->val);
    }
    return OK;
}

/** sequencially call given function with each element of given list (O(NxF)).
 * @param l the list
 * @param f the function
 */
void	forEach		(List* L, void(*)(void*)f){

}

/** compute and return the number of elements in given list (O(1)).
 * @param l the list
 * @return the number of elements in given list
 */
int	lengthList	(List* L){
    int n;
    Node * temp = L->head;
    while(temp){
        temp = temp->next;
        n++;
    }
    return  n++;
}

/** add given element to given list according to compFun function (O(N)).
 * @param l the list (supposedly sorted according to compFun function)
 * @param e the element to add
 * @return ERRALLOC if memory allocation failed
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status	addList	(List*L, void*e){
    if (!L->comp){
        return ERRUNABLE;
    }
    Node * newElt = (Node *)malloc(sizeof(Node));
    if (!newElt){
        return ERRALLOC;
    }
    memcpy(newElt->val, e, sizeof(L)/L->nelts);
    newElt->next = 0;
    Node * pr1 = L->head;
    Node * prev;
    if (!L->head){
        L->head->next = newElt;
    }
    else {
        while(pr1){
            prev = pr1;
            pr1 = pr1->next;

            if (L->comp(newElt->val, pr1->val) < 0){
                //insert before temp
                newElt->next = pr1;
                prev->next = newElt;

                break;
            }
        }
    }
    L->nelts ++;;
    return OK;
}

/** tests whether the list contains given element (O(N)).
 * @param l the list
 * @param e the searched element
 * @return 0 if element is not found in list
 * @return 1 if element is at the head of the list (no predecessor)
 * @return (a pointer to) the predecessor of the search element otherwise
 */
Node*	isInList	(List*L, void*e){
    if (!L->comp){
        return 0;
    }
    Node * pr1 = L->head;
    Node * prev;
    int isHead = 0;
    while(pr1){
            prev = pr1;
            pr1 = pr1->next;
            isHead ++;

            if (L->comp(e, pr1->val) == 0){
                if (isHead==1){
                    return 1;
                }
                else{
                    return prev;
                }
            }
    }
    return 0;
}



