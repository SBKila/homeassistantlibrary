#pragma once
#include <Arduino.h>
#include "debug.h"

namespace HALIB_NAMESPACE
{
    template <class T>
    struct LinkedListNode
    {
        T data;
        LinkedListNode<T> *next;
    };

    template <typename T>
    class LinkedList
    {
    protected:
        int _size = 0;
        LinkedListNode<T> *root;
        LinkedListNode<T> *last;

    public:
        LinkedList();
        virtual ~LinkedList();
        int getSize()
        {
            return _size;
        }
        T get(int index);
        int find(T, bool);
        /*
		append a T object in the end of the LinkedList;
	    */
        virtual bool append(T);
        virtual bool remove(T, bool = false);

        // shift() will remove and return the FIRST element
        T shift();
    };

    // Initialize LinkedList with false values
    template <typename T>
    LinkedList<T>::LinkedList()
    {
        root = NULL;
        last = NULL;
    };

    // Clear Nodes and free Memory
    template <typename T>
    LinkedList<T>::~LinkedList()
    {
        LinkedListNode<T> *tmp;
        while (root != NULL)
        {
            tmp = root;
            root = root->next;
            delete tmp;
        }
        last = NULL;
    };

    template <typename T>
    bool LinkedList<T>::append(T _t)
    {

        LinkedListNode<T> *tmp = new LinkedListNode<T>();
        tmp->data = _t;
        tmp->next = NULL;

        if (NULL != last)
        {
            // Already have elements inserted
            last->next = tmp;
            last = tmp;
        }
        else
        {
            // First element being inserted
            root = tmp;
            last = tmp;
        }

        _size++;
        return true;
    };

    template <typename T>
    T LinkedList<T>::get(int index)
    {
        LinkedListNode<T> *tmp = root;
        int offset = 0;

        while ((index != offset) && (NULL != tmp->next))
        {
            tmp = tmp->next;
            offset++;
        }

        if (index == offset)
        {
            return tmp->data;
        }
        else
        {
            return NULL;
        }
    }
    template <typename T>
    int LinkedList<T>::find(T _t, bool isPointerData)
    {
        LinkedListNode<T> *tmp = root;
        int position = 0;

        while ((tmp != NULL) && (isPointerData ? (*(tmp->data) != *_t) : (tmp->data != _t)))
        {
            tmp = tmp->next;
            position++;
        }

        if (NULL != tmp)
        {
            return position;
        }
        else
        {
            return -1;
        }
    }
    template <typename T>
    T LinkedList<T>::LinkedList::shift()
    {
        T data = NULL;
        if (NULL != root)
        {
            data = root->data;
            if (NULL != data)
            {
                remove(data);
            }
        }
        return data;
    }
    template <typename T>
    bool LinkedList<T>::remove(T _t, bool isPointerData)
    {
        LinkedListNode<T> *tmp = root;
        LinkedListNode<T> *prev = NULL;
        while (tmp != NULL)
        {
            if (isPointerData ? (*(tmp->data) == *_t) : (tmp->data == _t))
            {
                if (NULL == prev) // this is root
                {
                    if (NULL != tmp->next)
                    {
                        root = tmp->next;
                    }
                    else // this is last node
                    {
                        root = NULL;
                        last = NULL;
                    }
                }
                else // previous exist
                {
                    if (NULL != tmp->next) // have next
                    {
                        prev->next = tmp->next;
                    }
                    else // this is last node
                    {
                        prev->next = NULL;
                        last = prev;
                    }
                }
                _size--;
                if (NULL != tmp)
                {
                }
                delete tmp;
                return true;
            };
            // go to next node
            prev = tmp;
            tmp = tmp->next;
        }
        DEBUG_PRINTLN("LinkedList<T>::remove=false");
        return false;
    }
} // namespace HALIB_NAMESPACE