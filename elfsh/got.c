/*
** got.c for elfsh
** 
** Started on  Fri Nov  2 15:20:18 2001 mayhem
**
*/
#include "elfsh.h"



/* Command handler for GOT command */
int		cmd_got()
{
  regex_t	*tmp;
  elfshsect_t	*got;
  int		size;
  int		index;
  int		index2;
  elfsh_SAddr	offset;
  char		*name;
  char		off[50];
  char		buff[256];
  char		logbuf[BUFSIZ];
  void		*data;

  ELFSH_PROFILE_IN(__FILE__, __FUNCTION__, __LINE__);

  /* Get the regx and fetch the GOT data */
  FIRSTREGX(tmp);

  index2 = 0;
  if ((got = elfsh_get_got_by_idx(world.curjob->current, index2, &size)) == NULL)
    RET(-1);
  
  /* Loop on all .got */
  for (index2 = 0; got; index2++)
    {
  
      data = elfsh_get_raw(got);
    
      snprintf(logbuf, BUFSIZ - 1, " [Global Offset Table .::. GOT : %s ]\n [Object %s]\n\n", 
	       elfsh_get_section_name(world.curjob->current, got),
	       world.curjob->current->name);
      vm_output(logbuf);
      off[0] = 0;

      /* Loop and print all entry and their resolve */
      for (index = 0; index < size; index++)
	{

	  name = vm_resolve(world.curjob->current, *((elfsh_Addr *) data + index), &offset);

	  if (off != NULL)
	    snprintf(off, sizeof(off), " %s %s", 
		     vm_colorstr((offset < 0 ? "-" : "+")), 
		     vm_colornumber("%u", (u_int) (offset > 0 ? offset : offset - offset - offset)));


	  snprintf(buff, sizeof(buff), "%s : %s %s \t <%s%s>\n", 
		   vm_coloraddress(XFMT, got->shdr->sh_addr + (index * sizeof(elfsh_Addr))),
		   vm_colornumber("[%02u]", index), 
		   vm_coloraddress(XFMT, (elfsh_Addr) ((elfsh_Addr *) data)[index]), 
		   (name != NULL ? vm_colorstr(name) : vm_colorwarn("?")), 
		   (off[0] && name && offset ? off : ""));

	  if (!tmp || (tmp && !regexec(tmp, buff, 0, 0, 0)))
	    vm_output(buff);

	  vm_endline();
	}
      
      got = elfsh_get_got_by_idx(world.curjob->current, index2 + 1, &size);
      vm_output("\n");
    }

  ELFSH_PROFILE_ROUT(__FILE__, __FUNCTION__, __LINE__, 0); 
}

