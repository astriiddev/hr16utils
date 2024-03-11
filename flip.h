#ifndef __FLIP_H
#define __FLIP_H

/* HR16 ROM tools
 * flip.h - prototypes for the ROM loading, saving and flipping
 *
 * Copyright (C) 2008 Gordon JC Pearce
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#define BITFLIP(b) ((b) & 0x80) >> 7 | ((b) & 0x40) >> 5 | ((b) & 0x20) >> 3 | ((b) & 0x10) >> 1 | \
                   ((b) & 0x08) << 1 | ((b) & 0x04) << 3 | ((b) & 0x02) << 5 | ((b) & 0x01) << 7

int flip(unsigned char *buffer);
void set_title(unsigned char *buffer, int sample, char *title, int centre);
int set_offset(unsigned char *buffer, int sample, int offset);

#endif /* __FLIP_H */
