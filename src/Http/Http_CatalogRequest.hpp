#ifndef CATALOG_REQUEST_HPP_
#define CATALOG_REQUEST_HPP_

#include "../Vod/Vod_Request.hpp"

namespace Http
{
    class Server;

    class CatalogRequest : public Vod::Request
    {
    public:
        CatalogRequest ( int sock, const NetFlux::Net::InetAddress & address );
        ~CatalogRequest ( );

    protected:
        virtual bool requestEventAction ( );
        virtual bool writeEventAction ( );
        virtual bool exceptEventAction ( );

        virtual bool timeoutEventAction ( );
        virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

        void toString ( std::ostream & os ) const;

        Http::Server * mpserver;
        uint32_t mcursor;

    private:
        CatalogRequest ( ) = delete;
        CatalogRequest ( const CatalogRequest & ) = delete;
        CatalogRequest & operator= ( const CatalogRequest & ) = delete;


        friend class Server;
    };
}

#endif

