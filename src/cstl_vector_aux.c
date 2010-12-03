/*
 *  The implement of vector auxiliary function.
 *  Copyright (C)  2008,2009,2010  Wangbo
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Author e-mail: activesys.wb@gmail.com
 *                 activesys@sina.com.cn
 */

/** include section **/
#include <cstl/cstl_def.h>
#include <cstl/cstl_alloc.h>
#include <cstl/cstl_types.h>
#include <cstl/cstl_iterator.h>
#include <cstl/cstl_iterator_private.h>

#include <cstl/cstl_vector_iterator.h>
#include <cstl/cstl_vector_private.h>

#include "cstl_vector_aux.h"

/** local constant declaration and local macro section **/
#define _VECTOR_CAPACITY_SHRESHOLD_SIZE     16 /* capacity shreshold size */

/** local data type declaration and local struct, union, enum section **/

/** local function prototype section **/

/** exported global variable definition section **/

/** local global variable definition section **/

/** exported function implementation section **/
#ifndef NDEBUG
/**
 * Test iterator referenced data is within the vector.
 */
bool_t _vector_iterator_belong_to_vector(const vector_t* cpvec_vector, vector_iterator_t it_iter)
{
    assert(cpvec_vector != NULL);
    assert(it_iter._t_iteratortype == _RANDOM_ACCESS_ITERATOR);
    assert(it_iter._t_containertype == _VECTOR_CONTAINER);
    assert(_GET_VECTOR_CONTAINER(it_iter) == cpvec_vector);

    if(_GET_VECTOR_COREPOS(it_iter) >= cpvec_vector->_pc_start &&
       _GET_VECTOR_COREPOS(it_iter) <= cpvec_vector->_pc_finish)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Test the type that saved in the vector container and referenced by it_iter are same.
 */
bool_t _vector_same_vector_iterator_type(const vector_t* cpvec_vector, vector_iterator_t it_iter)
{
    assert(cpvec_vector != NULL);
    assert(_GET_VECTOR_CONTAINER(it_iter) != NULL);
    assert(_GET_VECTOR_CONTAINER_TYPE(it_iter) == _VECTOR_CONTAINER);
    assert(_GET_VECTOR_ITERATOR_TYPE(it_iter) == _RANDOM_ACCESS_ITERATOR);
    return _vector_same_type(cpvec_vector, _GET_VECTOR_CONTAINER(it_iter));
}

/**
 * Test vector is created by create_vector.
 */
bool_t _vector_is_created(const vector_t* cpvec_vector)
{
    assert(cpvec_vector != NULL);

    if(cpvec_vector->_t_typeinfo._t_style != _TYPE_C_BUILTIN &&
       cpvec_vector->_t_typeinfo._t_style != _TYPE_CSTL_BUILTIN &&
       cpvec_vector->_t_typeinfo._t_style != _TYPE_USER_DEFINE)
    {
        return false;
    }

    if(cpvec_vector->_pc_start != NULL || cpvec_vector->_pc_finish != NULL || cpvec_vector->_pc_endofstorage != NULL)
    {
        return false;
    }

    return _alloc_is_inited(&cpvec_vector->_t_allocater);
}

/**
 * Test vector is initialized by vector initialization functions.
 */
bool_t _vector_is_inited(const vector_t* cpvec_vector)
{
    assert(cpvec_vector != NULL);

    if(cpvec_vector->_t_typeinfo._t_style != _TYPE_C_BUILTIN &&
       cpvec_vector->_t_typeinfo._t_style != _TYPE_CSTL_BUILTIN &&
       cpvec_vector->_t_typeinfo._t_style != _TYPE_USER_DEFINE)
    {
        return false;
    }

    if(cpvec_vector->_pc_start == NULL && cpvec_vector->_pc_finish == NULL && cpvec_vector->_pc_endofstorage == NULL)
    {
        return true;
    }
    else if(cpvec_vector->_pc_start != NULL &&
            cpvec_vector->_pc_finish != NULL &&
            cpvec_vector->_pc_endofstorage != NULL &&
            cpvec_vector->_pc_finish >= cpvec_vector->_pc_start &&
            cpvec_vector->_pc_endofstorage >= cpvec_vector->_pc_start &&
            cpvec_vector->_pc_endofstorage >= cpvec_vector->_pc_finish)
    {
        return true;
    }
    else
    {
        return false;
    }
}
#endif /* NDEBUG */

/**
 * Test the type that saved in the vector container is same.
 */
bool_t _vector_same_type(const vector_t* cpvec_first, const vector_t* cpvec_second)
{
    assert(cpvec_first != NULL);
    assert(cpvec_second != NULL);
    return _type_is_same(_GET_VECTOR_TYPE_NAME(cpvec_first), _GET_VECTOR_TYPE_NAME(cpvec_second)) &&
           (cpvec_first->_t_typeinfo._pt_type == cpvec_second->_t_typeinfo._pt_type) &&
           (cpvec_first->_t_typeinfo._t_style == cpvec_second->_t_typeinfo._t_style);
}

/**
 * Initialize data within range [pby_start, pby_finish) according to vector element data type.
 */
void _vector_init_elem_range_auxiliary(vector_t* pvec_vector, char* pc_start, char* pc_finish)
{
    char* pc_pos = NULL;

    assert(pvec_vector != NULL);
    assert(pc_start != NULL);
    assert(pc_finish != NULL);
    assert(pc_start <= pc_finish);

    /* initialize new elements */
    if(_GET_VECTOR_TYPE_STYLE(pvec_vector) == _TYPE_CSTL_BUILTIN)
    {
        /* get element type name */
        char s_elemtypename[_TYPE_NAME_SIZE + 1];
        _type_get_elem_typename(_GET_VECTOR_TYPE_NAME(pvec_vector), s_elemtypename);

        for(pc_pos = pc_start; pc_pos < pc_finish; pc_pos += _GET_VECTOR_TYPE_SIZE(pvec_vector))
        {
            _GET_VECTOR_TYPE_INIT_FUNCTION(pvec_vector)(pc_pos, s_elemtypename);
        }
    }
    else
    {
        for(pc_pos = pc_start; pc_pos < pc_finish; pc_pos += _GET_VECTOR_TYPE_SIZE(pvec_vector))
        {
            bool_t b_result = _GET_VECTOR_TYPE_SIZE(pvec_vector);
            _GET_VECTOR_TYPE_INIT_FUNCTION(pvec_vector)(pc_pos, &b_result);
            assert(b_result);
        }
    }
}

/**
 * Obtain data from variable argument list, the data type and vector element data type are same.
 */
void _vector_get_varg_value_auxiliary(vector_t* pvec_vector, va_list val_elemlist, void* pv_varg)
{
    assert(pvec_vector != NULL);
    assert(pv_varg != NULL);

    _vector_init_elem_auxiliary(pvec_vector, pv_varg);
    _type_get_varg_value(&pvec_vector->_t_typeinfo, val_elemlist, pv_varg);
}

/**
 * Destroy data, the data type and vector element data type are same.
 */
void _vector_destroy_varg_value_auxiliary(vector_t* pvec_vector, void* pv_varg)
{
    assert(pvec_vector != NULL);
    assert(pv_varg != NULL);

    /* destroy varg value and free memory */
    bool_t b_result = _GET_VECTOR_TYPE_SIZE(pvec_vector);
    _GET_VECTOR_TYPE_DESTROY_FUNCTION(pvec_vector)(pv_varg, &b_result);
    assert(b_result);
}

/**
 * Calculate new capacity according to old size and insert size.
 */
size_t _vector_calculate_new_capacity(size_t t_old_size, size_t t_insert_size)
{
    size_t t_grow_size = (t_old_size + t_insert_size) / 2;
    if(t_grow_size < _VECTOR_CAPACITY_SHRESHOLD_SIZE)
    {
        t_grow_size = _VECTOR_CAPACITY_SHRESHOLD_SIZE;
    }

    return t_old_size + t_insert_size + t_grow_size;
}

/** local function implementation section **/

/** eof **/

