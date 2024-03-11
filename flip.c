/* HR16 ROM tools
 * flip.c - flip the lower 8 bits of the address in a buffer
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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flip.h"
#include "romtools.h"

/*  reverse the lower 8 bits of the address */
int flip(unsigned char *buffer) 
{
  int ah, al, i = 0;
  unsigned char *temp = NULL;

  /*  give ourselves space to work */
  if((temp = malloc(FIRMWARE_SIZE * sizeof *temp)) == NULL) 
  {
    printf("Error while allocating memory!\n");
    return 0;
  }

  memcpy(temp, buffer, FIRMWARE_SIZE * sizeof *temp);

  for(; i < FIRMWARE_SIZE; i++) 
  {
    ah = i & 0xFF00;
    al = BITFLIP(i & 0x00FF);

    buffer[ah+al] = temp[i];
  }

  free(temp);

  return 1;
}

void set_title(unsigned char *buffer, int sample, char *title, int centre) 
{
    const int l = strlen(title) < 16 ? strlen(title) : 16;
    const int c = centre ? (16 - l / 2) : 0;

    memset(&buffer[TITLE_ADDR + (16 * sample)], ' ', 16);
    memcpy(&buffer[TITLE_ADDR + (16 * sample) + c], title, l);
}

int set_offset(unsigned char *buffer, int sample, int offset) 
{
  if (sample < 1 || sample > 49) return 1; /*  FIXME - define proper error codes */

  sample -= 1; /*  run from 0 to 48 */
  sample *= 2; /*  two byte value */
  
  /*  should probably check that offset is a multiple of 16, rather than just truncating */
  offset = offset / 16;

  buffer[OFFSET_ADDR + sample + 0] = offset / 256;
  buffer[OFFSET_ADDR + sample + 1] = offset & 255;

  return 0;
}


