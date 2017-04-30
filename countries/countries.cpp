// countries.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <time.h>

#define NUM_COUNTRIES	2

struct CountryData {
	char *name;
	float lon, lat;
};

CountryData cdat[NUM_COUNTRIES] = { {"momo", 1, 1}, {"werwer", 2, 2}};


int main(int argc, char* argv[])
{
	struct _finddata_t	c_file;
	long				hFile;

	FILE	*f = fopen("countries.h", "wt");

	fputs("// country name and positions\n\n", f);
	fputs("#define NUM_COUNTRIES	267\n\n", f);
	fputs("struct Country {\n", f);
	fputs("    char *name;\n", f);
	fputs("    float lon, lat;\n", f);
	fputs("};\n\n", f);
	fputs("Country countries[NUM_COUNTRIES] = {\n", f);


    if( (hFile = _findfirst( "countries\\*.html", &c_file )) != -1L ) {
		char *buf;
		buf = new char[1024*100];
		do {
			FILE *f2;
			char fname[128];
			sprintf(fname, "countries\\%s", c_file.name);
			printf("%s\n",c_file.name);
			f2 = fopen(fname, "rt");
			fread(buf, c_file.size, 1, f2);
			fclose(f2);
			// inside file

			if (strstr(c_file.name, "us.html")) {
				f2 = f2;
			}
			
			// find country name
			char *pname;
			pname = strstr(buf, "<title>CIA");
			pname = strstr(pname, "k -- ")+5;
			*strchr(pname, '<') = 0;

			fputs("\n    {\"", f);
			fputs(pname, f);
			fputs("\", ", f);

			pname += strlen(pname)+1;
			pname = strstr(pname, "Geographic coordinates:");
			pname = strstr(pname, "\"2\">")+4;
			*strchr(pname, '<') = 0;

			int numbers[4];
			char letters[2];

			sscanf(pname, "%d %d %c, %d %d %c",	&numbers[0], &numbers[1], &letters[0],
												&numbers[2], &numbers[3], &letters[1]);												

			float lon, lat;

			lat = (float)numbers[0] + ( (float)numbers[1] / 60.f );
			lon = (float)numbers[2] + ( (float)numbers[3] / 60.f );

			if (letters[0] == 'S') {
				lat *= -1.f;
			}

			if (letters[1] == 'W') {
				lon *= -1.f;
			}

			fprintf(f, "%2.4f, %2.4f", lat, lon);

			fputs("},", f);
			
			// done with file
		} while( _findnext( hFile, &c_file ) == 0 );
		delete buf;
		_findclose( hFile );
	}

	fputs("};\n", f);

	fclose(f);

	return 0;
}
