#include <SDL3/SDL.h>
#include "core/types.h"

#define X_FRAC_ERR  0.1f 
#define Y_FRAC_ERR  0.6f  
#define W_FRAC_ERR  0.8f  
#define H_FRAC_ERR  0.1f   

void GUI_create_an_error(App *app, const char *message);
void render_error(App *app);
