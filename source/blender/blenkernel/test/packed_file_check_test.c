/* Regression test for repeated packed-file comparisons. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "BKE_global.h"
#include "BKE_packedFile.h"
#include "BKE_utildefines.h"
#include "BLI_blenlib.h"
#include "DNA_packedFile_types.h"
#include "DNA_scene_types.h"
#include "MEM_guardedalloc.h"

char bprogname[FILE_MAXDIR + FILE_MAXFILE];

static int open_descriptor_count(void)
{
	int fd, count = 0;
	int limit = getdtablesize();

	for (fd = 0; fd < limit; fd++) {
		if (fcntl(fd, F_GETFD) != -1) {
			count++;
		}
	}
	return count;
}

static int write_contents(const char *path, const char *contents, size_t size)
{
	int fd = open(path, O_WRONLY | O_TRUNC);
	ssize_t written;

	if (fd < 0) {
		return 0;
	}
	written = write(fd, contents, size);
	close(fd);
	return written == (ssize_t)size;
}

int main(void)
{
	Scene scene;
	char equal_path[] = "/tmp/blender-packed-equal-XXXXXX";
	char different_path[] = "/tmp/blender-packed-different-XXXXXX";
	char missing_path[] = "/tmp/blender-packed-missing-XXXXXX";
	char truncated_path[] = "/tmp/blender-packed-truncated-XXXXXX";
	const char packed_data[] = "packed payload";
	const char different_data[] = "changed payload";
	PackedFile *packed_file;
	int equal_fd = -1, different_fd = -1, missing_fd = -1, truncated_fd = -1;
	int baseline, index, result = 1;
	int equal_status, different_status, missing_status, truncated_status;

	memset(&scene, 0, sizeof(scene));
	G.scene = &scene;
	G.sce[0] = '\0';

	equal_fd = mkstemp(equal_path);
	different_fd = mkstemp(different_path);
	missing_fd = mkstemp(missing_path);
	truncated_fd = mkstemp(truncated_path);
	if (equal_fd < 0 || different_fd < 0 || missing_fd < 0 || truncated_fd < 0) {
		goto cleanup;
	}
	close(equal_fd);
	close(different_fd);
	close(missing_fd);
	close(truncated_fd);
	equal_fd = different_fd = missing_fd = truncated_fd = -1;
	unlink(missing_path);

	if (!write_contents(equal_path, packed_data, sizeof(packed_data) - 1) ||
	    !write_contents(different_path, different_data, sizeof(different_data) - 1) ||
	    !write_contents(truncated_path, packed_data, sizeof(packed_data) - 2)) {
		goto cleanup;
	}

	packed_file = newPackedFileMemory(
	    MEM_mallocN(sizeof(packed_data) - 1, "packed-file-test"), sizeof(packed_data) - 1);
	if (!packed_file) {
		goto cleanup;
	}
	memcpy(packed_file->data, packed_data, sizeof(packed_data) - 1);

	equal_status = checkPackedFile(equal_path, packed_file);
	different_status = checkPackedFile(different_path, packed_file);
	missing_status = checkPackedFile(missing_path, packed_file);
	truncated_status = checkPackedFile(truncated_path, packed_file);
	if (equal_status != PF_EQUAL || different_status != PF_DIFFERS || missing_status != PF_NOFILE ||
	    truncated_status != PF_DIFFERS) {
		fprintf(stderr, "packed-file status mismatch: equal=%d different=%d missing=%d truncated=%d\n",
		        equal_status, different_status, missing_status, truncated_status);
		freePackedFile(packed_file);
		goto cleanup;
	}

	baseline = open_descriptor_count();
	for (index = 0; index < 100; index++) {
		if (checkPackedFile(equal_path, packed_file) != PF_EQUAL) {
			freePackedFile(packed_file);
			goto cleanup;
		}
	}
	if (open_descriptor_count() != baseline) {
		fprintf(stderr, "packed-file descriptor count grew from %d to %d\n",
		        baseline, open_descriptor_count());
		freePackedFile(packed_file);
		goto cleanup;
	}

	freePackedFile(packed_file);
	result = 0;

cleanup:
	if (equal_fd >= 0) close(equal_fd);
	if (different_fd >= 0) close(different_fd);
	if (missing_fd >= 0) close(missing_fd);
	if (truncated_fd >= 0) close(truncated_fd);
	if (equal_path[0] != '\0') unlink(equal_path);
	if (different_path[0] != '\0') unlink(different_path);
	if (missing_path[0] != '\0') unlink(missing_path);
	if (truncated_path[0] != '\0') unlink(truncated_path);
	return result;
}
