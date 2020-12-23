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

    /// Construct an TensorKeyMode::Index type TensorKey.
    /// E.g. b = a[3]
    static TensorKey Index(int64_t index);

    /// Construct an TensorKeyMode::Slice type TensorKey.
    /// E.g. b = a[0:100:2]
    static TensorKey Slice(utility::optional<int64_t> start,
                           utility::optional<int64_t> stop,
                           utility::optional<int64_t> step);

    /// Construct an TensorKeyMode::IndexTensor type TensorKey (advanced
    /// indexing).
    static TensorKey IndexTensor(const Tensor& index_tensor);

    /// Getters will check the TensorKeyMode
    TensorKeyMode GetMode() const;

    int64_t GetIndex() const;

    int64_t GetStart() const;

    int64_t GetStop() const;

    int64_t GetStep() const;

    std::shared_ptr<Tensor> GetIndexTensor() const;

    /// When dim_size is know, convert the slice object such that
    /// start_is_none_ == stop_is_none_ == step_is_none_ == false
    /// E.g. if t.shape == (5,), t[:4]:
    ///      before compute: Slice(None,    4, None)
    ///      after compute : Slice(   0,    4,    1)
    /// E.g. if t.shape == (5,), t[1:]:
    ///      before compute: Slice(   1, None, None)
    ///      after compute : Slice(   1,    5,    1)
    TensorKey UpdateWithDimSize(int64_t dim_size) const;

    /// String representation of the TensorKey.
    std::string ToString() const;

protected:
    /// The fully specified constructor shall not be called directly. Use the
    /// factory functions instead.
    TensorKey(TensorKeyMode mode,
              int64_t index,
              utility::optional<int64_t> start,
              utility::optional<int64_t> stop,
              utility::optional<int64_t> step,
              const Tensor& index_tensor);

    void AssertMode(TensorKeyMode mode) const {
        if (mode != mode_) {
            utility::LogError("Wrong TensorKeyMode.");
        }
    }

    /// Depending on the mode, some properties may or may not be used.
    TensorKeyMode mode_;

    /// Properties for TensorKeyMode::Index.
    int64_t index_ = 0;

    /// Properties for TensorKeyMode::Slice.
    utility::optional<int64_t> start_ = utility::nullopt;
    utility::optional<int64_t> stop_ = utility::nullopt;
    utility::optional<int64_t> step_ = utility::nullopt;

    /// Properties for TensorKeyMode::IndexTensor.
    /// To avoid circular include, the pointer type is used. The index_tensor is
    /// shallow-copied when the TensorKey constructor is called.
    std::shared_ptr<Tensor> index_tensor_;
};

}  // namespace core
}  // namespace open3d
