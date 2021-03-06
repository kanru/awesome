/*
 * markup.h - markup header
 *
 * Copyright © 2008 Julien Danjou <julien@danjou.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef AWESOME_COMMON_MARKUP_H
#define AWESOME_COMMON_MARKUP_H

#include "common/buffer.h"

typedef struct markup_parser_data_t markup_parser_data_t;

typedef void (markup_on_elem_f)(markup_parser_data_t *, const char *,
                                const char **, const char **);

struct markup_parser_data_t
{
    buffer_t text;
    const char * const *elements;
    markup_on_elem_f *on_element;
    void *priv;
};

void markup_parser_data_init(markup_parser_data_t *);
void markup_parser_data_wipe(markup_parser_data_t *);
bool markup_parse(markup_parser_data_t *data, const char *, ssize_t);

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
