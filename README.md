## generating configure commands

```
export AUTOPOINT=$(brew --prefix gettext)/bin/autopoint
$AUTOPOINT
```
Change `po.m4:27` from:

```
AC_REQUIRE([AM_PROG_MKDIR_P])dnl defined by automake
```

to

```
AC_REQUIRE([AC_PROG_MKDIR_P])dnl defined by automake
```

then run

```
autoreconf --install
```

## configure command output

```
bash-4.3$ ./configure LDFLAGS=-L$(brew --prefix libdvdread)/lib CPPFLAGS=-I$(brew --prefix libdvdread)/include
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... build-aux/install-sh -c -d
checking for gawk... no
checking for mawk... no
checking for nawk... no
checking for awk... awk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking whether NLS is requested... yes
checking for msgfmt... no
checking for gmsgfmt... :
checking for xgettext... no
checking for msgmerge... no
checking for style of include used by make... GNU
checking for gcc... clang
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables...
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether clang accepts -g... yes
checking for clang option to accept ISO C89... none needed
checking whether clang understands -c and -o together... yes
checking dependency style of clang... gcc3
checking build system type... x86_64-apple-darwin15.5.0
checking host system type... x86_64-apple-darwin15.5.0
checking for ld used by GCC... /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld
checking if the linker (/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld) is GNU ld... no
checking for shared library run path origin... done
checking for CFPreferencesCopyAppValue... yes
checking for CFLocaleCopyCurrent... yes
checking for GNU gettext in libc... no
checking for iconv... yes
checking for working iconv... yes
checking how to link with libiconv... -liconv
checking for GNU gettext in libintl... no
checking whether to use NLS... no
checking whether to enable maintainer-specific portions of Makefiles... no
checking for clang option to accept ISO C99... none needed
checking whether ln -s works... yes
checking for DVDOpen in -ldvdread... yes
checking for DVDFileStat in -ldvdread... yes
checking how to run the C preprocessor... clang -E
checking for grep that handles long lines and -e... /usr/bin/grep
checking for egrep... /usr/bin/grep -E
checking for ANSI C header files... yes
checking for sys/types.h... yes
checking for sys/stat.h... yes
checking for stdlib.h... yes
checking for string.h... yes
checking for memory.h... yes
checking for strings.h... yes
checking for inttypes.h... yes
checking for stdint.h... yes
checking for unistd.h... yes
checking dvdread/dvd_reader.h usability... yes
checking dvdread/dvd_reader.h presence... yes
checking for dvdread/dvd_reader.h... yes
checking fcntl.h usability... yes
checking fcntl.h presence... yes
checking for fcntl.h... yes
checking libintl.h usability... no
checking libintl.h presence... no
checking for libintl.h... no
checking limits.h usability... yes
checking limits.h presence... yes
checking for limits.h... yes
checking locale.h usability... yes
checking locale.h presence... yes
checking for locale.h... yes
checking for stdint.h... (cached) yes
checking for stdlib.h... (cached) yes
checking for string.h... (cached) yes
checking for unistd.h... (cached) yes
checking for stdbool.h that conforms to C99... yes
checking for _Bool... yes
checking for an ANSI C-conforming const... yes
checking for inline... inline
checking for off_t... yes
checking for size_t... yes
checking for stdlib.h... (cached) yes
checking for GNU libc compatible malloc... yes
checking whether lstat correctly handles trailing slash... no
checking whether stat accepts an empty string... no
checking for mkdir... yes
checking for setlocale... yes
checking for strstr... yes
checking for special C compiler options needed for large files... no
checking for _FILE_OFFSET_BITS value needed for large files... no
getconf: no such configuration parameter `LFS_CFLAGS'
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating man/Makefile
config.status: creating po/Makefile.in
config.status: creating src/Makefile
config.status: creating config.h
config.status: executing po-directories commands
config.status: creating po/POTFILES
config.status: creating po/Makefile
config.status: executing depfiles commands
bash-4.3$ ls
ABOUT-NLS	Makefile	REAMDE.vcs	config.h.in	m4
AUTHORS		Makefile.am	aclocal.m4	config.log	man
COPYING		Makefile.in	autom4te.cache	config.status	po
ChangeLog	NEWS		build-aux	configure	src
INSTALL		README		config.h	configure.ac	stamp-h1
bash-4.3$
```

## build with

```
make
```

And that creates the executable in the src directory:

```
./src/dvdbackup
```

should display the help output
