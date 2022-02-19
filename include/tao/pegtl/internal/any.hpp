// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ANY_HPP
#define TAO_PEGTL_INTERNAL_ANY_HPP

#include "many.hpp"

namespace tao::pegtl::internal
{
   template< typename Peek >
   using any = many< 1, Peek >;

}  // namespace tao::pegtl::internal

#endif
