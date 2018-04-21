#include "Http_CatalogRequest.hpp"
#include "../Http/Http_Server.hpp"

#include <iostream>
#include <cstring>

namespace Http
{
    CatalogRequest::CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address )
        : Request ( sock, address ), mpserver ( nullptr ), mcursor ( 0 )
    {
        std::cout << * this << " : connection established" << std::endl;
    }

    CatalogRequest::~CatalogRequest ( )
    {
        if ( mpserver )
        {
            mpserver -> mrequests.erase ( this );
        }
    }

    bool CatalogRequest::writeEventAction ( )
    {
        if ( reading )
        {
            delete this;
            return false;
        }

        const char * data = mpserver -> mcatalog.c_str ( );
        size_t length = mpserver -> mcatalog.length ( );

        ssize_t sent = send ( data + mcursor, length - mcursor );

        if ( sent == -1 )
        {
            std::cout << * this << " : send failed -> killed" << std::endl;
            delete this;
            return false;
        }

        mcursor += sent;

        if ( mcursor >= length )
        {
            std::cout << * this << " : catalog sent (" << length << " bytes) -> closing connection" << std::endl;
            delete this;
            return false;
        }

        return true;
    }

    bool CatalogRequest::exceptEventAction ( )
    {
        std::cout << * this << " : unexpected exception -> killed" << std::endl;
        delete this;
        return false;
    }

    bool CatalogRequest::timeoutEventAction ( )
    {
        std::cout << * this << " : hasn't talked for a long time -> killed" << std::endl;
        delete this;
        return false;
    }

    void CatalogRequest::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        Vod::Request::chooseSubscription ( event );
        event.setTimeout ( 5000000 );
    }

    bool CatalogRequest::requestEventAction ( )
    {
        reading = false;
        return true;
    }

    void CatalogRequest::toString ( std::ostream & os ) const
    {
        os << "Catalog Request socket ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
}
