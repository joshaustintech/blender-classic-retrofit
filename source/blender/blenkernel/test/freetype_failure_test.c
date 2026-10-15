/* Regression for a FreeType face creation failure. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BKE_global.h"
#include "BKE_font.h"
#include "BKE_utildefines.h"
#include "BLI_blenlib.h"
#include "BLI_vfontdata.h"
#include "DNA_packedFile_types.h"
#include "DNA_vfont_types.h"

int BLI_vfontchar_from_freetypefont(VFont *vfont, unsigned long character);
char bprogname[FILE_MAXDIR + FILE_MAXFILE];

static int read_font_file(const char *path, PackedFile *packed_file)
{
	FILE *stream;
	long size;

	stream = fopen(path, "rb");
	if (!stream) return 0;
	if (fseek(stream, 0, SEEK_END) != 0 || (size = ftell(stream)) <= 0 ||
	    fseek(stream, 0, SEEK_SET) != 0) {
		fclose(stream);
		return 0;
	}
	packed_file->data = (unsigned char *)malloc(size);
	if (!packed_file->data || fread(packed_file->data, 1, size, stream) != (size_t)size) {
		free(packed_file->data);
		fclose(stream);
		return 0;
	}
	packed_file->size = (int)size;
	fclose(stream);
	return 1;
}

int main(int argc, char **argv)
{
	VFont vfont;
	VFontData data;
	struct TmpFont temporary;
	PackedFile packed;
	VChar *glyph;
	VFontData *valid_data;
	VFont valid_vfont;
	PackedFile valid_font;
	char invalid_font[] = "not a font";
	float valid_width;

	memset(&valid_font, 0, sizeof(valid_font));
	memset(&valid_vfont, 0, sizeof(valid_vfont));
	if (argc != 2 || !read_font_file(argv[1], &valid_font)) {
		fprintf(stderr, "usage: freetype_failure_test path/to/bfont.ttf\n");
		return 1;
	}

	memset(&G, 0, sizeof(G));
	memset(&vfont, 0, sizeof(vfont));
	memset(&data, 0, sizeof(data));
	memset(&temporary, 0, sizeof(temporary));
	memset(&packed, 0, sizeof(packed));
	vfont.data = &data;
	temporary.vfont = &vfont;
	temporary.pf = &packed;
	packed.data = (unsigned char *)invalid_font;
	packed.size = sizeof(invalid_font) - 1;
	G.ttfdata.first = G.ttfdata.last = &temporary;

	if (BLI_vfontchar_from_freetypefont(&vfont, 'A') != 0) {
		fprintf(stderr, "malformed font unexpectedly produced a glyph\n");
		return 1;
	}

	/* An existing temporary font with no retained bytes is another failure. */
	temporary.pf = NULL;
	if (BLI_vfontchar_from_freetypefont(&vfont, 'A') != 0) {
		fprintf(stderr, "missing packed font unexpectedly produced a glyph\n");
		return 1;
	}

	valid_data = BLI_vfontdata_from_freetypefont(&valid_font);
	if (!valid_data) {
		fprintf(stderr, "bundled valid font failed to load\n");
		free(valid_font.data);
		return 1;
	}
	valid_vfont.data = valid_data;
	glyph = valid_data->characters.first;
	while (glyph && glyph->index != 'A') glyph = glyph->next;
	if (!glyph || glyph->width <= 0.0f || !glyph->nurbsbase.first) {
		fprintf(stderr, "bundled font outline/metrics missing for A\n");
		free_vfont(&valid_vfont);
		free(valid_font.data);
		return 1;
	}
	valid_width = glyph->width;
	if (valid_width < 0.4811116f || valid_width > 0.4811137f) {
		fprintf(stderr, "bundled font A width changed: %.9g\n", valid_width);
		free_vfont(&valid_vfont);
		free(valid_font.data);
		return 1;
	}
	printf("valid font glyph A width: %.9g\n", valid_width);
	free_vfont(&valid_vfont);
	free(valid_font.data);

	return 0;
}
