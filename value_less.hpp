#ifndef VALUE_LESS
#define VALUE_LESS
#include <memory>
namespace misc
{
    template< typename t >
    struct value_less;

    template< typename t >
    struct value_less< std::shared_ptr< t > >
    {
        bool operator( )( const std::shared_ptr< t > & lhs, const std::shared_ptr< t > & rhs ) const
        { return * lhs < * rhs; }
    };
}
#endif //VALUE_LESS
