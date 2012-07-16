/*
 *   resources.cpp/.h were created to contain the resource loading/handling code. This should abstract away most platform issues and allow generic approaches.
 */

// #include "SDLU.h"
#include "SDL.h"
#include "SDL_image.h"
#include "main.h"
#include "resources.h"
#include <string.h>
#include <stdlib.h>


#if defined(__APPLE__) && !defined(TARGET_API_MAC_CARBON)
#include <string>
#include <cstdlib>
#endif

#if defined(__APPLE__)

static char candyCrisisResources[PATH_MAX];

// This is required in order to access the app bundle's resources.
// resourcesURL ends up containing a Posix format path to the resources folder.
// For debug purposes, we dump the resource location right now - this will go away
// once robust 'missing resource' handling is in place. If resources are missing at the
// moment, we get a Bus Error - which is ugly.
#include "CoreFoundation/CFBundle.h"
CFBundleRef mainBundle;
CFURLRef resourcesURL;

// return value handled by caller to exit cleanly if bundle blows up.
bool initializeBundleLoading() {
	mainBundle = CFBundleGetMainBundle();
	if(!mainBundle) {
		printf("Bundle not located. This will end badly.\n");
		CFRelease(mainBundle);
		return false;
	}
	resourcesURL = CFBundleCopyResourceURL(mainBundle,NULL,NULL,NULL);
	if(!CFURLGetFileSystemRepresentation(resourcesURL, true, (UInt8 *)candyCrisisResources, PATH_MAX)) {
		printf("Bundle resource not located. This will end badly.\n");
		CFRelease(mainBundle);
		CFRelease(resourcesURL);
		return false;
	}
//	printf("Bundle resource is at : %s\n", candyCrisisResources);
	CFRelease(mainBundle);
	CFRelease(resourcesURL);
	return true;
}

// End of Bundle handling chunk.
#endif

// Called from main() and sets up our resource location.
// Made a separate function to provide for expansion over time.
void InitResources() {
#if defined(__APPLE__)
	// Init bundle so that we can use the .app's resources. OS X-only
	if (!initializeBundleLoading()) {
		// Bundle initialisation blew up. We need to exit.
		printf("initializeBundleLoading() failed!\nExiting.\n");
		exit(1);
	}
#endif
}

MBoolean FileExists( const char *name )
{
	FILE* f = fopen( name, "rb" );
	if( f == NULL )
	{
//		printf("File %s not found\n", name);
		return false;
	}
	fclose( f );
	return true;
	
}

MBoolean PICTExists( int pictID )
{
	if( FileExists( QuickResourceName( "PICT", pictID, ".jpg" ) ) )
		return true;
	if( FileExists( QuickResourceName( "PICT", pictID, ".png" ) ) )
		return true;
	else
	{
		// SDL_Image > 1.2.7 error triage.
//		printf("Oh My Goodness, an error : %s\n", IMG_GetError());
		printf("Resource %s not found\n", QuickResourceName( "PICT", pictID, ".jpg/.png" ));
		return false;
	}
}

// This function simply concatenates the prefix, id (if defined) and extension
const char* QuickResourceName( const char* prefix, int id, const char* extension )
{	
	static char name[512];
	if( id ) 
	{
		sprintf( name, "%s/%s_%d%s", candyCrisisResources, prefix, id, extension );
	}
	else
	{
		sprintf( name, "%s/%s%s", candyCrisisResources, prefix, extension );
	}
	
	return name;
		
}


