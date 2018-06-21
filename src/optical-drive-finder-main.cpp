// From https://developer.apple.com/library/mac/documentation/DeviceDrivers/Conceptual/WorkingWStorage/WWStorage_StorageDevs/StorageDevFiles.html

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <sys/param.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOCDTypes.h>
#include <IOKit/storage/IOMediaBSDClient.h>
#include <CoreFoundation/CoreFoundation.h>

void MyCloseDrive( int fileDescriptor )
{
    close( fileDescriptor );
}

Boolean MyReadSector( int fileDescriptor )
{
    char *buffer;
    size_t numBytes;
    u_int32_t blockSize;

    if ( ioctl( fileDescriptor, DKIOCGETBLOCKSIZE, &blockSize ) == -1)
    {
        perror( "Error getting preferred block size." );
        // Set a reasonable block size instead.
        // kCDSectorSizeCDDA is defined in IOCDTypes.h as 2352.
        blockSize = kCDSectorSizeCDDA;
    }
    buffer = (char*) malloc( blockSize );
    numBytes = read( fileDescriptor, buffer, blockSize );
    free( buffer );
    return numBytes == blockSize ? true : false;
}


int MyOpenDrive( const char *deviceFilePath )
{
    int fileDescriptor;

    fileDescriptor = open( deviceFilePath, O_RDONLY );
    if ( fileDescriptor == -1 )
    {
        printf( "Error opening device %s: \n", deviceFilePath );
        perror( NULL );
    }

    return fileDescriptor;
}


kern_return_t MyGetDeviceFilePath( io_iterator_t mediaIterator,
                        char *deviceFilePath, CFIndex maxPathSize )
{
    io_object_t nextMedia;
    kern_return_t kernResult = KERN_FAILURE;

    *deviceFilePath = '\0';
    nextMedia = IOIteratorNext( mediaIterator );
    if ( nextMedia )
    {
        CFTypeRef   deviceFilePathAsCFString;
        deviceFilePathAsCFString = IORegistryEntryCreateCFProperty(
                                nextMedia, CFSTR( kIOBSDNameKey ),
                                kCFAllocatorDefault, 0 );
       *deviceFilePath = '\0';
        if ( deviceFilePathAsCFString )
        {
            size_t devPathLength;
            strcpy( deviceFilePath, _PATH_DEV );
            // Add "r" before the BSD node name from the I/O Registry
            // to specify the raw disk node. The raw disk node receives
            // I/O requests directly and does not go through the
            // buffer cache.
            strcat( deviceFilePath, "r");
            devPathLength = strlen( deviceFilePath );
            if ( CFStringGetCString( (CFStringRef) deviceFilePathAsCFString,
                                     deviceFilePath + devPathLength,
                                     maxPathSize - devPathLength,
                                     kCFStringEncodingASCII ) )
            {
                printf( "BSD path: %s\n", deviceFilePath );
                kernResult = KERN_SUCCESS;
            }
            CFRelease( deviceFilePathAsCFString );
        }
    }
    IOObjectRelease( nextMedia );
    return kernResult;
}


kern_return_t MyFindEjectableCDMedia( io_iterator_t *mediaIterator )
{
    mach_port_t         masterPort;
    kern_return_t       kernResult;
    CFMutableDictionaryRef   classesToMatch;

    kernResult = IOMasterPort( MACH_PORT_NULL, &masterPort );
    if ( kernResult != KERN_SUCCESS )
    {
        printf( "IOMasterPort returned %d\n", kernResult );
        return kernResult;
    }
    // CD media are instances of class kIOCDMediaClass.
    classesToMatch = IOServiceMatching( kIOMediaClass );
    if ( classesToMatch == NULL )
        printf( "IOServiceMatching returned a NULL dictionary.\n" );
    else
    {
        // Each IOMedia object has a property with key kIOMediaEjectableKey
        // which is true if the media is indeed ejectable. So add this
        // property to the CFDictionary for matching.
        CFDictionarySetValue( classesToMatch,
                        CFSTR( kIOMediaEjectableKey ), kCFBooleanTrue );
    }
    kernResult = IOServiceGetMatchingServices( masterPort,
                                classesToMatch, mediaIterator );
    if ( (kernResult != KERN_SUCCESS) || (*mediaIterator == 0) )
        printf( "No ejectable CD media found.\n kernResult = %d\n",
                    kernResult );

    return kernResult;
}

// this function was main() in the original source listing
int main( void )
{
    int status = 0;

    kern_return_t kernResult;
    io_iterator_t mediaIterator;
    char deviceFilePath[ MAXPATHLEN ];

    kernResult = MyFindEjectableCDMedia( &mediaIterator );
    if ( kernResult != KERN_SUCCESS ) {
        printf( "MyFindEjectableCDMedia returned %d\n", kernResult );
        return kernResult;
    }

    kernResult = MyGetDeviceFilePath( mediaIterator, deviceFilePath,
                    sizeof( deviceFilePath ) );
    if ( kernResult != KERN_SUCCESS ) {
        printf( "MyGetDeviceFilePath returned %d\n", kernResult );
        return kernResult;
    }

    // Now open the device we found, read a sector, and close the device.
    if ( deviceFilePath[ 0 ] != '\0' )
    {
        int fileDescriptor;

        fileDescriptor = MyOpenDrive( deviceFilePath );
        if (fileDescriptor != -1 )
        {
            if ( MyReadSector( fileDescriptor ) )
                printf( "Sector read successfully.\n" );
            else {
                printf("Could not read sector.\n");
                status = 1;
            }

            MyCloseDrive( fileDescriptor );
            printf( "Device closed.\n" );
        }
    }
    else {
        printf("No ejectable CD media found.\n");
        status = 1;
    }

    // Release the iterator.
    IOObjectRelease( mediaIterator );

    return status;
}
