#include <contrib/simpleweb/client_http.hpp>
#include <contrib/simpleweb/server_http.hpp>
#include <contrib/simpleweb/status_code.hpp>
#include <contrib/simpleweb/utility.hpp>
#include <contrib/simpleweb/client_ws.hpp> //need crytpo
#include <contrib/simpleweb/server_ws.hpp>

#ifdef CHENHAO_HAVE_OPENSSL // unused, if define as HAVE_OPENSSL, it will actived when found libopenssl from CMake anyway.
#include <contrib/simpleweb/client_https.hpp>
#include <contrib/simpleweb/client_wss.hpp>
#include <contrib/simpleweb/crypto.hpp>
#include <contrib/simpleweb/server_https.hpp>
#include <contrib/simpleweb/server_wss.hpp>
#endif
