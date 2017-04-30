// i dont use or compile this file, i just put this source aside
// because i wanted to take it out of the project (Since i don't use it anymore)
// but i don't use this because i've already converted all the data

struct Vector {
	float x, y, z;
};


// vector-file
struct VxpFeature {
	int		 nVerts;
	Vector	*pVerts;
};

struct Vxp {
	int			 nFeatures;
	VxpFeature	*pFeatures;
};


// reads a vxp ascii format and writes a bvd binary file
void conv_vxp(char *pFileName, char *pDestFile)
{
	Vxp *vxp = new Vxp;

	FILE	*f;
	int		size;
	char	*buf;

	// read file
	f = fopen(pFileName, "rb");
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = new char[size+1];
	fread(buf, size, 1, f);
	buf[size] = 0;
	fclose(f);

	f = fopen(pDestFile, "wb");

	char	*p1;
	float	unitSize;

	// read units
	p1 = strstr(buf, "Unit Distance");
	p1 = strstr(p1, ":")+2;
	sscanf(p1, "%f", &unitSize);

	// read number of features
	p1 = strstr(p1, "Number of Features");
	p1 = strstr(p1, ":")+2;
	sscanf(p1, "%d", &vxp->nFeatures);

	vxp->pFeatures = new VxpFeature[vxp->nFeatures];

	// write to binary file
	fwrite(&vxp->nFeatures, 4, 1, f);

	// simply scan
	for (int i=0; i< vxp->nFeatures; i++) {
		// read the next feature
		printf("reading feature %4d of %4d\n", i, vxp->nFeatures);

		// read number of vertices
		p1 = strstr(p1, "Number of Vertices")+20;
		sscanf(p1, "%d", &vxp->pFeatures[i].nVerts);

		// allocate vertices
		vxp->pFeatures[i].pVerts = new Vector[ vxp->pFeatures[i].nVerts ];

		// place p1
		p1 = strstr(p1, "Coordinates")+20;

		// write to binary file
		fwrite(&vxp->pFeatures[i].nVerts, 4, 1, f);

		// scan vertices
		for (int j=0; j<vxp->pFeatures[i].nVerts; j++) {
			float a, b;

			sscanf(p1, "%f %f", &a, &b);
			p1 = strstr(p1, ":")+1;

			a *= unitSize;
			b *= unitSize;

			// write to binary file
			fwrite(&a, 4, 1, f);
			fwrite(&b, 4, 1, f);
		}
	}

	fclose(f);
}
