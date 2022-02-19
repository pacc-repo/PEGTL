// Copyright (c) 2020-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_VISIT_HPP
#define TAO_PEGTL_VISIT_HPP

#include <type_traits>

#include "type_list.hpp"

namespace tao::pegtl
{
   namespace internal
   {
      template< typename T >
      struct wrap_as_type
      {
         using type = T;
      };

      template< typename Type, typename... Types >
      inline constexpr bool contains_v = ( std::is_same_v< Type, Types > || ... );

      template< typename Type, typename... Types >
      struct contains
         : std::bool_constant< contains_v< Type, Types... > >
      {};

      template< typename Type, typename... Types >
      struct contains< Type, type_list< Types... > >
         : contains< Type, Types... >
      {};

      template< typename Rules, typename Todo, typename Done >
      struct filter
      {
         using type = Todo;
      };

      template< typename Rule, typename... Rules, typename... Todo, typename... Done >
      struct filter< type_list< Rule, Rules... >, type_list< Todo... >, type_list< Done... > >
         : filter< type_list< Rules... >, std::conditional_t< contains_v< Rule, Todo..., Done... >, type_list< Todo... >, type_list< Todo..., Rule > >, type_list< Done... > >
      {};

      template< typename Rules, typename Todo, typename Done >
      using filter_t = typename filter< Rules, Todo, Done >::type;

      template< typename Done, typename... Rules >
      struct visit_list
      {
         using NextDone = type_list_concat< type_list< Rules... >, Done >;
         using NextSubs = type_list_concat< typename Rules::subs_t... >;
         using NextTodo = filter_t< NextSubs, empty_list, NextDone >;

         using type = typename std::conditional_t< std::is_same_v< NextTodo, empty_list >, wrap_as_type< NextDone >, visit_list< NextDone, NextTodo > >::type;
      };

      template< typename Done, typename... Rules >
      struct visit_list< Done, type_list< Rules... > >
         : visit_list< Done, Rules... >
      {};

      template< template< typename... > class Func, typename... Args, typename... Rules >
      void visit( type_list< Rules... > /*unused*/, Args&&... args )
      {
         ( Func< Rules >::visit( args... ), ... );
      }

   }  // namespace internal

   template< typename Grammar >
   using grammar_rule_list = typename internal::visit_list< empty_list, Grammar >::type;

   template< typename Grammar, typename Rule >
   inline constexpr bool grammar_contains_rule = internal::contains< Rule, grammar_rule_list< Grammar > >::value;

   template< typename Rule, template< typename... > class Func, typename... Args >
   void visit( Args&&... args )
   {
      internal::visit< Func >( grammar_rule_list< Rule >(), args... );
   }

}  // namespace tao::pegtl

#endif
