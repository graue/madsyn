Madsyn uses genetic programming to evolve weird noise.
You give it a sound formula, and it generates random
mutations of that formula and plays each of the original and
the mutations for a fixed length of time. Then you try again
with the mutation you like best.

This software is in the public domain.

## Usage

Madsyn takes a formula on the standard input and outputs
the audio on the standard output, in raw CD-audio format
(44.1 KHz, stereo, 16-bit little-endian linear PCM). This is
really convenient for me because I can pipe its output into
a sound-playing program like this:

    madsyn <formula | wavplay -sr

You can also redirect the output into a raw file to open
in an audio editor:

    madsyn &lt;formula &gt;output.raw

The full set of options is: `madsyn [-f] [-a amp]
[-l len_in_ms] [-m num_mutations]`

`-f`:
Instead of raw CD-audio format, output samples as 32-bit
floats. This lets you avoid clipping and manipulate the
sound using, for example, the effects in my Synth
software.

`-a amp`:
Set the output amplitude, as a ratio to the default. So
&quot;-a 0.5&quot; will cut the output amplitude in half,
making it about 6 dB softer.

`-l len_in_ms`:
Set the length in milliseconds for which each formula
(the original and the mutations) will be played. Default is
5000.

`-m num_mutations`:
The number of mutations to generate; default 3. Set to 0
if you only want to hear the input formula.

## Tips

Start with a simple formula, like the one in sine.madsyn
(`time 1000.0 * sin`), and eventually evolve
something complicated out of it.

The output can get extremely loud, so watch out for
that.

## Portability

It should be fairly portable, and is expected to compile
and work on BSD, Windows, and Linux, on little- and
big-endian CPUs. Nearly 20% of the code is portability goop
needed because Windows and Linux aren't as awesome as
BSD.
