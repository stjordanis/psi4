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

#include <libqt/qt.h>
#include <libtrans/integraltransform.h>
#include <libiwl/iwl.hpp>
#include <psifiles.h>

#include "occwave.h"
#include "defines.h"

using namespace boost;
using namespace psi;
using namespace std;

namespace psi{ namespace occwave{

void OCCWave::trans_ints_uhf()
{    
    //fprintf(outfile,"\n trans_ints is starting... \n"); fflush(outfile);
/********************************************************************************************/
/************************** Transform 2-electron int. to MO space ***************************/
/********************************************************************************************/  
    ints->update_orbitals();  
    ints->set_print(print_ - 2 >= 0 ? print_ - 2 : 0);
    ints->set_keep_dpd_so_ints(1);
    
    // Trans (OO|OO)
    timer_on("Trans (OO|OO)");
    ints->transform_tei(MOSpace::occ, MOSpace::occ, MOSpace::occ, MOSpace::occ, IntegralTransform::MakeAndKeep);
    timer_off("Trans (OO|OO)");
    
    // Trans (OO|OV)
    timer_on("Trans (OO|OV)");
    ints->transform_tei(MOSpace::occ, MOSpace::occ, MOSpace::occ, MOSpace::vir, IntegralTransform::ReadAndKeep);
    timer_off("Trans (OO|OV)");
    
    // Trans (OO|VV)
    timer_on("Trans (OO|VV)");
    ints->transform_tei(MOSpace::occ, MOSpace::occ, MOSpace::vir, MOSpace::vir, IntegralTransform::ReadAndNuke);
    timer_off("Trans (OO|VV)");

    // Trans (OV|OO)
    timer_on("Trans (OV|OO)");
    ints->transform_tei(MOSpace::occ, MOSpace::vir, MOSpace::occ, MOSpace::occ, IntegralTransform::MakeAndKeep);
    timer_off("Trans (OV|OO)");

    // Trans (OV|OV)
    timer_on("Trans (OV|OV)");
    ints->transform_tei(MOSpace::occ, MOSpace::vir, MOSpace::occ, MOSpace::vir, IntegralTransform::ReadAndKeep);
    timer_off("Trans (OV|OV)");
    
    // Trans (OV|VV)
    timer_on("Trans (OV|VV)");
    ints->transform_tei(MOSpace::occ, MOSpace::vir, MOSpace::vir, MOSpace::vir, IntegralTransform::ReadAndNuke);
    timer_off("Trans (OV|VV)");

    // Trans (VV|OO)
    timer_on("Trans (VV|OO)");
    ints->transform_tei(MOSpace::vir, MOSpace::vir, MOSpace::occ, MOSpace::occ, IntegralTransform::MakeAndKeep);
    timer_off("Trans (VV|OO)");
    

    // Trans (VV|OV)
    timer_on("Trans (VV|OV)");
    if (wfn_type_ == "OMP2") { 
        ints->transform_tei(MOSpace::vir, MOSpace::vir, MOSpace::occ, MOSpace::vir, IntegralTransform::ReadAndNuke);
    } 
    else ints->transform_tei(MOSpace::vir, MOSpace::vir, MOSpace::occ, MOSpace::vir, IntegralTransform::ReadAndKeep);
    timer_off("Trans (VV|OV)");
    
//if (wfn_type_ == "OMP3" || wfn_type_ == "OCEPA" || wfn_type_ == "CEPA") { 
if (wfn_type_ != "OMP2") { 
    // Trans (VV|VV)
    timer_on("Trans (VV|VV)");
    ints->transform_tei(MOSpace::vir, MOSpace::vir, MOSpace::vir, MOSpace::vir, IntegralTransform::ReadAndNuke);
    timer_off("Trans (VV|VV)");
} 
    
/********************************************************************************************/
/************************** sort chem -> phys ***********************************************/
/********************************************************************************************/  
     dpdbuf4 K, G;
     
     psio_->open(PSIF_LIBTRANS_DPD, PSIO_OPEN_OLD);
     
     // Build MO ints    
     timer_on("Sort chem -> phys");
     
     // (OO|OO) -> <OO|OO>
     timer_on("Sort (OO|OO) -> <OO|OO>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,O]"),
                  ID("[O>=O]+"), ID("[O>=O]+"), 0, "MO Ints (OO|OO)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,O]"), ID("[O,O]"), "MO Ints <OO|OO>");
     dpd_buf4_close(&K);
     
     // (oo|oo) -> <oo|oo>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,o]"),
                  ID("[o>=o]+"), ID("[o>=o]+"), 0, "MO Ints (oo|oo)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[o,o]"), ID("[o,o]"), "MO Ints <oo|oo>");
     dpd_buf4_close(&K);
     
     // (OO|oo) -> <Oo|Oo>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[o,o]"),
                  ID("[O>=O]+"), ID("[o>=o]+"), 0, "MO Ints (OO|oo)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,o]"), ID("[O,o]"), "MO Ints <Oo|Oo>");
     dpd_buf4_close(&K);
     timer_off("Sort (OO|OO) -> <OO|OO>");
     
 
  
     // (OO|OV) -> <OO|OV>
     timer_on("Sort (OO|OV) -> <OO|OV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,V]"),
                  ID("[O>=O]+"), ID("[O,V]"), 0, "MO Ints (OO|OV)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,O]"), ID("[O,V]"), "MO Ints <OO|OV>");
     dpd_buf4_close(&K);
     
     // (oo|ov) -> <oo|ov>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,v]"),
                  ID("[o>=o]+"), ID("[o,v]"), 0, "MO Ints (oo|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[o,o]"), ID("[o,v]"), "MO Ints <oo|ov>");
     dpd_buf4_close(&K);
     
     // (OO|ov) -> <Oo|Ov>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[o,v]"),
                  ID("[O>=O]+"), ID("[o,v]"), 0, "MO Ints (OO|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,o]"), ID("[O,v]"), "MO Ints <Oo|Ov>");
     dpd_buf4_close(&K);
     
     // (OV|oo) -> <Oo|Vo>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[o,o]"),
                  ID("[O,V]"), ID("[o>=o]+"), 0, "MO Ints (OV|oo)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,o]"), ID("[V,o]"), "MO Ints <Oo|Vo>");
     dpd_buf4_close(&K);
     timer_off("Sort (OO|OV) -> <OO|OV>");
     
     
     
     // (OV|OV) -> <OO|VV>
     timer_on("Sort (OV|OV) -> <OO|VV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints (OV|OV)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,O]"), ID("[V,V]"), "MO Ints <OO|VV>");
     dpd_buf4_close(&K);
     
     // (ov|ov) -> <oo|vv>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints (ov|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[o,o]"), ID("[v,v]"), "MO Ints <oo|vv>");
     dpd_buf4_close(&K);
     
     // (OV|ov) -> <Oo|Vv>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[o,v]"),
                  ID("[O,V]"), ID("[o,v]"), 0, "MO Ints (OV|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,o]"), ID("[V,v]"), "MO Ints <Oo|Vv>");
     dpd_buf4_close(&K);
     
     // (OV|ov) -> <Ov|Vo>: <Ia||Bj> = <Ia|Bj> = (IB|ja)
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[o,v]"),
                  ID("[O,V]"), ID("[o,v]"), 0, "MO Ints (OV|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , psqr, ID("[O,v]"), ID("[V,o]"), "MO Ints <Ov|Vo>");
     dpd_buf4_close(&K);
     timer_off("Sort (OV|OV) -> <OO|VV>");
     
    
     // (OO|VV) -> <OV|OV>
     timer_on("Sort (OO|VV) -> <OV|OV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[V,V]"),
                  ID("[O>=O]+"), ID("[V>=V]+"), 0, "MO Ints (OO|VV)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,V]"), ID("[O,V]"), "MO Ints <OV|OV>");
     dpd_buf4_close(&K);
     
     // (oo|vv) -> <ov|ov>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[v,v]"),
                  ID("[o>=o]+"), ID("[v>=v]+"), 0, "MO Ints (oo|vv)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[o,v]"), ID("[o,v]"), "MO Ints <ov|ov>");
     dpd_buf4_close(&K);
     
     // (OO|vv) -> <Ov|Ov>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[v,v]"),
                  ID("[O>=O]+"), ID("[v>=v]+"), 0, "MO Ints (OO|vv)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,v]"), ID("[O,v]"), "MO Ints <Ov|Ov>");
     dpd_buf4_close(&K);
     
     // (VV|oo) -> <Vo|Vo>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[o,o]"),
                  ID("[V>=V]+"), ID("[o>=o]+"), 0, "MO Ints (VV|oo)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[V,o]"), ID("[V,o]"), "MO Ints <Vo|Vo>");
     dpd_buf4_close(&K);
     timer_off("Sort (OO|VV) -> <OV|OV>");
     
     
     
     
     // (OV|VV) -> <OV|VV>
     timer_on("Sort (OV|VV) -> <OV|VV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[V,V]"),
                  ID("[O,V]"), ID("[V>=V]+"), 0, "MO Ints (OV|VV)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,V]"), ID("[V,V]"), "MO Ints <OV|VV>");
     dpd_buf4_close(&K);
     
     // (ov|vv) -> <ov|vv>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[v,v]"),
                  ID("[o,v]"), ID("[v>=v]+"), 0, "MO Ints (ov|vv)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[o,v]"), ID("[v,v]"), "MO Ints <ov|vv>");
     dpd_buf4_close(&K);
     
     // (OV|vv) -> <Ov|Vv>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[v,v]"),
                  ID("[O,V]"), ID("[v>=v]+"), 0, "MO Ints (OV|vv)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[O,v]"), ID("[V,v]"), "MO Ints <Ov|Vv>");
     dpd_buf4_close(&K);
     
     // (VV|ov) -> <Vo|Vv>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[o,v]"),
                  ID("[V>=V]+"), ID("[o,v]"), 0, "MO Ints (VV|ov)");
     dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[V,o]"), ID("[V,v]"), "MO Ints <Vo|Vv>");
     dpd_buf4_close(&K);
     timer_off("Sort (OV|VV) -> <OV|VV>");
     
       
    if (wfn_type_ == "OMP2") timer_off("Sort chem -> phys");
    else {
      // (VV|VV) -> <VV|VV>
      timer_on("Sort (VV|VV) -> <VV|VV>");
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[V,V]"),
                  ID("[V>=V]+"), ID("[V>=V]+"), 0, "MO Ints (VV|VV)");
      dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[V,V]"), ID("[V,V]"), "MO Ints <VV|VV>");
      dpd_buf4_close(&K);
      
      // (vv|vv) -> <vv|vv>
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[v,v]"), ID("[v,v]"),
                  ID("[v>=v]+"), ID("[v>=v]+"), 0, "MO Ints (vv|vv)");
      dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[v,v]"), ID("[v,v]"), "MO Ints <vv|vv>");
      dpd_buf4_close(&K);
      
      // (VV|vv) -> <Vv|Vv>
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[v,v]"),
                  ID("[V>=V]+"), ID("[v>=v]+"), 0, "MO Ints (VV|vv)");
      dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , prqs, ID("[V,v]"), ID("[V,v]"), "MO Ints <Vv|Vv>");
      dpd_buf4_close(&K);
      timer_off("Sort (VV|VV) -> <VV|VV>");
      timer_off("Sort chem -> phys");
    }
     
/********************************************************************************************/
/************************** Antisymmetrized Ints ********************************************/
/********************************************************************************************/
      timer_on("Antisymmetrize integrals");
     // <OO||OO>:  <IJ||KL> =  <IJ|KL> - <IJ|LK> 
     timer_on("Make <OO||OO>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,O]"),
                  ID("[O,O]"), ID("[O,O]"), 0, "MO Ints <OO|OO>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OO||OO>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,O]"),
                  ID("[O,O]"), ID("[O,O]"), 0, "MO Ints <OO||OO>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,O]"),
                  ID("[O,O]"), ID("[O,O]"), 0, "MO Ints <OO|OO>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            for(int kl = 0; kl < K.params->coltot[h]; ++kl){
                int k = K.params->colorb[h][kl][0];
                int l = K.params->colorb[h][kl][1];
		int lk = G.params->colidx[l][k];
                K.matrix[h][ij][kl] -= G.matrix[h][ij][lk];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G);

 
     // <oo||oo>:  <ij||kl> =  <ij|kl> - <ij|lk> 
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,o]"),
                  ID("[o,o]"), ID("[o,o]"), 0, "MO Ints <oo|oo>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <oo||oo>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,o]"),
                  ID("[o,o]"), ID("[o,o]"), 0, "MO Ints <oo||oo>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,o]"),
                  ID("[o,o]"), ID("[o,o]"), 0, "MO Ints <oo|oo>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            for(int kl = 0; kl < K.params->coltot[h]; ++kl){
                int k = K.params->colorb[h][kl][0];
                int l = K.params->colorb[h][kl][1];
		int lk = G.params->colidx[l][k];
                K.matrix[h][ij][kl] -= G.matrix[h][ij][lk];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G); 
     timer_off("Make <OO||OO>");
   

     // <OO||OV>:  <IJ||KA> = <IJ|KA> - <JI|KA>
     timer_on("Make <OO||OV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,V]"),
                  ID("[O,O]"), ID("[O,V]"), 0, "MO Ints <OO|OV>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OO||OV>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,V]"),
                  ID("[O,O]"), ID("[O,V]"), 0, "MO Ints <OO||OV>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[O,V]"),
                  ID("[O,O]"), ID("[O,V]"), 0, "MO Ints <OO|OV>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            int i = K.params->roworb[h][ij][0];
            int j = K.params->roworb[h][ij][1];
            int ji = G.params->rowidx[j][i];
            for(int ka = 0; ka < K.params->coltot[h]; ++ka){ 
                K.matrix[h][ij][ka] -= G.matrix[h][ji][ka];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G);
    

    
     // <oo||ov>:   <ij||ka> = <ij|ka> - <ji|ka>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,v]"),
                  ID("[o,o]"), ID("[o,v]"), 0, "MO Ints <oo|ov>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <oo||ov>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,v]"),
                  ID("[o,o]"), ID("[o,v]"), 0, "MO Ints <oo||ov>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[o,v]"),
                  ID("[o,o]"), ID("[o,v]"), 0, "MO Ints <oo|ov>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            int i = K.params->roworb[h][ij][0];
            int j = K.params->roworb[h][ij][1];
            int ji = G.params->rowidx[j][i];
            for(int ka = 0; ka < K.params->coltot[h]; ++ka){ 
                K.matrix[h][ij][ka] -= G.matrix[h][ji][ka];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G);
     timer_off("Make <OO||OV>");


    
     // <OO||VV>:  <IJ||AB> = <IJ|AB> - <IJ|BA> 
     timer_on("Make <OO||VV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[V,V]"),
                  ID("[O,O]"), ID("[V,V]"), 0, "MO Ints <OO|VV>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OO||VV>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[V,V]"),
                  ID("[O,O]"), ID("[V,V]"), 0, "MO Ints <OO||VV>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[V,V]"),
                  ID("[O,O]"), ID("[V,V]"), 0, "MO Ints <OO|VV>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
	dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            for(int ab = 0; ab < K.params->coltot[h]; ++ab){
                int a = K.params->colorb[h][ab][0];
                int b = K.params->colorb[h][ab][1];
		int ba = G.params->colidx[b][a];
		K.matrix[h][ij][ab] -= G.matrix[h][ij][ba];                
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);

     
     // <oo||vv>:  <ij||ab> = <ij|ab> - <ij|ba> 
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[v,v]"),
                  ID("[o,o]"), ID("[v,v]"), 0, "MO Ints <oo|vv>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <oo||vv>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[v,v]"),
                  ID("[o,o]"), ID("[v,v]"), 0, "MO Ints <oo||vv>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[v,v]"),
                  ID("[o,o]"), ID("[v,v]"), 0, "MO Ints <oo|vv>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
	dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ij = 0; ij < K.params->rowtot[h]; ++ij){
            for(int ab = 0; ab < K.params->coltot[h]; ++ab){
                int a = K.params->colorb[h][ab][0];
                int b = K.params->colorb[h][ab][1];
		int ba = G.params->colidx[b][a];
		K.matrix[h][ij][ab] -= G.matrix[h][ij][ba];                
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
    timer_off("Make <OO||VV>");

    
      // <OV||OV>:  <IA||JB> = <IB|JA> - (IB|JA)
     timer_on("Make <OV||OV>");
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints <OV|OV>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OV|OV> - (OV|OV)");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints <OV|OV> - (OV|OV)");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints (OV|OV)");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int row = 0; row < K.params->rowtot[h]; ++row){
            for(int col = 0; col < K.params->coltot[h]; ++col){
                K.matrix[h][row][col] -= G.matrix[h][row][col];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
    
    // <IB||JA> => <IA||JB>
    dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints <OV|OV> - (OV|OV)");
    dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , psrq, ID("[O,V]"), ID("[O,V]"), "MO Ints <OV||OV>");
    dpd_buf4_close(&K);
     
     // <ov||ov>:  <ia||jb> = <ib|ja> - (ib|ja)
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints <ov|ov>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <ov|ov> - (ov|ov)");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints <ov|ov> - (ov|ov)");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints (ov|ov)");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int row = 0; row < K.params->rowtot[h]; ++row){
            for(int col = 0; col < K.params->coltot[h]; ++col){
                K.matrix[h][row][col] -= G.matrix[h][row][col];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
    
    // <ib||ja> => <ia||jb>
    dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints <ov|ov> - (ov|ov)");
    dpd_buf4_sort(&K, PSIF_LIBTRANS_DPD , psrq, ID("[o,v]"), ID("[o,v]"), "MO Ints <ov||ov>");
    dpd_buf4_close(&K);
    timer_off("Make <OV||OV>");
    

    /* 
     // <OV||OV>:  <IA||JB> = <IA|JB> - (IB|JA)
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints <OV|OV>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OV||OV>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints <OV||OV>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[O,V]"),
                  ID("[O,V]"), ID("[O,V]"), 0, "MO Ints (OV|OV)");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        for(int ia = 0; ia < K.params->rowtot[h]; ++ia){
            int i = K.params->roworb[h][ia][0];
            int a = K.params->roworb[h][ia][1];
            int Gi = K.params->psym[i];
            for(int jb = 0; jb < K.params->coltot[h]; ++jb){
                int j = K.params->colorb[h][jb][0];
                int b = K.params->colorb[h][jb][1];
                int Gb = K.params->ssym[b];
                int Gib = Gi^Gb;
	        dpd_buf4_mat_irrep_init(&G, Gib);
                dpd_buf4_mat_irrep_rd(&G, Gib);
		int ib = G.params->rowidx[i][b];
		int ja = G.params->colidx[j][a];
		K.matrix[h][ia][jb] -= G.matrix[Gib][ib][ja];                
                dpd_buf4_mat_irrep_close(&G, Gib);
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);


     // <ov||ov>:  <ia||jb> = <ia|jb> - (ib|ja)
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints <ov|ov>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <ov||ov>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints <ov||ov>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[o,v]"),
                  ID("[o,v]"), ID("[o,v]"), 0, "MO Ints (ov|ov)");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        for(int ia = 0; ia < K.params->rowtot[h]; ++ia){
            int i = K.params->roworb[h][ia][0];
            int a = K.params->roworb[h][ia][1];
            int Gi = K.params->psym[i];
            for(int jb = 0; jb < K.params->coltot[h]; ++jb){
                int j = K.params->colorb[h][jb][0];
                int b = K.params->colorb[h][jb][1];
                int Gb = K.params->ssym[b];
                int Gib = Gi^Gb;
	        dpd_buf4_mat_irrep_init(&G, Gib);
                dpd_buf4_mat_irrep_rd(&G, Gib);
		int ib = G.params->rowidx[i][b];
		int ja = G.params->colidx[j][a];
		K.matrix[h][ia][jb] -= G.matrix[Gib][ib][ja];                
                dpd_buf4_mat_irrep_close(&G, Gib);
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
    */

     /*
     // <OV||VV>:  <IA||BC> = <IA|BC> - <IA|CB> 
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[V,V]"),
                  ID("[O,V]"), ID("[V,V]"), 0, "MO Ints <OV|VV>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <OV||VV>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[V,V]"),
                  ID("[O,V]"), ID("[V,V]"), 0, "MO Ints <OV||VV>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[O,V]"), ID("[V,V]"),
                  ID("[O,V]"), ID("[V,V]"), 0, "MO Ints <OV|VV>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
	dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ia = 0; ia < K.params->rowtot[h]; ++ia){
            for(int bc = 0; bc < K.params->coltot[h]; ++bc){
                int b = K.params->colorb[h][bc][0];
                int c = K.params->colorb[h][bc][1];
		int cb = G.params->colidx[c][b];
		K.matrix[h][ia][bc] -= G.matrix[h][ia][cb];                
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
    

     // <ov||vv>:  <ia||bc> = <ia|bc> - <ia|cb>
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[v,v]"),
                  ID("[o,v]"), ID("[v,v]"), 0, "MO Ints <ov|vv>");
     dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <ov||vv>");
     dpd_buf4_close(&K);
     dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[v,v]"),
                  ID("[o,v]"), ID("[v,v]"), 0, "MO Ints <ov||vv>");
     dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[o,v]"), ID("[v,v]"),
                  ID("[o,v]"), ID("[v,v]"), 0, "MO Ints <ov|vv>");
     for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
	dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ia = 0; ia < K.params->rowtot[h]; ++ia){
            for(int bc = 0; bc < K.params->coltot[h]; ++bc){
                int b = K.params->colorb[h][bc][0];
                int c = K.params->colorb[h][bc][1];
		int cb = G.params->colidx[c][b];
		K.matrix[h][ia][bc] -= G.matrix[h][ia][cb];                
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&G);
           
      // <VV||VV>: <AB||CD> = <AB|CD> - <AB|DC>
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[V,V]"),
                  ID("[V,V]"), ID("[V,V]"), 0, "MO Ints <VV|VV>");
      dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <VV||VV>");
      dpd_buf4_close(&K);
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[V,V]"),
                  ID("[V,V]"), ID("[V,V]"), 0, "MO Ints <VV||VV>");
      dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[V,V]"), ID("[V,V]"),
                  ID("[V,V]"), ID("[V,V]"), 0, "MO Ints <VV|VV>");
      for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ab = 0; ab < K.params->rowtot[h]; ++ab){
            for(int cd = 0; cd < K.params->coltot[h]; ++cd){
                int c = K.params->colorb[h][cd][0];
                int d = K.params->colorb[h][cd][1];
		int dc = G.params->colidx[d][c];
                K.matrix[h][ab][cd] -= G.matrix[h][ab][dc];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G);



      // <vv||vv>: <ab||cd> = <ab|cd> - <ab|dc>
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[v,v]"), ID("[v,v]"),
                  ID("[v,v]"), ID("[v,v]"), 0, "MO Ints <vv|vv>");
      dpd_buf4_copy(&K, PSIF_LIBTRANS_DPD, "MO Ints <vv||vv>");
      dpd_buf4_close(&K);
      dpd_buf4_init(&K, PSIF_LIBTRANS_DPD, 0, ID("[v,v]"), ID("[v,v]"),
                  ID("[v,v]"), ID("[v,v]"), 0, "MO Ints <vv||vv>");  
      dpd_buf4_init(&G, PSIF_LIBTRANS_DPD, 0, ID("[v,v]"), ID("[v,v]"),
                  ID("[v,v]"), ID("[v,v]"), 0, "MO Ints <vv|vv>");
      for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&K, h);
        dpd_buf4_mat_irrep_init(&G, h);
        dpd_buf4_mat_irrep_rd(&K, h);
        dpd_buf4_mat_irrep_rd(&G, h);
        for(int ab = 0; ab < K.params->rowtot[h]; ++ab){
            for(int cd = 0; cd < K.params->coltot[h]; ++cd){
                int c = K.params->colorb[h][cd][0];
                int d = K.params->colorb[h][cd][1];
		int dc = G.params->colidx[d][c];
                K.matrix[h][ab][cd] -= G.matrix[h][ab][dc];
            }
        }
        dpd_buf4_mat_irrep_wrt(&K, h);
        dpd_buf4_mat_irrep_close(&K, h);
        dpd_buf4_mat_irrep_close(&G, h);
     }
     dpd_buf4_close(&K);
     dpd_buf4_close(&G);
     */
     timer_off("Antisymmetrize integrals");


/********************************************************************************************/
/************************** Transform 1-electron int. to MO space ***************************/
/********************************************************************************************/        
      // Trans H matrix
      timer_on("Trans OEI");
      HmoA->copy(Hso);
      HmoB->copy(Hso);
      HmoA->transform(Ca_);
      HmoB->transform(Cb_);
      timer_off("Trans OEI");
      
      if (print_ > 1) {
	HmoA->print();
	HmoB->print();
      }
      
      // Trans Fock matrix    
      if (orb_opt_ == "TRUE") {
      timer_on("Build Fock");
      fock_alpha();      
      fock_beta();   
      timer_off("Build Fock");
      }

      else if (orb_opt_ == "FALSE" || reference == "ROHF") {
	double *mo_ints = init_array(ntri);
        IWL::read_one(psio_.get(), PSIF_OEI, PSIF_MO_A_FOCK, mo_ints, ntri, 0, 0, outfile);
        FockA->set(mo_ints);
        IWL::read_one(psio_.get(), PSIF_OEI, PSIF_MO_B_FOCK, mo_ints, ntri, 0, 0, outfile);
        FockB->set(mo_ints);
        free(mo_ints);
      }

      else if (orb_opt_ == "FALSE" && reference != "ROHF") {
         for(int h = 0; h < nirrep_; ++h){
             for(int i = 0; i < occpiA[h]; ++i) FockA->set(h, i, i, epsilon_a_->get(h,i));
             for(int i = 0; i < occpiB[h]; ++i) FockB->set(h, i, i, epsilon_b_->get(h,i));
             for(int a = 0; a < virtpiA[h]; ++a) FockA->set(h, a + occpiA[h], a + occpiA[h], epsilon_a_->get(h, a + occpiA[h]));
             for(int a = 0; a < virtpiB[h]; ++a) FockB->set(h, a + occpiB[h], a + occpiB[h], epsilon_b_->get(h, a + occpiB[h]));
         }
      }

      timer_on("Build Denominators");
      if (orb_opt_ == "TRUE" || reference == "ROHF") denominators_uhf();
      else if (orb_opt_ == "FALSE" && reference != "ROHF") denominators_ump2();
      timer_off("Build Denominators");
      psio_->close(PSIF_LIBTRANS_DPD, 1);
      //fprintf(outfile,"\n trans_ints done. \n"); fflush(outfile);
 
}//


void OCCWave::denominators_uhf()
{
    //fprintf(outfile,"\n denominators is starting... \n"); fflush(outfile);
    dpdbuf4 D;
    dpdfile2 Fo,Fv;
    
    double *aOccEvals = new double [nacooA];
    double *bOccEvals = new double [nacooB];
    double *aVirEvals = new double [nacvoA];
    double *bVirEvals = new double [nacvoB];
    
    // Pick out the diagonal elements of the Fock matrix, making sure that they are in the order
    // used by the DPD library, i.e. starting from zero for each space and ordering by irrep
    
    int aOccCount = 0, bOccCount = 0, aVirCount = 0, bVirCount = 0;
    
    //Diagonal elements of the Fock matrix
    for(int h = 0; h < nirrep_; ++h){
        for(int i = 0; i < aoccpiA[h]; ++i) aOccEvals[aOccCount++] = FockA->get(h, i + frzcpi_[h], i + frzcpi_[h]);
	for(int i = 0; i < aoccpiB[h]; ++i) bOccEvals[bOccCount++] = FockB->get(h, i + frzcpi_[h], i + frzcpi_[h]);
        for(int a = 0; a < avirtpiA[h]; ++a) aVirEvals[aVirCount++] = FockA->get(h, occpiA[h] + a, occpiA[h] + a); 
	for(int a = 0; a < avirtpiB[h]; ++a) bVirEvals[bVirCount++] = FockB->get(h, occpiB[h] + a, occpiB[h] + a); 
    }
    
    // Build denominators
    // The alpha-alpha spin case
    dpd_buf4_init(&D, PSIF_LIBTRANS_DPD, 0, ID("[O,O]"), ID("[V,V]"),
                  ID("[O,O]"), ID("[V,V]"), 0, "D <OO|VV>");
    for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&D, h);
        for(int row = 0; row < D.params->rowtot[h]; ++row){
            int i = D.params->roworb[h][row][0];
            int j = D.params->roworb[h][row][1];
            for(int col = 0; col < D.params->coltot[h]; ++col){
                int a = D.params->colorb[h][col][0];
                int b = D.params->colorb[h][col][1];
                D.matrix[h][row][col] = 1.0/(aOccEvals[i] + aOccEvals[j] - aVirEvals[a] - aVirEvals[b]);
            }
        }
        dpd_buf4_mat_irrep_wrt(&D, h);
        dpd_buf4_mat_irrep_close(&D, h);
    }
    if (print_ > 2) dpd_buf4_print(&D, outfile, 1);
    dpd_buf4_close(&D);
    

    // The beta-beta spin case 
    dpd_buf4_init(&D, PSIF_LIBTRANS_DPD, 0, ID("[o,o]"), ID("[v,v]"),
                  ID("[o,o]"), ID("[v,v]"), 0, "D <oo|vv>");
    for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&D, h);
        for(int row = 0; row < D.params->rowtot[h]; ++row){
            int i = D.params->roworb[h][row][0];
            int j = D.params->roworb[h][row][1];
            for(int col = 0; col < D.params->coltot[h]; ++col){
                int a = D.params->colorb[h][col][0];
                int b = D.params->colorb[h][col][1];
                D.matrix[h][row][col] = 1.0/(bOccEvals[i] + bOccEvals[j] - bVirEvals[a] - bVirEvals[b]);
            }
        }
        dpd_buf4_mat_irrep_wrt(&D, h);
        dpd_buf4_mat_irrep_close(&D, h);
    }
    if (print_ > 2) dpd_buf4_print(&D, outfile, 1);
    dpd_buf4_close(&D);
    
    
    // The alpha-beta spin case 
    dpd_buf4_init(&D, PSIF_LIBTRANS_DPD, 0, ID("[O,o]"), ID("[V,v]"),
                  ID("[O,o]"), ID("[V,v]"), 0, "D <Oo|Vv>");
    for(int h = 0; h < nirrep_; ++h){
        dpd_buf4_mat_irrep_init(&D, h);
        for(int row = 0; row < D.params->rowtot[h]; ++row){
            int i = D.params->roworb[h][row][0];
            int j = D.params->roworb[h][row][1];
            for(int col = 0; col < D.params->coltot[h]; ++col){
                int a = D.params->colorb[h][col][0];
                int b = D.params->colorb[h][col][1];
                D.matrix[h][row][col] = 1.0/(aOccEvals[i] + bOccEvals[j] - aVirEvals[a] - bVirEvals[b]);
            }
        }
        dpd_buf4_mat_irrep_wrt(&D, h);
        dpd_buf4_mat_irrep_close(&D, h);
    }
    if (print_ > 2) dpd_buf4_print(&D, outfile, 1);
    dpd_buf4_close(&D);
    
    //Print
    if(print_ > 1){
      fprintf(outfile,"\n \n"); fflush(outfile);
      for(int i = 0; i<nacooA; i++) {
	fprintf(outfile,"\taOccEvals[%1d]: %20.14f\n", i, aOccEvals[i]); 
	fflush(outfile);
      }
      
      fprintf(outfile,"\n \n"); fflush(outfile);
      for(int i = 0; i<nacooB; i++) {
	fprintf(outfile,"\tbOccEvals[%1d]: %20.14f\n", i, bOccEvals[i]); 
	fflush(outfile);
      }
      
      fprintf(outfile,"\n \n"); fflush(outfile);
      for(int i = 0; i<nacvoA; i++) {
	fprintf(outfile,"\taVirEvals[%1d]: %20.14f\n", i, aVirEvals[i]); 
	fflush(outfile);
      }
      
      fprintf(outfile,"\n \n"); fflush(outfile);
      for(int i = 0; i<nacvoB; i++) {
	fprintf(outfile,"\tbVirEvals[%1d]: %20.14f\n", i, bVirEvals[i]);
	fflush(outfile);
      }      
    }
    
    delete [] aOccEvals;
    delete [] bOccEvals;
    delete [] aVirEvals;
    delete [] bVirEvals;

 
    // Off-diagonal elements of the Fock matrix    
    // Build Occupied-Occupied block
    // The alpha-alpha spin case 
    dpd_file2_init(&Fo, PSIF_LIBTRANS_DPD, 0, ID('O'), ID('O'), "F <O|O>");
    dpd_file2_mat_init(&Fo);
    for(int h = 0; h < nirrep_; ++h){
        for(int i = 0 ; i < aoccpiA[h]; ++i){
            for(int j = 0 ; j < aoccpiA[h]; ++j){
		if (i != j) Fo.matrix[h][i][j] = FockA->get(h, i + frzcpi_[h], j + frzcpi_[h]);
		else Fo.matrix[h][i][j] = 0.0;
            }
        }
    }
    dpd_file2_mat_wrt(&Fo);
    dpd_file2_close(&Fo);
    
    if (print_ > 2) {
      dpd_file2_init(&Fo, PSIF_LIBTRANS_DPD, 0, ID('O'), ID('O'), "F <O|O>");
      dpd_file2_mat_init(&Fo);
      dpd_file2_mat_print(&Fo, outfile);
      dpd_file2_close(&Fo);
    }
    
    
    // The beta-beta spin case 
    dpd_file2_init(&Fo, PSIF_LIBTRANS_DPD, 0, ID('o'), ID('o'), "F <o|o>");
    dpd_file2_mat_init(&Fo);
    for(int h = 0; h < nirrep_; ++h){
        for(int i = 0 ; i < aoccpiB[h]; ++i){
            for(int j = 0 ; j < aoccpiB[h]; ++j){
		if (i != j) Fo.matrix[h][i][j] = FockB->get(h, i + frzcpi_[h], j + frzcpi_[h]);
		else Fo.matrix[h][i][j] = 0.0;
            }
        }
    }
    dpd_file2_mat_wrt(&Fo);
    dpd_file2_close(&Fo);
    
    if (print_ > 2) {
      dpd_file2_init(&Fo, PSIF_LIBTRANS_DPD, 0, ID('o'), ID('o'), "F <o|o>");
      dpd_file2_mat_init(&Fo);
      dpd_file2_mat_print(&Fo, outfile);
      dpd_file2_close(&Fo);
    }
    
    
    // Build Virtual-Virtual block
    // The alpha-alpha spin case 
    dpd_file2_init(&Fv, PSIF_LIBTRANS_DPD, 0, ID('V'), ID('V'), "F <V|V>");
    dpd_file2_mat_init(&Fv);
    for(int h = 0; h < nirrep_; ++h){
        for(int i = 0 ; i < avirtpiA[h]; ++i){
            for(int j = 0 ; j < avirtpiA[h]; ++j){
                if (i != j) Fv.matrix[h][i][j] = FockA->get(h, i + occpiA[h], j + occpiA[h]);
		else Fv.matrix[h][i][j] = 0.0;
            }
        }
    }
    dpd_file2_mat_wrt(&Fv);
    dpd_file2_close(&Fv);
    
    if (print_ > 2) {
      dpd_file2_init(&Fv, PSIF_LIBTRANS_DPD, 0, ID('V'), ID('V'), "F <V|V>");
      dpd_file2_mat_init(&Fv);
      dpd_file2_mat_print(&Fv, outfile);
      dpd_file2_close(&Fv);
    }
    
    
    // The beta-beta spin case 
    dpd_file2_init(&Fv, PSIF_LIBTRANS_DPD, 0, ID('v'), ID('v'), "F <v|v>");
    dpd_file2_mat_init(&Fv);
    for(int h = 0; h < nirrep_; ++h){
        for(int i = 0 ; i < avirtpiB[h]; ++i){
            for(int j = 0 ; j < avirtpiB[h]; ++j){
                if (i != j) Fv.matrix[h][i][j] = FockB->get(h, i + occpiB[h], j + occpiB[h]);
		else Fv.matrix[h][i][j] = 0.0;
            }
        }
    }
    dpd_file2_mat_wrt(&Fv);
    dpd_file2_close(&Fv);
    
    if (print_ > 2) {
      dpd_file2_init(&Fv, PSIF_LIBTRANS_DPD, 0, ID('v'), ID('v'), "F <v|v>");
      dpd_file2_mat_init(&Fv);
      dpd_file2_mat_print(&Fv, outfile);
      dpd_file2_close(&Fv);
    }    

//fprintf(outfile,"\n denominators done. \n"); fflush(outfile);
}// end denominators
}} // End Namespaces

