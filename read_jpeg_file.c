#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

unsigned char *raw_image = NULL;

int width = 320;
int height = 240;
int bytes_per_pixel = 3;   /* or 1 for GRACYSCALE images */
int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

int read_jpeg_file( char *filename )
{
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        JSAMPROW row_pointer[1];
        
        FILE *infile = fopen( filename, "rb" );
        unsigned long location = 0;
        int i = 0;
        
        if ( !infile )
        {
                printf("Error opening jpeg file %s\n!", filename );
                return -1;
        }

        cinfo.err = jpeg_std_error( &jerr );

        jpeg_create_decompress( &cinfo );

        jpeg_stdio_src( &cinfo, infile );

        jpeg_read_header( &cinfo, TRUE );


        jpeg_start_decompress( &cinfo );
        printf("components = %d\n", cinfo.num_components);

        raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );

        row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );

        while( cinfo.output_scanline < cinfo.image_height )
        {
                jpeg_read_scanlines( &cinfo, row_pointer, 1 );
                for( i=0; i<cinfo.image_width*cinfo.num_components;i++) 
                        raw_image[location++] = row_pointer[0][i];
        }

        jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        free( row_pointer[0] );
        fclose( infile );

        return 1;
}

