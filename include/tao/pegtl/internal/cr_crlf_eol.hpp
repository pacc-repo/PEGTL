// Copyright (c) 2016-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_CR_CRLF_EOL_HPP
#define TAO_PEGTL_INTERNAL_CR_CRLF_EOL_HPP

#include "eol_pair.hpp"

namespace tao::pegtl::internal
{
   struct cr_crlf_eol
   {
      static constexpr int ch = '\r';

      template< typename ParseInput >
      [[nodiscard]] static eol_pair eol_match( ParseInput& in ) noexcept( noexcept( in.size( 2 ) ) )
      {
         eol_pair p = { false, in.size( 2 ) };
         if( p.second ) {
            if( in.peek_char() == '\r' ) {
               in.bump_to_next_line( 1 + ( ( p.second > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) );
               p.first = true;
            }
         }
         return p;
      }
   };

}  // namespace tao::pegtl::internal

#endif
