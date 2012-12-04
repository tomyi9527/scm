// Copyright (C) 2011-2012 Robert Kooima
//
// LIBSCM is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITH-
// OUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.

#ifndef SCM_SYSTEM_HPP
#define SCM_SYSTEM_HPP

#include "scm-scene.hpp"
#include "scm-cache.hpp"

//------------------------------------------------------------------------------

class scm_system
{
public:

    // External Interface

    scm_system();
   ~scm_system();

    int        add_scene(int);
    void       del_scene(int);
    scm_scene *get_scene(int);
    int        get_scene_count() const;

#if 0
    int        add_step(int);
    void       del_step(int);
    scm_step  *get_step(int);
    int        get_step_count() const;
#endif

    // Internal Interface

    scm_cache *get_scm_cache(const std::string&);
    int        get_scm_index(const std::string&);

private:

    scm_model  *model;
    scm_scene_v scenes;
    scm_cache_v caches;
    scm_file_s  files;

#if 0
    scm_path_p  path;
    scm_step    here;
#endif
};

//------------------------------------------------------------------------------

#endif
