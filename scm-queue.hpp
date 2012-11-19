// Copyright (C) 2011-2012 Robert Kooima
//
//  LIBSCM is free software; you can redistribute it and/or modify it under the
//  terms of the GNU General Public License as published by the Free Software
//  Foundation; either version 2 of the License, or (at your option) any later
//  version.
//
//  This program is distributed in the hope that it will be useful, but WITH-
//  OUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//  more details.

#ifndef SCM_QUEUE_HPP
#define SCM_QUEUE_HPP

#include <SDL.h>
#include <SDL_thread.h>

#include <set>

//------------------------------------------------------------------------------

template <typename T> class queue
{
public:

    queue(int);
   ~queue();

    bool try_insert(T&);
    bool try_remove(T&);

    void insert(T);
    T    remove( );

private:

    SDL_sem   *full_slots;
    SDL_sem   *free_slots;
    SDL_mutex *data_mutex;

    std::set<T> S;
};

//------------------------------------------------------------------------------

template <typename T> queue<T>::queue(int n)
{
    full_slots = SDL_CreateSemaphore(0);
    free_slots = SDL_CreateSemaphore(n);
    data_mutex = SDL_CreateMutex();
}

template <typename T> queue<T>::~queue()
{
    SDL_DestroyMutex(data_mutex);
    SDL_DestroySemaphore(free_slots);
    SDL_DestroySemaphore(full_slots);
}

//------------------------------------------------------------------------------

template <typename T> bool queue<T>::try_insert(T& d)
{
    if (SDL_SemTryWait(free_slots) == 0)
    {
        SDL_mutexP(data_mutex);
        {
            S.insert(d);
        }
        SDL_mutexV(data_mutex);
        SDL_SemPost(full_slots);
        return true;
    }
    return false;
}

template <typename T> bool queue<T>::try_remove(T& d)
{
    if (SDL_SemTryWait(full_slots) == 0)
    {
        SDL_mutexP(data_mutex);
        {
            d   = *(S.begin());
            S.erase(S.begin());
        }
        SDL_mutexV(data_mutex);
        SDL_SemPost(free_slots);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

template <typename T> void queue<T>::insert(T d)
{
    SDL_SemWait(free_slots);
    SDL_mutexP(data_mutex);
    {
        S.insert(d);
    }
    SDL_mutexV(data_mutex);
    SDL_SemPost(full_slots);
}

template <typename T> T queue<T>::remove()
{
    T d;

    SDL_SemWait(full_slots);
    SDL_mutexP(data_mutex);
    {
        d   = *(S.begin());
        S.erase(S.begin());
    }
    SDL_mutexV(data_mutex);
    SDL_SemPost(free_slots);

    return d;
}

//------------------------------------------------------------------------------

#endif