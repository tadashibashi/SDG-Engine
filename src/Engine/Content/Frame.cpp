/* =============================================================================
 * Frame
 * 
 * 
 * ===========================================================================*/
#include "Frame.h"
#include <Engine/Log.h>

void
SDG::Frame::Log() const
{
    SDG_CORE_LOG("\n===== Frame: \"{0}\"\n"
                   "x: {1}, y: {2}, w: {3}, h: {4},\n"
                   "ox: {5}, oy: {6}, ow: {7}, oh: {8},\n"
                   "pivX: {9}, pivY: {10},\n"
                   "rotated: {11}",
                   key, x, y, w, h, ox, oy, ow, oh, pivX, pivY,
                   (rotated) ? "true" : "false");
}