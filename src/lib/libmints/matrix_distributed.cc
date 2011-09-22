/*
 *  matrix_distributed.cc
 *  distributed_matrix
 *
 *  Created by Ben Mintz on 8/24/11.
 *  Copyright 2011 __MintzInc__. All rights reserved.
 *
 */

#include <exception.h>
#include <libciomr/libciomr.h>
#include <libpsio/psio.hpp>
#include <libiwl/iwl.hpp>
#include <libqt/qt.h>
#include <libmints/integral.h>
#include <libdpd/dpd.h>
#include "factory.h"
#include "wavefunction.h"
#include "dimension.h"
#include "matrix_distributed.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <ctype.h>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>

using namespace boost;
using namespace psi;
using namespace std;

// In molecule.cc
namespace psi {
extern int str_to_int(const std::string& s);
extern double str_to_double(const std::string& s);
}

#ifdef HAVE_MADNESS

void Distributed_Matrix::clear_matrix()
{
    int tile_sz_ = 0;
    int ntiles_ = 0;
    int nrows_ = 0;
    int ncols_ = 0;
    int nelements_ = 0;
    free_vector(row_split_);
    free_vector(col_split_);
    free_vector(tile_nrows_);
    free_vector(tile_ncols_);
    free_map(global_local_tile_);
    free_vector(local_global_tile_);
    for (int i=0; i < tiles_.size(); i++) {
        free_vector(tiles_[i]);
    }
    free_vector(tiles_);
}

Distributed_Matrix::Distributed_Matrix()
    : madness::WorldObject<Distributed_Matrix>(*Communicator::world->get_madworld())
{
    me_ = Communicator::world->me();
    nprocs_ = Communicator::world->nproc();
    nthreads_ = Communicator::world->nthread();
    comm_ = Communicator::world->communicator();
    print_mutex_ = Communicator::world->get_mutex();
    madworld_ = Communicator::world->get_madworld();

    clear_matrix();

    process_pending();
}

Distributed_Matrix::Distributed_Matrix(const int &nrows, const int &ncols,
                                       const int &tile_sz, const std::string &name)
    : madness::WorldObject<Distributed_Matrix>(*Communicator::world->get_madworld())
{
    me_ = Communicator::world->me();
    nprocs_ = Communicator::world->nproc();
    nthreads_ = Communicator::world->nthread();
    comm_ = Communicator::world->communicator();
    print_mutex_ = Communicator::world->get_mutex();
    madworld_ = Communicator::world->get_madworld();

    common_init(nrows, ncols, tile_sz, name);

    process_pending();
}

void Distributed_Matrix::common_init(const int &nrows, const int &ncols,
                                     const int &tile_sz, const std::string &name)
{
    clear_matrix();

    nrows_ = nrows;
    ncols_ = ncols;
    tile_sz_ = tile_sz;
    nelements_ = nrows_ * ncols_;
    name_ = name;

    if (nrows_%tile_sz_ == 0 && ncols%tile_sz_ == 0)
        ntiles_ = (nrows_/tile_sz_) * (ncols_/tile_sz_);
    else if (nrows_%tile_sz_ == 0 && ncols%tile_sz_ != 0)
        ntiles_ = (nrows_/tile_sz_) * (ncols_/tile_sz_ + 1);
    else if (nrows_%tile_sz_ != 0 && ncols%tile_sz_ == 0)
        ntiles_ = (nrows_/tile_sz_ + 1) * (ncols_/tile_sz_);
    else if (nrows_%tile_sz_ != 0 && ncols%tile_sz_ != 0)
        ntiles_ = (nrows_/tile_sz_ + 1) * (ncols_/tile_sz_ + 1);

    std::cout << "ntiles = " << ntiles_ << std::endl;

    for (int i=0, local=0; i < ntiles_; i++) {
        if (me_ == owner(i)) {
            global_local_tile_.insert(std::pair<int,int>(i,local));
            local_global_tile_.push_back(i);
            local++;
        }
    }

    // The rows and columns are split according to row_split and col_split
    for (int i=0; i < nrows_/tile_sz_; i++) {
        row_split_.push_back(tile_sz_);
    }
    for (int i=0; i < ncols/tile_sz_; i++) {
        col_split_.push_back(tile_sz_);
    }
    // This adds the left over rows to the last tile
    if (nrows_%tile_sz_ != 0) {
        row_split_.push_back(nrows_%tile_sz_);
    }
    // This adds in the left over columns to the last tile
    if (ncols_%tile_sz_ != 0) {
        col_split_.push_back(ncols_%tile_sz_);
    }

//    for (int i=0; i < row_split_.size(); i++) {
//        std::cout << "row_split[" << i << "] = " << row_split_[i] << std::endl;
//    }
//    for (int i=0; i < col_split_.size(); i++) {
//        std::cout << "col_split[" << i << "] = " << col_split_[i] << std::endl;
//    }


    tile_nrows_.clear();
    tile_ncols_.clear();
    for (int i=0, ij=0; i < row_split_.size(); i++) {
        for (int j=0; j < col_split_.size(); j++, ij++) {
            if (me_ == owner(ij)) {
                tile_nrows_.push_back(row_split_[i]);
                tile_ncols_.push_back(col_split_[j]);
            }
        }
    }

//    for (int ij=0; ij < ntiles_; ij++) {
//        int local_ij = global_local_tile_[ij];
//        std::cout << "tile[" << ij << "] = " << tile_nrows_[local_ij] <<
//                     " x " << tile_ncols_[local_ij] << std::endl;
//    }

    for (int t=0; t < ntiles_; t++) {
        if (me_ == owner(t)) {
            int nrow = tile_nrows_[global_local_tile_[t]];
            int ncol = tile_ncols_[global_local_tile_[t]];
            tiles_.push_back( std::vector<double>(nrow*ncol, 0.0) );
        }
    }

//    std::map<int,int>::iterator it;
//    for (it = global_local_tile_.begin();
//         it != global_local_tile_.end();
//         it++) {
//        std::cout << "proc " << me_ << ": global = " << it->first << ": local = " << it->second << std::endl;
//    }

}


madness::Void Distributed_Matrix::print_tile(const int &t) const
{
    if (me_ == 0) {
        if (nelements_ != 0) {
            madness::Future<std::vector<double> > tile = task(owner(t), &Distributed_Matrix::get_tile, t);
            madness::Future<int> row = task(owner(t), &Distributed_Matrix::t_nrow, t);
            madness::Future<int> col = task(owner(t), &Distributed_Matrix::t_ncol, t);

            task(me_, &Distributed_Matrix::print_mat, t, tile, row, col);
        }
        else {
            if (name_.size() != 0)
                fprintf(outfile, "  ## %s (is empty) ##\n", name_.c_str());
            else
                fprintf(outfile, "  ## Distributed Matrix (is empty) ##\n");
        }
    }
    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::print_all_tiles() const
{
    if (me_ == 0) {
        if (nelements_ != 0) {
            for (int t=0; t < ntiles_; t++) {
                madness::Future<std::vector<double> > tile = task(owner(t), &Distributed_Matrix::get_tile, t);
                madness::Future<int> row = task(owner(t), &Distributed_Matrix::t_nrow, t);
                madness::Future<int> col = task(owner(t), &Distributed_Matrix::t_ncol, t);

                task(me_, &Distributed_Matrix::print_mat, t, tile, row, col);
            }
        }
        else {
            if (name_.size() != 0)
                fprintf(outfile, "  ## %s (is empty) ##\n", name_.c_str());
            else
                fprintf(outfile, "  ## Distributed Matrix (is empty) ##\n");
        }
    }
    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::print_mat(const int &tile, const std::vector<double> &a,
                                            const int &m, const int &n) const
{
    print_mutex_->lock();

    std::string fname;
    if (name_.size() != 0) fname = name_ + ": Owner " + to_string(owner(tile)) +
            ": Tile " + to_string(tile);
    else fname = ": Owner " + to_string(owner(tile)) + ": Tile " + to_string(tile);

    fprintf(outfile, "\n  ## %s ##\n", fname.c_str());

    if (a.size() == 0) {
        fprintf(outfile, "\n\t## %s ## (empty)\n", fname.c_str());
    }
    else {
        // Should only be called by process 0
        const int print_ncol = 5;
        int num_frames = int(n/print_ncol);
        int num_frames_rem = n%print_ncol; //adding one for changing 0->1 start
        int num_frame_counter = 0;
        //for each frame
        for(num_frame_counter=0;num_frame_counter<num_frames;num_frame_counter++){
            fprintf(outfile,"\n");
            for(int j=print_ncol*num_frame_counter+1;j<print_ncol*num_frame_counter+print_ncol+1;j++){
                if(j==print_ncol*num_frame_counter+1){ fprintf(outfile,"%18d",j); }
                else{ fprintf(outfile,"        %5d",j); }
            }
            fprintf(outfile,"\n\n");

            for(int k=1; k<=m; ++k){
                for(int j=print_ncol*num_frame_counter+1;j<print_ncol*num_frame_counter+print_ncol+2;j++){
                    if(j==print_ncol*num_frame_counter+1){ fprintf(outfile,"%5d",k);}
                    else{ fprintf(outfile," %12.7f",a[(k-1)*n + (j-2)]); }
                }
                fprintf(outfile,"\n");
            }
        }

        // ALREADY DID THE FULL FRAMES BY THIS POINT
        // NEED TO TAKE CARE OF THE REMAINDER
        if(num_frames_rem != 0){
            fprintf(outfile,"\n");
            for(int j=print_ncol*num_frame_counter+1;j<=n;j++){
                if(j==print_ncol*num_frame_counter+1){ fprintf(outfile,"%18d",j); }
                else{ fprintf(outfile,"        %5d",j); }
            }
            fprintf(outfile,"\n\n");

            for(int k=1; k<=m; ++k){
                for(int j=print_ncol*num_frame_counter+1;j<n+2;j++){
                    if(j==print_ncol*num_frame_counter+1){ fprintf(outfile,"%5d",k); }
                    else{ fprintf(outfile," %12.7f",a[(k-1)*n + (j-2)]); }
                }
                fprintf(outfile,"\n");
            }
        }
        fprintf(outfile,"\n\n");
    }
    print_mutex_->unlock();

    return madness::None;
}

madness::Void  Distributed_Matrix::identity()
{
    for (int i=0, ij=0; i < row_split_.size(); i++) {
        for (int j=0; j < col_split_.size(); j++, ij++) {
            if (me_ == owner(ij)) {
                if (i == j)
                    task(owner(ij), &Distributed_Matrix::set_tile_to_identity, ij);
                else
                    task(owner(ij), &Distributed_Matrix::zero_tile, ij);
            }
        }
    }
    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::set_tile_to_identity(const int &t)
{
    int t_local = global_local_tile_[t];
    int rows = tile_nrows_[t_local];
    int cols = tile_ncols_[t_local];

    memset(&(tiles_[t_local][0]), 0, tiles_[t_local].size()*sizeof(double));

    for (int i=0; i < rows; i++) {
        for (int j=0; j < cols; j++) {
            if (i == j) tiles_[t_local][i*cols + j] = 1.0;
        }
    }
    return madness::None;
}

madness::Void Distributed_Matrix::zero()
{
    for (int t=0; t < ntiles_; t++) {
        if (me_ == owner(t)) {
            task(owner(t), &Distributed_Matrix::zero_tile, t);
        }
    }
    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::zero_tile(const int &t)
{
    int t_local = global_local_tile_[t];
    memset(&(tiles_[t_local][0]), 0, tiles_[t_local].size()*sizeof(double));
    return madness::None;
}

madness::Future<double> Distributed_Matrix::get_val(const int &row, const int &col)
{
    if (nelements_ != 0) {
        if (row < nrows_ && col < ncols_) {
            int tile = row/tile_sz_ * col_split_.size() + col/tile_sz_;
            int own = owner(tile);

            if (me_ == own) {
                int r = row%tile_sz_;
                int c = col%tile_sz_;
                return task(own, &Distributed_Matrix::return_tile_val, tile, r, c);
            }
            else
                return task(own, &Distributed_Matrix::get_val, row, col);
        }
        else
            throw PSIEXCEPTION("The value is out of bounds.\n");

    }
    else {
        throw PSIEXCEPTION("There are no values to get in an empty distributed matrix.\n");
    }
}

madness::Future<double> Distributed_Matrix::return_tile_val(const int &t, const int &row,
                                                       const int &col)
{
    int t_loc = global_local_tile_[t];
    int ncol = tile_ncols_[t_loc];
    return madness::Future<double> (tiles_[t_loc][row*ncol + col]);
}

madness::Void Distributed_Matrix::set_val(const int &row, const int &col, const double &val)
{
    if (nelements_ != 0) {
        if (row < nrows_ && col < ncols_) {
            int tile = row/tile_sz_ * col_split_.size() + col/tile_sz_;
            int own = owner(tile);

            if (me_ == own) {
                int r = row%tile_sz_;
                int c = col%tile_sz_;

                set_tile_value(tile, r, c, val);
            }
            else {
                task(own, &Distributed_Matrix::set_val, row, col, val);
            }
        }
        else
            throw PSIEXCEPTION("The value is out of bounds.\n");

    }
    else {
        throw PSIEXCEPTION("There are no values to set in an empty distributed matrix.\n");
    }

    return madness::None;
}

madness::Void Distributed_Matrix::set_tile_value(const int &t, const int &row,
                                                 const int &col, const double &val)
{
    int t_loc = global_local_tile_[t];
    int ncol = tile_ncols_[t_loc];
    tiles_[t_loc][row*ncol + col] = val;

    return madness::None;
}

Distributed_Matrix& Distributed_Matrix::operator= (const Distributed_Matrix &rhs)
{
    this->clear_matrix();

    if (name_.size() == 0) common_init(rhs.nrows_, rhs.ncols_, rhs.tile_sz_, rhs.name_);
    else common_init(rhs.nrows_, rhs.ncols_, rhs.tile_sz_, name_);

    for (int t=0; t < ntiles_; t++) {
        if (me_ == owner(t)) {
            madness::Future<std::vector<double> > tile = rhs.task(owner(t), &Distributed_Matrix::get_tile, t);
            this->task(owner(t), &Distributed_Matrix::copy_tile, t, tile);
        }
    }
    // We need a sync here to make sure that all of the copying is done before moving on
    Communicator::world->sync();
    return *this;
}

Distributed_Matrix& Distributed_Matrix::operator= (const Distributed_Matrix *rhs)
{
    this->clear_matrix();

    if (name_.size() == 0) common_init(rhs->nrows_, rhs->ncols_, rhs->tile_sz_, rhs->name_);
    else common_init(rhs->nrows_, rhs->ncols_, rhs->tile_sz_, name_);

    for (int t=0; t < ntiles_; t++) {
        if (me_ == owner(t)) {
            madness::Future<std::vector<double> > tile = rhs->task(owner(t), &Distributed_Matrix::get_tile, t);
            this->task(owner(t), &Distributed_Matrix::copy_tile, t, tile);
        }
    }
    // We need a sync here to make sure that all of the copying is done before moving on
    Communicator::world->sync();
    return *this;
}


madness::Void Distributed_Matrix::copy_tile(const int &t, const std::vector<double> &tile) {
    tiles_[global_local_tile_[t]] = tile;
    return madness::None;
}

madness::Void Distributed_Matrix::operator +=(const Distributed_Matrix &rhs)
{
    if (*this == rhs) {
        for (int t=0; t < ntiles_; t++) {
            if (me_ == owner(t)) {
                madness::Future<std::vector<double> > tile = rhs.task(me_, &Distributed_Matrix::get_tile, t);
                this->task(me_, &Distributed_Matrix::sum_tile, t, tile);
            }
        }
    }
    else throw PSIEXCEPTION("The matrices being added are not the same.\n");

    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::operator +=(const Distributed_Matrix *rhs)
{
    if (*this == rhs) {
        for (int t=0; t < ntiles_; t++) {
            if (me_ == owner(t)) {
                madness::Future<std::vector<double> > tile = rhs->task(me_, &Distributed_Matrix::get_tile, t);
                this->task(me_, &Distributed_Matrix::sum_tile, t, tile);
            }
        }
    }
    else throw PSIEXCEPTION("The matrices being added are not the same.\n");

    Communicator::world->sync();
    return madness::None;
}


Distributed_Matrix Distributed_Matrix::operator +(const Distributed_Matrix &rhs)
{
    if (*this == rhs) {
        Distributed_Matrix result = *this;
        result += rhs;
        // The += operator has a sync, so we don't need one here
        return result;
    }
    else throw PSIEXCEPTION("The matrices that are being added are not the same.\n");
}

Distributed_Matrix Distributed_Matrix::operator +(const Distributed_Matrix *rhs)
{
    if (*this == rhs) {
        Distributed_Matrix result = *this;
        result += rhs;
        // The += operator has a sync, so we don't need one here
        return result;
    }
    else throw PSIEXCEPTION("The matrices that are being added are not the same.\n");
}



madness::Void Distributed_Matrix::sum_tile(const int &t, const std::vector<double> &tile) {
    for (int i=0; i < tile.size(); i++)
        tiles_[global_local_tile_[t]][i] += tile[i];
    return madness::None;
}

madness::Void Distributed_Matrix::fill(const double &val)
{
    if (this->nelements_ != 0) {
        for (int t=0; t < ntiles_; t++) {
            if (me_ == owner(t))
                this->task(me_, &Distributed_Matrix::fill_tile, t, val);
        }
    }
    else throw PSIEXCEPTION("The matrix you are tryin to fill is empty.\n");

    Communicator::world->sync();
    return madness::None;
}

madness::Void Distributed_Matrix::fill_tile(const int &t, const double &val)
{
    int loc_t = global_local_tile_[t];

    for (int i=0; i < tiles_[loc_t].size(); i++)
        tiles_[loc_t][i] = val;

    return madness::None;
}

bool Distributed_Matrix::operator ==(const Distributed_Matrix &rhs)
{
    if ( this->nrows_ != rhs.nrows_ ) return false;
    else if ( this->ncols_ != rhs.ncols_ ) return false;
    else if ( this->tile_sz_ != rhs.tile_sz_ ) return false;
    else if ( this->nelements_ != rhs.nelements_ ) return false;
    else if ( this->ntiles_ != rhs.ntiles_ ) return false;
    else if ( this->row_split_ != rhs.row_split_ ) return false;
    else if ( this->col_split_ != rhs.col_split_ ) return false;
    else if ( this->tile_nrows_ != rhs.tile_nrows_ ) return false;
    else if ( this->tile_ncols_ != rhs.tile_ncols_ ) return false;
    else return true;
}

bool Distributed_Matrix::operator ==(const Distributed_Matrix *rhs)
{
    if ( this->nrows_ != rhs->nrows_ ) return false;
    else if ( this->ncols_ != rhs->ncols_ ) return false;
    else if ( this->tile_sz_ != rhs->tile_sz_ ) return false;
    else if ( this->nelements_ != rhs->nelements_ ) return false;
    else if ( this->ntiles_ != rhs->ntiles_ ) return false;
    else if ( this->row_split_ != rhs->row_split_ ) return false;
    else if ( this->col_split_ != rhs->col_split_ ) return false;
    else if ( this->tile_nrows_ != rhs->tile_nrows_ ) return false;
    else if ( this->tile_ncols_ != rhs->tile_ncols_ ) return false;
    else return true;
}

bool Distributed_Matrix::operator !=(const Distributed_Matrix &rhs)
{
    if (*this == rhs) return false;
    else return true;
}

bool Distributed_Matrix::operator !=(const Distributed_Matrix *rhs)
{
    if (*this == rhs) return false;
    else return true;
}


Distributed_Matrix Distributed_Matrix::operator* (const Distributed_Matrix &rhs)
{

    if (this->nrows_ == rhs.ncols_ && this->tile_sz_ == rhs.tile_sz_) {
        madness::TaskAttributes attr;
        attr.set_highpriority(true);

        Distributed_Matrix result(this->nrows_, rhs.ncols_, this->tile_sz_, "Multiply Result");

        result.zero();

        int lmat_row = this->row_split_.size();
        int lmat_col = this->col_split_.size();
        int rmat_col = rhs.col_split_.size();
        int result_col = result.col_split_.size();

        for (int i=0; i < lmat_row; i++) {
            for (int j=0; j < lmat_col; j++) {
                int ij = i*lmat_col + j;
                for (int k=0; k < rmat_col; k++) {
                    int ik = i*result_col + k;

                    if (me_ == owner(ik)) {
                        int jk = j*rmat_col + k;

                        madness::Future<std::vector<double> > A = this->task(owner(ij), &Distributed_Matrix::get_tile, ij);
                        madness::Future<std::vector<double> > B = rhs.task(owner(jk), &Distributed_Matrix::get_tile, jk);
                        madness::Future<int> a_row = this->task(owner(ij), &Distributed_Matrix::t_nrow, ij);
                        madness::Future<int> a_col = this->task(owner(ij), &Distributed_Matrix::t_ncol, ij);
                        madness::Future<int> b_col = rhs.task(owner(jk), &Distributed_Matrix::t_ncol, jk);

                        result.task(owner(ik), &Distributed_Matrix::mxm,
                                    ik, A, B, a_row, a_col, b_col);
                    }

                }
            }
        }

        Communicator::world->sync();

        return result;
    }
    else {
        throw PSIEXCEPTION("The columns of A do not match the rows of B.\n");
    }
}

madness::Void Distributed_Matrix::mxm(const int &t,
                                      const std::vector<double> &a,
                                      const std::vector<double> &b,
                                      const int &a_row,
                                      const int &a_col,
                                      const int &b_col)
{
    int c_local = global_local_tile_[t];
    C_DGEMM('n', 'n', a_row, b_col, a_col, 1.0,
            const_cast<double*>(&a[0]), a_col,
            const_cast<double*>(&b[0]), b_col, 1.0,
            &(tiles_[c_local][0]), b_col);
    return madness::None;
}


#endif // End of HAVE_MADNESS
