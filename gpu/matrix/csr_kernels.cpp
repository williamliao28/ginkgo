/*******************************<GINKGO LICENSE>******************************
Copyright 2017-2018

Karlsruhe Institute of Technology
Universitat Jaume I
University of Tennessee

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************<GINKGO LICENSE>*******************************/

#include "core/matrix/csr_kernels.hpp"


#include "core/base/exception_helpers.hpp"
#include "core/base/math.hpp"
#include "gpu/base/cusparse_bindings.hpp"


namespace gko {
namespace kernels {
namespace gpu {
namespace csr {


template <typename ValueType, typename IndexType>
void spmv(const matrix::Csr<ValueType, IndexType> *a,
          const matrix::Dense<ValueType> *b, matrix::Dense<ValueType> *c)
{
    // TODO: add implementation for int64 and multiple RHS
    auto handle = cusparse::init();
    auto descr = cusparse::create_mat_descr();
    ASSERT_NO_CUSPARSE_ERRORS(
        cusparseSetPointerMode(handle, CUSPARSE_POINTER_MODE_HOST));

    auto row_ptrs = a->get_const_row_ptrs();
    auto col_idxs = a->get_const_col_idxs();
    auto alpha = one<ValueType>();
    auto beta = zero<ValueType>();
    if (b->get_padding() != 1 || c->get_padding() != 1) NOT_IMPLEMENTED;

    cusparse::spmv(handle, CUSPARSE_OPERATION_NON_TRANSPOSE, a->get_num_rows(),
                   a->get_num_cols(), a->get_num_stored_elements(), &alpha,
                   descr, a->get_const_values(), row_ptrs, col_idxs,
                   b->get_const_values(), &beta, c->get_values());

    cusparse::destroy(descr);
    cusparse::destroy(handle);
};


GKO_INSTANTIATE_FOR_EACH_VALUE_AND_INDEX_TYPE(GKO_DECLARE_CSR_SPMV_KERNEL);

template <typename ValueType, typename IndexType>
void advanced_spmv(const matrix::Dense<ValueType> *alpha,
                   const matrix::Csr<ValueType, IndexType> *a,
                   const matrix::Dense<ValueType> *b,
                   const matrix::Dense<ValueType> *beta,
                   matrix::Dense<ValueType> *c)
{
    // TODO: add implementation for int64 and multiple RHS
    auto handle = cusparse::init();
    auto descr = cusparse::create_mat_descr();

    auto row_ptrs = a->get_const_row_ptrs();
    auto col_idxs = a->get_const_col_idxs();

    if (b->get_padding() != 1 || c->get_padding() != 1) NOT_IMPLEMENTED;

    cusparse::spmv(handle, CUSPARSE_OPERATION_NON_TRANSPOSE, a->get_num_rows(),
                   a->get_num_cols(), a->get_num_stored_elements(),
                   alpha->get_const_values(), descr, a->get_const_values(),
                   row_ptrs, col_idxs, b->get_const_values(),
                   beta->get_const_values(), c->get_values());

    cusparse::destroy(descr);
    cusparse::destroy(handle);
};

GKO_INSTANTIATE_FOR_EACH_VALUE_AND_INDEX_TYPE(
    GKO_DECLARE_CSR_ADVANCED_SPMV_KERNEL);


template <typename ValueType, typename IndexType>
void convert_to_dense(matrix::Dense<ValueType> *result,
                      const matrix::Csr<ValueType, IndexType> *source)
    NOT_IMPLEMENTED;

GKO_INSTANTIATE_FOR_EACH_VALUE_AND_INDEX_TYPE(
    GKO_DECLARE_CSR_CONVERT_TO_DENSE_KERNEL);


template <typename ValueType, typename IndexType>
void move_to_dense(matrix::Dense<ValueType> *result,
                   matrix::Csr<ValueType, IndexType> *source) NOT_IMPLEMENTED;

GKO_INSTANTIATE_FOR_EACH_VALUE_AND_INDEX_TYPE(
    GKO_DECLARE_CSR_MOVE_TO_DENSE_KERNEL);


}  // namespace csr
}  // namespace gpu
}  // namespace kernels
}  // namespace gko
