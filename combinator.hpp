#ifndef COMBINATOR_HPP
#define COMBINATOR_HPP
#include <map>
#include <boost/any.hpp>
#include <typeindex>
#include <memory>
namespace misc
{
    template< typename T >
    struct fix_struct
    {
        T t;
        template< typename ... ARG >
        auto operator ( )( const ARG & ... arg ) const
        { return t( * this, arg ... ); }
        template< typename ... ARG >
        auto operator ( )( const ARG & ... arg )
        { return t( * this, arg ... ); }
    };

    template< typename T >
    fix_struct< T > fix( const T & t ) { return fix_struct< T >( { t } ); }

    template< typename T >
    struct remember_fix_struct
    {
        T t;
        remember_fix_struct( const T & t ) : t( t ) { }
        struct type_erasure
        {
            virtual ~type_erasure( ) { }
            virtual boost::any process( const boost::any & ) = 0;
        };
        template< typename ... ARG >
        struct arg_erasure : type_erasure
        {
            remember_fix_struct const * that;
            arg_erasure( remember_fix_struct const * t ) : that( t ) { }
            typedef decltype(
                std::declval< T >( )(
                std::declval< T >( ),
                std::declval< ARG >( ) ... ) ) ret_type;
            typedef decltype( std::make_tuple( std::declval< ARG >( ) ... ) ) tuple_type;
            mutable std::map< tuple_type, ret_type > cache;
            template
            <
                typename ... EXPANDED,
                typename = std::enable_if_t
                <
                    std::tuple_size< std::tuple< EXPANDED ... > >::value <
                    std::tuple_size< std::tuple< ARG ... > >::value
                >
            >
            ret_type func( const std::tuple< ARG ... > & a, const EXPANDED & ... e )
            {
                return func(
                        a,
                        e ...,
                        std::get< std::tuple_size< std::tuple< EXPANDED ... > >::value >( a ) );
            }
            template
            <
                typename ... EXPANDED,
                std::enable_if_t
                <
                    std::tuple_size< std::tuple< EXPANDED ... > >::value ==
                    std::tuple_size< std::tuple< ARG ... > >::value,
                    bool
                > = true
            >
            ret_type func( const std::tuple< ARG ... > &, const EXPANDED & ... arg )
            {
                auto tuple = std::make_tuple( arg ... );
                if ( cache.count( tuple ) == 0 )
                {
                    cache[ tuple ] =
                    that->t(
                        [this](const ARG & ... arg_ )
                        { return (*that)( arg_ ... ); }, arg ... );
                }
                return cache[ tuple ];
            }
            virtual boost::any process( const boost::any & a )
            { return func( boost::any_cast< std::tuple< ARG ... > >( a ) ); }
        };
        mutable std::map< std::type_index, std::unique_ptr< type_erasure > > m;
        template< typename ... ARG >
        auto operator( )( const ARG & ... arg ) const
        {
            auto tuple = std::make_tuple( arg ... );
            std::type_index id( typeid( tuple ) );
            if ( m.count( id ) == 0 )
            {
                m.insert( std::make_pair(
                              id,
                              std::unique_ptr< type_erasure >( new arg_erasure< ARG ... >( this ) ) ) );
            }
            return boost::any_cast< decltype( t( t, std::declval< ARG >( ) ... ) ) >
                    ( m[ id ]->process( boost::any( tuple ) ) );
        }
    };

    template< typename T >
    remember_fix_struct< T > remember_fix( const T & t ) { return remember_fix_struct< T >( t ); }
}
#endif // COMBINATOR_HPP
