#ifndef common_BASE_COPYABLE_H
#define common_BASE_COPYABLE_H

namespace common
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace common

#endif  // common_BASE_COPYABLE_H
