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
    *e = temp->val;
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
    // Create the new node
    Node * newElt = (Node *)malloc(sizeof(Node));
    if (!newElt){
        return ERRALLOC;
    }
    newElt->val = e;
    newElt->next = 0;
    //insert the new node
    Node * pNext = L->head;
    Node *prev;
    int count = -1;
    while(pNext){
        prev = pNext;
        pNext = pNext->next;
        count ++;
        if (count > L->nelts){
            return  ERRINDEX;
        }
        else if (count == p){
            if (p==L->nelts-1){
                // adding   the tail
                pNext = newElt;
                prev->next = pNext;
                L->nelts++;
                return OK;
            }
            else if (p==0){
                newElt->next = prev;
                L->head = newElt;
                L->nelts++;
                return OK;
            }
            else{
                newElt->next = pNext;
                prev->next = newElt;
                L->nelts++;
            }
        }
    }

    // add to the head of the empty list
    if (!L->head && p == 0){
        L->head = newElt;
        L->nelts++;
        return OK;
    }
    return OK;
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
    int count = -1;
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
            if (p==L->nelts-1){
                // removing   the tail
                free(temp);
            }
            else if (p==0){
                // removing the head
                L->head = temp;
                free(prev);
            }
            else{
                prev->next = temp->next;
                free(temp);
            }
            L->nelts--;
            return OK;
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
    int count = -1;
    while(temp){
        prev = temp;
        temp = temp->next;
        if (count > L->nelts){
            return  ERRINDEX;
        }
        if (L->comp(prev->val, e)==0){
            isFound =1;
            if (count==L->nelts-1){
                // removing   the tail
                free(temp);
            }
            else if (count==0){
                // removing the head
                L->head = temp;
                free(prev);
            }
            else{
                prev->next = temp->next;
                free(temp);
            }
            L->nelts--;
            return OK;
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
    while(temp){
        L->pr(temp->val);
        temp = temp->next;
        if (temp){
            printf("  --  ");
        }
    }
    return OK;
}

/** sequencially call given function with each element of given list (O(NxF)).
 * @param l the list
 * @param f the function
 */
void	forEach		(List* L, void(*f)(void*)){
    if (!f){
        return;
    }
    Node * temp = L->head;
    while(temp){
        f(temp->val);
        temp = temp->next;
    }
}

/** compute and return the number of elements in given list (O(1)).
 * @param l the list
 * @return the number of elements in given list
 */
int	lengthList	(List* L){
    return  L->nelts;
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
    //memcpy(newElt->val, e, sizeof(L)/L->nelts);
    newElt->val = e;
    newElt->next = 0;

    Node * pr1 = L->head;
    Node * prev ;
    while(pr1){
        prev = pr1;
        pr1 = pr1->next;
        if (!pr1){
            if( L->comp(newElt->val, prev->val)==1){
                // adding   the tail
                pr1 = newElt;
                prev->next = pr1;
                L->nelts++;
                break;
            }
        }
        else  {
           if ( L->comp(newElt->val, pr1->val) <=0){
               //insert between node
               newElt->next = pr1;
               prev->next = newElt;
               L->nelts ++;
               break;
           }
        }
    }
    if (!L->head ){
        pr1 = newElt;
        L->head = pr1;
        L->nelts ++;
    }
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
    if (!L->comp || !L->head){
        return 0;
    }
    Node * pr1 = L->head;
    Node * prev = L->head;
    int isHead = 0;
    while(pr1){
            prev = pr1;
            pr1 = pr1->next;
        if (L->comp(e, prev->val) == 0 && (prev== L->head)){
            return (Node*)1;
        }
        else if (L->comp(e, prev->val) == 0 ){
            return prev;
        }
    }
    return 0;
}



