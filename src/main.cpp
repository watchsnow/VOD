#include "Vod/Vod_Server.hpp"
#include <cstdlib>

static Vod::Server * vodServer = nullptr;

#ifndef _WIN32
#include <csignal>
static void __attribute__ ( ( noreturn ) ) intHandler ( int )
{
    if ( vodServer )
    {
        delete vodServer;
    }
    exit ( EXIT_SUCCESS );
}
#endif

int main ( )
{
#ifndef _WIN32
    struct sigaction act;
    memset ( &act, 0, sizeof ( act ) );
    act.sa_handler = intHandler;
    sigaction ( SIGINT, &act, 0 );
#endif

    vodServer = new Vod::Server;
    if ( ! vodServer -> start ( ) )
    {
        delete vodServer;
        return EXIT_FAILURE;
    }

    delete vodServer;
    return EXIT_SUCCESS;
}
