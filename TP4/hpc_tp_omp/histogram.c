
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>


/* Wrapper to check for errors */
#define CHECK_ERROR(a)                        \
if (a) {                                      \
    fprintf(stderr, "*** Error :\n" #a "\n"); \
    exit(1);                                  \
}



#define DEFAULT_NTHREADS 1

#define IMG_DATA_OFFSET_POS 10
#define BITS_PER_PIXEL_POS 28
#define PAGE_SIZE 0x1000

#define array_size(tab) (sizeof(tab) / sizeof(tab[0]))


typedef void (*calc_func_t)(uint8_t *, int32_t, int32_t [256], int32_t [256], int32_t [256]);



int64_t clocktime() {
   struct timeval full_time;

   gettimeofday(&full_time, NULL);
   return (int64_t) ((full_time.tv_usec + full_time.tv_sec * 1000000) / 1000);
}


/**
 * test_endianess
 */
static bool test_endianess() {
    uint32_t num = 0x12345678;
    uint8_t * low = (uint8_t *) (&num);
    if (*low ==  0x78) {
        return false;
    }
    else if (*low == 0x12) {
        return true;
    }
    else {
        fprintf(stderr, "*** Error: Invalid value found in memory\n");
        exit(1);
    }
}


/**
 * swap_bytes
 */
static void swap_bytes(uint8_t * bytes, int32_t num_bytes) {
    for (int32_t i = 0; i < num_bytes / 2; i++) {
        printf("Swapping %d and %d\n", bytes[i], bytes[num_bytes - i - 1]);
        uint8_t tmp = bytes[i];
        bytes[i] = bytes[num_bytes - i - 1];
        bytes[num_bytes - i - 1] = tmp;
    }
}


static void init_histogram(int32_t red[256], int32_t gre[256], int32_t blu[256]) {
    memset(&red[0], 0, sizeof(int32_t) * 256);
    memset(&gre[0], 0, sizeof(int32_t) * 256);
    memset(&blu[0], 0, sizeof(int32_t) * 256);
}


/* *
 * calc_seq
 * Sequential computation of the histogram
 * Serves as a reference result
 */
void calc_seq(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {
    for (int32_t i = 0; i < size; i += 3) {
        blu[data[i]] += 1;
        gre[data[i + 1]] += 1;
        red[data[i + 2]] += 1;
    }
}


void calc_omp0(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}


void calc_omp_atomic_inc(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}


void calc_omp_critical(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}


void calc_omp_atomic(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}


void calc_omp_reduction(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}


void calc_omp_private_atomic(uint8_t * data, int32_t size, int32_t red[256], int32_t gre[256], int32_t blu[256]) {

}



static bool check_results(int32_t red[256], int32_t gre[256], int32_t blu[256], int32_t red_ref[256], int32_t gre_ref[256], int32_t blu_ref[256]) {
    for (int32_t i = 0; i < 256; i += 1) {
        if (red[i] != red_ref[i] || blu[i] != blu_ref[i] || gre[i] != gre_ref[i]) {
            return false;
        }
    }
    return true;
}


static void usage(char * name) {
    printf("Usage: %s <options> <bitmap filename>\n", name);
    printf("options:\n");
    printf("   -n <num_threads> : Number of threads (default = %d).\n", DEFAULT_NTHREADS);
    printf("   -d               : Dump results for plotting in file 'data.txt' (default = false)\n");
}


int32_t main(int32_t argc, char * argv[]) {

    int32_t fd;
    uint8_t * fdata;
    struct stat finfo;
    char * fname;

    int32_t red[256];
    int32_t gre[256];
    int32_t blu[256];

    int32_t red_ref[256];
    int32_t gre_ref[256];
    int32_t blu_ref[256];

    bool dump_results = false;
    int32_t num_threads = DEFAULT_NTHREADS;

    calc_func_t func_tab[] = {  calc_omp0,   calc_omp_atomic_inc,   calc_omp_critical,   calc_omp_atomic,   calc_omp_reduction,   calc_omp_private_atomic, };
    char * func_name[]     = { "calc_omp0", "calc_omp_atomic_inc", "calc_omp_critical", "calc_omp_atomic", "calc_omp_reduction", "calc_omp_private_atomic" };

    int32_t min_len = 1;
    for (int32_t i = 0; i < array_size(func_name); i += 1) {
        if (strlen(func_name[i]) > min_len) {
            min_len = strlen(func_name[i]);
        }
    }

    int32_t ch;

    while ((ch = getopt(argc, argv, "n:hd")) != EOF) {
        switch (ch) {
            case 'n':
                num_threads = atol(optarg);
                if (num_threads < 1) {
                    fprintf(stderr, "*** Error: the number of threads should be a positive integer\n");
                    exit(1);
                }
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case 'd':
                dump_results = true;
                break;
            default:
                usage(argv[0]);
                exit(1);
                break;
        }
    }

    #ifdef _OPENMP
    omp_set_num_threads(num_threads);
    #else
    fprintf(stderr, "*** Warning: _OPENMP not defined (did you compile with -fopenmp?). The actual number of threads used will be 1.\n");
    #endif

    if (argc == 1) {
        usage(argv[0]);
        exit(1);
    }

    fname = argv[argc - 1];

    // Read in the file
    CHECK_ERROR((fd = open(fname, O_RDONLY)) < 0);
    // Get the file info (for file length)
    CHECK_ERROR(fstat(fd, &finfo) < 0);
    // Memory map the file
    CHECK_ERROR((fdata = mmap(0, finfo.st_size - 1, PROT_READ, MAP_PRIVATE, fd, 0)) == NULL);

    if ((fdata[0] != 'B') || (fdata[1] != 'M')) {
        fprintf(stderr, "*** Error: File %s is not a valid bitmap file. Exiting\n", fname);
        exit(1);
    }

    bool swap = test_endianess();

    uint16_t * bitsperpixel = (uint16_t *) &fdata[BITS_PER_PIXEL_POS];
    if (swap) {
        swap_bytes((uint8_t *) (bitsperpixel), sizeof(*bitsperpixel));
    }

    if (*bitsperpixel != 24) { // ensure there are 3 bytes per pixel
        fprintf(stderr, "*** Error: Invalid bitmap format: this application only accepts 24-bit pictures. Exiting\n");
        exit(1);
    }

    uint16_t * data_pos = (uint16_t *) &fdata[IMG_DATA_OFFSET_POS];
    if (swap) {
        swap_bytes((uint8_t *) (data_pos), sizeof(*data_pos));
    }

    int32_t imgdata_bytes = (int32_t) finfo.st_size - (int32_t) (*data_pos);
    int32_t num_pixels = imgdata_bytes / 3;

    printf("*** Starting Histogram ***\n");
    printf(" - Image: %s\n", fname);
    printf(" - Number of threads: %d\n", num_threads);
    printf(" - Number of pixels: %d\n", num_pixels);

    /* Sequential computation */
    {
        int64_t start = clocktime();
        init_histogram(red_ref, gre_ref, blu_ref);
        calc_seq(fdata, num_pixels * 3, red_ref, gre_ref, blu_ref);
        int64_t end = clocktime();
        printf(" - %-*s completed %8" PRId64 " ms\n", min_len, "calc_seq", end - start);
    }

    for (int32_t i = 0; i < array_size(func_tab); i += 1) {
        init_histogram(red, gre, blu);
        int64_t start = clocktime();

        func_tab[i](fdata, num_pixels * 3, red, gre, blu);

        int64_t end = clocktime();
        bool ok = check_results(red, gre, blu, red_ref, gre_ref, blu_ref);

        printf(" - %-*s completed %8" PRId64 " ms %s\n", min_len, func_name[i], end - start, ok ? "[OK]" : "[KO]");
    }

    CHECK_ERROR(munmap(fdata, finfo.st_size + 1) < 0);
    CHECK_ERROR(close(fd) < 0);

    if (dump_results) {
        const char * data_fname = "data.txt";
        FILE * f = fopen(data_fname, "w");
        if (f == NULL) {
            fprintf(stderr, "*** Error opening file %s\n", data_fname);
            exit(1);
        }
        printf(" - Dumping results...\n");
        for (int32_t j = 0; j < 256; j += 1) {
            fprintf(f, "%6d %6d %6d %6d\n", j, red_ref[j], gre_ref[j], blu_ref[j]);
        }
        fclose(f);
    }


    printf("*** End of Histogram ***\n");

    return 0;
}

// Local Variables:
// tab-width: 4
// c-basic-offset: 4
// c-file-offsets:((innamespace . 0)(inline-open . 0))
// indent-tabs-mode: nil
// End:

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4

