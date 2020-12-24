// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include "open3d/utility/Console.h"
#include "open3d/utility/Optional.h"

namespace open3d {
namespace core {

// Avoids circular include.
class Tensor;

// Same as utility::nullopt. Provides a similar Python slicing API.
constexpr utility::nullopt_t None{utility::nullopt_t::init()};

/// A class to represent one of:
/// 1) tensor index
/// e.g. t[0], t[2]
/// 2) tensor slice
/// e.g. t[0:10:2], t[:-1], t[3:]
///
/// Example usage:
/// ```cpp
/// Tensor x({2, 3, 4}, Dtype::Float32);
/// // Equivalent to y = x[1, :3, 0:-1:2] in Python
/// Tensor y = t.GetItem({TensorKey::Index(1),
///                       TensorKey::Slice(None, 3, None),
///                       TensorKey::Slice(0, -1, 2)});
/// ```
class TensorKey {
public:
    enum class TensorKeyMode { Index, Slice, IndexTensor };
    TensorKeyMode GetMode() const;
    std::string ToString() const;

    ~TensorKey() {}

    static TensorKey Index(int64_t index);
    static TensorKey Slice(utility::optional<int64_t> start,
                           utility::optional<int64_t> stop,
                           utility::optional<int64_t> step);
    static TensorKey IndexTensor(const Tensor& index_tensor);

public:
    /// For TensorKeyMode::Index.
    int64_t GetIndex() const;

    /// For TensorKeyMode::Slice.
    int64_t GetStart() const;
    int64_t GetStop() const;
    int64_t GetStep() const;
    TensorKey UpdateWithDimSize(int64_t dim_size) const;

    /// For TensorKeyMode::IndexTensor.
    std::shared_ptr<Tensor> GetIndexTensor() const;

private:
    class Impl;
    class IndexImpl;
    class SliceImpl;
    class IndexTensorImpl;
    std::shared_ptr<Impl> impl_;
    TensorKey(const std::shared_ptr<Impl>& impl);
};

}  // namespace core
}  // namespace open3d
