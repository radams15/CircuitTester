#!/bin/perl
use Cwd;

use warnings;
use strict;

my $ROOT = getcwd;

sub make_win {
	`rm -rf winbuild`;
	`mkdir -p winbuild`;
	chdir 'winbuild';
	
	`mingw64-cmake -DEIGEN3_INCLUDE_DIR=$ROOT/lib/eigen/ -DCMAKE_BUILD_TYPE=release ..`;
	`mingw64-make -j4`;
	
	chdir "$ROOT";
	
	`perl movewindeps.pl`;
}

sub make_lin {
	`rm -rf linbuild`;
	`mkdir -p linbuild`;
	chdir "linbuild";
	
	`cmake -DEIGEN3_INCLUDE_DIR=$ROOT/lib/eigen/ -DCMAKE_BUILD_TYPE=release ..`;
	`make -j4`;
	
	chdir "$ROOT";
}

sub move {
	`mkdir -p out`;
	`cp -r $ROOT/linbuild/src/main $ROOT/out/linux`;
	`cp -r $ROOT/winbuild/src/main $ROOT/out/windows`;
	`rm -rf $ROOT/out/windows/CMakeFiles $ROOT/out/windows/*_autogen $ROOT/out/windows/Makefile $ROOT/out/windows/*.cmake`;
	`rm -rf $ROOT/out/linux/CMakeFiles $ROOT/out/linux/*_autogen $ROOT/out/linux/Makefile $ROOT/out/linux/*.cmake`;
	
	chdir "$ROOT/out/linux";
	`zip -rq $ROOT/out/linux.zip .`;
	
	chdir "$ROOT/out/windows";
	`zip -rq $ROOT/out/windows.zip .`;
	
	chdir "$ROOT/";
	
	`rm -rf $ROOT/out/windows $ROOT/out/linux`;
}

sub archive {

}

`rm -rf out/`;

#make_win();
#make_lin();
move();
archive();
