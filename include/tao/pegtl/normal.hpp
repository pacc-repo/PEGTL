// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_NORMAL_HPP
#define TAO_PEGTL_NORMAL_HPP

#include <type_traits>
#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "nothing.hpp"
#include "parse_error.hpp"
#include "rewind_mode.hpp"

#include "internal/demangle.hpp"
#include "internal/dusel_mode.hpp"
#include "internal/duseltronik.hpp"
#include "internal/has_apply.hpp"
#include "internal/has_apply0.hpp"
#include "internal/skip_control.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule >
      struct normal
      {
         template< typename Input, typename... States >
         static void start( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void success( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void failure( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void raise( const Input& in, States&&... /*unused*/ )
         {
            throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
         }

         template< template< typename... > class Action, typename Input, typename... States >
         static auto apply0( const Input& /*unused*/, States&&... st )
            -> decltype( Action< Rule >::apply0( st... ) )
         {
            return Action< Rule >::apply0( st... );
         }

         template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
         static auto apply( const Iterator& begin, const Input& in, States&&... st )
            -> decltype( Action< Rule >::apply( std::declval< const typename Input::action_t& >(), st... ) )
         {
            const typename Input::action_t action_input( begin, in );
            return Action< Rule >::apply( action_input, st... );
         }

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename Input,
                   typename... States >
         [[nodiscard]] static bool match( Input& in, States&&... st )
         {
            constexpr char use_control = !internal::skip_control< Rule >;
            constexpr char use_action = use_control && ( A == apply_mode::action ) && ( !std::is_base_of_v< nothing< Rule >, Action< Rule > > );
            constexpr char use_apply_void = use_action && internal::has_apply< Control< Rule >, void, Action, const typename Input::iterator_t&, const Input&, States... >::value;
            constexpr char use_apply_bool = use_action && internal::has_apply< Control< Rule >, bool, Action, const typename Input::iterator_t&, const Input&, States... >::value;
            constexpr char use_apply0_void = use_action && internal::has_apply0< Control< Rule >, void, Action, const Input&, States... >::value;
            constexpr char use_apply0_bool = use_action && internal::has_apply0< Control< Rule >, bool, Action, const Input&, States... >::value;
            static_assert( !use_action || use_apply_bool || use_apply_void || use_apply0_bool || use_apply0_void, "actions not disabled but no apply() or apply0() found" );
            static_assert( use_apply_void + use_apply_bool + use_apply0_void + use_apply0_bool < 2, "both apply() and apply0() defined" );
            constexpr auto mode = static_cast< dusel_mode >( use_control + use_apply_void + 2 * use_apply_bool + 3 * use_apply0_void + 4 * use_apply0_bool );
            return internal::duseltronik< Rule, A, M, Action, Control, mode >::match( in, st... );
         }
      };

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
