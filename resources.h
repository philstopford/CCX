/*
 *  resources.h
 *  CCX
 *
 *  Created by Phil on 11/09/2009.
 *
 */

#include "MTypes.h"

MBoolean FileExists( const char *name );
MBoolean PICTExists( int pictID );
const char* QuickResourceName( const char* prefix, int id, const char* extension );
void InitResources();
#if defined(__APPLE__)
bool initializeBundleLoading();
char *absoluteBundleResourcesPath(const char *name_of_file_in_bundle);
#endif
