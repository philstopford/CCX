// prefs.h

void LoadPrefs( void );
void SavePrefs( void );

#ifdef __APPLE__ && newprefs
SDLKey ASCII_SDLKey(int value);
void LoadPrefsMac();
void SavePrefsMac();
#endif

#ifndef newprefs
unsigned char *FindPrefsLine( unsigned char *prefsText, long prefsLength, long searchCode, long dataQuantity );

typedef struct
{
	long itemName;
	void *itemPointer;
	short size;
}
PrefList;
#endif
