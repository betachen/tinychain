#include <tinychain/tinychain.hpp>
#include <tinychain/network.hpp>
#include <mongoose/mongoose.h>

namespace tinychain
{
//using namespace mgbubble;
//
static std::string  string_msg;

static int s_done = 0;
static int s_is_connected = 0;

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;
  (void) nc;

  switch (ev) {
    case MG_EV_CONNECT: {
      int status = *((int *) ev_data);
      if (status != 0) {
        printf("-- Connection error: %d\n", status);
      }
      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      printf("-- Connected\n");
      s_is_connected = 1;
      break;
    }
    case MG_EV_POLL: {
       log::info("chenhao") << string_msg;
      mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, string_msg.c_str(), string_msg.size());
      string_msg = "";
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      printf("%.*s\n", (int) wm->size, wm->data);
      break;
    }
    case MG_EV_CLOSE: {
      if (s_is_connected) printf("-- Disconnected\n");
      s_done = 1;
      break;
    }
  }
}


void ws_send(std::string&& msg) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *chat_server_url =  "ws://127.0.0.1:8000";

  mg_mgr_init(&mgr, NULL);

  string_msg = msg;
  nc = mg_connect_ws(&mgr, ev_handler, chat_server_url, msg.c_str(), NULL);
  if (nc == nullptr) {
    log::error("network")<<"Invalid address";
    return;
  }

  while (!s_done) {
    mg_mgr_poll(&mgr, 100);
  }
  mg_mgr_free(&mgr);

}


} //tinychain

