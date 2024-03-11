/* HR16 ROM tools
 * romutil.c - save a flipped copy of the ROM
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

/* 
 * this isn't great
 * it reads in a file called "hr16b.bin" and emits one called "out.bin"
 * the flipping is reversible so if you modify a ROM you can run this
 * program to flip the bytes the "right way round", hexedit the ROM, then
 * run the program again *having named the edited file 'hr16b.bin'* to
 * flip them back
*/ 

#include <stdio.h>
#include <stdlib.h>

#include <argp.h>
#include <errno.h>
#include <string.h>

#include "romtools.h"
#include "flip.h"

const char *argp_program_version = "romutil 0.0";
const char *argp_program_bug_address = "<gordon@gjcp.net>";

/* Program documentation. */
static char doc[] = "romutil - poke Alesis HR16B ROMs";
     
/* A description of the arguments we accept. */
static char args_doc[] = "FILENAME";
     
/* The options we understand. */
static struct argp_option options[] = 
{
    { "flip", 'f', 0, 0, "output the file \"flipped\"", 0 },
    { "sample", 's', "NUMBER", 0, "sample number to view or edit", 0 },
    { "address", 'a', "OFFSET", 0, "offset address of sample", 0 },
    { "title", 't', "TITLE", 0, "title for sample", 0 },
    { "centre", 'c', 0, 0, "centre the title", 0 },
    { "output", 'o', "FILE", 0, "output file", 0 },
  { 0 }
};

struct arguments {
  char *args[1];
  int flip;
  int centre;
  int sample_number;
  int offset;
  char *sample_title;
  char *output_file;
  char *file;
};
     
/*  argument parsing bits */
static error_t parse_opt (int key, char *arg, struct argp_state *state) 
{
  struct arguments *arguments = state->input;

  switch(key) 
  {
    case 'o': arguments->output_file = arg; break;
    case 'a': arguments->offset = atoi(arg); break;
    case 'f': arguments->flip = 1; break;
    case 'c': arguments->centre = 1; break;
    
    case 's': 
      arguments->sample_number = atoi(arg);
      
      if (arguments->sample_number > 49 || arguments->sample_number < 1) 
      {
        arguments->sample_number = 0;
        return ARGP_KEY_ERROR;
      }
      
      break;

    case 't': arguments->sample_title = arg; break;

    /*  too many arguments? */
    case ARGP_KEY_ARG:

      if (state->arg_num > 0) argp_usage(state);

      arguments->file = arg;
      break;

    /*  not enough arguments? */
    case ARGP_KEY_END:

      if (state->arg_num < 1) argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}    

static struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };


int main(int argc, char **argv) 
{
  int rom_dirty = 0;
  unsigned char msg[16], *buffer = NULL;
  
  FILE *file;
  struct arguments arguments;

  memset(&arguments, 0, sizeof(arguments));

  if(argp_parse (&argp, argc, argv, 0, 0, &arguments) != 0)
    return -1;
  
  if((file = fopen(arguments.file, "rb")) == NULL) 
  {
    printf("Error while opening %s!\n%s\n", arguments.file, strerror(errno));
    return -1;
  }

  fseek(file, 0, SEEK_END);

  if(ftell(file) != FIRMWARE_SIZE)
  {
    printf("File not HR16 OS Firmware!\nExiting...\n");
    fclose(file);

    return -1;
  }

  rewind(file);
  
  if ((buffer = malloc(sizeof(char)*FIRMWARE_SIZE)) == NULL)
  {
    printf("Error while allocating memory!\n");
    fclose(file);
    
    return -1;
  }

  if(fread(buffer,1,FIRMWARE_SIZE,file) < FIRMWARE_SIZE)
  {
    printf("Error while reading firmware!\n");

    free(buffer);
    fclose(file);
    
    return -1;
  }

  if(!flip(buffer))
  {
    printf("Could not decrypt firmware!\n");

    free(buffer);
    fclose(file);

    return -1;
  }

  fclose(file);

  if (arguments.offset && arguments.sample_number) 
  {
    set_offset(buffer,arguments.sample_number,arguments.offset);
    rom_dirty = 1;  
  }
  
  if (arguments.sample_number) 
  {
    if (arguments.sample_title) 
    {
      set_title(buffer, arguments.sample_number, arguments.sample_title, arguments.centre);
      rom_dirty = 1;
    } 
    else 
    {
      int off = 0;
      
      memcpy(msg, &buffer[TITLE_ADDR+(16*arguments.sample_number)],16);

      off = 256 * buffer[0x6bfb + (arguments.sample_number * 2)];
      off += buffer[0x6bfb + (1 + arguments.sample_number * 2)];
      off *= 16;

      printf("'%s' at %05x\n",msg, off);
    }
  }
  
  if (rom_dirty || arguments.flip) 
  {
    if (!arguments.flip) flip(buffer);
    
    if (!arguments.output_file)
      arguments.output_file = arguments.file;

    file = fopen(arguments.output_file,"wb");
    
    fwrite (buffer,1,FIRMWARE_SIZE,file);
    
    fclose(file);
  }
  
  free(buffer);

  return 0;
}

