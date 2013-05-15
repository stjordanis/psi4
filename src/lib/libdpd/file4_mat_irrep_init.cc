/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
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
 *@END LICENSE
 */

/*! \file
    \ingroup DPD
    \brief Enter brief description of file here 
*/
#include <cstdio>
#include <libciomr/libciomr.h>
#include "dpd.h"
#define EXTERN
#include "dpd.gbl"

namespace psi {

/* dpd_file4_mat_irrep_init(): Allocates and initializes memory for a
** matrix for a single irrep of a dpd four-index file.
**
** Arguments:
**   dpdfile4 *File: A pointer to the input dpdfile.
**   int irrep: The irrep number to be prepared.
*/

int dpd_file4_mat_irrep_init(dpdfile4 *File, int irrep)
{
  int my_irrep, rowtot, coltot;
  long int size;

  my_irrep = File->my_irrep;
  rowtot = File->params->rowtot[irrep];
  coltot = File->params->coltot[irrep^my_irrep];
  size = ((long) rowtot) * ((long) coltot);

  if(File->incore) return 0;  /* We've already got the memory */

  if(size) File->matrix[irrep] = dpd_block_matrix(rowtot,coltot);

  return 0;
}

}
