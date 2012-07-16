#include "SDLKeyLookup.h"

// Since SDL doesn't provide a useful way to do this, we need to do the conversion manually.
// The ints are ASCII values, so we simply send back the right SDLKey as needed.
// Values are all taken from the SDLKey system itself.
// To silence a compiler warning, there is SDLK_UNKNOWN returned if control passes right through.
SDLKey ASCII_SDLKey(int value) {
	switch (value) {
		case 8:
			return SDLK_BACKSPACE;
			break;
		case 9:
			return SDLK_TAB;
			break;
		case 12:
			return SDLK_CLEAR;
			break;			
		case 13:
			return SDLK_RETURN;
			break;
		case 19:
			return SDLK_PAUSE;
			break;
		case 27:
			return SDLK_ESCAPE;
			break;
		case 32:
			return SDLK_SPACE;
			break;
		case 33:
			return SDLK_EXCLAIM;
			break;
		case 34:
			return SDLK_QUOTEDBL;
			break;
		case 35:
			return SDLK_HASH;
			break;
		case 36:
			return SDLK_DOLLAR;
			break;
		case 38:
			return SDLK_AMPERSAND;
			break;
		case 39:
			return SDLK_QUOTE;
			break;
		case 40:
			return SDLK_LEFTPAREN;
			break;
		case 41:
			return SDLK_RIGHTPAREN;
			break;
		case 42:
			return SDLK_ASTERISK;
			break;
		case 43:
			return SDLK_PLUS;
			break;
		case 44:
			return SDLK_COMMA;
			break;
		case 45:
			return SDLK_MINUS;
			break;
		case 46:
			return SDLK_PERIOD;
			break;
		case 47:
			return SDLK_SLASH;
			break;
		case 48:
			return SDLK_0;
			break;
		case 49:
			return SDLK_1;
			break;
		case 50:
			return SDLK_2;
			break;
		case 51:
			return SDLK_3;
			break;
		case 52:
			return SDLK_4;
			break;
		case 53:
			return SDLK_5;
			break;
		case 54:
			return SDLK_6;
			break;
		case 55:
			return SDLK_7;
			break;
		case 56:
			return SDLK_8;
			break;
		case 57:
			return SDLK_9;
			break;
		case 58:
			return SDLK_COLON;
			break;
		case 59:
			return SDLK_SEMICOLON;
			break;
		case 60:
			return SDLK_LESS;
			break;
		case 61:
			return SDLK_EQUALS;
			break;
		case 62:
			return SDLK_GREATER;
			break;
		case 63:
			return SDLK_QUESTION;
			break;
		case 64:
			return SDLK_AT;
			break;
		case 91:
			return SDLK_LEFTBRACKET;
			break;
		case 92:
			return SDLK_BACKSLASH;
			break;
		case 93:
			return SDLK_RIGHTBRACKET;
			break;
		case 94:
			return SDLK_CARET;
			break;
		case 95:
			return SDLK_UNDERSCORE;
			break;
		case 96:
			return SDLK_BACKQUOTE;
			break;
		case 97:
			return SDLK_a;
			break;
		case 98:
			return SDLK_b;
			break;
		case 99:
			return SDLK_c;
			break;
		case 100:
			return SDLK_d;
			break;
		case 101:
			return SDLK_e;
			break;
		case 102:
			return SDLK_f;
			break;
		case 103:
			return SDLK_g;
			break;
		case 104:
			return SDLK_h;
			break;
		case 105:
			return SDLK_i;
			break;
		case 106:
			return SDLK_j;
			break;
		case 107:
			return SDLK_k;
			break;
		case 108:
			return SDLK_l;
			break;
		case 109:
			return SDLK_m;
			break;
		case 110:
			return SDLK_n;
			break;
		case 111:
			return SDLK_o;
			break;
		case 112:
			return SDLK_p;
			break;
		case 113:
			return SDLK_q;
			break;
		case 114:
			return SDLK_r;
			break;
		case 115:
			return SDLK_s;
			break;
		case 116:
			return SDLK_t;
			break;
		case 117:
			return SDLK_u;
			break;
		case 118:
			return SDLK_v;
			break;
		case 119:
			return SDLK_w;
			break;
		case 120:
			return SDLK_x;
			break;
		case 121:
			return SDLK_y;
			break;
		case 122:
			return SDLK_z;
			break;
		case 127:
			return SDLK_DELETE;
			break;
		case 273:
			return SDLK_UP;
			break;
		case 274:
			return SDLK_DOWN;
			break;
		case 275:
			return SDLK_RIGHT;
			break;
		case 276:
			return SDLK_LEFT;
			break;
		case 277:
			return SDLK_INSERT;
			break;
		case 278:
			return SDLK_HOME;
			break;
		case 279:
			return SDLK_END;
			break;
		case 280:
			return SDLK_PAGEUP;
			break;
		case 281:
			return SDLK_PAGEDOWN;
			break;
		default:
			// Simplify the world key handling conversion.
			if (159 < value < 256 ) {
				value = value-160;
				switch (value) {
					case 0:
						return SDLK_WORLD_0;
						break;
					case 1:
						return SDLK_WORLD_1;
						break;
					case 2:
						return SDLK_WORLD_2;
						break;
					case 3:
						return SDLK_WORLD_3;
						break;
					case 4:
						return SDLK_WORLD_4;
						break;
					default:
						break;
				}
			}
			break;
	}
	return SDLK_UNKNOWN;
}