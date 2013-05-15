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
    \ingroup CCENERGY
    \brief Enter brief description of file here 
*/
#include <cstdio>
#include <cstdlib>
#include <libdpd/dpd.h>
#include <libqt/qt.h>
#include "MOInfo.h"
#include "Params.h"
#define EXTERN
#include "globals.h"

namespace psi { namespace ccenergy {

void Fae_build(void)
{
  int h,a,e,nirreps;
  int ma,fe,ef,m,f,M,A,Gm,Ga,Ge,Gf,Gma,nrows,ncols;
  double *X;
  dpdfile2 tIA, tia;
  dpdfile2 FME, Fme;
  dpdfile2 fAB, fab, fIA, fia;
  dpdfile2 FAE, Fae;
  dpdfile2 FAEt, Faet;
  dpdbuf4 F_anti, F, D_anti, D;
  dpdbuf4 tautIJAB, tautijab, tautIjAb, taut;

  nirreps = moinfo.nirreps;

  if(params.ref == 0) { /** RHF **/
    dpd_file2_init(&fAB, PSIF_CC_OEI, 0, 1, 1, "fAB");
    dpd_file2_copy(&fAB, PSIF_CC_OEI, "FAE");
    dpd_file2_close(&fAB);
  }
  else if(params.ref == 1) { /** ROHF **/
    dpd_file2_init(&fAB, PSIF_CC_OEI, 0, 1, 1, "fAB");
    dpd_file2_copy(&fAB, PSIF_CC_OEI, "FAE");
    dpd_file2_close(&fAB);

    dpd_file2_init(&fab, PSIF_CC_OEI, 0, 1, 1, "fab");
    dpd_file2_copy(&fab, PSIF_CC_OEI, "Fae");
    dpd_file2_close(&fab);
  }
  else if(params.ref == 2) { /** UHF **/
    dpd_file2_init(&fAB, PSIF_CC_OEI, 0, 1, 1, "fAB");
    dpd_file2_copy(&fAB, PSIF_CC_OEI, "FAE");
    dpd_file2_close(&fAB);

    dpd_file2_init(&fab, PSIF_CC_OEI, 0, 3, 3, "fab");
    dpd_file2_copy(&fab, PSIF_CC_OEI, "Fae");
    dpd_file2_close(&fab);
  }

  if(params.ref == 0) { /** RHF **/
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
  
    dpd_file2_mat_init(&FAE);
    dpd_file2_mat_rd(&FAE);

    /*
      for(h=0; h < moinfo.nirreps; h++) {
      for(a=0; a < FAE.params->rowtot[h]; a++) 
      FAE.matrix[h][a][a] = 0;
      }
    */

    dpd_file2_mat_wrt(&FAE);
    dpd_file2_mat_close(&FAE);
    dpd_file2_close(&FAE);
  }
  else if(params.ref == 1) { /** ROHF **/
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_init(&Fae, PSIF_CC_OEI, 0, 1, 1, "Fae");
  
    dpd_file2_mat_init(&FAE);
    dpd_file2_mat_rd(&FAE);
    dpd_file2_mat_init(&Fae);
    dpd_file2_mat_rd(&Fae);
  
    for(h=0; h < moinfo.nirreps; h++) {
      
      for(a=0; a < FAE.params->rowtot[h]; a++) 
	for(e=0; e < FAE.params->coltot[h]; e++) 
	  FAE.matrix[h][a][e] *= (1 - (a==e));

      for(a=0; a < Fae.params->rowtot[h]; a++) 
	for(e=0; e < Fae.params->coltot[h]; e++)
	  Fae.matrix[h][a][e] *= (1 - (a==e));
      
    }

    dpd_file2_mat_wrt(&FAE);
    dpd_file2_mat_close(&FAE);
    dpd_file2_mat_wrt(&Fae);
    dpd_file2_mat_close(&Fae);

    dpd_file2_close(&FAE);
    dpd_file2_close(&Fae);
  }
  else if(params.ref == 2) { /** UHF **/
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_init(&Fae, PSIF_CC_OEI, 0, 3, 3, "Fae");
  
    dpd_file2_mat_init(&FAE);
    dpd_file2_mat_rd(&FAE);
    dpd_file2_mat_init(&Fae);
    dpd_file2_mat_rd(&Fae);
  
    for(h=0; h < moinfo.nirreps; h++) {
      
      for(a=0; a < FAE.params->rowtot[h]; a++) 
	for(e=0; e < FAE.params->coltot[h]; e++) 
	  FAE.matrix[h][a][e] *= (1 - (a==e));

      for(a=0; a < Fae.params->rowtot[h]; a++) 
	for(e=0; e < Fae.params->coltot[h]; e++)
	  Fae.matrix[h][a][e] *= (1 - (a==e));
      
    }

    dpd_file2_mat_wrt(&FAE);
    dpd_file2_mat_close(&FAE);
    dpd_file2_mat_wrt(&Fae);
    dpd_file2_mat_close(&Fae);

    dpd_file2_close(&FAE);
    dpd_file2_close(&Fae);
  }

  if(params.ref == 0) { /** RHF **/
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_init(&fIA, PSIF_CC_OEI, 0, 0, 1, "fIA");
    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_contract222(&tIA, &fIA, &FAE, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&fIA);
    dpd_file2_close(&FAE);

    /*
      dpd_file2_init(&FAE, CC_OEI, 0, 1, 1, "FAE");
      dpd_buf4_init(&F_anti, CC_FINTS, 0, 10, 5, 10, 5, 1, "F <ia|bc>");
      dpd_buf4_init(&F, CC_FINTS, 0, 10, 5, 10, 5, 0,"F <ia|bc>");
      dpd_file2_init(&tIA, CC_OEI, 0, 0, 1, "tIA");
      dpd_dot13(&tIA, &F_anti, &FAE, 0, 0, 1.0, 1.0);
      dpd_dot13(&tIA, &F, &FAE, 0, 0, 1.0, 1.0);
      dpd_file2_close(&tIA);
      dpd_buf4_close(&F_anti);
      dpd_buf4_close(&F);
      dpd_file2_close(&FAE);
    */

    /* Out-of-core algorithm for F->FAE added 3/20/05 - TDC */
    /* Fae <-- t(m,f) [2 <ma|fe> - <ma|ef>] */
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_mat_init(&FAE);
    dpd_file2_mat_rd(&FAE);
    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_mat_init(&tIA);
    dpd_file2_mat_rd(&tIA);
    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 10, 5, 10, 5, 0,"F <ia|bc>");
    for(Gma=0; Gma < nirreps; Gma++) {
      dpd_buf4_mat_irrep_row_init(&F, Gma);
      X = init_array(F.params->coltot[Gma]);

      for(ma=0; ma < F.params->rowtot[Gma]; ma++) {
	dpd_buf4_mat_irrep_row_rd(&F, Gma, ma);
	m = F.params->roworb[Gma][ma][0];
	a = F.params->roworb[Gma][ma][1];
	Gm = F.params->psym[m];
	Ga = Ge = Gm ^ Gma;  /* Fae is totally symmetric */
	Gf = Gm; /* T1 is totally symmetric */
	M = m - F.params->poff[Gm];
	A = a - F.params->qoff[Ga];

	zero_arr(X, F.params->coltot[Gma]);

	/* build spin-adapted F-integrals for current ma */
	for(fe=0; fe < F.params->coltot[Gma]; fe++) {
	  f = F.params->colorb[Gma][fe][0];
	  e = F.params->colorb[Gma][fe][1];
	  ef = F.params->colidx[e][f];
	  X[fe] = 2.0 * F.matrix[Gma][0][fe] - F.matrix[Gma][0][ef];
	}
	
	nrows = moinfo.virtpi[Gf];
	ncols = moinfo.virtpi[Ge];
	if(nrows && ncols)
	  C_DGEMV('t',nrows,ncols,1.0,&X[F.col_offset[Gma][Gf]],ncols,
		  tIA.matrix[Gm][M],1,1.0,
		  FAE.matrix[Ga][A],1);
      }

      free(X);
      dpd_buf4_mat_irrep_row_close(&F, Gma);
    }
    dpd_buf4_close(&F);
    dpd_file2_mat_close(&tIA);
    dpd_file2_close(&tIA);
    dpd_file2_mat_wrt(&FAE);
    dpd_file2_mat_close(&FAE);
    dpd_file2_close(&FAE);

    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 0, 5, 0, 5, 0, "D 2<ij|ab> - <ij|ba>");
    dpd_buf4_init(&tautIjAb, PSIF_CC_TAMPS, 0, 0, 5, 0, 5, 0, "tautIjAb");
    dpd_contract442(&tautIjAb, &D, &FAE, 3, 3, -1, 1);
    dpd_buf4_close(&D);
    dpd_buf4_close(&tautIjAb);

    /* Build the tilde intermediates */
    dpd_file2_copy(&FAE, PSIF_CC_OEI, "FAEt");
    dpd_file2_close(&FAE);

    dpd_file2_init(&FAEt, PSIF_CC_OEI, 0, 1, 1, "FAEt");

    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_init(&FME, PSIF_CC_OEI, 0, 0, 1, "FME");
    dpd_contract222(&tIA, &FME, &FAEt, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&FME);
  
    dpd_file2_close(&FAEt);
  }
  else if(params.ref == 1) { /** ROHF **/
    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_init(&Fae, PSIF_CC_OEI, 0, 1, 1, "Fae");

    dpd_file2_init(&fIA, PSIF_CC_OEI, 0, 0, 1, "fIA");
    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_contract222(&tIA, &fIA, &FAE, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&fIA);

    dpd_file2_init(&fia, PSIF_CC_OEI, 0, 0, 1, "fia");
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 0, 1, "tia");
    dpd_contract222(&tia, &fia, &Fae, 1, 1, -0.5, 1);
    dpd_file2_close(&tia);
    dpd_file2_close(&fia);

    dpd_buf4_init(&F_anti, PSIF_CC_FINTS, 0, 10, 5, 10, 5, 1, "F <ia|bc>");
    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 10, 5, 10, 5, 0,"F <ia|bc>");
    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 0, 1, "tia");

    dpd_dot13(&tIA, &F_anti, &FAE, 0, 0, 1.0, 1.0);
    dpd_dot13(&tia, &F, &FAE, 0, 0, 1.0, 1.0);

    dpd_dot13(&tia, &F_anti, &Fae, 0, 0, 1.0, 1.0);
    dpd_dot13(&tIA, &F, &Fae, 0, 0, 1.0, 1.0);

    dpd_file2_close(&tIA);
    dpd_file2_close(&tia);
    dpd_buf4_close(&F_anti);
    dpd_buf4_close(&F);

    dpd_buf4_init(&D_anti, PSIF_CC_DINTS, 0, 2, 5, 2, 5, 0, "D <ij||ab> (i>j,ab)");

    dpd_buf4_init(&tautIJAB, PSIF_CC_TAMPS, 0, 2, 5, 2, 7, 0, "tautIJAB");
    dpd_buf4_init(&tautijab, PSIF_CC_TAMPS, 0, 2, 5, 2, 7, 0, "tautijab");

    dpd_contract442(&tautIJAB, &D_anti, &FAE, 2, 2, -1, 1);
    dpd_contract442(&tautijab, &D_anti, &Fae, 2, 2, -1, 1);

    dpd_buf4_close(&D_anti);
    dpd_buf4_close(&tautIJAB);
    dpd_buf4_close(&tautijab);

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 0, 5, 0, 5, 0, "D <ij|ab>");
    dpd_buf4_init(&tautIjAb, PSIF_CC_TAMPS, 0, 0, 5, 0, 5, 0, "tautIjAb");

    dpd_contract442(&tautIjAb, &D, &Fae, 3, 3, -1, 1);
    dpd_contract442(&tautIjAb, &D, &FAE, 2, 2, -1, 1);

    dpd_buf4_close(&D);
    dpd_buf4_close(&tautIjAb);


    /* Build the tilde intermediates */
    dpd_file2_copy(&FAE, PSIF_CC_OEI, "FAEt");
    dpd_file2_copy(&Fae, PSIF_CC_OEI, "Faet");

    dpd_file2_close(&FAE);
    dpd_file2_close(&Fae);

    dpd_file2_init(&FAEt, PSIF_CC_OEI, 0, 1, 1, "FAEt");
    dpd_file2_init(&Faet, PSIF_CC_OEI, 0, 1, 1, "Faet");

    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_init(&FME, PSIF_CC_OEI, 0, 0, 1, "FME");
    dpd_contract222(&tIA, &FME, &FAEt, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&FME);
  
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 0, 1, "tia");
    dpd_file2_init(&Fme, PSIF_CC_OEI, 0, 0, 1, "Fme");
    dpd_contract222(&tia, &Fme, &Faet, 1, 1, -0.5, 1);
    dpd_file2_close(&tia);
    dpd_file2_close(&Fme);

    dpd_file2_close(&FAEt);
    dpd_file2_close(&Faet); 
  }
  else if(params.ref == 2) { /** UHF **/

    dpd_file2_init(&FAE, PSIF_CC_OEI, 0, 1, 1, "FAE");
    dpd_file2_init(&Fae, PSIF_CC_OEI, 0, 3, 3, "Fae");

    dpd_file2_init(&fIA, PSIF_CC_OEI, 0, 0, 1, "fIA");
    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_contract222(&tIA, &fIA, &FAE, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&fIA);

    dpd_file2_init(&fia, PSIF_CC_OEI, 0, 2, 3, "fia");
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 2, 3, "tia");
    dpd_contract222(&tia, &fia, &Fae, 1, 1, -0.5, 1);
    dpd_file2_close(&tia);
    dpd_file2_close(&fia);

    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 2, 3, "tia");

    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 20, 5, 20, 5, 1, "F <IA|BC>");
    dpd_dot13(&tIA, &F, &FAE, 0, 0, 1, 1);
    dpd_buf4_close(&F);

    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 27, 29, 27, 29, 0, "F <iA|bC>");
    dpd_dot13(&tia, &F, &FAE, 0, 0, 1, 1);
    dpd_buf4_close(&F);

    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 30, 15, 30, 15, 1, "F <ia|bc>");
    dpd_dot13(&tia, &F, &Fae, 0, 0, 1, 1);
    dpd_buf4_close(&F);

    dpd_buf4_init(&F, PSIF_CC_FINTS, 0, 24, 28, 24, 28, 0, "F <Ia|Bc>");
    dpd_dot13(&tIA, &F, &Fae, 0, 0, 1, 1);
    dpd_buf4_close(&F);

    dpd_file2_close(&tIA);
    dpd_file2_close(&tia);

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 2, 5, 2, 5, 0, "D <IJ||AB> (I>J,AB)");
    dpd_buf4_init(&taut, PSIF_CC_TAMPS, 0, 2, 5, 2, 7, 0, "tautIJAB");
    dpd_contract442(&taut, &D, &FAE, 2, 2, -1, 1);
    dpd_buf4_close(&taut);
    dpd_buf4_close(&D);

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 22, 28, 22, 28, 0, "D <Ij|Ab>");
    dpd_buf4_init(&taut, PSIF_CC_TAMPS, 0, 22, 28, 22, 28, 0, "tautIjAb");
    dpd_contract442(&taut, &D, &FAE, 2, 2, -1, 1);
    dpd_buf4_close(&taut);
    dpd_buf4_close(&D);

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 12, 15, 12, 15, 0, "D <ij||ab> (i>j,ab)");
    dpd_buf4_init(&taut, PSIF_CC_TAMPS, 0, 12, 15, 12, 17, 0, "tautijab");
    dpd_contract442(&taut, &D, &Fae, 2, 2, -1, 1);
    dpd_buf4_close(&taut);
    dpd_buf4_close(&D);

    dpd_buf4_init(&D, PSIF_CC_DINTS, 0, 22, 28, 22, 28, 0, "D <Ij|Ab>");
    dpd_buf4_init(&taut, PSIF_CC_TAMPS, 0, 22, 28, 22, 28, 0, "tautIjAb");
    dpd_contract442(&taut, &D, &Fae, 3, 3, -1, 1);
    dpd_buf4_close(&taut);
    dpd_buf4_close(&D);

    /* Build the tilde intermediates */
    dpd_file2_copy(&FAE, PSIF_CC_OEI, "FAEt");
    dpd_file2_copy(&Fae, PSIF_CC_OEI, "Faet");

    dpd_file2_close(&FAE);
    dpd_file2_close(&Fae);

    dpd_file2_init(&FAEt, PSIF_CC_OEI, 0, 1, 1, "FAEt");
    dpd_file2_init(&Faet, PSIF_CC_OEI, 0, 3, 3, "Faet");

    dpd_file2_init(&tIA, PSIF_CC_OEI, 0, 0, 1, "tIA");
    dpd_file2_init(&FME, PSIF_CC_OEI, 0, 0, 1, "FME");
    dpd_contract222(&tIA, &FME, &FAEt, 1, 1, -0.5, 1);
    dpd_file2_close(&tIA);
    dpd_file2_close(&FME);
  
    dpd_file2_init(&tia, PSIF_CC_OEI, 0, 2, 3, "tia");
    dpd_file2_init(&Fme, PSIF_CC_OEI, 0, 2, 3, "Fme");
    dpd_contract222(&tia, &Fme, &Faet, 1, 1, -0.5, 1);
    dpd_file2_close(&tia);
    dpd_file2_close(&Fme);


    dpd_file2_close(&FAEt);
    dpd_file2_close(&Faet); 
  }
}
}} // namespace psi::ccenergy
