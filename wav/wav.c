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
#include <stdbool.h>
#include <math.h> // pow(), sin(), etc.

double midi_to_freq(int midi_code) {
	// 69 == A_4 == 440Hz
	return 440 * pow(2.0, (midi_code-69)/12.0);
}

// repeated calls construct a wave
int16_t get_value_at(double time, double frequency) {
	//return ((int16_t)(time * frequency * 2) % 2) * 8192; // square
	// return (int16_t) (sin(time * 2 * M_PI * frequency) * 8192); // sine
	// return fmod(time * frequency, 1) * 8192; // sawtooth
	// return (1 - fmod(time * frequency, 1)) * 8192; // reverse sawtooth
	return (12 * sin(time * 2 * M_PI * frequency)
		+ 12 * sin(time * 2 * M_PI * frequency * 2)
		+ 12 * sin(time * 2 * M_PI * frequency * 4)
		+ 1 * sin(time * 2 * M_PI * frequency * 7)
		+ 1 * sin(time * 2 * M_PI * frequency * 9)
		+ 1 * sin(time * 2 * M_PI * frequency * 11)) * 8192 / 10;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s out.wav\n", argv[0]);
		return 1;
	}
	printf("Writing to %s...\n", argv[1]);

	char *filename = argv[1];

	FILE *fp = fopen(filename, "w+"); // TODO: is this the mode I want? How exactly does it differ from `w`?

	int notes[] = {
		64, 62, 60, 62,  64, 64, 64, -1, // Ma-ry had a lit-tle lamb
		62, 62, 62, -1,  64, 67, 67, -1, // lit-tle lamb, lit-tle lamb
		64, 62, 60, 62,  64, 64, 64, 64, // Ma-ry had a lit-tle lamb whose
		62, 62, 64, 62,  60              // fleece was white as snow
		};
	int note_count = 29;
	float note_duration = 0.4; // seconds
	float time_between_notes = 0.1; // seconds

	// Generate the sound
	unsigned int const num_channels = 1;
	unsigned int const sample_rate = 44100;
	float length = note_count * (note_duration + time_between_notes); // seconds
	unsigned int const num_samples = sample_rate * length;
	unsigned int const bits_per_sample = 16;
	int16_t *samples = malloc(sizeof(int16_t) * num_samples);

	for (unsigned int i = 0; i < num_samples; i++) {
		double time = i / (double) sample_rate;
		int active_note_index = time / (note_duration + time_between_notes);
		bool done_with_note = time - active_note_index * (note_duration + time_between_notes) > note_duration;
		bool active = !done_with_note && (notes[active_note_index] >= 0);
		if (active) {
			samples[i] = get_value_at(time, midi_to_freq(notes[active_note_index]));
		} else {
			samples[i] = 0;
		}
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
