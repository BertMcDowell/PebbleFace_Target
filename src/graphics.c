#include <pebble.h>
#include "graphics.h"

static InverterLayer* g_inverter_layer=NULL;
  
void add_invert(Layer * parent)
{
    if (!g_inverter_layer) 
    {
		  g_inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
		  layer_add_child(parent, inverter_layer_get_layer(g_inverter_layer));
		  layer_mark_dirty(inverter_layer_get_layer(g_inverter_layer));
    }
}

void remove_invert()
{
    if (g_inverter_layer != NULL) 
    {
		  layer_remove_from_parent(inverter_layer_get_layer(g_inverter_layer));
		  inverter_layer_destroy(g_inverter_layer);
		  g_inverter_layer = NULL;
    }
}