#ifndef RESTRICTION_HPP
#define RESTRICTION_HPP
#include "can_call.hpp"
#include "expansion.hpp"
namespace common
{
    struct non_returnable
    {
        non_returnable( non_returnable && ) = delete;
        non_returnable( const non_returnable & ) = delete;
        non_returnable( ) = delete;
    };

    template< typename T >
    struct restriction_helper
    {
        template< typename ... ARG, typename = std::enable_if_t< can_call< T, ARG ... >::value > >
        non_returnable operator ( )( ARG && ... ) const = delete;
    };

    template< typename T1, typename T2 >
    expansion< T1, T2 > restriction( const T1 & t1, const T2 & ) { return make_expansion( restriction_helper< T2 >( ), t1 ); }
}
#endif // RESTRICTION_HPP
