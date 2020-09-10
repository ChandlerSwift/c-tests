/*
 * A short program to demonstrate writing WAV files. Inspired by a Javascript
 * version, of all things: https://www.youtube.com/watch?v=udbA7u1zYfc
 * 
 * There's some pretty good documentation (referenced in the above video) on
 * the WAV file format available at http://soundfile.sapp.org/doc/WaveFormat/
 * and some more at http://www.topherlee.com/software/pcm-tut-wavformat.html .
 */
#include <stdio.h>
#include <stdint.h> // int16_t, etc.
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s out.wav\n", argv[0]);
		return 1;
	}
	printf("Writing to %s...\n", argv[1]);

	char *filename = argv[1];

	FILE *fp = fopen(filename, "w+"); // TODO: is this the mode I want? How exactly does it differ from `w`?

	// Generate the sound
	unsigned int const num_channels = 1;
	unsigned int const sample_rate = 44100;
	unsigned int const length = 2; // seconds
	unsigned int const num_samples = sample_rate * length;
	unsigned int const bits_per_sample = 16;
	int16_t *samples = malloc(sizeof(int16_t) * num_samples);

	int high = 0; // a bool, really.
	int since_last_switched = 0;
	int wavelength = 2;
	for (unsigned int i = 0; i < num_samples; i++) {
		if (since_last_switched >= wavelength) {
			since_last_switched = 0;
			high = !high;
		}
		if (i % 500 == 0) { // Do something to make it interesting
			wavelength++;
		}
		samples[i] = high ? 16384 : -16384;
		since_last_switched++;
	}

	// Offset  Size  Name             Description
	//
	// The canonical WAVE format starts with the RIFF header:
	// 0         4   ChunkID          Contains the letters "RIFF" in ASCII form
	//                                (0x52494646 big-endian form).
	fputs("RIFF", fp);

	// 4         4   ChunkSize        36 + SubChunk2Size, or more precisely:
	//                                4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	//                                This is the size of the rest of the chunk 
	//                                following this number.  This is the size of the 
	//                                entire file in bytes minus 8 bytes for the
	//                                two fields not included in this count:
	//                                ChunkID and ChunkSize.
	uint32_t subchunk_2_size = num_samples * num_samples * bits_per_sample / 8; // as defined later
	uint32_t chunk_size = 36 + subchunk_2_size;
	fwrite(&chunk_size, 4, 1, fp);
	// 8         4   Format           Contains the letters "WAVE"
	//                                (0x57415645 big-endian form).
	fputs("WAVE", fp);


	// The "WAVE" format consists of two subchunks: "fmt " and "data":
	// The "fmt " subchunk describes the sound data's format:
	// 
	// 12        4   Subchunk1ID      Contains the letters "fmt "
	//                                (0x666d7420 big-endian form).
	fputs("fmt ", fp);
	// 16        4   Subchunk1Size    16 for PCM.  This is the size of the
	//                                rest of the Subchunk which follows this number.
	uint32_t subchunk_1_size = 16;
	fwrite(&subchunk_1_size, 4, 1, fp);
	// 20        2   AudioFormat      PCM = 1 (i.e. Linear quantization)
	//                                Values other than 1 indicate some 
	//                                form of compression.
	uint16_t audio_fmt = 1;
	fwrite(&audio_fmt, 2, 1, fp);
	// 22        2   NumChannels      Mono = 1, Stereo = 2, etc.
	uint16_t num_channels_uint16_t = 1;
	fwrite(&num_channels_uint16_t, 2, 1, fp);
	// 24        4   SampleRate       8000, 44100, etc.
	uint32_t sample_rate_uint32_t = 44100;
	fwrite(&sample_rate_uint32_t, 4, 1, fp);
	// 28        4   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
	uint32_t byte_rate = sample_rate * num_channels * bits_per_sample / 8;
	fwrite(&byte_rate, 4, 1, fp);
	// 32        2   BlockAlign       == NumChannels * BitsPerSample/8
	//                                The number of bytes for one sample including
	//                                all channels. I wonder what happens when
	//                                this number isn't an integer?
	uint16_t block_align = num_channels * bits_per_sample / 8;
	fwrite(&block_align, 2, 1, fp);
	// 34        2   BitsPerSample    8 bits = 8, 16 bits = 16, etc.
	uint16_t bps_uint16_t = bits_per_sample; // TODO: is there a neater way to do this cast?
	fwrite(&bps_uint16_t, 2, 1, fp);
	//           2   ExtraParamSize   if PCM, then doesn't exist
	//           X   ExtraParams      space for extra parameters

	// The "data" subchunk contains the size of the data and the actual sound:
	// 
	// 36        4   Subchunk2ID      Contains the letters "data"
	//                                (0x64617461 big-endian form).
	fputs("data", fp);
	// 40        4   Subchunk2Size    == NumSamples * NumChannels * BitsPerSample/8
	//                                This is the number of bytes in the data.
	//                                You can also think of this as the size
	//                                of the read of the subchunk following this 
	//                                number.
	fwrite(&subchunk_2_size, 4, 1, fp);
	
	// 44        *   Data             The actual sound data.
	fwrite(samples, bits_per_sample / 8, num_samples, fp);

	// TODO: fclose() -- unnecessary in this case since we just die, but still
	// good practice.
}
