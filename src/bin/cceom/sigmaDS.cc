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
    \ingroup CCEOM
    \brief Enter brief description of file here 
*/
#include <cstdio>
#include <libqt/qt.h>
#include "MOInfo.h"
#include "Params.h"
#include "Local.h"
#define EXTERN
#include "globals.h"

namespace psi { namespace cceom {

void WmaijDS(int i, int C_irr);
void WabejDS(int i, int C_irr);
void WbmfeDS(int i, int C_irr);
void WnmjeDS(int i, int C_irr);

/* This function computes the H-bar doubles-singles block contribution
to a Sigma vector stored at Sigma plus 'i' */

void sigmaDS(int i, int C_irr) {

#ifdef TIME_CCEOM
  timer_on("WmaijDS"); WmaijDS(i, C_irr); timer_off("WmaijDS");
  timer_on("WabejDS"); WabejDS(i, C_irr); timer_off("WabejDS");
  timer_on("WnmjeDS"); WnmjeDS(i, C_irr); timer_off("WnmjeDS");
  timer_on("WbmfeDS"); WbmfeDS(i, C_irr); timer_off("WbmfeDS");
#else
  WmaijDS(i, C_irr);
  WabejDS(i, C_irr);
  WnmjeDS(i, C_irr);
  WbmfeDS(i, C_irr);
#endif

  return;
}

}} // namespace psi::cceom
