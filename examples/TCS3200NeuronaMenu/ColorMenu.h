#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#define MAX_DEPTH 5

using namespace Menu;

// ---  Define INPUTS ---
serialIn serial(Serial);

MENU_OUTPUTS(out, MAX_DEPTH
  ,SERIAL_OUT(Serial)
  ,NONE
);
